#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>


using namespace std;
class FileReader {
public:
	static std::string ReadFromFile(char* filename) {
		std::string code;
		std::ifstream file;
		file.exceptions(std::ifstream::badbit); // enable exceptions on this file
		try
		{
			file.open(filename);
			std::stringstream fileStream;
			fileStream << file.rdbuf();
			file.close();
			code = fileStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "Failed to load file!" << std::endl;
			return nullptr;
		}
		return code;
	}

};