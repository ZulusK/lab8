//
// Created by zulus on 12.05.17.
//

#include <string>
#include <CounriesStorage.h>
#include <fstream>
#include <iostream>

using namespace std;

Country::Country(const std::string &name, const std::string &officialName, const std::string &abbr,
                 const std::string &citizen, int id) {
    jsn = json_object();
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
    json_object_set(jsn, "name", json_string(name.c_str()));
}

const std::string &Country::getOfficialName() const {
    return officialName;
}

void Country::setOfficialName(const std::string &officialName) {
    Country::officialName = officialName;
    json_object_set(jsn, "officialName", json_string(officialName.c_str()));
}

const std::string &Country::getAbbr() const {
    return abbr;
}

void Country::setAbbr(const std::string &abbr) {
    Country::abbr = abbr;
    json_object_set(jsn, "abbr", json_string(abbr.c_str()));
}

const std::string &Country::getCitizen() const {
    return citizen;
}

void Country::setCitizen(const std::string &citizen) {
    Country::citizen = citizen;
    json_object_set(jsn, "citizen", json_string(citizen.c_str()));
}

int Country::getId() const {
    return id;
}

void Country::setId(int id) {
    Country::id = id;
    if (id != 0) {
        json_object_set(jsn, "id", json_integer(id));
    }
}

json_t *Country::json() {
    return jsn;
}

Country *Country::load(json_t *jobject) {
    Country *c = NULL;
    int id = json_integer_value(json_object_get(jobject, "entry-number"));
    auto jitem = json_array_get(json_object_get(jobject, "item"), 0);
    if (jitem) {
        string name = json_string_value(json_object_get(jobject, "name"));
        string offName = json_string_value(json_object_get(jobject, "official-name"));
        string citizen = json_string_value(json_object_get(jobject, "citizen-names"));
        string abbr = json_string_value(json_object_get(jobject, "country"));
        c = new Country(name, offName, abbr, citizen, id);
    }
    return c;
}

CounriesStorage::CounriesStorage() {}

CounriesStorage *CounriesStorage::load(const std::string &filename) {
    CounriesStorage *storage = new CounriesStorage();
    ifstream t(filename);
    cout<<filename<<endl;
    if (t.is_open()) {
        std::string str;
        t.seekg(0, std::ios::end);
        str.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());
        t.close();
        cout << str << endl;
    }
    return storage;
}

