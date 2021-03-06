#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

// include this before spdlog to prevent issues with stb
#include <emmintrin.h>

// logging
#include "log.hpp"

using uint = unsigned int;

// file operations
static std::string parse(const std::string& path)
{
	std::ifstream infile(path);
	std::string line;
	std::stringstream ss;

	while (getline(infile, line))
	ss << line << '\n';

	return ss.str();
}

inline std::string directory(const std::string& path)
{
	return path.substr(0, path.find_last_of('\\'));
}

inline std::string path(const std::string& directory, const std::string& relative_path)
{
	return std::string(directory + "\\" + relative_path);
}
