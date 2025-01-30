#include<string>
#include<vector>
#include<map>
#include"index.h"
#include"search_server.h"
void SearchServer::operator=(const std::string& list){
//structural copy > void InvertedIndex::MapThread::operator=(const std::string& stringWords)
	std::string word;
	bool end = 0;
	for(unsigned short l0 = 0; !end; ++l0){
		switch(list[l0]){
			case 0:{ end = 1; }case ' ':{
				const auto sizeFirst = words.size();
				unsigned char l1;
				for(l1 = 0; l1 < sizeFirst; ++l1){
					if(word == words[l1]){ break; }
				}
				if(l1 == sizeFirst){
					words.push_back(word);
				}
				word = "";
			}break;
			default:{ word += list[l0]; }break;
		}
	}
}
void SearchServer::operator()(std::map<std::string, std::vector<Index>>& index){
	std::vector<Index> relevance;

	const auto sizeWords = words.size();
	unsigned char l0
	, flag;

	for(l0 = 0, flag = 0; l0 < sizeWords; ++l0){
		if(index.find(words[l0]) != index.end()){ ++flag; }
	}
	if(flag < sizeWords){ return; }
	unsigned short coincidences = 65'535;
	for(l0 = 0; l0 < sizeWords; ++l0){
		const auto sizeIndex = index[words[l0]].size();
		if(coincidences > sizeIndex){ coincidences = sizeIndex; }
	}
	unsigned short i;
	for(i = 0; i < coincidences; ++i){
		relevance.push_back(index[words[0]][i]);
		for(l0 = 0; l0 < sizeWords; ++l0){
			const auto& word = words[l0];
			relevance[i].count += index[word][i].count;
			
		}
	}
	const auto sizeRelevance = relevance.size();
	unsigned short max = 0;
	for(i = 0; i < sizeRelevance; ++i){
		if(max < relevance[i].count){ max = relevance[i].count; }
	}
	std::string cache, floatText;
	for(i = 0; i < sizeRelevance; ++i){
		cache = std::to_string((float)relevance[i].count / max);
		floatText = "";
		for(unsigned char x = 0; x < 5; ++x){ floatText += cache[x]; }
		GET[floatText].push_back(relevance[i].wordID);
	}
	words = {};
}
std::map<std::string, std::vector<unsigned short>>& SearchServer::operator()(){return GET;}