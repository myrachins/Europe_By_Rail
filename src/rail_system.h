#ifndef _RAILSYSTEM_H_
#define _RAILSYSTEM_H_

#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <queue>
#include <vector>
#include <assert.h>
#include <climits>      // INT_MAX
#include <algorithm>    // std::find

#include "service.h"
#include "city.h"

class Cheapest
{
public:
    Cheapest() {}

    bool operator()(const City* city1, const City* city2)
    {
        return city1->total_fee > city2->total_fee;
    }

    bool operator()(const City& city1, const City& city2)
    {
        return city1.total_fee > city2.total_fee;
    }

};

// For test only 
struct Route
{
    std::string from;
    std::string to;
    int fee;
    int distance;
    // recovered route. If no route was found, holds an empty vector.
    std::vector<std::string> cities;
    
    Route(const std::string& f, const std::string& t, int fe, int d, const std::vector<std::string>& cities)
            : from(f), to(t), fee(fe), distance(d), cities(cities) {};

    bool operator==(Route const& rhs) const
    {
        return rhs.from == this->from
               && rhs.to == this->to
               && rhs.fee == this->fee
               && rhs.distance == this->distance;
    };
};


class RailSystem
{
protected:
    std::map<std::string, std::list<Service*> > outgoing_services;
    std::map<std::string, City*> cities;
    
    /// Throws std::logic_error in case of incorrect file.
    void load_services(const std::string& filename);

    void reset();

    std::vector<std::string> recover_route(const std::string& city);

    /// Throws std::invalid_argument in case of invalid cities.
    std::pair<int, int> calc_route(std::string from, std::string to);

public:

    RailSystem(const std::string& filename);

    ~RailSystem();

    void output_cheapest_route(const std::string& from, const std::string& to);

    bool is_valid_city(const std::string& name);

    /// For test only.  
    Route getCheapestRoute(const std::string& from, const std::string& to);
};

#endif // _RAILSYSTEM_H_
