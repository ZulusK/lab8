//
// Created by zulus on 12.05.17.
//

#include <string>
#include <CountriesStorage.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <TextProcessor.h>

using namespace std;

CountriesStorage::CountriesStorage() {}

string Country::toString() {
    char *jsonString = json_dumps(jsn, JSON_INDENT(2));
    string s(jsonString);
    free(jsonString);
    return s;
}

Country::~Country() {
    json_decref(jsn);
}

CountriesStorage *CountriesStorage::load(const std::string &filename) {
    CountriesStorage *storage = new CountriesStorage();
    string str = TextProcessor::read(filename);
    storage->add(str);
    return storage;
}

void CountriesStorage::add(Country *country) {
    if (country->getId() == 0) {
        country->setId(id++);
    }
    this->items[country->getId()] = country;
}

void CountriesStorage::add(std::string &jstr) {
    json_error_t err;
    //get array
    auto jDoc = json_loads(jstr.c_str(), 0, &err);
    const char *key;
    json_t *value;
    //for each object in document, parse and add it to map
    json_object_foreach(jDoc, key, value) {
        add(Country::load(value));
    }
    json_decref(jDoc);
}

bool nameFilter(Country *c, void *key) {
    return c->getName().compare(*(string *) key) == 0;
}

bool officialNameFilter(Country *c, void *key) {
    return c->getOfficialName().compare(*(string *) key) == 0;
}

bool abbrFilter(Country *c, void *key) {
    return c->getAbbr().compare(*(string *) key) == 0;
}

bool colorFilter(Country *c, void *key) {
    return c->getColor().compare(*(string *) key) == 0;
}

bool idFilter(Country *c, void *key) {
    return c->getId() == stoi(*(string *) key);
}

bool positiveFilter(Country *c, void *key) {
    return true;
}

string CountriesStorage::get(bool(*isValid)(Country *c, void *key), const string &key) {
    auto jarr = json_array();
    //get elements by filter
    //convert them to json string
    for (auto it = items.begin(); it != items.end(); it++) {
        if (isValid(it->second, (void *) &key)) {
            json_array_append_new(jarr, it->second->json());
        }
    }
    // create JSON document string
    char *jsonString = json_dumps(jarr, JSON_INDENT(2));
    string jstr(jsonString);
    free(jsonString);
    json_decref(jarr);
    return jstr;
}

string CountriesStorage::get(const string &key, const string &value) {
    if (key.compare("name") == 0) {
        return get(nameFilter, value);
    } else if (key.compare("officialName") == 0) {
        return get(officialNameFilter, value);
    } else if (key.compare("abbr") == 0) {
        return get(abbrFilter, value);
    } else if (key.compare("id") == 0) {
        return get(idFilter, value);
    } else if (key.compare("color") == 0) {
        return get(colorFilter, value);
    } else {
        return get(positiveFilter, value);
    }
}

CountriesStorage::~CountriesStorage() {
    for (auto it = items.begin(); it != items.end(); it++) {
        delete it->second;
    }
    items.clear();
}

int CountriesStorage::size() {
    return items.size();
}

string CountriesStorage::get(int id) {
    return get(idFilter, to_string(id));
}