#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "route.h"
#include <bitset>
#include <functional>

class RoutingTable
{
private:
	std::vector<Route*> routes;
public:
	RoutingTable(const char* file) { readTable(file); }

  //  void readTable(const char* file) {
  //      std::ifstream csv(file);
  //      std::string line = "";
  //      std::string ipAdress = "";
  //      std::string mask = "";
  //      std::string nextHopIpAdress = "";
  //      std::string time = "";
  //      // Skip the first line
  //      std::getline(csv, line);
  //      while (std::getline(csv, line)) {
  //          // Split the line by ";"
  //          size_t pos = 0;
  //          std::string token;
  //          int i = 0;
		//	while ((pos = line.find(";")) != std::string::npos) {
		//		token = line.substr(0, pos);
		//		switch (i) {
		//		case 1:
		//			//split string by "/"
		//			ipAdress = token.substr(0, token.find("/"));
		//			mask = token.substr(token.find("/") + 1, token.length());
		//			break;
		//		case 3:
		//			//split string by " " and take the second part
		//			nextHopIpAdress = token.substr(token.find(" ") + 1, token.length());
		//			break;
		//		}
		//		line.erase(0, pos + 1);
		//		if (i == 3) {
		//			//last part of the line
		//			time = line;
		//			routes.push_back(new Route(ipAdress, mask, nextHopIpAdress, time));
		//		}

		//		i++;
		//	}
  //      }
		//csv.close();
  //  }
    void readTable(const char* file) {
        std::ifstream csv(file);
        std::string line = "";
        std::string ipAdress = "";
        std::string mask = "";
        std::string nextHopIpAdress = "";
        std::string time = "";
        //Skip the header line
        std::getline(csv, line);
        while (std::getline(csv, line)) {
            // Split the line by ";" and store in a vector
            std::vector<std::string> tokens;
            std::stringstream ss(line);
            while (std::getline(ss, line, ';')) {
                tokens.push_back(line);
            }

            // Check if the line has enough tokens
            if (tokens.size() >= 5) {
                // Extract data from tokens (assuming order is consistent)
                ipAdress = tokens[1];
                mask = ipAdress.substr(ipAdress.find("/") + 1);
                ipAdress = ipAdress.substr(0, ipAdress.find("/"));
                nextHopIpAdress = tokens[3].substr(tokens[3].find(" ") + 1);
                time = tokens[4];

                // Create and store route
                routes.push_back(new Route(ipAdress, mask, nextHopIpAdress, time));
				tokens.clear();
				ss.clear();
            }
        }
        csv.close();
    }

	//bool matchWithAdress(std::vector<Route*> zaznam, std::string targetIP) {
	//	bool vysledok = false;
	//	std::vector<Route*> newZaznam;
	//	std::vector<Route*>::iterator it;
	//	/*
	//	std::string octet1 = targetIP.substr(0, targetIP.find("."));
	//	targetIP.erase(0, targetIP.find(".") + 1);
	//	std::string octet2 = targetIP.substr(0, targetIP.find("."));
	//	targetIP.erase(0, targetIP.find(".") + 1);
	//	std::string octet3 = targetIP.substr(0, targetIP.find("."));
	//	targetIP.erase(0, targetIP.find(".") + 1);
	//	std::string octet4 = targetIP;
	//	std::bitset<8> octet1b(std::stoi(octet1));
	//	std::bitset<8> octet2b(std::stoi(octet2));
	//	std::bitset<8> octet3b(std::stoi(octet3));
	//	std::bitset<8> octet4b(std::stoi(octet4));*/
	//	//all bitsets to char array
	//	/*char octet1c[8];
	//	char octet2c[8];
	//	char octet3c[8];
	//	char octet4c[8];
	//	octet1b.to_string().copy(octet1c, 8);
	//	octet2b.to_string().copy(octet2c, 8);
	//	octet3b.to_string().copy(octet3c, 8);
	//	octet4b.to_string().copy(octet4c, 8);*/
	//	//concatenate all char arrays
	//	//std::string targetIPBinary = octet1b.to_string() + octet2b.to_string() + octet3b.to_string() + octet4b.to_string();
	//	for (it = zaznam.begin(); it < zaznam.end(); it++) {
	//		//unsigned int mask = (*it)->getPrefix();
	//		if((*it)->isPartOfSubnet(targetIP)) {
	//			std::cout << (*it)->getIpAdress() << " " << (*it)->getMask() << " " << (*it)->getNextHopIpAdress() << " " << (*it)->getTime() << std::endl;
	//			newZaznam.push_back(*it);
	//			vysledok = true;
	//		}
	//		else
	//		{
	//			vysledok = false;
	//		}
	//	}
	//	//vypis zaznamu
	//	for (it = newZaznam.begin(); it != newZaznam.end(); it++) {
	//		std::cout << (*it)->getIpAdress() << " " << (*it)->getMask() << " " << (*it)->getNextHopIpAdress() << " " << (*it)->getTime() << std::endl;
	//	}

	//	return vysledok;
	//}

	std::function<bool(std::vector<Route*>, std::string)> matchWithAddress = [](std::vector<Route*> zaznam, std::string targetIP) -> bool {
		std::string octet1 = targetIP.substr(0, targetIP.find("."));
		targetIP.erase(0, targetIP.find(".") + 1);
		std::string octet2 = targetIP.substr(0, targetIP.find("."));
		targetIP.erase(0, targetIP.find(".") + 1);
		std::string octet3 = targetIP.substr(0, targetIP.find("."));
		targetIP.erase(0, targetIP.find(".") + 1);
		std::string octet4 = targetIP;
		std::bitset<8> octet1b(std::stoi(octet1));
		std::bitset<8> octet2b(std::stoi(octet2));
		std::bitset<8> octet3b(std::stoi(octet3));
		std::bitset<8> octet4b(std::stoi(octet4));
		std::string targetIPBinary = octet1b.to_string() + octet2b.to_string() + octet3b.to_string() + octet4b.to_string();
		bool vysledok = false;
		std::vector<Route*> newZaznam;
		std::vector<Route*>::iterator it;
		it = zaznam.begin();
		while (it != zaznam.end()) {
			if ((*it)->isPartOfSubnet(targetIPBinary)) {
				//std::cout << (*it)->getIpAdress() << " " << (*it)->getMask() << " " << (*it)->getNextHopIpAdress() << " " << (*it)->getTime() << std::endl;
				newZaznam.push_back(*it);
				vysledok = true;
			}
			else {
				vysledok = false;
			}
			it++;
		}
		/*for (it = zaznam.begin(); it < zaznam.end(); it++) {
			if ((*it)->isPartOfSubnet(targetIP)) {
				std::cout << (*it)->getIpAdress() << " " << (*it)->getMask() << " " << (*it)->getNextHopIpAdress() << " " << (*it)->getTime() << std::endl;
				newZaznam.push_back(*it);
				vysledok = true;
			}
			else {
				vysledok = false;
			}
		}*/
		if (newZaznam.size() > 0) {
			for (auto route : newZaznam) {
				std::cout << route->getIpAdress() << " " << route->getMask() << " " << route->getNextHopIpAdress() << " " << route->getTime() << std::endl;
			}
		}
		else
		{
			std::cout << "No matching route found" << std::endl;
		}

		return vysledok;
		};

	//geter pre routes
	std::vector<Route*> getRoutes() {
		return routes;
	}

	void printTable() {
		int i = 1;
		for (auto route : routes) {
			std::cout << route->getIpAdress() << " " << route->getMask() << " " << route->getNextHopIpAdress() << " " << route->getTime() << " " << i << " length:" << routes.size() << std::endl;
			i++;
		}
	}
	~RoutingTable() {
		std::cout << "-------------------------------" << std::endl;
		std::cout << "Destructor called" << std::endl;
		std::cout << "-------------------------------" << std::endl;
		for (auto route : routes) {
			delete route;
		}
		routes.clear();
	}

};
