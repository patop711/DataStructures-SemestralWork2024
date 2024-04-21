#pragma once
#include <string>
#include <iostream>
#include <stdlib.h>
#include "libds/constants.h"
#include "route_table_reader.h"
#include "algorithms.h"
#include "route.h"
#include "libds/amt/implicit_sequence.h"
#include "libds/amt/explicit_hierarchy.h"
#include "octet.h"

class Program
{
private:
	std::string menoSuboru;
	ds::amt::ImplicitSequence<Route> zoznam;
	ds::amt::MultiWayExplicitHierarchy<Octet> hierarchia;
public:
	Program();
	void run();
	void runStage1();
	void runStage2();
	~Program();
};

Program::Program()
{
	std::cout << "Zadajte nazov CSV suboru s routovacou tabulkou: ";
	std::cin >> this->menoSuboru;
	this->run();
}

void Program::run()
{
	system("cls");
	bool run = true;
	std::cout << "Vitajte v programu pre spracovanie smerovac�ch tabuliek" << std::endl;
	std::cout << "Vyberte si �rove�" << std::endl;
	std::cout << "1. Zobrazenie z�znamov pod�a predik�tu" << std::endl;
	std::cout << "2. Zobrazenie z�znamov pod�a hierarchie" << std::endl;
	std::cout << "3. Zme� n�zov s�boru" << std::endl;
	std::cout << "4. Koniec" << std::endl;
	std::string input;
	std::cin >> input;
	if (input == "1")
	{
		system("cls");
		this->runStage1();
	}
	else if (input == "2")
	{
		system("cls");
		this->runStage2();
	}
	else if (input == "3")
	{
		system("cls");
		std::cout << "Zadajte nazov CSV suboru s routovacou tabulkou: ";
		std::cin >> this->menoSuboru;
		this->zoznam.clear();
		this->hierarchia.clear();
		this->run();
	}
	else if (input == "4")
	{
		run = false;
	}
	else
	{
		std::cout << "Zadali ste nespr�vnu oper�ciu" << std::endl;
	}


}

void Program::runStage1()
{
	RoutingTable().readTableForSequence(this->menoSuboru, this->zoznam);
	ds::amt::ImplicitSequence<Route> platneZaznamy;
	bool run = true;
	std::string input;
	while (run)
	{
		std::cout << "Vyber si predik�t 1/2:" << std::endl;
		std::cout << "1. matchWithAddress" << std::endl;
		std::cout << "2. matchLifetime" << std::endl;
		std::cout << "3. Koniec" << std::endl;
		platneZaznamy.clear(); //<-- vymazanie zoznamu predchadzajucich zaznamov
		std::cin >> input;

		if (input == "1")
		{
			system("cls");
			std::string ip;
			std::cout << "Zadajte IP adresu v tvare 0-255.0-255.0-255.0-255 :" << std::endl;
			std::cin >> ip;

			Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::filter(zoznam.begin(), zoznam.end(),
				[&ip](auto zdroj)->bool
				{
					return Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::matchWithAddress(zdroj, ip);
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
			this->run();
		}
		else
		{
			std::cout << "Zadali ste nespr�vnu oper�ciu" << std::endl;
		}

		if (platneZaznamy.size() == 0 && (input == "1" || input == "2"))
		{
			std::cout << "Neboli n�jden� �iadne zhody" << std::endl;
		}
		else
		{
			//string format IP ADRESA MASKA NEXT_HOP LIFETIME
			if (platneZaznamy.size() != 0)
			{
				std::cout << "IP ADRESA";
				std::cout << std::setw(8) << "MASKA";
				std::cout << std::setw(10) << "NEXT_HOP";
				std::cout << std::setw(14) << "LIFETIME" << std::endl;
			}
			for (auto& udaj : platneZaznamy)
			{
				std::cout << udaj.toString() << std::endl;
			}
		}
	}
}


void Program::runStage2()
{
	system("cls");

	bool runStage2Program = true;
	this->hierarchia.clear();
	ds::amt::ImplicitSequence<Route*> rZoznam;

	auto koren = Octet("RT KORE�"); // <-- REPREZENTUJE KORE� HIERARCHIE
	this->hierarchia.emplaceRoot().data_ = koren;
	RoutingTable()._readTableForHierarchy(this->menoSuboru, &hierarchia, rZoznam);

	ds::amt::MultiWayExplicitHierarchyBlock<Octet>* aktualnaVetva = this->hierarchia.accessRoot();
	std::string inputOperation;
	while (runStage2Program)
	{
		system("cls");
		std::cout << "Aktu�lny vrchol: " << aktualnaVetva->data_.octet << std::endl;
		for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
		{
			std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_.octet << std::endl;
		}

		std::cout << "Vyber si oper�ciu: " << std::endl;
		std::cout << "\t[1] - Presu� sa na nadraden� jednotku. " << std::endl;
		std::cout << "\t[2] - Presu� sa na podraden� jednotku. " << std::endl;
		std::cout << "\t[3] - Prejdi podraden� prvou �rov�ou. " << std::endl;
		std::cout << std::endl;
		std::cout << "\t[0] - Ukon�i �rove�. " << std::endl;
		std::cout << "Vo�ba: \n";

		std::cin >> inputOperation;
		if (inputOperation._Equal("1"))
		{
			// Ak existuje parent, tak ho spr�stupn�m, ak nie, opakujem vo�bu
			if (aktualnaVetva->parent_ != nullptr)
			{
				aktualnaVetva = this->hierarchia.accessParent(*aktualnaVetva);
			}
		}
		else if (inputOperation._Equal("2"))
		{
			if (aktualnaVetva->sons_->size() != 0)
			{
				std::cout << "Zadaj index syna, na ktor�ho sa chce� presun��. \n";
				std::string index;
				std::cin >> index;
				int indexSyna = std::stoi(index);
				aktualnaVetva = hierarchia.accessSon(*aktualnaVetva, indexSyna);
			}
		}
		else if (inputOperation._Equal("3"))
		{
			//Algorithms <ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Octet, std::string> alg;
			bool isNullPtr = false;
			Route init;
			Route& route = init;

			bool UkonciZobrazenieZaznamov = false;

			std::string vyberMetody;
			std::string predikat;
			system("cls");

			std::cout << "Vyber si met�du 1/2:" << std::endl;
			std::cout << "1. matchWithAddress" << std::endl;
			std::cout << "2. matchLifetime" << std::endl;

			std::cout << "Met�da: ";
			std::cin >> vyberMetody;
			std::cout << std::endl;
			std::cout << "Zadaj predik�t: ";
			std::cin >> predikat;
			std::cout << std::endl;

			ds::amt::ImplicitSequence<Route> platneZaznamy;
			ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator begin(&this->hierarchia, aktualnaVetva);
			switch (stoi(vyberMetody))
			{
			case 1:
				Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Octet, std::string>::filter(begin, hierarchia.end(),
					[&predikat, &isNullPtr ,&route](Octet prehladavane)->bool
					{
						if (prehladavane.referenceToRoute == nullptr)
						{
							isNullPtr = true;
							return false;
						}
						else
						{
							route = *prehladavane.referenceToRoute;
							return Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Route, std::string>::matchWithAddress(route, predikat);
						}
					},
					[&platneZaznamy](Octet zaznam)->void
					{
							platneZaznamy.insertLast().data_ = *zaznam.referenceToRoute;
							//std::cout << platneZaznamy.accessLast()->data_.toString() << std::endl;
						/*platneZaznamy.insertLast().data_ = *zaznam.referenceToRoute;
						std::cout << platneZaznamy.accessLast()->data_.toString() << std::endl;*/
					}
				);
				break;
			case 2:
				Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Octet, std::string>::filter(begin, hierarchia.end(),
					[&predikat, &isNullPtr, &route](Octet prehladavane)->bool
					{
						if (prehladavane.referenceToRoute == nullptr)
						{
							isNullPtr = true;
							return false;
						}
						else
						{
							route = *prehladavane.referenceToRoute;
							return Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Route, std::string>::matchLifetime(route, predikat);
						}
					},
					[&platneZaznamy/*, &route*/](Octet zaznam)->void
					{
						platneZaznamy.insertLast().data_ = *zaznam.referenceToRoute;
						//std::cout << platneZaznamy.accessLast()->data_.toString() << std::endl;
					}
				);
				break;
			}

			while(!UkonciZobrazenieZaznamov)
			{
				if (platneZaznamy.size() == 0 && (vyberMetody == "1" || vyberMetody == "2"))
				{
					std::cout << "Neboli n�jden� �iadne zhody" << std::endl;
				}
				else
				{
					//string format IP ADRESA MASKA NEXT_HOP LIFETIME
					if (platneZaznamy.size() != 0)
					{
						std::cout << "IP ADRESA";
						std::cout << std::setw(8) << "MASKA";
						std::cout << std::setw(10) << "NEXT_HOP";
						std::cout << std::setw(14) << "LIFETIME" << std::endl;
					}
					for (auto& udaj : platneZaznamy)
					{
						std::cout << udaj.toString() << std::endl;
					}
				}
				std::cout << "Chcete pokra�ova� v zobrazen� z�znamov? [A/N]" << std::endl;
				std::string pokracovat;
				std::cin >> pokracovat;
				if (pokracovat._Equal("N") || pokracovat._Equal("n"))
				{
					UkonciZobrazenieZaznamov = true;
				}
			}

		}
		else if (inputOperation._Equal("0"))
		{
			runStage2Program = false;
			this->run();
		}
		else
		{
			std::cout << "Zadali ste nespr�vnu oper�ciu" << std::endl;
		}
	}
}

Program::~Program()
{
	zoznam.clear();
	hierarchia.clear();
}