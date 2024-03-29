#pragma once
#include <string>
#include <iostream>
#include <stdlib.h>
#include "route_table_reader.h"
#include "route.h"
class Program
{
private:
	RoutingTable* rt;
	std::vector<Route*> zoznam;
public:
	Program();
	void run();
	~Program();
};

Program::Program()
{
	rt = new RoutingTable("RT.csv");
	zoznam = rt->getRoutes();
	this->run();
}

void Program::run()
{
	auto matchWithAddress = [](Route* address, std::string targetIP) {
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

		if (address->isPartOfSubnet(targetIPBinary))
		{
			return true;
		}
		return false;
		};

	auto matchLifetime = [](Route* address, std::string lifetime)
		{
			if (address->getLifeTimeSeconds()._Equal(lifetime))
			{
				return true;
			}
			return false;
		};
	bool run = true;
	std::string input;
	while (run)
	{
		std::cout << "Vyberte si operaciu:" << std::endl;
		std::cout << "1. matchWithAddress" << std::endl;
		std::cout << "2. matchLifetime" << std::endl;
		std::cout << "3. Koniec" << std::endl;

		std::cin >> input;
		if (input == "1")
		{
			std::vector<Route*> adressMatch;
			system("cls");
			std::string ip;
			std::cout << "Zadajte IP adresu v tvare 0-254.0-254.0-254.0-254 :" << std::endl;
			std::cin >> ip;
			
			for (auto it = zoznam.begin(); it != zoznam.end(); ++it)
			{
				if (matchWithAddress(*it, ip))
				{
					adressMatch.push_back(*it);
					std::cout << adressMatch.at(adressMatch.size() - 1)->toString() << std::endl;
				}
			}
			if(adressMatch.size() == 0)
			{
				std::cout << "Neboli najdene ziadne zhody" << std::endl;
			}

		}
		else if (input == "2")
		{
			std::vector<Route*> lifetimeMatch;
			system("cls");
			std::string lifetime;
			std::cout << "Zadajte lifetime v sekundovom tvare (napr. 1h = 3600 sek.):" << std::endl;
			std::cin >> lifetime;

            for (auto it = zoznam.begin(); it != zoznam.end(); ++it)
            {
                if (matchLifetime(*it, lifetime))
                {
                    lifetimeMatch.push_back(*it);
                    std::cout << lifetimeMatch.at(lifetimeMatch.size() - 1)->toString() << std::endl;
                }
            }
			if (lifetimeMatch.size() == 0)
			{
				std::cout << "Neboli najdene ziadne zhody" << std::endl;
			}
		}
		else if (input == "3")
		{
			run = false;
		}
		else
		{
			std::cout << "Zadali ste nespravnu operaciu" << std::endl;
		}
	}
}

Program::~Program()
{
	zoznam.clear();
	delete rt;
}