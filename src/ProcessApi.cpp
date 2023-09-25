#include "ProcessApi.hpp"

#include <iostream>

ProcessApi::ProcessApi(MongoDB &mongo, BitcoinCore &bitcoinCore,
                       blocksci::Blockchain &chain)
    : mongo(mongo), bitcoinCore(bitcoinCore), chain(chain) {}

std::string ProcessApi::getTxData(const utility::string_t &req)
{
    std::string hash = utility::conversions::to_utf8string(req);
    try
    {
        blocksci::Transaction tx(hash, chain.getAccess());
        json res;
        auto block = tx.block();
        auto inputs = tx.inputs();
        auto outputs = tx.outputs();
        int64_t fee, inputValue, outputValue;

        res["txid"] = tx.getHash().GetHex();
        res["n_input"] = tx.inputCount();
        res["n_output"] = tx.outputCount();
        res["block_hash"] = block.getHash().GetHex();
        res["block_height"] = block.height();
        res["size"] = tx.totalSize();
        res["time"] = block.timestamp();
        res["ver"] = tx.getVersion();
        res["lock_time"] = tx.locktime();

        fee = inputValue = outputValue = 0;
        if (tx.isCoinbase())
        {
            for (const auto &input : inputs)
            {
                res["inputs"].push_back({{"coinbase", true}, {"sequence", input.sequenceNumber()}});
            }
        }
        else
        {
            for (const auto &input : inputs)
            {
                res["inputs"].push_back(MakeInputData(input));
                inputValue += input.getValue();
            }
        }
        for (const auto &output : outputs)
        {
            res["outputs"].push_back(MakeOutputData(output));
            outputValue += output.getValue();
        }

        res["input_value"] = std::move(inputValue);
        res["output_value"] = std::move(outputValue);
        res["fee"] = inputValue == 0 ? 0 : inputValue - outputValue;

        res["profile"] = mongo.getProfile(hash);
        return res.dump();
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

std::string ProcessApi::getWalletData(const utility::string_t &req)
{
    clock_t start, finish;
    double duration;
    std::string hash = utility::conversions::to_utf8string(req);
    auto address = blocksci::getAddressFromString(hash, chain.getAccess());
    if (!address)
    {
        throw std::runtime_error("Invalid address");
    }

    json res;
    auto viewTxs = address->getTransactions();
    auto viewInputTxs = address->getInputTransactions();
    uint64_t total_sent = 0, total_received = 0, n_tx = 0, n_sent_tx = 0, n_rcv_tx = 0;
    time_t last_sent_time = 0, last_recv_time = 0;

    start = clock();
    for (const auto &tx : viewTxs)
    {
        int64_t localSentValue = 0, localReceivedValue = 0;
        bool sent = false, received = false;
        time_t timestamp = tx.block().timestamp();

        for (const auto &input : tx.inputs())
        {
            if (input.getAddress() == *address)
            {
                sent = true;
                localSentValue += input.getValue();
            }
        }

        for (const auto &output : tx.outputs())
        {
            if (output.getAddress() == *address)
            {
                received = true;
                localReceivedValue += output.getValue();
            }
        }

        total_sent += localSentValue;
        total_received += localReceivedValue;

        if (sent)
        {
            if (last_sent_time < timestamp)
                last_sent_time = std::move(timestamp);
            ++n_sent_tx;
        }
        if (received)
        {
            if (last_recv_time < timestamp)
                last_recv_time = std::move(timestamp);
            ++n_rcv_tx;
        }
        ++n_tx;
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << "완료 : " << duration << "초" << std::endl;

    res["addr"] = hash;
    res["n_tx"] = n_tx;
    res["n_sent_tx"] = n_sent_tx;
    res["n_rcv_tx"] = n_rcv_tx;
    if (viewTxs.begin() == viewTxs.end())
        res["first_seen_receiving"] = nullptr;
    else
        res["first_seen_receiving"] = (*viewTxs.begin()).block().timestamp();
    if (viewInputTxs.begin() == viewInputTxs.end())
        res["first_seen_sending"] = nullptr;
    else
        res["first_seen_sending"] = (*viewInputTxs.begin()).block().timestamp();
    if (last_recv_time == 0)
        res["last_seen_receiving"] = nullptr;
    else
        res["last_seen_receiving"] = last_recv_time;
    if (last_sent_time == 0)
        res["last_seen_sending"] = nullptr;
    else
        res["last_seen_sending"] = last_sent_time;
    res["total_received"] = total_received;
    res["total_sent"] = total_sent;
    res["final_balance"] = total_received - total_sent;
    res["cluster"] = mongo.clusterFindByAddr(hash);
    res["profile"] = mongo.getProfile(hash);

    return res.dump();
}

std::string ProcessApi::getTxInWallet(const std::string &hash, const time_t &startDate, const time_t &endDate)
{
    auto address = blocksci::getAddressFromString(hash, chain.getAccess());
    if (!address)
    {
        throw std::runtime_error("Invalid address");
    }
    json res;
    auto viewTxs = address->getTransactions();

    std::deque<json> txQue;
    int64_t localSentValue, localReceivedValue;

    for (const auto &tx : viewTxs)
    {
        if (tx.block().timestamp() < startDate)
            continue;
        if (tx.block().timestamp() > endDate)
            break;
        json txDoc;
        localSentValue = localReceivedValue = 0;
        txDoc["txid"] = tx.getHash().GetHex();
        txDoc["timestamp"] = tx.block().timestamp();
        for (const auto &input : tx.inputs())
        {
            if (input.getAddress() == *address)
                localSentValue += input.getValue();
        };
        for (const auto &output : tx.outputs())
        {
            if (output.getAddress() == *address)
                localReceivedValue += output.getValue();
        };
        txDoc["value"] = localReceivedValue - localSentValue;
        txDoc["fee"] = tx.fee();
        txDoc["index"] = tx.txNum;
        txQue.push_front(std::move(txDoc));
    };

    res["txs"] = txQue;

    return res.dump();
}

std::string ProcessApi::getClusterData(const utility::string_t &req)
{
    std::string target = utility::conversions::to_utf8string(req);
    std::optional<json> maybeResult;
    std::regex hexPattern("^[0-9a-fA-F]{24}$");
    if (std::regex_match(target, hexPattern))
        maybeResult = mongo.clusterFindById(target);
    else
        maybeResult = mongo.clusterFindByName(target);

    if (!maybeResult)
        throw std::runtime_error("Invalid Cluster");
    json rawData = *maybeResult;
    json res;
    res["_id"] = rawData["_id"]["$oid"];
    res["name"] = rawData["name"];
    res["n_wallet"] = rawData["address"].size();
    res["constructor"] = rawData["metadata"]["constructor"];
    res["date_created"] = rawData["metadata"]["date_created"];
    res["date_last_modified"] = rawData["metadata"]["date_last_modified"];
    res["last_modifier"] = rawData["metadata"]["last_modifier"];
    res["profile"] = mongo.getProfile(rawData["_id"]["$oid"]);
    for (const auto &addr : rawData["address"])
    {
        json doc;
        auto balance = blocksci::getAddressFromString(addr, chain.getAccess())->calculateBalance(-1);
        doc["address"] = addr;
        doc["balance"] = balance;
        res["wallet"].push_back(std::move(doc));
    }

    return res.dump();
}

json ProcessApi::MakeInputData(blocksci::Input input)
{
    json res;
    json prevData;

    res["txid"] = input.getSpentTx().getHash().GetHex();
    res["n"] = input.inputIndex();

    auto address = onlyAddress(input.getAddress().toString());
    prevData["addr"] = address.empty() ? "Unknown" : address;
    prevData["value"] = input.getValue();

    res["prev_out"] = prevData;

    return res;
}

json ProcessApi::MakeOutputData(blocksci::Output output)
{
    json res;
    auto address = onlyAddress(output.getAddress().toString());
    res["addr"] = address.empty() ? "Unknown" : address;
    res["value"] = output.getValue();
    res["n"] = output.outputIndex();
    res["spent"] = output.isSpent();

    auto spendingInput = output.getSpendingInput();
    if (spendingInput)
    {
        json spender;
        spender["txid"] = spendingInput->transaction().getHash().GetHex();
        spender["n"] = spendingInput->inputIndex();
        res["spender"] = spender;
    }
    else
    {
        res["spnder"] = json::object();
    }

    return res;
}

std::string ProcessApi::onlyAddress(const std::string &fullString)
{
    std::string delimiter = "(";
    return fullString.substr(fullString.find(delimiter) + 1, fullString.size() - fullString.find(delimiter) - 2);
}

/*
std::string ProcessApi::getWalletData(const utility::string_t &req)
{
    clock_t start, finish;
    double duration;
    std::string hash = utility::conversions::to_utf8string(req);
    auto address = blocksci::getAddressFromString(hash, chain.getAccess());
    if (!address)
    {
        throw std::runtime_error("Invalid address");
    }

    std::cout << address->calculateBalance(-1) << std::endl;

    json res;
    auto viewTxs = address->getTransactions();
    auto viewInputTxs = address->getInputTransactions();

    std::atomic<int64_t> total_sent(0);
    std::atomic<int64_t> total_received(0);
    std::atomic<uint32_t> n_sent_tx(0);
    std::atomic<uint32_t> n_rcv_tx(0);
    std::atomic<uint32_t> n_tx(0);
    std::atomic<uint32_t> last_sent_time(0);
    std::atomic<uint32_t> last_recv_time(0);

    const unsigned int maxConcurrentThreads = 300;
    std::vector<std::future<void>> futures;
    std::mutex vec_mutex;
    std::vector<json> txDocs;

    start = clock();
    for (const auto &tx : viewTxs)
    {
        while (futures.size() >= maxConcurrentThreads)
        {
            futures.erase(std::remove_if(futures.begin(), futures.end(),
                                         [](const std::future<void> &f)
                                         { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }),
                          futures.end());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        futures.push_back(std::async(std::launch::async, [&, tx]()
                                     {
        json txDoc;
        int64_t localSentValue = 0, localReceivedValue = 0;
        bool sent = false, received = false;
        uint32_t timestamp = tx.block().timestamp();
        txDoc["txid"] = tx.getHash().GetHex();
        txDoc["time"] = timestamp;
        txDoc["fee"] = tx.fee();
        txDoc["index"] = tx.txNum;

        auto inputFuture = std::async(std::launch::async, [&]() {
            for (const auto &input : tx.inputs())
            {
                if (input.getAddress() == *address)
                {
                    sent = true;
                    localSentValue += input.getValue();
                }
            }
        });

        auto outputFuture = std::async(std::launch::async, [&]() {
            for (const auto &output : tx.outputs())
            {
                if (output.getAddress() == *address)
                {
                    received = true;
                    localReceivedValue += output.getValue();
                }
            }
        });

        inputFuture.wait();
        outputFuture.wait();

        total_sent += localSentValue;
        total_received += localReceivedValue;

        txDoc["value"] = localReceivedValue - localSentValue;
        addTxToVector(txDoc, txDocs, vec_mutex);

        if (sent)
        {
            last_sent_time = last_sent_time.load() > timestamp ? last_sent_time.load() : timestamp;
            ++n_sent_tx;
        }
        if (received)
        {
            last_recv_time = last_recv_time.load() > timestamp ? last_recv_time.load() : timestamp;
            ++n_rcv_tx;
        }
        ++n_tx; }));
    }
    for (const auto &future : futures)
        future.wait();

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << "삽입 : " << duration << "초" << std::endl;

    start = clock();
    // txDoc["time"] 값을 기준으로 내림차순 정렬
    std::sort(txDocs.begin(), txDocs.end(), [](const json &a, const json &b)
              { return a["index"] > b["index"]; });
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << "정렬 : " << duration << "초" << std::endl;

    int64_t totalSent = total_sent;
    int64_t totalReceived = total_received;
    uint32_t nTx = n_tx;
    uint32_t nSentTx = n_sent_tx;
    uint32_t nRcvTx = n_rcv_tx;
    uint32_t lastRecvTime = last_recv_time;
    uint32_t lastSentTime = last_sent_time;

    res["txs"] = txDocs;
    res["addr"] = hash;
    res["n_tx"] = nTx;
    res["n_sent_tx"] = nSentTx;
    res["n_rcv_tx"] = nRcvTx;
    if (viewTxs.begin() == viewTxs.end())
        res["first_seen_receiving"] = nullptr;
    else
        res["first_seen_receiving"] = (*viewTxs.begin()).block().timestamp();
    if (viewInputTxs.begin() == viewInputTxs.end())
        res["first_seen_sending"] = nullptr;
    else
        res["first_seen_sending"] = (*viewInputTxs.begin()).block().timestamp();
    if (lastRecvTime == 0)
        res["last_seen_receiving"] = nullptr;
    else
        res["last_seen_receiving"] = lastRecvTime;
    if (lastSentTime == 0)
        res["last_seen_sending"] = nullptr;
    else
        res["last_seen_sending"] = lastSentTime;
    res["total_received"] = totalReceived;
    res["total_sent"] = totalSent;
    res["final_balance"] = totalReceived - totalSent;
    res["cluster"] = mongo.clusterFindByAddr(hash);
    res["profile"] = mongo.getProfile(hash);

    return res.dump();
}


void ProcessApi::addTxToVector(json &tx, std::vector<json> &vec, std::mutex &vec_mutex)
{
    std::lock_guard<std::mutex> lock(vec_mutex);
    vec.push_back(std::move(tx));
}
*/
