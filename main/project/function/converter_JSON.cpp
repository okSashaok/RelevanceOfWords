#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<fstream>
#include"nlohmann/json.hpp"
#include"auxiliary_resource/search_server.h"
#include"auxiliary_resource/print_JSON.h"
#include"converter_JSON.h"
std::vector<std::string> ConverterJSON::GetTextDocuments(){
	std::fstream FileCheck;
	FileCheck.open("config.json", std::fstream::in);
	nlohmann::json JSON;
	FileCheck >> JSON;
	FileCheck.close();
	if(JSON["config"]["version"] != "0.1"){ std::cout << "has incorrect file version\n"; }
	JSON = JSON["files"];
	std::vector<std::string> textFiles;
	const auto max = JSON.size();
	for(unsigned short i = 0; i < max; ++i, FileCheck.close()){
		FileCheck.open(JSON[i].get<std::string>());
		if(!FileCheck){
			std::cout << JSON[i] << " not found\n";
			textFiles.push_back("");
			continue;
		}
		char letter;
		textFiles.push_back({});
		const auto sizeTextFiles = textFiles.size() - 1;
		while(FileCheck.get(letter)){ textFiles[sizeTextFiles] += letter; }
	}
	return textFiles;
}
unsigned short ConverterJSON::GetResponsesLimit(){
	std::fstream FileCheck;
	FileCheck.open("config.json", std::fstream::in);
	nlohmann::json JSON;
	FileCheck >> JSON;
	FileCheck.close();
	return JSON["config"]["max_responses"];
}
std::vector<std::string> ConverterJSON::GetRequests(){
	std::fstream FileCheck;
	FileCheck.open("requests.json", std::fstream::in);
	nlohmann::json JSON;
	FileCheck >> JSON;
	FileCheck.close();
	std::vector<std::string> requests_json;
	const auto max = JSON["requests"].size();
	for(unsigned short i = 0; i < max; ++i){ requests_json.push_back(JSON["requests"][i]); }
	return requests_json;
}
void ConverterJSON::putAnswers(std::vector<SearchServer>& relevance){
	std::string GET_printJSON;
	PrintJSON printJSON(GET_printJSON);
	printJSON("answers");
	unsigned short flag;/*
	0	"result": "false"
	1	"result": "true"
	2+	"result": "true"
		"relevance": {}
	*/
	auto max_comment = GetResponsesLimit();
	const auto size = relevance.size();
	for(unsigned short l0 = 0; l0 < size; ++l0){
		flag = 0;
		std::stringstream cache;
		cache << "request" << std::setw(3) << std::setfill('0') << std::to_string(l0 + 1);
		printJSON(cache.str().c_str());
		const auto& request = relevance[l0]();
		if(!max_comment);
		else{
			for(auto it = request.rbegin(); it != request.rend(); ++it){
				if(it->first != "0.000"){
					flag += it->second.size();
				}
				if(max_comment == 1 && flag > max_comment){
					flag = 1;
					break;
				}
				if(flag > 1){ break; }
			}
		}
		if(flag == 0){ printJSON("result", "false"); }
		else if(flag == 1){ printJSON("result", "true"); }
		else{
			printJSON("result", "true");
			printJSON("relevance");
		}
		//unsigned short max_kay=GetResponsesLimit()-1;
		unsigned char min = 0;
		for(auto it = request.rbegin(); it != request.rend(); ++it){
			if(min > max_comment){ break; }
			const auto& comment = it->first;
			if(comment == "0.000"){ break; }
			const auto& kay = it->second;
			const auto max = kay.size();
			for(unsigned char l1 = 0; l1 < max; ++l1){
				if(++min > max_comment){ break; }
				//if(l1<max_kay){break;}
				printJSON(
					("docid_" + std::to_string(kay[l1])).c_str()
					,stof(comment)
					,5
				);
			}
		}
		for(unsigned char i = 0, max = (flag > 1 ? 2 : 1); i < max; ++i){ --printJSON; }
	}
	printJSON--;
	std::fstream FileCreation;
	FileCreation.open("answers.json", std::fstream::out);
	FileCreation << GET_printJSON;
	FileCreation.close();
}