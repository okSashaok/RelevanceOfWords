#pragma once
#include<string>
#include<vector>
#include<map>
#include"index.h"
class InvertedIndex{
	private:
	std::map<std::string, std::vector<Index>> GET;
	struct MapThread{
		std::vector<std::string> words;
		std::vector<Index> index;
		void operator=(const std::string& stringWords);
	};
	public:
	InvertedIndex(std::vector<std::string>& files);
	std::map<std::string, std::vector<Index>>& operator()();
};