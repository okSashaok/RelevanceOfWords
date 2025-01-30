#pragma once
#include<vector>
#include<thread>
#include<mutex>
inline void manager(auto& object, auto& first, auto& size){
	auto THC = std::thread::hardware_concurrency();
	if(THC > size){THC = size;}
	auto CONST = size / THC;
	std::vector<std::thread> flows;
	size_t i
	, flow;
	if(THC == 1){
		for(i = 0; i < size; ++i){
			object[i] = first[i];
		}
	}
	else{
		std::mutex M;
		for(i = 0, flow = 0; i < THC; ++i){
			flows.push_back(std::thread([&](){
				M.lock();
				auto min = (++flow) * CONST - CONST
				,max = (flow == THC ? size : (flow) * CONST);
				M.unlock();
				for(; min < max; ++min){
					object[min] = first[min];
				}
			}));
		}
		for(i = 0; i < THC; ++i){ flows[i].join(); }
	}
}
inline void manager(auto& object, auto& first, auto& size, auto& second){
	auto THC = std::thread::hardware_concurrency();
	if(THC > size){THC = size;}
	auto CONST = size / THC;
	std::vector<std::thread> flows;
	size_t i
	, flow;
	if(THC == 1){
		for(i = 0; i < size; ++i){
			object[i] = first[i];
			object[i](second);
		}
	}
	else{
		std::mutex M;
		for(i = 0, flow = 0; i < THC; ++i){
			flows.push_back(std::thread([&](){
				M.lock();
				auto min = (++flow) * CONST - CONST
				,max = (flow == THC ? size : (flow) * CONST);
				M.unlock();
				for(; min < max; ++min){
					object[min] = first[min];
					object[min](second);
				}
			}));
		}
		for(i = 0; i < THC; ++i){ flows[i].join(); }
	}
}