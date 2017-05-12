//
// Created by zulus on 12.05.17.
//

#ifndef LAB8_COUNRIESSTORAGE_H
#define LAB8_COUNRIESSTORAGE_H

#include <vector>
#include <jansson.h>
#include <string>

class Country {
    std::string name;
    std::string officialName;
    std::string abbr;
    std::string citizen;
    int id;
    json_t *jsn;
public:
    Country(const std::string &name, const std::string &officialName, const std::string &abbr,
            const std::string &citizen, int id);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getOfficialName() const;

    void setOfficialName(const std::string &officialName);

    const std::string &getAbbr() const;

    void setAbbr(const std::string &abbr);

    const std::string &getCitizen() const;

    void setCitizen(const std::string &citizen);

    int getId() const;

    void setId(int id);

    Country *load(json_t *jobject);

    json_t *json();
};

class CounriesStorage {
    std::vector<Country *> countries;
};


#endif //LAB8_COUNRIESSTORAGE_H
