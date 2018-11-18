#pragma warning (disable:4786)
#pragma warning (disable:4503)

#include <iostream>
#include <fstream>
#include <regex>
#include <climits>

#include "rail_system.h"


RailSystem::RailSystem(const std::string& filename)
{
    load_services(filename);
}

void RailSystem::reset()
{
    std::map<std::string, City*>::iterator iter;
    for(iter = cities.begin(); iter != cities.end(); ++iter)
    {
        delete iter->second;
    }
}

RailSystem::~RailSystem()
{
    reset(); // deleting all allocated Cities objects

    std::map<std::string, std::list<Service*>>::iterator mapIter;
    for(mapIter = outgoing_services.begin(); mapIter != outgoing_services.end(); ++mapIter)
    {
        std::list<Service*>::iterator lstIter; // start itering through all list
        for(lstIter = mapIter->second.begin(); lstIter != mapIter->second.end(); ++lstIter)
        {
            delete *lstIter;
        }
    }
}

void RailSystem::load_services(const std::string& filename)
{
    std::ifstream in(filename);
    std::string line;
    std::string pattern = R"([a-zA-Z]+\s+[a-zA-Z]+\s+[0-9]+\s+[0-9]+\s*)";
    std::regex r(pattern);

    if(!in.is_open())
        throw std::logic_error("File can not be opened");

    while (std::getline(in, line))
    {
        if(!std::regex_match(line, r))
        {
            in.close();
            throw std::logic_error("line doesn't match the pattern");
        }

        add_service(line);
    }

    in.close();
}

void RailSystem::add_service(const std::string& line)
{
    std::istringstream iStrStream(line);
    std::string fromCity, toCity; // cities
    int fee, dist; // weights

    iStrStream >> fromCity;
    iStrStream >> toCity;
    iStrStream >> fee;
    iStrStream >> dist;

    add_city(fromCity);
    add_city(toCity);

    Service* service = new Service(toCity, fee, dist);
    outgoing_services[fromCity].push_back(service);
}

void RailSystem::add_city(const std::string& cityName)
{
    std::map<std::string, City*>::iterator iter;

    iter = cities.find(cityName);
    if(iter == cities.end()) // it means we first see this city
    {
        City* city = new City(cityName);
        cities.insert(std::pair<std::string, City*>(cityName, city)); // adding city
        outgoing_services.insert(std::pair<std::string, std::list<Service*>>(cityName, std::list<Service*>()));
        // adding empty services fo new city
    }
}

std::pair<int, int> RailSystem::calc_route(std::string from, std::string to)
{
    cityCheck(from);
    cityCheck(to);
    initialization(from);

    std::string currCityName = from;
    std::pair<int, int> bestOption;

    while (true)
    {
        relax(currCityName);
        if(currCityName == to) // if we relaxed desired node
        {
            City* lastCity = cities[currCityName];
            return std::pair<int, int>(lastCity->total_fee, lastCity->total_distance);
        }

        currCityName = findMinFromPick(currCityName);
        if(currCityName == "") break; // if all possible cities are visited but desired city wasn't reached
    }

    return std::pair<int, int>(INT_MAX, INT_MAX);
}

void RailSystem::cityCheck(const std::string& cityName)
{
    std::map<std::string, City*>::iterator mapIter;
    mapIter = cities.find(cityName);
    if(mapIter == cities.end()) // if city was't found in map
        throw std::invalid_argument("City " + cityName + " wasn't found");
}

void RailSystem::initialization(std::string& start)
{
    std::map<std::string, City*>::iterator mapIter;
    for(mapIter = cities.begin(); mapIter != cities.end(); ++mapIter)
    {
        mapIter->second->total_fee = INT_MAX;
        mapIter->second->total_distance = INT_MAX;
    }

    cities[start]->total_fee = 0;
    cities[start]->total_distance = 0;
    cities[start]->visited = true;
    // marking start node
}

std::string RailSystem::findMinFromPick(std::string& cityName)
{
    std::string nearestCityName; // "" by default
    City* bestCity = nullptr;

    std::map<std::string, City*>::iterator mapIter;
    for(mapIter = cities.begin(); mapIter != cities.end(); ++mapIter)
    {
        City* currCity = mapIter->second;
        if(!currCity->visited && (!bestCity || bestCity->total_fee > currCity->total_fee))
        {
            bestCity = currCity;
            nearestCityName = bestCity->name;
        }
    }

    return nearestCityName;
}

void RailSystem::relax(std::string& cityName)
{
    City* currCity = cities[cityName];
    std::list<Service*> services = outgoing_services[cityName];

    std::list<Service*>::iterator lstIter;
    for(lstIter = services.begin(); lstIter != services.end(); ++lstIter)
    {
        City* siblingCity = cities[(*lstIter)->destination]; // current next sibling
        if(!siblingCity->visited && siblingCity->total_fee > currCity->total_fee + (*lstIter)->fee)
        {
            siblingCity->total_fee = currCity->total_fee + (*lstIter)->fee;
            siblingCity->total_distance = currCity->total_distance + (*lstIter)->distance;
            siblingCity->from_city = cityName;
        }
    }

    currCity->visited = true;
}

std::vector<std::string> RailSystem::recover_route(const std::string& city)
{
    cityCheck(city);
    if(cities[city]->from_city == "") // if there in no road
        return std::vector<std::string>();

    std::vector<std::string> reachedCities;
    std::string currCityName = city;

    while (currCityName != "")
    {
        reachedCities.push_back(currCityName);
        City* currCity = cities[currCityName];
        currCityName = currCity->from_city;
    }

    std::reverse(reachedCities.begin(), reachedCities.end());
    return reachedCities;
}

bool RailSystem::is_valid_city(const std::string& name)
{
    std::map<std::string, City*>::iterator mapIter;
    mapIter = cities.find(name);

    return !(mapIter == cities.end());
}

void RailSystem::output_cheapest_route(const std::string& from, const std::string& to)
{
    calc_route(from, to);
    std::vector<std::string> cities = recover_route(to);

    std::vector<std::string>::iterator city;
    for(city = cities.begin(); city != cities.end(); ++city)
    {
        std::cout << *city << std::endl;
    }
}

Route RailSystem::getCheapestRoute(const std::string& from, const std::string& to)
{
    std::pair<int, int> result = calc_route(from, to);

    return Route(from, to, result.first, result.second, recover_route(to));
}
