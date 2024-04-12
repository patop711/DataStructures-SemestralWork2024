#pragma once
#include <string>
#include <iostream>
#include <stdlib.h>
#include "route_table_reader.h"
#include "algorithms.h"
#include "route.h"
#include "libds/amt/implicit_sequence.h"

class Program
{
private:
	ds::amt::ImplicitSequence<Route> zoznam;
public:
	Program(std::string menoSuboru);
	void run();
	~Program();
};

Program::Program(std::string menoSuboru)
{
	RoutingTable().readTable(menoSuboru,this->zoznam);
	this->run();
}

void Program::run()
{
	ds::amt::ImplicitSequence<Route> platneZaznamy;
	bool run = true;
	std::string input;
	while (run)
	{
		std::cout << "Vyber si predikát 1/2:" << std::endl;
		std::cout << "1. matchWithAddress" << std::endl;
		std::cout << "2. matchLifetime" << std::endl;
		std::cout << "3. Koniec" << std::endl;
		platneZaznamy.clear(); //<-- vymazanie zoznamu predchadzajucich zaznamov
		std::cin >> input;

		if (input == "1")
		{
			system("cls");
			std::string ip;
			std::cout << "Zadajte IP adresu v tvare 0-254.0-254.0-254.0-254 :" << std::endl;
			std::cin >> ip;

			Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::filter(zoznam.begin(), zoznam.end(),
				[&ip](auto zdroj)->bool
				{
					return Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::matchWithAddress(zdroj,ip);
				}, 
				[&platneZaznamy](auto zaznam)->void
				{
					platneZaznamy.insertLast().data_ = zaznam;
				}
			);
		}
		else if (input == "2")
		{
			system("cls");
			std::string lifetime;
			std::cout << "Zadajte lifetime v sekundovom tvare (napr. 1h = 3600 sek.):" << std::endl;
			std::cin >> lifetime;
			system("cls");

			Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::filter(zoznam.begin(), zoznam.end(),
				[&lifetime](auto zdroj)->bool
				{
					return Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::matchLifetime(zdroj, lifetime);
				},
				[&platneZaznamy](auto zaznam)->void
				{
					platneZaznamy.insertLast().data_ = zaznam;
				}
			);
		}
		else if (input == "3")
		{
			run = false;
		}
		else
		{
			std::cout << "Zadali ste nesprávnu operáciu" << std::endl;
		}

		if (platneZaznamy.size() == 0 && (input == "1" || input == "2"))
		{
			std::cout << "Neboli nájdené žiadne zhody" << std::endl;
		}
		else
		{
			for (auto& udaj : platneZaznamy)
			{
				std::cout << udaj.toString() << std::endl;
			}
		}
	}
}

Program::~Program()
{
	zoznam.clear();
}