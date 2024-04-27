#include <fstream>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "citation.h"

#include "third_parties/nlohmann/json.hpp"

std::vector<Citation *> loadCitations(const std::string &filename) {
    // FIXME: load citations from file
    std::vector<Citation *> vec_citations;
    std::ifstream file{filename};
    nlohmann::json data = nlohmann::json::parse(file);
    for (auto &item: data["citations"]) {
        int type;
        Citation *citation;
        std::string id = item["id"].get<std::string>();
        type = (item["type"].get<std::string>() == "book") +
               2 * (item["type"].get<std::string>() == "webpage") +
               3 * (item["type"].get<std::string>() == "article");
        switch (type) {
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

int main(int argc, char **argv) {
    // "docman", "-c", "citations.json", "input.txt"

    auto citations = loadCitations(argv[2]);
    for (auto c:citations) {
        std::cout<<c->getId();
    }
    std::vector<Citation *> printedCitations{};

    // FIXME: read all input to the string, and process citations in the input text
//     auto input = readFromFile(argv[3]);
    // ...

    std::ostream &output = std::cout;

    // output << input;  // print the paragraph first
    // output << "\nReferences:\n";

    for (auto c: printedCitations) {
        // FIXME: print citation
    }

    for (auto c: citations) {
        delete c;
    }
}
