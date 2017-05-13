/** @file
    @brief Country data type
*/

#ifndef LAB8_COUNTRY_H
#define LAB8_COUNTRY_H

#include <jansson.h>
#include <string>

/**
    @class Country
    @brief defines Country class
 */
class Country {
    std::string name;
    std::string color;
    std::string officialName;
    std::string abbr;
    std::string citizen;
    int id;
    json_t *jsn;
public:
    /**
     @brief constructor of Country class
     @param name title of Country
     @param officialName official name of Country
     @param abbr short name of Country
     @param citizen name if citizen of Country
     @param color color of Country
     @param id unique id of Country
     */
    Country(const std::string &name, const std::string &officialName, const std::string &abbr,
            const std::string &citizen, const std::string &color, int id = 0);

    /**
     @brief destructor of Country class
     */
    ~Country();

    /**
     @brief get title of Country
     @return tittle of Country
     */
    const std::string &getName() const;

    /**
     @brief set title of Country
     @param name new title's value
     */
    void setName(const std::string &name);


    /**
     @brief get official name of Country
     @return official name of Country
     */
    const std::string &getOfficialName() const;

    /**
     @brief set official name of Country
     @param officialName new official name's value
     */
    void setOfficialName(const std::string &officialName);


    /**
     @brief get color of Country
     @return color of Country
     */
    const std::string &getColor() const;

    /**
     @brief set color of Country
     @param abbr  new color's value
     */
    void setColor(const std::string &color);


    /**
     @brief get short name of Country
     @return short name of Country
     */
    const std::string &getAbbr() const;

    /**
     @brief set short name of Country
     @param abbr  new short name
     */
    void setAbbr(const std::string &abbr);


    /**
     @brief get citizen name of Country
     @return citizen's name of Country
     */
    const std::string &getCitizen() const;

    /**
     @brief set citizen's name of Country
     @param citizen new citizen's name's value
     */
    void setCitizen(const std::string &citizen);


    /**
     @brief get id of Country
     @return id of Country
     */
    int getId() const;

    /**
     @brief set id of Country
     @param id new id's value
     */
    void setId(int id);

    /**
     @brief create  Country from JSON-object
     @return parsed Country
     */
    static Country *load(json_t *jobject);

    /**
     @brief get JSON-object of Country
     @return JSON-object of Country
     */
    json_t *json();

    /**
     @brief get string presenattion of Country
     @return string presenattion  of Country
     */
    std::string toString();
};


#endif //LAB8_COUNTRY_H
