
#include <iostream>
#include "AddNode.hpp"


int main(){
	blocksci::Blockchain chain("/home/bitcoin-core/.blocksci/config.json");
  return 0;
}
/*
1. 검색
	[1] Wallet Processing Function | ((string)address, pre_info : 전 txid, timelimit)
		f_tx[];
		std_wallet = (wallet)findwalletinfo(address);
		// <Vector>result[] = { target = address | txid, type = w | tx};
		result[]; 
		for( std_wallet.txFind() )
			if( pre_info == std_wallet.txFind()[i] )
				return;
			f_tx[] = address.txFind(); // 초기 데이터에 관련 tx 넣기
		for(f_tx[])
			if(f_tx[].time < timelimit)
				-1; // 삭제
		for(f_tx[]) {
			//tmp[] = [2](f_tx[], timelimit);
			result[] = [1]( [2]( f_tx[], timelimit).vin_address, timelimit );
			result[] = [1]( [2]( f_tx[], timelimit).vout_address, timelimit );
			makeNode();
		}
		return result;


	[2] Transaction Processing Function | (txid : string, timelimit)
		f_wallet[];
		std_txid = (transaction)findtxinfo(txid);
		// <Vector>result[] = { target = address | txid, type = w | tx};
		result[];
		for( std_txid.wFind() ) 
			f_wallet[] = txid.wFind(); // 초기 데이터에 관련 wallet 주소 넣기
		for( f_wallet[] ) {	
			// result.append(f_wallet[i]);		
			// tmp[] = [1]( f_wallet[i], txid, timelimit )
			result[] = [2]( [1](f_wallet[], timelimit)., timelimit );
			makeNode(txid, transaction);
			makeEdge(txid, txid.);
		}
		return result;

makeNode( (string)address, auto type )
makeEdge( string txid, auto in_w[], auto out_w[] )
*/
getNode:: getNode(const std::string& target, const std::string& type){
  if(type == "wallet"){
    auto paddr = blocksci::getAddressFromString(target, chain.getAccess());
    waddress.insert(paddr);
    getWalletInfo(target);
  }else {
    //auto tx = blockchain.txWithHash(target);
    blocksci::Transaction tx(target, chain.getAccess());
    txid.insert(tx);
    getTxInfo(target);
  }
}

getNode::getWalletInfo(const std::string& address, const std::string& preinfo){
  
}
getNode::getTxInfo(const std::auto& txid){
	if(isIn3Months(txid.timestamp)){
		for(const auto &input : txid.ipnuts){
			auto inputaddress = input.getAddress();
		} // INPUT
		for(const auto &output : tx.outputs()){
			auto outaddress = output.getAddress();
		} // OUTPUT
	}
}

bool isIn3Months(time_t blockTime){
  time_t current = std::time(nullptr);
  time_t dest = current - DTIME;
  return blockTime >= dest;
}