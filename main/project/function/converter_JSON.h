#pragma once
#include<string>
#include<vector>
#include"auxiliary_resource/search_server.h"
class ConverterJSON{
	private:
	public:
	ConverterJSON()=default;
	std::vector<std::string> GetTextDocuments();
	unsigned short GetResponsesLimit();
	std::vector<std::string> GetRequests();
	void putAnswers(std::vector<SearchServer>& relevance);
};