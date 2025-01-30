#include<string>
#include<vector>
#include<map>
#include"global_resource/manager.h"
#include"index.h"
#include"inverted_index.h"
void InvertedIndex::MapThread::operator=(const std::string& stringWords){
	std::string word;
	bool end = 0;
	for(unsigned short l0 = 0; !end; ++l0){
		switch(stringWords[l0]){
			case 0:{ end = 1; }case ' ':{
				const auto sizeFirst = words.size();
				unsigned char l1;
				for(l1 = 0; l1 < sizeFirst; ++l1){
					if(word == words[l1]){ break; }
				}
				if(l1 == sizeFirst){
					words.push_back(word);
					index.push_back({});
				}
				index[l1].count += 1;
				word = "";
			}break;
			default:{ word += stringWords[l0]; }break;
		}
	}
}
InvertedIndex::InvertedIndex(std::vector<std::string>& files){
	const auto size = files.size();
	std::vector<MapThread> mapThread(size);
	manager(mapThread, files, size);
	for(unsigned short l0 = 0; l0 < size; ++l0){
		const auto sizeFirst = mapThread[l0].words.size();
		for(unsigned char l1 = 0; l1 < sizeFirst; ++l1){
			mapThread[l0].index[l1].wordID = l0;
			GET[(mapThread[l0].words[l1])].push_back(mapThread[l0].index[l1]);
		}
	}
}
std::map<std::string, std::vector<Index>>& InvertedIndex::operator()(){ return GET; }