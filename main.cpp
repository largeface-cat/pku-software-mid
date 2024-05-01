#define EXIT std::exit(1);
#define TEST(expr) try{expr;} catch (std::exception& e) {EXIT}

#include <iostream>
#include <vector>
#include "third_parties/nlohmann/json.hpp"
#include "utils.hpp"
#include "citation.h"

using namespace std::literals;

std::vector<Citation*> loadCitations(const std::string& filename);
std::vector<Citation*> parseCitations(std::string& input, std::vector<Citation*>& citations);

int main(int argc, char** argv)
{
    if ((argc != 4 and argc != 6) or argv[1] != std::string{"-c"}
        or (argv[3] == "-o"s and argc != 6)
        or (argv[3] != "-o"s and argc != 4))
        EXIT

    std::vector<Citation*> citations{};
    TEST(citations = loadCitations(argv[2]);)

    std::vector<Citation*> printedCitations{};

    std::string input{};

    if (argv[3] == "-o"s) input = readFromFile(argv[5]);
    else input = readFromFile(argv[3]);
    if (input.empty())
        EXIT

    printedCitations = parseCitations(input, citations);

    if (argv[3] == "-o"s) freopen(argv[4], "w", stdout);

    std::cout << input;
    std::cout << "\nReferences:\n";

    for (auto c : printedCitations)
    {
        std::cout << "[" + c->getId() + "] " + c->info() + "\n";
    }

    for (auto c : citations)
    {
        delete c;
    }
    return 0;
}




std::vector<Citation*> loadCitations(const std::string& filename)
{
    std::vector<Citation*> vec_citations;
    Citation* citation;
    std::ifstream file{filename};
    int type;

    auto data = nlohmann::json::parse(file);
    TEST(if (data["citations"].empty()) {EXIT})

    for (auto& item : data["citations"])
    {
        auto id = item["id"].get<std::string>();
        type = (item["type"].get<std::string>() == "book") +
            2 * (item["type"].get<std::string>() == "webpage") +
            3 * (item["type"].get<std::string>() == "article");
        switch (type)
        {
        default:
            EXIT
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
        TEST(citation->fill();)
        vec_citations.push_back(citation);
    }
    return vec_citations;
}


std::vector<Citation*> parseCitations(std::string& input, std::vector<Citation*>& citations)
{
    int flag = 0;
    auto id = ""s;
    std::set<std::string> idInput{};
    std::vector<Citation*> parsedCitations{};
    for (const auto s : input)
    {
        if (s == '[')
        {
            flag++;
            continue;
        }
        if (s == ']')
        {
            flag--;
            idInput.insert(id);
            id = ""s;
        }
        if (flag == 1)
        {
            id += s;
        }
        if (flag != 0 and flag != 1)
            EXIT
    }

    int found_cnt = 0;
    std::sort(citations.begin(), citations.end(),
              [](Citation* a, Citation* b) { return a->getId() < b->getId(); });
    for (auto c : citations)
    {
        if (idInput.find(c->getId()) != idInput.end())
        {
            parsedCitations.push_back(c);
            found_cnt += 1;
        }
    }
    if (found_cnt != idInput.size() or flag != 0)
        EXIT
    return parsedCitations;
}
