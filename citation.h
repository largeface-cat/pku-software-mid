#pragma once
#ifndef CITATION_H
#define CITATION_H
#include <cpp-httplib/httplib.h>

inline httplib::Client client{API_ENDPOINT};

class Citation
{
private:
    int type;
    std::string id;

public:
    Citation(const int& t, std::string& id) : type(t), id(id)
    {
    }

    std::string getId() { return id; }
    virtual void fill() = 0;
    virtual std::string info() = 0;
    virtual ~Citation() = default;
};

class BookCitation : public Citation
{
private:
    std::string isbn, author, title, publisher, year;

public:
    BookCitation(const int& t, std::string& id, std::string isbn)
        : Citation(t, id), isbn(std::move(isbn))
    {
    }

    void fill() override
    {
        auto isbn_response = client.Get("/isbn/" + encodeUriComponent(isbn));
        if (isbn_response->status != 200)
            EXIT
        nlohmann::json data = nlohmann::json::parse(isbn_response->body);
        author = data["author"].get<std::string>();
        title = data["title"].get<std::string>();
        publisher = data["publisher"].get<std::string>();
        year = data["year"].get<std::string>();
    }

    std::string info() override
    {
        return "book: " + author + ", " + title + ", " + publisher + ", " + year;
    }
};

class WebCitation : public Citation
{
private:
    std::string title, url;

public:
    WebCitation(const int& t, std::string& id, std::string url)
        : Citation(t, id), url(std::move(url))
    {
    }

    void fill() override
    {
        auto web_response = client.Get("/title/" + encodeUriComponent(url));
        if (web_response->status != 200)
            EXIT
        nlohmann::json data = nlohmann::json::parse(web_response->body);
        title = data["title"].get<std::string>();
    };

    std::string info() override
    {
        return "webpage: " + title + ". Available at " + url;
    }
};

class ArticleCitation : public Citation
{
private:
    std::string author, title, journal, year, volume, issue;
    nlohmann::json data;

public:
    ArticleCitation(const int& t, std::string& id, nlohmann::json& data)
        : Citation(t, id), data(data)
    {
    }

    void fill() override
    {
        author = data["author"].get<std::string>();
        title = data["title"].get<std::string>();
        journal = data["journal"].get<std::string>();
        year = std::to_string(data["year"].get<int>());
        volume = std::to_string(data["volume"].get<int>());
        issue = std::to_string(data["issue"].get<int>());
    };

    std::string info() override
    {
        return "article: " + author + ", " + title + ", "
            + journal + ", " + year + ", " + volume + ", " + issue;
    }
};
#endif
