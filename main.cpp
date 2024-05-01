#include <fstream>
#include <iostream>
#include <vector>
#include <exception>
#include "citation.h"

#include "third_parties/nlohmann/json.hpp"
using namespace std::literals;

std::vector<Citation*> loadCitations(const std::string& filename)
{
    // FIXME: load citations from file
    std::vector<Citation*> vec_citations;
    std::ifstream file{filename};

    nlohmann::json data = nlohmann::json::parse(file);
    std::cout<<data<<std::endl;
    // std::cout << data << std::endl;
    try
    {
        if (data["citations"].empty())
        {
            std::exit(1);
        }
    }
    catch (std::exception& e)
    {
        std::exit(1);
    }
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
            citation = new BookCitation{1, id, item["isbn"].get<std::string>()};
            break;
        case 2:
            citation = new WebCitation{2, id, item["url"].get<std::string>()};
            break;
        case 3:
            citation = new ArticleCitation{3, id, item};
            break;
        }
        try
        {
            citation->fill();
        }
        catch (std::exception& e)
        {
            std::exit(1);
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
        // std::cout << "Enter the text:\n";
        while (std::getline(std::cin, line)) // todo check if it works
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


std::vector<Citation*> parseCitations(std::string& input, std::vector<Citation*>& citations)
{
    int flag = 0;
    auto id = ""s;
    std::set<std::string> idInput{};
    std::vector<Citation*> parsedCitations{};
    for (auto s : input)
    {
        if (s == '[')
        {
            flag += 1;
            continue;
        }
        if (s == ']')
        {
            flag -= 1;
            idInput.insert(id);
            id = ""s;
        }
        if (flag == 1)
        {
            id += s;
        }
        if (flag != 0 and flag != 1)
        {
            std::exit(1);
        }
    }
    int found = 0;
    std::sort(citations.begin(), citations.end(),
              [](Citation* a, Citation* b) { return a->getId() < b->getId(); });
    for (auto c : citations)
    {
        if (idInput.find(c->getId()) != idInput.end())
        {

            parsedCitations.push_back(c);
            found += 1;
        }
    }
    if (found != idInput.size())
    {
        std::exit(1);
    }
    if (flag != 0)
    {
        std::exit(1);
    }
    return parsedCitations;
}


int main(int argc, char** argv)
{
    // "docman", "-c", "citations.json", "input.txt"
    if ((argc != 4 and argc != 6) or argv[1] != std::string{"-c"} or (argv[3] == "-o"s and argc != 6) or (argv[3] !=
        "-o"s and argc != 4))
    {
        std::exit(1);
    }
    std::vector<Citation*> citations{};
    try
    {
        citations = loadCitations(argv[2]);
    }
    catch (std::exception& e)
    {
        std::exit(1);
    }

    std::vector<Citation*> printedCitations{};

    // FIXME: read all input to the string, and process citations in the input text
    std::string input{};


    // ...
    if (argv[3] == "-o"s) input = readFromFile(argv[5]);
    else input = readFromFile(argv[3]);
    printedCitations = parseCitations(input, citations);
    std::cout<<input<<std::endl;
    std::cout.flush();
    if (argv[3] == "-o"s)
    {
        freopen(argv[4], "w", stdout);
    }

    std::cout << input; // print the paragraph first
    std::cout << "\nReferences:\n";

    for (auto c : printedCitations)
    {
        // FIXME: print citation
        std::cout << "[" + c->getId() + "] " + c->info() << "\n";
    }

    for (auto c : citations)
    {
        delete c;
    }
}
