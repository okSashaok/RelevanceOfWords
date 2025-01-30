#include<iostream>
#include<string>
#include"project/core.h"
int main(){
	Modules root;
	std::string error;
	if(root.initialization(error)){
		std::cout<<error;
		return 0;
	}
	return root.main();
}
