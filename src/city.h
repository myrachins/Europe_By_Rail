#ifndef _CITY_H_
#define _CITY_H_

class City {

public:

    // name of the city
    std::string name;

    // bookkeeping info
    bool visited;
    int total_fee;
    int total_distance;
    std::string from_city;

    City() : 
        name(""), 
        visited(false), 
        total_fee(0), 
        total_distance(0), 
        from_city("") {}

    City(std::string const &s): 
        name(s), 
        visited(false), 
        total_fee(0), 
        total_distance(0), 
        from_city("") {}
};

#endif
