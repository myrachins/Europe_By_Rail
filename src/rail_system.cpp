#pragma warning (disable:4786)
#pragma warning (disable:4503)

#include <fstream>
#include <regex>

#include "rail_system.h"

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

void RailSystem::reset()
{
    std::map<std::string, City*>::iterator iter = cities.begin();

    while (iter != cities.end())
    {
        delete iter->second;
        ++iter;
    }
}
