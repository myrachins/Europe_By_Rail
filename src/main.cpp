#pragma warning (disable:4786)
#pragma warning (disable:4503)

#include <iostream>
#include <fstream>
#include <string>

#include "city.h"
#include "service.h"
#include "rail_system.h"

using namespace std;

// For test
bool check(Route & expected,RailSystem &rs)
{
    Route actual = rs.getCheapestRoute(expected.from,expected.to);
    return actual == expected;
}

int main(int argc, char* argv[]) 
{

    try 
    {

        RailSystem rs("../../res/services.txt");
        
        while (true) 
        {

            cout << "\n\nEnter a start and destination city:  ('quit' to exit)\n";

            string from, to;
            cin >> from;
            if (from == "quit") break;
            cin >> to;

            if (rs.is_valid_city(from) && rs.is_valid_city(to)) 
            {
                rs.output_cheapest_route(from, to);
            }
            else 
            {
                cout << "Please enter valid cities\n\n";
            }

        }

        return EXIT_SUCCESS;

    }
    catch (exception& e) 
    {
        cout << e.what() << endl;
    }
    catch (...) 
    {
        cout << "Unknown exception caught\n";
    }

    return EXIT_FAILURE;
}
