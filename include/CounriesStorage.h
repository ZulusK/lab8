//
// Created by zulus on 12.05.17.
//

#ifndef LAB8_COUNRIESSTORAGE_H
#define LAB8_COUNRIESSTORAGE_H

#include <map>
#include <Country.h>

class CounriesStorage {
private:
    std::map<int, Country *> items;
    int id = 1;

    CounriesStorage();

public:
    virtual ~CounriesStorage();

private:
    void add(Country *country);

    void add(std::string &jstr);

    std::string get(bool (*isValid)(Country *, void *), const std::string &key);

public:
    static CounriesStorage *load(const std::string &filename);

    std::string get(const std::string &key, const std::string &value);


    int size();
};


#endif //LAB8_COUNRIESSTORAGE_H
