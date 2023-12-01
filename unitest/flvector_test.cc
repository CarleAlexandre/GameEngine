#include "test.hh"

void fltest(void) {
	fl::vec<std::string> vec1;

	vec1.push_back("caps");
	std::cout << "\ncapacity:" << vec1.capacity << "\npush_back(1) test:\n";

	std::string str[2] = {"lol", "caps"};
	vec1.push_back(str);
	std::cout << "\ncapacity:" << vec1.capacity << "\npush_back(N) test:\n";
	std::cout << vec1.size()<< " : size\n";
	for (unsigned long long i = 0; i < vec1.size(); i++) {
		std::cout << vec1[i] << '\n';
	}

	vec1.pop_back(1);
	std::cout << "\ncapacity:" << vec1.capacity << "\npop_back test:\n";
	std::cout << vec1.size()<< " : size\n";
	for (unsigned long long i = 0; i < vec1.size(); i++) {
		std::cout << vec1[i] << '\n';
	}

	vec1.pop_first(1);
	std::cout << "\ncapacity:" << vec1.capacity << "\npop_first test:\n";
	std::cout << vec1.size()<< " : size\n";
	for (unsigned long long i = 0; i < vec1.size(); i++) {
		std::cout << vec1[i] << '\n';
	}

	vec1 += "coucou";
	std::cout << "\ncapacity:" << vec1.capacity << "\n+=1 test:\n";
	std::cout << vec1.size()<< " : size\n";
	for (unsigned long long i = 0; i < vec1.size(); i++) {
		std::cout << vec1[i] << '\n';
	}

	vec1 += str;
	std::cout << "\ncapacity:" << vec1.capacity << "\n+=N test:\n";
	std::cout << vec1.size()<< " : size\n";
	for (unsigned long long i = 0; i < vec1.size(); i++) {
		std::cout << vec1[i] << '\n';
	}

	vec1 += str;
	std::cout << "\ncapacity:" << vec1.capacity << "\n+=N test:\n";
	std::cout << vec1.size()<< " : size\n";
	for (unsigned long long i = 0; i < vec1.size(); i++) {
		std::cout << vec1[i] << '\n';
	}

	vec1.erase(2);
	std::cout << "\ncapacity:" << vec1.capacity << "\n+=N test:\n";
	std::cout << vec1.size()<< " : size\n";
	for (unsigned long long i = 0; i < vec1.size(); i++) {
		std::cout << vec1[i] << '\n';
	}
}
