#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<filesystem>
#include"project/function/auxiliary_resource/global_resource/manager.h"
#include"project/function/auxiliary_resource/index.h"
#include"project/function/auxiliary_resource/inverted_index.h"
#include"project/function/auxiliary_resource/search_server.h"
#include"project/function/converter_JSON.h"
#include"project/core.h"
#include "gtest/gtest.h"
const char* FIXED{"[FIXED test] "}
,* DEPLOYMENT{"[DEPLOYMENT]\n"}
,* DEP_OK{"[DEP     OK]\n"};
namespace Ntest{//TEST
	auto files = 1'000
	,max_responses = 10
	,requestsSize = 1'000
	,lines = 1'000
	,words = 10
	,symbols = 100;
	std::vector<const char*> Vwords{
		"Move"
		,"Copybook"
		,"Krechet"
		,"Rocky"
		,"Afternoon"
		,"Krechet"
		,"Grab"
		,"Photographer"
		,"A"
		,"Little"
		,""/*NULL*/
	};
	void mixingVectorString(std::vector<std::string>& object, auto& test){
		std::string cache;
		for(size_t l0 = 0; l0 < test; ++l0){
			cache = "";
			for(size_t l1 = 0; l1 < Ntest::requestsSize; ++l1){
				if(l1 && l1 != Ntest::requestsSize){ cache += " "; }
				cache += Ntest::Vwords[(rand() % (Ntest::Vwords.size() - 1))];
			}
			object.push_back(cache);
		}
	}
	void mixingWords(std::vector<std::string>& object){
		std::string cache;
		for(size_t l0 = 0; l0 < Ntest::files; ++l0){
			cache = "";
			for(size_t l1 = 0; l1 < Ntest::words; ++l1){
				if(l1 && l1 != Ntest::requestsSize){ cache += " "; }
				cache += Ntest::Vwords[(rand() % (Ntest::Vwords.size() - 1))];
			}
			object.push_back(cache);
		}
	}
}
namespace Function{
	TEST(ConverterJSON, GetTextDocuments){
		//TEST Ntest::files
		std::cout << DEPLOYMENT << FIXED << Ntest::files << " files!!!\n";
		std::filesystem::create_directory("resources");
		std::fstream FileCreation
		, FileNew;
		FileCreation.open("config.json", std::fstream::out);
		FileCreation << "{\n\t\"config\": {\n\t\t\"name\": \"SearchEngine\",\n\t\t\"version\": \"0.1\",\n\t\t\"max_responses\": 5\n\t},\n\t\"files\": [";
		for(size_t i = 1; ;){
			FileNew.open("resources/file" + std::to_string(i) + ".txt", std::fstream::out);
			FileNew << "Test file";
			FileNew.close();
			FileCreation << "\n\t\t\"../TEST/resources/file" + std::to_string(i) + ".txt\"";
			if(i < Ntest::files){
				++i;
				FileCreation << ",";
			}
			else{ break; }
		}
		FileCreation << "\n\t]\n}";
		FileCreation.close();
		ConverterJSON test;
		std::cout << DEP_OK;
		EXPECT_EQ(test.GetTextDocuments().size(), Ntest::files);
		std::filesystem::remove_all("resources");
		std::filesystem::remove("config.json");
	}
	TEST(ConverterJSON, GetResponsesLimit){
		//TEST Ntest::max_responses
		std::cout << FIXED << Ntest::max_responses << " max_responses!!!\n";
		std::fstream FileCreation;
		FileCreation.open("config.json", std::fstream::out);
		FileCreation << "{\n\t\"config\": {\n\t\t\"name\": \"SearchEngine\",\n\t\t\"version\": \"0.1\",\n\t\t\"max_responses\": " + std::to_string(Ntest::max_responses) + "\n\t}\n}";
		FileCreation.close();
		ConverterJSON test;
		//
		EXPECT_EQ(test.GetResponsesLimit(), Ntest::max_responses);
		std::filesystem::remove("config.json");
	}
	TEST(ConverterJSON, GetRequests){
		//TEST	Ntest::lines
		//		Ntest::words
		//		Ntest::symbols
		std::cout << FIXED << Ntest::lines << " lines, " << Ntest::words << " words, " << Ntest::symbols << " symbols!!!\n";
		std::vector<std::string> result;
		std::fstream FileCreation;
		FileCreation.open("requests.json", std::fstream::out);
		FileCreation << "{\n\t\"requests\": [";
		char letter;
		std::string cache;
		for(size_t l0 = 0, l1, l2; l0 < Ntest::lines; ++l0){
			if(l0 && l0 != Ntest::lines){ FileCreation << ","; }
			FileCreation << "\n\t\t\"";
			cache = "";
			for(l1 = 0; l1 < Ntest::words; ++l1){
				if(l1 && l1 != Ntest::words){ cache += " "; }
				for(l2 = 0; l2 < Ntest::symbols; ++l2){
					do{
						letter = (rand() % 256) - 128;
					}while(letter < 'A' || letter > 'Z' && letter < 'a' || letter > 'z');
					cache += letter;
				}
			}
			FileCreation << cache + "\"";
			result.push_back(cache);
		}
		FileCreation << "\n\t]\n}";
		FileCreation.close();
		ConverterJSON test;
		//
		EXPECT_EQ(test.GetRequests(), result);
		std::filesystem::remove("requests.json");
	}
	TEST(ConverterJSON, putAnswers_GLOBAL){
		//TEST	Ntest::Vwords
		//		Ntest::files
		//		Ntest::requestsSize
		//		Ntest::max_responses
		//GLOBAL	NstreamOperator::PrintJSON
		std::cout << DEPLOYMENT << FIXED << "Vwords (";
		for(size_t i = 0; Ntest::Vwords[i][0];){
			std::cout << Ntest::Vwords[i];
			if(Ntest::Vwords[++i][0]){ std::cout << " "; }
		}
		std::cout << "), " << Ntest::requestsSize << " lines(requests), " << Ntest::files << " files, " << Ntest::max_responses << " max_responses!!!\n";
		std::vector<std::string> files;
		Ntest::mixingVectorString(files, Ntest::files);
		InvertedIndex invertedIndex(files);
		std::vector<std::string> list;
		Ntest::mixingWords(list);
		unsigned short sizeList = list.size();
		std::vector<SearchServer> searchServer(sizeList);
		manager(searchServer, list, sizeList, invertedIndex());
		std::fstream FileCreation;
		FileCreation.open("config.json", std::fstream::out);
		FileCreation << "{\n\t\"config\": {\n\t\t\"name\": \"SearchEngine\",\n\t\t\"version\": \"0.1\",\n\t\t\"max_responses\": " + std::to_string(Ntest::max_responses) + "\n\t}\n}";
		FileCreation.close();
		ConverterJSON test;
		std::cout << DEP_OK;
		EXPECT_EQ(sizeList, Ntest::requestsSize);
		test.putAnswers(searchServer);
		std::filesystem::remove("config.json");
	}
}
namespace AuxiliaryResource{
	namespace InvertedIndexTEST{
		TEST(InvertedIndex, SET){
			//TEST	Ntest::Vwords
			//		Ntest::files
			//		Ntest::requestsSize
			std::cout << FIXED << "Vwords (";
			for(size_t i = 0; Ntest::Vwords[i][0]; ){
				std::cout << Ntest::Vwords[i];
				if(Ntest::Vwords[++i][0]){ std::cout << " "; }
			}
			std::cout << "), " << Ntest::requestsSize << " lines(requests), " << Ntest::files << " files!!!\n";
			std::vector<std::string> files;
			Ntest::mixingVectorString(files, Ntest::files);
			//
			InvertedIndex invertedIndex(files);
		}
		TEST(InvertedIndex, GET){
			//TEST	Ntest::Vwords
			//		Ntest::files
			//		Ntest::requestsSize
			std::cout << FIXED << "Vwords (";
			for(size_t i = 0; Ntest::Vwords[i][0]; ){
				std::cout << Ntest::Vwords[i];
				if(Ntest::Vwords[++i][0]){ std::cout << " "; }
			}
			std::cout << "), " << Ntest::requestsSize << " lines(requests), " << Ntest::files << " files!!!\n";
			std::vector<std::string> files;
			Ntest::mixingVectorString(files, Ntest::files);
			InvertedIndex invertedIndex(files);
			//
			std::map<std::string, std::vector<Index>> index = invertedIndex();
		}
	}
	namespace SearchServerTEST{
		TEST(SearchServer, SET){
			//TEST	Ntest::Vwords
			//		Ntest::files
			//		Ntest::requestsSize
			//		Ntest::words
			std::cout << FIXED << "Vwords (";
			for(size_t i = 0; Ntest::Vwords[i][0]; ){
				std::cout << Ntest::Vwords[i];
				if(Ntest::Vwords[++i][0]){ std::cout << " "; }
			}
			std::cout << "), " << Ntest::requestsSize << " lines(requests), " << Ntest::files << " files!!!\n";
			std::vector<std::string> files;
			Ntest::mixingVectorString(files, Ntest::files);
			InvertedIndex invertedIndex(files);
			std::vector<std::string> list;
			Ntest::mixingWords(list);
			auto sizeList = list.size();
			//
			std::vector<SearchServer> searchServer(sizeList);
			manager(searchServer, list, sizeList, invertedIndex());
		}
	}
}
namespace Initialization{
	TEST(initialization, TEST_error_0){
		std::filesystem::remove("config.json");
		Modules root;
		std::string error;
		EXPECT_EQ(root.initialization(error), true);
		EXPECT_EQ(error, "config file is missing\n");
	}
	TEST(initialization, TEST_error_1){
		Modules root;
		std::string error;
		std::fstream FileCreation;
		FileCreation.open("config.json", std::fstream::out);
		FileCreation << "{}";
		FileCreation.close();
		EXPECT_EQ(root.initialization(error), true);
		EXPECT_EQ(error, "config file is empty\n");
		std::filesystem::remove("config.json");
	}
	TEST(initialization, TEST_return_0){
		Modules root;
		std::string error;
		std::fstream FileCreation;
		FileCreation.open("config.json", std::fstream::out);
		FileCreation << "{\n\t\"config\": {\n\t\t\"name\": \"SearchEngine\",\n\t\t\"version\": \"0.1\",\n\t\t\"max_responses\": 5\n\t}\n}";
		FileCreation.close();
		EXPECT_EQ(root.initialization(error), false);
		EXPECT_EQ(error, "");
		std::filesystem::remove("config.json");
	}
}