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
        blocksci::Block block(tx.getBlockHeight(), chain.getAccess());
        json res;
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
        for (auto input : inputs)
        {
            res["inputs"].push_back(MakeInputData(input));
            inputValue += input.getValue();
        }
        for (auto output : outputs)
        {
            res["outputs"].push_back(MakeOutputData(output));
            outputValue += output.getValue();
        }

        res["input_value"] = inputValue;
        res["output_value"] = outputValue;
        res["fee"] = inputValue - outputValue;

        res["profile"] = mongo.getProfile(hash);
        return res.dump();
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

json ProcessApi::MakeInputData(blocksci::Input input)
{
    json res;
    json prevData;
    res["txid"] = input.getSpentTx().getHash().GetHex();
    res["n"] = input.inputIndex();
    prevData["value"] = input.getValue();
    prevData["addr"] = onlyAddress(input.getAddress().toString());
    res["prev_out"] = prevData;

    return res;
}

json ProcessApi::MakeOutputData(blocksci::Output output)
{
    json res;
    res["addr"] = onlyAddress(output.getAddress().toString());
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