//
// Created by zulus on 12.05.17.
//

#include "CounriesStorage.h"

Country::Country(const std::string &name, const std::string &officialName, const std::string &abbr,
                 const std::string &citizen, int id) {
    jsn=json_object();
    setName(name);
    setOfficialName(officialName);
    setAbbr(abbr);
    setCitizen(citizen);
    setId(id);
}

const std::string &Country::getName() const {
    return name;
}

void Country::setName(const std::string &name) {
    Country::name = name;
}

const std::string &Country::getOfficialName() const {
    return officialName;
}

void Country::setOfficialName(const std::string &officialName) {
    Country::officialName = officialName;
}

const std::string &Country::getAbbr() const {
    return abbr;
}

void Country::setAbbr(const std::string &abbr) {
    Country::abbr = abbr;
}

const std::string &Country::getCitizen() const {
    return citizen;
}

void Country::setCitizen(const std::string &citizen) {
    Country::citizen = citizen;
}

int Country::getId() const {
    return id;
}

void Country::setId(int id) {
    Country::id = id;
}


