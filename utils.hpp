#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>
#include <fstream>

const std::string API_ENDPOINT{"http://docman.lcpu.dev"};

inline std::string encodeUriComponent(const std::string& s) {
    std::string encoded;
    char c;
    for (size_t i = 0; i < s.length(); i++) {
        c = s[i];
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded += c;
        } else if (c == ' ') {
            encoded += '+';
        } else {
            encoded += '%';
            // convert to hex
            std::stringstream ss;
            ss << std::hex << (int) c;
            encoded += ss.str();
        }
    }
    return encoded;
}

inline std::string readFromFile(const std::string& filename)
{
    std::string content, line;
    if (filename == "-")
    {
        while (std::getline(std::cin, line))
        {
            content += line + "\n";
        }
        return content;
    }
    std::ifstream file{filename};
    while (std::getline(file, line))
    {
        content += line + "\n";
    }
    return content;
}

#endif 
