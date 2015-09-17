#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Log {
	public:
		static void out(int i, std::string s) {
			cout << i << ": " << s;
		};

		static void out(std::string s) {
			cout << "X: " << s << std::endl;
		};

		static void outln(int i, std::string s) {
			cout << i << ": " << s << std::endl;
		};

		static void outln(std::string s) {
			cout << "X: " << s << std::endl;
		};

		static void fileOut(string name, string data) {
			ofstream myfile;
			myfile.open ("src/Web/json/"+name);
			myfile << data;
			myfile.close();
		}
};

#endif
