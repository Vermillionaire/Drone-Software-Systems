#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>

class Log {
	public:
		static void out(int i, std::string s) {
			std::cout << i << ": " << s;
		};

		static void out(std::string s) {
			std::cout << "X: " << s << std::endl;
		};

		static void outln(int i, std::string s) {
			std::cout << i << ": " << s << std::endl;
		};

		static void outln(std::string s) {
			std::cout << "X: " << s << std::endl;
		};
};

#endif