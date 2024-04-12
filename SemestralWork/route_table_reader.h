#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <functional>
#include <fstream>
#include <sstream>
#include "Route.h"
#include "libds/amt/implicit_sequence.h"
class RoutingTable
{
public:
    RoutingTable();
    ~RoutingTable();
    void readTable(std::string menoSuboru, ds::amt::ImplicitSequence<Route>& sekvencia);
};

RoutingTable::RoutingTable()
{
}

void RoutingTable::readTable(std::string menoSuboru, ds::amt::ImplicitSequence<Route>& sekvencia)
{
    std::ifstream csv(menoSuboru);
    std::string line;
    std::string ipAdress;
    std::string mask;
    std::string nextHopIpAdress;
    std::string time;

    //Skipnutie headeru
    if (csv.is_open())
    {
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

                sekvencia.insertLast().data_ = Route(ipAdress, mask, nextHopIpAdress, time);
                tokens.clear();
                ss.clear();
            }
        }
    }
    csv.close();
}

RoutingTable::~RoutingTable()
{
}