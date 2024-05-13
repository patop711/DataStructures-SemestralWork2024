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
	std::string menoSuboru;
	ds::amt::ImplicitSequence<Route> zoznam;
	ds::amt::MultiWayExplicitHierarchy<Octet> hierarchia;

public:
	Program();
	void run();
	void runStage1();
	void runStage2();
	void runStage3();
	void stage_4(bool& zobrazujZaznamy, ds::amt::ImplicitSequence<Route> sekvencia) const;
	//void runStage5Bonus();<-- TODO
	~Program();
};

Program::Program()
{
	std::cout << "Zadajte nazov CSV suboru s routovacou tabulkou: ";
	std::cin >> this->menoSuboru;
	RoutingTable().readTableForSequence(this->menoSuboru, this->zoznam);
	this->run();
}

void Program::run()
{
	system("cls");
	std::cout << "Vitajte v programu pre spracovanie smerovac�ch tabuliek" << std::endl;
	std::cout << std::endl;
	std::cout << "1. Zobrazenie z�znamov pod�a predik�tu + sortovanie" << std::endl;
	std::cout << "2. Zobrazenie z�znamov pod�a hierarchie + sortovanie" << std::endl;
	std::cout << "3. Zobrazenie z�znamov z tabulky" << std::endl;
	std::cout << "4. Zme� n�zov s�boru" << std::endl;
	std::cout << "5. Koniec" << std::endl;
	std::cout << std::endl;
	std::cout << "Vyberte si �rove�/akciu: ";

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
			this->zoznam.clear();
			this->hierarchia.clear();
			this->run();
			break;
		case 5:
			this->runProgram = false;
			break;
		default:
			std::cout << "Zadali ste nespr�vnu oper�ciu" << std::endl;
			break;

		}
		/*if (input == "1")
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
			this->runStage3();
		}
		else if (input == "4")
		{
			system("cls");
			std::cout << "Zadajte nazov CSV suboru s routovacou tabulkou: ";
			std::cin >> this->menoSuboru;
			this->zoznam.clear();
			this->hierarchia.clear();
			this->run();

		}
		else if (input == "5")
		{
			this->runProgram = false;
		}
		else
		{
			std::cout << "Zadali ste nespr�vnu oper�ciu" << std::endl;
		}*/
	}


}

void Program::runStage1()
{
	this->zoznam.clear();
	RoutingTable().readTableForSequence(this->menoSuboru, this->zoznam);
	ds::amt::ImplicitSequence<Route> platneZaznamy;
	bool runStage1Program = true;
	std::string input;
	while (runStage1Program)
	{
		bool zobrazujZaznamy = false;
		std::cout << "Vyber si predik�t 1/2:" << std::endl;
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

			Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::filter(zoznam.begin(), zoznam.end(),
				[&ip](auto zdroj) -> bool {
					return Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::matchWithAddress(zdroj, ip);
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

			Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::filter(zoznam.begin(), zoznam.end(),
				[&lifetime](auto zdroj) -> bool {
					return Algorithms<ds::amt::ImplicitSequence<Route>::IteratorType, Route, std::string>::matchLifetime(zdroj, lifetime);
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
			std::cout << "Zadali ste nespr�vnu oper�ciu" << std::endl;
		}


		while (zobrazujZaznamy)
		{
			if (platneZaznamy.size() == 0 && (input == "1" || input == "2"))
			{
				std::cout << "Neboli n�jden� �iadne zhody" << std::endl;
			}
			else
			{
				for (auto& udaj : platneZaznamy)
				{
					udaj.toString();
				}

				this->stage_4(zobrazujZaznamy, platneZaznamy);
			}
			/*std::cout << "Chcete pokra�ova� v zobrazen� z�znamov? [A/N]" << std::endl;
			std::string pokracovat;
			std::cin >> pokracovat;

			if (pokracovat._Equal("N") || pokracovat._Equal("n"))
			{
				UkonciZobrazenieZaznamov = true;
			}

			}*/

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
	this->hierarchia.clear();
	ds::amt::ImplicitSequence<Route*> rZoznam;

	auto koren = Octet("RT KORE�"); // <-- REPREZENTUJE KORE� HIERARCHIE
	this->hierarchia.emplaceRoot().data_ = koren;
	RoutingTable().readTableForHierarchy(this->menoSuboru, &hierarchia, rZoznam);

	ds::amt::MultiWayExplicitHierarchyBlock<Octet>* aktualnaVetva = this->hierarchia.accessRoot();
	std::string inputOperation;
	while (runStage2Program)
	{
		system("cls");
		switch (indexVetvy)
		{

		case 0:
			std::cout << "Aktu�lny vrchol: " << aktualnaVetva->data_.octet << " [Stupe�: " << indexVetvy << "]" << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_.octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 1:
			std::cout << "Aktu�lny vrchol: " << aktualnaVetva->data_.octet << " [Stupe�: " << indexVetvy << "]" << " IP: " << oktet1 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_.octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 2:
			std::cout << "Aktu�lny vrchol: " << aktualnaVetva->data_.octet << " [Stupe�: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_.octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 3:
			std::cout << "Aktu�lny vrchol: " << aktualnaVetva->data_.octet << " [Stupe�: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << "->" << oktet3 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_.octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 4:
			std::cout << "Aktu�lny vrchol: " << aktualnaVetva->data_.octet << " [Stupe�: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << "->" << oktet3 << "->" << oktet4 << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_.octet << std::endl;
			}
			std::cout << std::endl;
			break;
		case 5:
			std::cout << "Aktu�lny vrchol: " << aktualnaVetva->data_.octet << " [Stupe�: " << indexVetvy << "]" " IP: " << oktet1 << "->" << oktet2 << "->" << oktet3 << "->" << oktet4 << "->" << prefix << std::endl;
			for (int i = 0; i < aktualnaVetva->sons_->size(); ++i)
			{
				std::cout << "[" << i << "]" << aktualnaVetva->sons_->access(i)->data_->data_.octet << std::endl;
			}
			std::cout << std::endl;
			break;
		}

		std::cout << "Vyber si oper�ciu:" << std::endl;
		std::cout << "[1] - Presu� sa na nadraden� oktet." << std::endl;
		std::cout << "[2] - Presu� sa na podraden� oktet. " << std::endl;
		std::cout << "[3] - Prejdi podraden� vetvy predik�tmi z prvej �rovne." << std::endl;
		std::cout << "[4] - Ukon�i druh� �rove�. " << std::endl;
		std::cout << "Vo�ba: ";

		std::cin >> inputOperation;
		switch (std::stoi(inputOperation))
		{

		case 1:
		{
			if (aktualnaVetva->parent_ != nullptr)
			{
				aktualnaVetva = this->hierarchia.accessParent(*aktualnaVetva);
				indexVetvy--;
			}
			break;
		}
		case 2:
		{
			if (aktualnaVetva->sons_->size() != 0)
			{
				std::cout << "Zadaj index syna, na ktor�ho sa chce� presun��: ";
				std::string index;
				std::cin >> index;
				int indexSyna = std::stoi(index);
				aktualnaVetva = hierarchia.accessSon(*aktualnaVetva, indexSyna);
				indexVetvy++;
				switch (indexVetvy)
				{
				case 1:
					oktet1 = aktualnaVetva->data_.octet;
					break;
				case 2:
					oktet2 = aktualnaVetva->data_.octet;
					break;
				case 3:
					oktet3 = aktualnaVetva->data_.octet;
					break;
				case 4:
					oktet4 = aktualnaVetva->data_.octet;
					break;
				case 5:
					prefix = aktualnaVetva->data_.octet;
					break;
				}
			}
			break;
		}
		case 3:
		{
			Route init;
			Route& route = init;

			bool zobrazujZaznamy = true;

			std::string vyberMetody;
			std::string predikat;
			system("cls");

			std::cout << "Vyber si predik�t:" << std::endl;
			std::cout << "1. matchWithAddress" << std::endl;
			std::cout << "2. matchLifetime" << std::endl;
			std::cout << "3. Koniec" << std::endl;

			std::cout << "Predik�t: ";
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
					[&predikat, &route](Octet prehladavane)->bool
					{
						return prehladavane.referenceToRoute != nullptr ?
							Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Route, std::string>::matchWithAddress(*prehladavane.referenceToRoute, predikat) : false;
					},
					[&platneZaznamy](Octet zaznam)->void
					{
						platneZaznamy.insertLast().data_ = *zaznam.referenceToRoute;
					}
				);
				break;
			case 2:
				Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Octet, std::string>::filter(begin, hierarchia.end(),
					[&predikat, &route](Octet prehladavane)->bool
					{
						return prehladavane.referenceToRoute != nullptr ?
							Algorithms<ds::amt::MultiWayExplicitHierarchy<Octet>::PreOrderHierarchyIterator, Route, std::string>::matchLifetime(*prehladavane.referenceToRoute, predikat) : false;
					},
					[&platneZaznamy](Octet zaznam)->void
					{
						platneZaznamy.insertLast().data_ = *zaznam.referenceToRoute;
					}
				);
				break;
			}

			while (zobrazujZaznamy)
			{
				if (platneZaznamy.size() == 0 && (vyberMetody == "1" || vyberMetody == "2"))
				{
					std::cout << "Neboli n�jden� �iadne zhody" << std::endl;
				}
				else
				{
					for (auto& udaj : platneZaznamy)
					{
						udaj.toString();
					}

					this->stage_4(zobrazujZaznamy, platneZaznamy);
				}
				/*std::cout << "Chcete pokra�ova� v zobrazen� z�znamov? [A/N]" << std::endl;
				std::string pokracovat;
				std::cin >> pokracovat;

				if (pokracovat._Equal("N") || pokracovat._Equal("n"))
				{
					UkonciZobrazenieZaznamov = true;
				}

				}*/

			}
			break;
		}
		case 4:
		{
			this->hierarchia.clear();

			for (auto& route : rZoznam)
			{
				delete route;
			}

			rZoznam.clear();
			runStage2Program = false;
			this->run();
			break;
		}
		default: std::cout << "Zadali ste nespr�vnu oper�ciu" << std::endl;
		}
	}
}

void Program::runStage3()
{
	/*auto hashFunction = [](const std::string& key)
		{
			static size_t randomizer = 0;
			size_t hash = 0;
			for (char ch : key) {
				hash = hash * 2 + ch;
			}
			hash += randomizer;
			randomizer += 5;
			return hash;
		};*/

	ds::adt::HashTable<std::string, Route> hashTable([](const std::string& kluc)
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
		}, this->zoznam.size());

	RoutingTable().loadDataToTable(&hashTable, this->zoznam);

	bool runStage3Program = true;
	std::string input;
	while (runStage3Program)
	{
		std::cout << "1. Zobrazenie IP podla next-hop" << std::endl;
		std::cout << "2. Koniec" << std::endl;
		std::cout << "Vyberte si oper�ciu: ";
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
					(*iterator).data_.toString();
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

void Program::stage_4(bool& zobrazujZaznamy, ds::amt::ImplicitSequence<Route> sekvencia) const
{
	if (this->runProgram)
	{
		std::string sortovat;
		std::cout << "Chcete zoradi� z�znamy? [A/N]" << std::endl;
		std::cin >> sortovat;
		std::string vyberSortovania;
		if (sortovat._Equal("A") || sortovat._Equal("a"))
		{

			std::cout << "1. comparePrefix (IP adresa v prefixe prv�ho z�znamu je ni��ia ako IP adresa v prefixe druh�ho z�znamu)" << std::endl;
			std::cout << "2. compareTime (�as pr�tomnosti prv�ho z�znamu je ni��� ako �as pr�tomnosti druh�ho z�znamu)" << std::endl;
			std::cout << "Vyberte si sp�sob zoradenia: ";
			std::cin >> vyberSortovania;
		}
		else if (sortovat._Equal("N") || sortovat._Equal("n"))
		{
			zobrazujZaznamy = false;

		}

		if (zobrazujZaznamy)
		{
			switch (std::stoi(vyberSortovania))
			{
			case 1:
				SortingAlgorithm::comparePrefix(&sekvencia, [](Route prvyKontrolovany, Route druhyKontrolovany)->bool
					{
						//IP adresa v prefixe prv�ho z�znamu je ni��ia ako IP adresa v prefixe druh�ho z�znamu.Ak s� IP adresy v prefixoch rovnak�, tak sa porovn�, �i je maska v prefixe prv�ho z�znamu men�ia ako maska v prefixe druh�ho z�znamu.
						uint32_t ipPrva = SortingAlgorithm().ipToInteger(prvyKontrolovany.getIpAdress());
						uint32_t ipDruha = SortingAlgorithm().ipToInteger(druhyKontrolovany.getIpAdress());
						if (ipPrva < ipDruha) {
							return true;  // IP adresa prv�ho prefixu je men�ia
						}
						else if (ipPrva == ipDruha) {
							return prvyKontrolovany.getPrefix() < druhyKontrolovany.getPrefix();  // Ak s� IP adresy rovnak�, porovn�me masky
						}
						return false;

						return ipPrva < ipDruha ? true : prvyKontrolovany.getPrefix() < druhyKontrolovany.getPrefix();
					});
				break;
			case 2:
				SortingAlgorithm::compareTime(&sekvencia, [](Route prvyKontrolovany, Route druhyKontrolovany)->bool
					{
						//�as pr�tomnosti prv�ho z�znamu je ni��� ako �as pr�tomnosti druh�ho z�znamu.

						return prvyKontrolovany.getLifeTimeSecondsInt() != druhyKontrolovany.getLifeTimeSecondsInt() ? prvyKontrolovany.getLifeTimeSecondsInt() < druhyKontrolovany.getLifeTimeSecondsInt() : false;
					});
				break;
			}
			//bool vysledky = true;
			std::string koniecZobrazenia;
			while (zobrazujZaznamy)
			{
				for (auto& udaj : sekvencia)
				{
					udaj.toString();
				}
				std::cout << "Chcete pokra�ova� v zobrazen� z�znamov? [A/N]" << std::endl;
				std::cin >> koniecZobrazenia;
				if (koniecZobrazenia._Equal("N") || koniecZobrazenia._Equal("n"))
				{
					zobrazujZaznamy = false;
				}
			}
		}
	}
}

Program::~Program()
{
	zoznam.clear();
	hierarchia.clear();
}
