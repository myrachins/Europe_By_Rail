#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <string>

class Service 
{
public:
    int fee;
    int distance;
    std::string destination;

    Service(std::string city, int f, int d) : 
        fee(f),
        distance(d),
        destination(city) {}
};

#endif
