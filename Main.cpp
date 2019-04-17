#include "GapBuffer.h"
#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <algorithm>

template<typename TimeT = std::chrono::nanoseconds>
struct measure {
	template<typename F, typename ...Args>
	static auto duration(F&& func, Args&& ... args) {
		auto start = std::chrono::steady_clock::now();
		std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
		return std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - start);
	}

	template<typename F, typename ...Args>
	static typename TimeT::rep execution(F && func, Args && ... args) {
		auto start = std::chrono::steady_clock::now();
		std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
		auto duration = std::chrono::duration_cast<TimeT>
			(std::chrono::steady_clock::now() - start);
		return duration.count();
	}
};


void test1(std::vector<std::string>& v) {
	GapBuffer gb;
	gb.insert(v[0]);
	auto pos = 0;
	for (int i = 1; i < v.size(); ++i) {
		if (i % 1000 == 0) {
			pos += 1000;
			gb.moveCursor(pos);
		}
		gb.insert(v[i]);
	}
}
void test2(std::vector<std::string>& v) {
	std::string str = v[0];
	auto pos = 0;
	for (int i = 1; i < v.size(); ++i) {
		if (i % 1000 == 0)
			pos += 1000;
		str.insert(pos+(i % 1000) * v[i].size(), v[i]);
	}
}

std::string random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

int main() {

	GapBuffer gb;
	gb.insert("Hola mundo");
	gb.insertAt(5, "hermoso ");
	gb.insertAt(13, "y genial ");
	gb.insertAt(gb.size(), "!");

	std::cout << gb.getStr() << '\n';

	const int N = 1000, step = 100;
	for (int i = 0; i < N; ++i) {
		std::vector<std::string> v((i+1)*step);
		for (int k = 0; k < v.size(); ++k)
			v[k] = random_string(10);

		int t1, t2;
		t1 = ((measure<>::duration(test1, v) + measure<>::duration(test1, v)) / 2).count() / 1000;
		t2 = ((measure<>::duration(test2, v) + measure<>::duration(test2, v)) / 2).count() / 1000;
		std::cout << (i + 1) * step <<' ' << t1 << ' ' << t2 << '\n';
	}
	return 0;
}