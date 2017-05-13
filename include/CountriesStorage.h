/** @file
    @brief Storage of Counties-objects
*/


#ifndef LAB8_COUNRIESSTORAGE_H
#define LAB8_COUNRIESSTORAGE_H

#include <map>
#include <Country.h>

/**
    @class CounriesStorage
    @brief defines Country data storage
 */
class CountriesStorage {
private:
    std::map<int, Country *> items;
    int id = 1;

    /**
    @brief default private constructor of CountriesStorage
    */
    CountriesStorage();

private:
    /**
     @brief add new country to data storage
     @param country refernce to added country
     */
    void add(Country *country);

    /**
    @brief parse JSON-string and add parsed country to data storage
    @param jstr JSON-string, that contains country's data
    */
    void add(std::string &jstr);

    /**
     @brief returns all countries Which contain such a key and value
     @param isValid callback function, for filtering all countries
     @param key value to search
     @return JSON-string presentation of each county
     */
    std::string get(bool (*isValid)(Country *, void *), const std::string &key);

public:
    /**
     @brief parse JSON-string and fill CountryStorage by parsed data
     @param filename name of file with JSON-objects
     @return created CountriesStorage
     */
    static CountriesStorage *load(const std::string &filename);

    /**
     @brief returns all countries that satisfy the
     @param key name of field to filter
     @param value value to filter
     @return JSON-string presentation of each country
     */
    std::string get(const std::string &key, const std::string &value);

    /**
     @brief get count of counties at storage
     @return count of countries
     */
    int size();

    /**
     return country by its id
     @param id id of object to search
     @return JSON-string presentation of each county
     */
    std::string get(int id);

    /**
    @brief destructor of CountriesStorage
    */
    virtual ~CountriesStorage();

};


#endif //LAB8_COUNRIESSTORAGE_H
