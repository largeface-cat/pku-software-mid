#include <fstream>
#include <iostream>
#include <vector>
#include <exception>
#include "utils.hpp"
#include "citation.h"

#include "third_parties/nlohmann/json.hpp"
using namespace std::literals;
std::vector<Citation*> loadCitations(const std::string& filename)
{
    // FIXME: load citations from file
    std::vector<Citation*> vec_citations;
    std::ifstream file{filename};
    nlohmann::json data = nlohmann::json::parse(file);
    for (auto& item : data["citations"])
    {
        int type;
        Citation* citation;
        std::string id = item["id"].get<std::string>();
        type = (item["type"].get<std::string>() == "book") +
            2 * (item["type"].get<std::string>() == "webpage") +
            3 * (item["type"].get<std::string>() == "article");
        switch (type)
        {
        default:
            std::exit(1);
        case 1:
            citation = new BookCitation{1, id};
        case 2:
            citation = new WebCitation{2, id};
        case 3:
            citation = new ArticleCitation{3, id};
        }
        vec_citations.push_back(citation);
    }
    return vec_citations;
}


std::string readFromFile(const std::string& filename)
{
    std::string content;
    std::string line;
    if (filename == "-")
    {
        std::cout << "Enter the text:\n";
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
class Output
{
private:
    std::ostream* output;
public:
    Output(std::ostream* output) : output{output} {}
    template <typename T>
    Output& operator<<(const T& t)
    {
        *output << t;
        return *this;
    }
    Output& operator=(std::ofstream* file)
    {
        output = file;
        return *this;
    }
};
int main(int argc, char** argv)
{
    // "docman", "-c", "citations.json", "input.txt"
    if (argc < 4 or argv[1] != std::string{"-c"})
    {
        std::cerr << "Invalid arguments" << std::endl;
        std::exit(1);
    }
    std::vector<Citation*> citations{};
    try
    {
        citations = loadCitations(argv[2]);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }

    std::vector<Citation*> printedCitations{};

    // FIXME: read all input to the string, and process citations in the input text
    std::string input{};


    // ...
    Output out {&std::cout};
    if (argv[3] == "-o"s)
    {
        if (argc != 6)
        {
            std::cerr << "Invalid arguments" << std::endl;
            std::exit(1);
        }
        input = readFromFile(argv[5]);
        auto os = std::ofstream{argv[4]};
        out = &os;
    }
    else
    {
        input = readFromFile(argv[3]);
    }

    out << input; // print the paragraph first
    out << "\nReferences:\n";

    for (auto c : printedCitations)
    {
        // FIXME: print citation
    }

    for (auto c : citations)
    {
        delete c;
    }
}
