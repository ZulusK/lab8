//
// Created by zulus on 13.05.17.
//

#include <Country.h>

using namespace std;

Country::Country(const std::string &name, const std::string &officialName, const std::string &abbr,
                 const std::string &citizen, const std::string &color, int id) {
    jsn = json_object();
    setName(name);
    setOfficialName(officialName);
    setAbbr(abbr);
    setCitizen(citizen);
    setId(id);
    setColor(color);
}

const std::string &Country::getName() const {
    return name;
}

void Country::setName(const std::string &name) {
    Country::name = name;
    auto jrm = json_object_get(jsn, "name");
    if (!json_is_null(jrm))
        json_decref(jrm);
    json_object_set_new(jsn, "name", json_string(name.c_str()));
}

const std::string &Country::getOfficialName() const {
    return officialName;
}

void Country::setOfficialName(const std::string &officialName) {
    Country::officialName = officialName;
    auto jrm = json_object_get(jsn, "officialName");
    if (!json_is_null(jrm))
        json_decref(jrm);
    json_object_set_new(jsn, "officialName", json_string(officialName.c_str()));
}

const std::string &Country::getAbbr() const {
    return abbr;
}

void Country::setAbbr(const std::string &abbr) {
    Country::abbr = abbr;
    auto jrm = json_object_get(jsn, "abbr");
    if (!json_is_null(jrm))
        json_decref(jrm);
    json_object_set_new(jsn, "abbr", json_string(abbr.c_str()));
}

const std::string &Country::getCitizen() const {
    return citizen;
}

void Country::setCitizen(const std::string &citizen) {
    Country::citizen = citizen;
    auto jrm = json_object_get(jsn, "citizen");
    if (!json_is_null(jrm))
        json_decref(jrm);
    json_object_set_new(jsn, "citizen", json_string(citizen.c_str()));
}

int Country::getId() const {
    return id;
}

void Country::setId(int id) {
    Country::id = id;
    if (id > 0) {
        auto jrm = json_object_get(jsn, "id");
        if (!json_is_null(jrm))
            json_decref(jrm);
        json_object_set_new(jsn, "id", json_integer(id));
    }
}

json_t *Country::json() {
    auto jclone = json_copy(jsn);
    return jclone;
}

Country *Country::load(json_t *jobject) {
    Country *c = NULL;
    int id = atoi(json_string_value(json_object_get(jobject, "entry-number")));
    string color = json_string_value(json_object_get(jobject, "color"));
    auto jitem = json_array_get(json_object_get(jobject, "item"), 0);
    if (jitem) {
        string name(json_string_value(json_object_get(jitem, "name")));
        string offName(json_string_value(json_object_get(jitem, "official-name")));
        string citizen(json_string_value(json_object_get(jitem, "citizen-names")));
        string abbr(json_string_value(json_object_get(jitem, "country")));
        c = new Country(name, offName, abbr, citizen, color, id);
    }
    return c;
}

const string &Country::getColor() const {
    return color;
}

void Country::setColor(const string &color) {
    auto jrm = json_object_get(jsn, "color");
    if (!json_is_null(jrm))
        json_decref(jrm);
    json_object_set_new(jsn, "color", json_string(color.c_str()));
    Country::color = color;
}
