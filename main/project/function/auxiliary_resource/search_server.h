#pragma once
#include<string>
#include<vector>
#include<map>
#include"index.h"
class SearchServer{
	private:
	std::vector<std::string> words;
	std::map<std::string, std::vector<unsigned short>> GET;
	public:
	void operator=(const std::string& list);
	void operator()(std::map<std::string, std::vector<Index>>& index);
	std::map<std::string, std::vector<unsigned short>>& operator()();
};