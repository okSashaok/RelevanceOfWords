#pragma once
#include"function/converter_JSON.h"
class Modules{
	private:
	ConverterJSON JSON;
	public:
	int main();
	bool initialization(std::string& error);
	Modules()=default;
	~Modules()=default;
};