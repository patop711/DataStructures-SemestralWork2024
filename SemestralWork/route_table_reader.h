#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <functional>
#include <fstream>
#include <sstream>
#include "Route.h"
#include "algorithms.h"
class RoutingTable
{
private:
    std::vector<Route*> routes;
public:
    RoutingTable(const char* menoSuboru);
    ~RoutingTable();
    void readTable(const char* menoSuboru);
    std::vector<Route*> getRoutes();
    std::vector<Route*>::iterator getBegin();
    std::vector<Route*>::iterator getEnd();
};

RoutingTable::RoutingTable(const char* menoSuboru)
{
	readTable(menoSuboru);
}

void RoutingTable::readTable(const char* menoSuboru)
{
    std::ifstream csv(menoSuboru);
    std::string line = "";
    std::string ipAdress = "";
    std::string mask = "";
    std::string nextHopIpAdress = "";
    std::string time = "";
    //Skipnutie headeru
    std::getline(csv, line);
    while (std::getline(csv, line)) {
        // Splitnutie udajov pomocou ";"
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        while (std::getline(ss, line, ';')) {
            tokens.push_back(line);
        }

        //Kontrola ci sa v riadku nachadzaju vsetky potrebne udaje
        if (tokens.size() >= 5) {
            ipAdress = tokens[1];
            mask = ipAdress.substr(ipAdress.find("/") + 1);
            ipAdress = ipAdress.substr(0, ipAdress.find("/"));
            nextHopIpAdress = tokens[3].substr(tokens[3].find(" ") + 1);
            time = tokens[4];


            routes.push_back(new Route(ipAdress, mask, nextHopIpAdress, time));
            tokens.clear();
            ss.clear();
        }
    }
    csv.close();
}

std::vector<Route*> RoutingTable::getRoutes() 
{
	return routes;
}

std::vector<Route*>::iterator RoutingTable::getBegin() 
{
	return routes.begin();
}

std::vector<Route*>::iterator RoutingTable::getEnd() 
{
	return routes.end();
}

RoutingTable::~RoutingTable()
{
	for(auto route : routes)
	{
		delete route;
	}
	routes.clear();
}