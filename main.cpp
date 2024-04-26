#include <fstream>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "citation.h"

std::vector<Citation*> loadCitations(const std::string& filename) {
    // FIXME: load citations from file
}

int main(int argc, char** argv) {
    // "docman", "-c", "citations.json", "input.txt"

    auto citations = loadCitations(argv[2]);
    std::vector<Citation*> printedCitations{};

    // FIXME: read all input to the string, and process citations in the input text
    // auto input = readFromFile(argv[3]);
    // ...

    std::ostream& output = std::cout;

    // output << input;  // print the paragraph first
    // output << "\nReferences:\n";
    
    for (auto c : printedCitations) {
        // FIXME: print citation
    }

    for (auto c : citations) {
        delete c;
    }
}
