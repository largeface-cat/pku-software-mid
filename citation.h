#pragma once
#ifndef CITATION_H
#define CITATION_H

#include <string>
#include <utility>

class Citation {
private:
    int type;
    std::string id{""};
public:
    Citation(const int &t, std::string &id) : type(t), id(id) {}
    std::string getId() { return id; }
};

class BookCitation : public Citation {
private:
    std::string isbn{""}, author{""}, title{""}, press{""}, year{""};
public:
    BookCitation(const int &t, std::string &id) : Citation(t, id) {};
};

class WebCitation : public Citation {
private:
    std::string title{""}, url{""};
public:
    WebCitation(const int &t, std::string &id) : Citation(t, id) {};
};

class ArticleCitation : public Citation {
private:
    std::string author{""}, title{""}, paper{""}, year{""}, vol{""}, no{""};
public:
    ArticleCitation(const int &t, std::string &id) : Citation(t, id) {};
};
#endif