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
#include "libds/adt/table.h"
#include "octet.h"
#include "sorting_algorithm.h"

class Program
{
private:
	bool runProgram = true;
	bool hierarchyLoaded = false;
	std::string menoSuboru;
	ds::amt::ImplicitSequence<Route*>* zoznam;
	ds::amt::ImplicitSequence<Route*>* hierarchySequence;
	ds::amt::MultiWayExplicitHierarchy<Octet*>* hierarchia;
	Octet* koren = nullptr;

public:
	Program();
	void run();
	void runStage1();
	void runStage2();
	void runStage3();
	//void runStage5Bonus();<-- TODO
	~Program();
private:
	void stage_4(ds::amt::ImplicitSequence<Route*> sekvencia) const;
	void destroy();
};

Program::Program()
{
	this->zoznam = new ds::amt::ImplicitSequence<Route*>(); //<-- sekvencia
	this->hierarchia = new ds::amt::MultiWayExplicitHierarchy<Octet*>(); //<-- hierarchia
	this->hierarchySequence = new ds::amt::ImplicitSequence<Route*>(); //<-- sekvencia pre hierarchiu
	std::cout << "Zadajte nazov CSV suboru s routovacou tabulkou: ";
	std::cin >> this->menoSuboru;
	RoutingTable().readTableForSequence(this->menoSuboru, this->zoznam);
	this->run();
}

void Program::run()
{
	system("cls");
	std::cout << "Vitajte v programu pre spracovanie smerovacích tabuliek" << std::endl;
	std::cout << std::endl;
	std::cout << "1. Zobrazenie záznamov pod¾a predikátu + sortovanie" << std::endl;
	std::cout << "2. Zobrazenie záznamov pod¾a hierarchie + sortovanie" << std::endl;
	std::cout << "3. Zobrazenie záznamov z tabulky" << std::endl;
	std::cout << "4. Zmeò názov súboru" << std::endl;
	std::cout << "5. Koniec" << std::endl;
	std::cout << std::endl;
	std::cout << "Vyberte si úroveò/akciu: ";

	std::string input;
	std::cin >> input;
	while (runProgram)
	{
		switch (std::stoi(input))
		{
		case 1:
			system("cls");
			this->runStage1();
			break;
		case 2:
			system("cls");
			this->runStage2();
			break;
		case 3:
			system("cls");
			this->runStage3();
			break;
		case 4:
			system("cls");
			std::cout << "Zadajte nazov CSV suboru s routovacou tabulkou: ";
			std::cin >> this->menoSuboru;
			this->destroy();
			this->hierarchyLoaded = false;
			this->run();
			break;
		case 5:
			this->runProgram = false;
			break;
		default:
			std::cout << "Zadali ste nesprávnu operáciu" << std::endl;
			break;

		}
	}


}

void Program::runStage1()
{
	//this->zoznam.clear();
	if (this->zoznam->isEmpty())
	{
		RoutingTable().readTableForSequence(this->menoSuboru, this->zoznam);
	}
	ds::amt::ImplicitSequence<Route*> platneZaznamy;
	bool runStage1Program = true;
	std::string input;
	while (runStage1Program)
	{
		bool zobrazujZaznamy = false;
		std::cout << "Vyber si predikát 1/2:" << std::endl;
		std::cout << "1. matchWithAddress" << std::endl;
		std::cout << "2. matchLifetime" << std::endl;
		std::cout << "3. Koniec" << std::endl;
		platneZaznamy.clear(); //<-- vymazanie zoznamu predchadzajucich zaznamov
		std::cin >> input;

		switch (std::stoi(input)) {
		case 1: {
			system("cls");
			zobrazujZaznamy = true;

			std::string ip;
			std::cout << "Zadajte IP adresu v tvare 0-255.0-255.0-255.0-255: ";
			std::cin >> ip;

			Algorithms<ds::amt::ImplicitSequence<Route*>::IteratorType, Route*, std::string>::filter(zoznam->begin(), zoznam->end(),
				[&ip](auto zdroj) -> bool {
					return Algorithms<ds::amt::ImplicitSequence<Route*>::IteratorType, Route*, std::string>::matchWithAddress(zdroj, ip);
				},
				[&platneZaznamy](auto zaznam) -> void {
					platneZaznamy.insertLast().data_ = zaznam;
				}
			);
			break;
		}

		case 2: {
			system("cls");
			zobrazujZaznamy = true;
			std::string lifetime;
			std::cout << "Zadajte lifetime v sekundovom tvare (napr. 1h = 3600 sek.): ";
			std::cin >> lifetime;
			system("cls");

			Algorithms<ds::amt::ImplicitSequence<Route*>::IteratorType, Route*, std::string>::filter(zoznam->begin(), zoznam->end(),
				[&lifetime](auto zdroj) -> bool {
					return Algorithms<ds::amt::ImplicitSequence<Route*>::IteratorType, Route*, std::string>::matchLifetime(zdroj, lifetime);
				},
				[&platneZaznamy](auto zaznam) -> void {
					platneZaznamy.insertLast().data_ = zaznam;
				}
			);
			break;
		}

		case 3: {
			runStage1Program = false;
			this->run();
			break;
		}

		default:
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
				udaj->toString();
			}

			this->stage_4(platneZaznamy);
			platneZaznamy.clear();
		}
	}
}


void Program::runStage2()
{
	system("cls");
	int indexVetvy = 0;
	std::string oktet1;
	std::string oktet2;
	std::string oktet3;
	std::string oktet4;
	std::string prefix;

	bool runStage2Program = true;
	//this->hierarchia.clear();
	//ds::amt::ImplicitSequence<Route*> rZoznam;

	if (!this->hierarchyLoaded)
	{
		this->hierarchyLoaded = true;
		this->koren = new Octet("RT KOREÒ"); // <-- REPREZENTUJE KOREÒ HIERARCHIE
		this->hierarchia->emplaceRoot().data_ = koren;
		RoutingTable().readTableForHierarchy(this->menoSuboru, this->hierarchia, this->hierarchySequence);
	}

	ds::amt::MultiWayExplicitHierarchyBlock<Octet*>* aktualnaVetva = this->hierarchia->accessRoot();
	std::string inputOperation;
	while (runStage2Program)
	{
		system("cls");
		switch (indexVetvy)
		{

		case 0:
			std::cout << "Aktuálny vrchol: " << aktualnaVetva->data_->octet << " [Stupeò: " << indexVetvy << "]" << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_->octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 1:
			std::cout << "Aktuálny vrchol: " << aktualnaVetva->data_->octet << " [Stupeò: " << indexVetvy << "]" << " IP: " << oktet1 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_->octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 2:
			std::cout << "Aktuálny vrchol: " << aktualnaVetva->data_->octet << " [Stupeò: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_->octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 3:
			std::cout << "Aktuálny vrchol: " << aktualnaVetva->data_->octet << " [Stupeò: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << "->" << oktet3 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_->octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 4:
			std::cout << "Aktuálny vrchol: " << aktualnaVetva->data_->octet << " [Stupeò: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << "->" << oktet3 << "->" << oktet4 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_->octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 5:
			std::cout << "Aktuálny vrchol: " << aktualnaVetva->data_->octet << " [Stupeò: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << "->" << oktet3 << "->" << oktet4 << "->" << prefix << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_->octet << std::endl;
			}
			std::cout << std::endl;
			break;
		}

		std::cout << "Vyber si operáciu:" << std::endl;
		std::cout << "[1] - Presuò sa na nadradený oktet." << std::endl;
		std::cout << "[2] - Presuò sa na podradený oktet. " << std::endl;
		std::cout << "[3] - Prejdi podradené vetvy predikátmi z prvej úrovne." << std::endl;
		std::cout << "[4] - Ukonèi druhú úroveò. " << std::endl;
		std::cout << "Vo¾ba: ";

		std::cin >> inputOperation;
		switch (std::stoi(inputOperation))
		{

		case 1:
		{
			if (aktualnaVetva->parent_ != nullptr)
			{
				aktualnaVetva = this->hierarchia->accessParent(*aktualnaVetva);
				indexVetvy--;
			}
			break;
		}
		case 2:
		{
			if (aktualnaVetva->sons_->size() != 0)
			{
				std::cout << "Zadaj index syna, na ktorého sa chceš presunú: ";
				std::string index;
				std::cin >> index;
				int indexSyna = std::stoi(index);
				aktualnaVetva = hierarchia->accessSon(*aktualnaVetva, indexSyna);
				indexVetvy++;
				switch (indexVetvy)
				{
				case 1:
					oktet1 = aktualnaVetva->data_->octet;
					break;
				case 2:
					oktet2 = aktualnaVetva->data_->octet;
					break;
				case 3:
					oktet3 = aktualnaVetva->data_->octet;
					break;
				case 4:
					oktet4 = aktualnaVetva->data_->octet;
					break;
				case 5:
					prefix = aktualnaVetva->data_->octet;
					break;
				}
			}
			break;
		}
		case 3:
		{
			Route init;
			Route& route = init;

			//bool zobrazujZaznamy = true;

			std::string vyberMetody;
			std::string predikat;
			system("cls");

			std::cout << "Vyber si predikát:" << std::endl;
			std::cout << "1. matchWithAddress" << std::endl;
			std::cout << "2. matchLifetime" << std::endl;
			std::cout << "3. Koniec" << std::endl;

			std::cout << "Predikát: ";
			std::cin >> vyberMetody;
			std::cout << std::endl;

			std::cout << "Zadaj predikát: ";
			std::cin >> predikat;
			std::cout << std::endl;

			ds::amt::ImplicitSequence<Route*> platneZaznamy;
			ds::amt::MultiWayExplicitHierarchy<Octet*>::PreOrderHierarchyIterator begin(this->hierarchia, aktualnaVetva);
			switch (stoi(vyberMetody))
			{
			case 1:
				Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet*>::PreOrderHierarchyIterator, Octet*, std::string>::filter(begin, hierarchia->end(),
					[&predikat, &route](Octet* prehladavane)->bool
					{
						return prehladavane->referenceToRoute != nullptr ?
							Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Route*, std::string>::matchWithAddress(prehladavane->referenceToRoute, predikat) : false;
					},
					[&platneZaznamy](Octet* zaznam)->void
					{
						platneZaznamy.insertLast().data_ = zaznam->referenceToRoute;
					}
				);
				break;
			case 2:
				Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet*>::PreOrderHierarchyIterator, Octet*, std::string>::filter(begin, hierarchia->end(),
					[&predikat, &route](Octet* prehladavane)->bool
					{
						return prehladavane->referenceToRoute != nullptr ?
							Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Route*, std::string>::matchLifetime(prehladavane->referenceToRoute, predikat) : false;
					},
					[&platneZaznamy](Octet* zaznam)->void
					{
						platneZaznamy.insertLast().data_ = zaznam->referenceToRoute;
					}
				);
				break;
			}
				if (platneZaznamy.size() == 0 && (vyberMetody == "1" || vyberMetody == "2"))
				{
					std::cout << "Neboli nájdené žiadne zhody" << std::endl;
				}
				else
				{
					for (auto& udaj : platneZaznamy)
					{
						udaj->toString();
					}

					this->stage_4(platneZaznamy);
					platneZaznamy.clear();
				}
			break;
		}
		case 4:
		{
			runStage2Program = false;
			this->run();
			break;
		}
		default: std::cout << "Zadali ste nesprávnu operáciu" << std::endl;
		}
	}
}

void Program::runStage3()
{
	ds::adt::HashTable<std::string, Route*> hashTable([](const std::string& kluc)
		{
			static size_t randomizer = 0;
			size_t hash = 0;
			for (char cislo : kluc) {
				hash = hash * 2 + cislo;
			}
			hash += randomizer;
			randomizer += 5;
			//std::cout << "Hash: " << hash << std::endl;
			return hash;
		}, this->zoznam->size());

	RoutingTable().loadDataToTable(&hashTable, this->zoznam);

	bool runStage3Program = true;
	std::string input;
	while (runStage3Program)
	{
		std::cout << "1. Zobrazenie IP podla next-hop" << std::endl;
		std::cout << "2. Koniec" << std::endl;
		std::cout << "Vyberte si operáciu: ";
		std::cin >> input;
		system("cls");
		switch (std::stoi(input))
		{
		case 1:
		{
			std::string key;
			std::cout << "Zadajte next-hop: ";
			std::cin >> key;

			for (auto iterator = hashTable.begin(); iterator != hashTable.end(); ++iterator)
			{
				if ((*iterator).key_ == key)
				{
					std::cout << (*iterator).key_ << " -> ";
					(*iterator).data_->toString();
				}
			}
			break;
		}
		case 2:
		{
			runStage3Program = false;
			hashTable.clear();
			this->run();

			break;
		}
		}
	}
}

void Program::stage_4(ds::amt::ImplicitSequence<Route*> sekvencia) const
{
	if (this->runProgram)
	{
		std::string sortovat;
		std::cout << "Chcete zoradi záznamy? [A/N]" << std::endl;
		std::cin >> sortovat;
		bool sortuj = false;
		std::string vyberSortovania;
		if (sortovat._Equal("A") || sortovat._Equal("a"))
		{
			sortuj = true;
			std::cout << "1. comparePrefix (IP adresa v prefixe prvého záznamu je nižšia ako IP adresa v prefixe druhého záznamu)" << std::endl;
			std::cout << "2. compareTime (Èas prítomnosti prvého záznamu je nižší ako èas prítomnosti druhého záznamu)" << std::endl;
			std::cout << "Vyberte si spôsob zoradenia: ";
			std::cin >> vyberSortovania;
		}
		else if (sortovat._Equal("N") || sortovat._Equal("n"))
		{
			sortuj = false;

		}

		if (sortuj)
		{
			switch (std::stoi(vyberSortovania))
			{
			case 1:
				SortingAlgorithm::comparePrefix(&sekvencia);
				break;
			case 2:
				SortingAlgorithm::compareTime(&sekvencia);
				break;
			}
			bool vysledky = true;
			std::string koniecZobrazenia;
			while (vysledky)
			{
				for (auto& udaj : sekvencia)
				{
					udaj->toString();
				}
				std::cout << "Chcete pokraèova v zobrazení záznamov? [A/N]" << std::endl;
				std::cin >> koniecZobrazenia;
				if (koniecZobrazenia._Equal("N") || koniecZobrazenia._Equal("n"))
				{
					vysledky = false;
				}
			}
		}
	}
}

void Program::destroy()
{
	//vymazanie sekvencie
	for (auto& route : *(this->zoznam))
	{
		delete route;
	}
	this->zoznam->clear();

	//vymazanie hierarchie
	for (auto& octet : *(this->hierarchia))
	{
		octet->referenceToRoute = nullptr;
		delete octet;
	}
	this->hierarchia->clear();

	//vymazanie sekvencie pre hierarchiu
	for (auto& route : *(this->hierarchySequence))
	{
		delete route;
	}
	this->hierarchySequence->clear();
}

Program::~Program()
{
	this->destroy();

	delete this->hierarchia;
	delete this->zoznam;
	delete this->hierarchySequence;
}
