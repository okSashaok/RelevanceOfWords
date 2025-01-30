#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include"nlohmann/json.hpp"
#include"function/auxiliary_resource/global_resource/manager.h"
#include"function/auxiliary_resource/index.h"
#include"function/auxiliary_resource/inverted_index.h"
#include"function/auxiliary_resource/search_server.h"
#include"core.h"
int Modules::main(){
	std::vector<std::string> files = JSON.GetTextDocuments()
	,list = JSON.GetRequests();
	unsigned short sizeList = list.size();
	InvertedIndex invertedIndex(files);
	std::vector<SearchServer> searchServer(sizeList);
	manager(searchServer, list, sizeList, invertedIndex());
	JSON.putAnswers(searchServer);
	return 0;
}
bool Modules::initialization(std::string& error){
	std::fstream FileCheck;
	FileCheck.open("config.json", std::fstream::in);
	if(!FileCheck){
		error += "config file is missing\n";
		return true;
	}
	nlohmann::json JSON;
	FileCheck >> JSON;
	FileCheck.close();
	for(auto it = JSON.begin(); ; ++it){
		if(it == JSON.end()){
		error += "config file is empty\n";
		return true;
		}
		else if(it.key() == "config"){
			std::cout << (std::string)JSON["config"]["name"] << "\n";
			return false;
		}
	}
}