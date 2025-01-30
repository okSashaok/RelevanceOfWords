#include<string>
#include"print_JSON.h"
void PrintJSON::tab(){
	for(unsigned char i = 0; i < layer; ++i){ GET += "\t"; }
}
void PrintJSON::insert(){
	if(pair){ GET += ",\n"; }
	else{
		if(layer++){ GET += ": "; }
		GET += "{\n";
	}
	tab();
}
PrintJSON::PrintJSON(std::string& operGET)
: GET(operGET){};
void PrintJSON::operator()(const char* kay){
	insert();
	GET += (std::string)"\"" + kay + "\"";
	pair = 0;
}
void PrintJSON::operator()(const char* kay, const char* comment){
	insert();
	GET += (std::string)"\"" + kay + "\": \"" + comment + "\"";
	pair = 1;
}
void PrintJSON::operator()(const char* kay, float comment, unsigned char accuracy = 8){
	insert();
	std::string cache = std::to_string(comment)
	,commentSET;
	for(unsigned char i = 0; i < accuracy; ++i){ commentSET += cache[i]; }
	GET += (std::string)"\"" + kay + "\": " + commentSET.c_str();
	pair = 1;
}
void PrintJSON::operator--(){
	if(pair && layer){
		GET += "\n";
		--layer;
		tab();
		GET += "}";
	}
}
void PrintJSON::operator--(int){
	while(layer){ operator--(); }
}