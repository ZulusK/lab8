//
// Created by zulus on 13.05.17.
//

#ifndef LAB8_COUNTRY_H
#define LAB8_COUNTRY_H

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
            const std::string &citizen, int id = 0);

    ~Country();

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

    static Country *load(json_t *jobject);

    json_t *json();

    std::string toString();
};


#endif //LAB8_COUNTRY_H
