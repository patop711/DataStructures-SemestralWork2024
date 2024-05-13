#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Route.h"
#include "libds/amt/implicit_sequence.h"
#include "libds/amt/explicit_hierarchy.h"
#include "libds/adt/table.h"
#include "octet.h"

/*
* Trieda ktorá spracúvava dáta zo súboru a poskytuje operacie
*/
class RoutingTable
{
public:
	RoutingTable();
	~RoutingTable();
	void readTableForSequence(std::string menoSuboru, ds::amt::ImplicitSequence<Route>& sekvencia);
	void readTableForHierarchy(std::string menoSuboru, ds::amt::MultiWayExplicitHierarchy<Octet>* hierarchia, ds::amt::ImplicitSequence<Route*>& sekvencia);
	void loadDataToTable(ds::adt::HashTable<std::string, Route>* table, ds::amt::ImplicitSequence<Route> routes);
};

RoutingTable::RoutingTable()
{
}

void RoutingTable::readTableForSequence(std::string menoSuboru, ds::amt::ImplicitSequence<Route>& sekvencia)
{
	std::ifstream csv(menoSuboru);
	std::string line;
	std::string ipAdress;
	std::string mask;
	std::string nextHopIpAdress;
	std::string time;

	if (csv.is_open())
	{
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

				sekvencia.insertLast().data_ = Route(ipAdress, mask, nextHopIpAdress, time);
				tokens.clear();
				ss.clear();
			}
		}
	}
	else
	{
		throw std::runtime_error("Súbor " + menoSuboru + " sa nepodarilo otvoriť, alebo neexistuje!");
	}
	csv.close();
}

void RoutingTable::readTableForHierarchy(std::string menoSuboru, ds::amt::MultiWayExplicitHierarchy<Octet>* hierarchia, ds::amt::ImplicitSequence<Route*>& sekvencia)
{
	std::ifstream csv(menoSuboru);
	std::string line = "";
	std::string ipAdress = "";
	std::string mask = "";
	std::string nextHopIpAdress = "";
	std::string time = "";

	std::string oktet1 = "";
	std::string oktet2 = "";
	std::string oktet3 = "";
	std::string oktet4 = "";
	std::string prefix = "";

	size_t indexOfOktet1 = 0; //<-- index oktetu 1
	size_t indexOfOktet2 = 0; //<-- index oktetu 2
	size_t indexOfOktet3 = 0; //<-- index oktetu 3
	size_t indexOfOktet4 = 0; //<-- index oktetu 4
	size_t indexOfPrefixOktet5 = 0; //<-- index prefixu

	auto koren = hierarchia->accessRoot(); //<-- koren
	ds::amt::MultiWayExplicitHierarchyBlock<Octet>* blockOktet1 = koren; //<-- oktet1
	ds::amt::MultiWayExplicitHierarchyBlock<Octet>* blockOktet2 = koren; //<-- oktet2
	ds::amt::MultiWayExplicitHierarchyBlock<Octet>* blockOktet3 = koren; //<-- oktet3
	ds::amt::MultiWayExplicitHierarchyBlock<Octet>* blockOktet4 = koren; //<-- oktet4
	ds::amt::MultiWayExplicitHierarchyBlock<Octet>* blockOktet5 = koren; //<-- prefix


	if (csv.is_open())
	{
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

				oktet1 = ipAdress.substr(0, ipAdress.find("."));
				oktet2 = ipAdress.substr(ipAdress.find(".") + 1, ipAdress.find(".", ipAdress.find(".") + 1) - ipAdress.find(".") - 1);
				oktet3 = ipAdress.substr(ipAdress.find(".", ipAdress.find(".") + 1) + 1, ipAdress.find(".", ipAdress.find(".", ipAdress.find(".") + 1) + 1) - ipAdress.find(".", ipAdress.find(".") + 1) - 1);
				oktet4 = ipAdress.substr(ipAdress.find(".", ipAdress.find(".", ipAdress.find(".") + 1) + 1) + 1);
				prefix = "/" + mask;

				Route* route = new Route(ipAdress, mask, nextHopIpAdress, time);
				sekvencia.insertLast().data_ = route;
				tokens.clear();
				ss.clear();

					//Vytvorenie hierarchie v ktorej sa už vyskytujú vrcholy oktetov a prefixov kde v prefix je odkaaz na záznam z IS
					/*
					*						_____
					*					   |KOREŇ|
					*						-----
					*					 /
					*					|
					*				   |0| <-- OKTET 1
					*					|
					*				   |0| <-- OKTET 2
					*					|
					*				   |0| <-- OKTET 3
					*					|
					*				   |0| <-- OKTET 4
					*					|
					*				  |/0| <-- OKTETO-PREFIX
					*					|
					*				|ZAZNAM| <-- ZAZNAM
					*/
				//KROKY PRE PRIDANIE ZAZNAMOV DO HIERARCHIE
				for (size_t i = 1; i <= 5; i++)
				{
					if (i == 1)
					{
						/*
						* Tu sa kontroluje ci sa v hierarchii náhodou nenachádza už oktet 1 s hodnotou ktorá sa nachádza v hierarchii,
						* ak ano tak sa nastaví blockOktet1 na tento oktet 1, ak sa ešte nenachádza v hierarchii tak sa vytvorí nový oktet 1 a nastaví sa blockOktet1 na tento nový oktet 1
						*
						*/
						bool foundOktet1 = false;
						size_t velkostKorena = hierarchia->accessRoot()->sons_->size();

						if (velkostKorena == 0) //<-- ak sa este nevytvorila ziadna vetva oktetov a prefixu ktorý je spojeny zo záznamom z IS
						{
							//indexOfOktet1 = blockOktet1->sons_->size();
							//indexOfOktet1, 

							indexOfOktet1 = 0;
							indexOfOktet2 = 0;
							indexOfOktet3 = 0;
							indexOfOktet4 = 0;
							indexOfPrefixOktet5 = 0; //<-- indexy oktetov a prefixu su nastavene na nulu

							blockOktet1 = &hierarchia->emplaceSon(*koren, indexOfOktet1); //<-- vytvorenie oktetu 1 od korena
							blockOktet1->data_ = Octet(oktet1); //<-- priradenie dát do oktetu 1
							indexOfOktet1++; //<-- inkrementacia indexu oktetu o 1

							blockOktet1 = hierarchia->accessSon(*koren, indexOfOktet1 - 1); //<-- nastavenie blockOktet1 na oktet 1 ktorý sme práve vytvorili
						}
						else //<-- ak sa uz vytvorili nejake vetvy oktetov od korena
						{
							for (size_t j = 0; j < velkostKorena; j++) //<-- prejdeme for cyklom cez všetky oktety prvého stupňa
							{
								/*
								* Tu sa kontroluje ci sa v hierarchii náhodou nenachádza už oktet 1 s rovnakou hodnotou
								*/
								if (hierarchia->accessSon(*hierarchia->accessRoot(), j)->data_.octet._Equal(oktet1)) //<-- ak sa v hierarchii nachádza oktet 1 s rovnakou hodnotou ako je oktet 1 z IS
								{
									blockOktet1 = hierarchia->accessSon(*hierarchia->accessRoot(), j); //<-- nastavíme blockOktet1 na tento oktet 1
									foundOktet1 = true; //<-- nastavíme foundOktet1 na true
									break; //<-- ukončíme for cyklus
								}
							}
							if (!foundOktet1) //<-- inak ak sa nenašiel oktet 1 s rovnakou hodnotou ako je oktet 1 z IS
							{
								blockOktet1 = &hierarchia->emplaceSon(*hierarchia->accessRoot(), indexOfOktet1); //<-- vytvoríme nový oktet 1 prvého stupňa/levelu a nastavíme blockOktet1 na tento nový oktet 1
								blockOktet1->data_ = Octet(oktet1); //<-- priradíme dáta do oktetu 1

								//Zresetujeme indexy oktetov a prefixu jeho nasledovníkov
								indexOfOktet2 = 0; //<-- index oktetu 2
								indexOfOktet3 = 0; //<-- index oktetu 3
								indexOfOktet4 = 0; //<-- index oktetu 4
								indexOfPrefixOktet5 = 0; //<-- index prefixu
								//<-- indexy oktetov a prefixu sa nastavia znova na nulu kedze sme už vytvorili nový oktet 1

								indexOfOktet1++; //<-- inkrementujeme index oktetu 1 o 1 (od korena)

								blockOktet1 = hierarchia->accessSon(*hierarchia->accessRoot(), indexOfOktet1 - 1); //<-- nastavíme blockOktet1 na oktet 1 ktorý sme práve vytvorili
								//indexOfOktet1 - 1 je preto aby sme sa dostali na oktet 1 ktorý sme práve vytvorili(t.j. môže byt napríklad velkost
								//korena 1 a ked vytvorime dalsi tak sa zvysi velkost korena na 2 a my chceme aby sme sa dostali na oktet 1 ktorý sme práve vytvorili
							}
						}
					}
					else if (i == 2) //<-- ak sa jedná o druhý oktet
					{
						bool foundOktet2 = false;
						size_t velkostBlokuOktet1 = blockOktet1->sons_->size();

						if (velkostBlokuOktet1 == 0) //<-- ak sa ešte nevytvorili žiadne vetvy oktetov a prefixov v aktuaálnom oktete(blockOktet1)
						{
							indexOfOktet2 = 0; //<-- index oktetu 2
							indexOfOktet3 = 0; //<-- index oktetu 3
							indexOfOktet4 = 0; //<-- index oktetu 4
							indexOfPrefixOktet5 = 0; //<-- index prefixu
							//<-- indexy oktetov a prefixu sa nastavia znova na nulu kedze sme už vytvorili nový oktet 1

							blockOktet2 = &hierarchia->emplaceSon(*blockOktet1, indexOfOktet2); //<-- vytvorenie oktetu 2 ako potomka od oktetu 1
							blockOktet2->data_ = Octet(oktet2); //<-- priradenie dát do oktetu 2
							indexOfOktet2++; //<-- inkrementacia indexu oktetu2 o +1

							blockOktet2 = hierarchia->accessSon(*blockOktet1, indexOfOktet2 - 1); //<-- nastavenie blockOktet2 na oktet 2 ktorý sme práve vytvorili
						}
						else //<-- ak sa už vytvorili nejaké vetvy oktetov a prefixov v aktuálnom oktete(blockOktet1)
						{
							for (size_t j = 0; j < velkostBlokuOktet1; j++) //<-- prejdeme for cyklom cez vsetky bloky oktetov 2 ktoré sa nachádzajú v oktete 1
							{
								//hierarchia->accessSon(*blockOktet1, j)->data_.octet == oktet2
								if (blockOktet1->sons_->access(j)->data_->data_.octet._Equal(oktet2)) //<-- ak sa v oktete 1 nachádza oktet 2 s rovnakou hodnotou ako je oktet 2 z IS
								{
									blockOktet2 = hierarchia->accessSon(*blockOktet1, j); //<-- nastavíme blockOktet2 na tento oktet 2
									foundOktet2 = true; //<-- nastavíme foundOktet2 na true
									break; //<-- ukončíme for cyklus
								}
								
							}
							if (!foundOktet2) //<-- inak ak sa nenašiel oktet 2 s rovnakou hodnotou ako je oktet 2 z IS
							{
								indexOfOktet2 = velkostBlokuOktet1; //<-- index oktetu 2 sa nastaví na veľkosť bloku oktetov 2 (aj ked je nula tak sa nastaví na 0)
								blockOktet2 = &hierarchia->emplaceSon(*blockOktet1, indexOfOktet2); //<-- vytvoríme nový oktet 2 ako syna od oktetu 1 a dam mu index
								blockOktet2->data_ = Octet(oktet2); //<-- priradíme dáta do oktetu 2

								//indexOfOktet2 = 0; //<-- index oktetu 2
								indexOfOktet3 = 0; //<-- index oktetu 3
								indexOfOktet4 = 0; //<-- index oktetu 4
								indexOfPrefixOktet5 = 0; //<-- index prefixu

								indexOfOktet2++; //<-- inkrementujeme index oktetu 2 o +1

								blockOktet2 = hierarchia->accessSon(*blockOktet1, indexOfOktet2 - 1); //<-- nastavíme refernciu na blockOktet2 na oktet 2 ktorý sme práve vytvorili
							}
						}
					}
					else if (i == 3) //<-- ak sa jedná o tretí oktet
					{
						bool foundOktet3 = false;
						size_t velkostBlokuOktet2 = blockOktet2->sons_->size(); //<-- zistíme veľkosť bloku oktetov 2, preto lebo sa nachádzame v oktete 2 a chceme vedieť koľko oktetov 3 sa v ňom nachádza //VZDY TO PADNE TU!!!

						if (velkostBlokuOktet2 == 0) //<-- ak sa ešte nevytvorili žiadne vetvy oktetov alebo prefixov v aktuaálnom oktete predka(blockOktet2)
						{
							//indexOfOktet1 = blockOktet1->sons_->size();
							//indexOfOktet1, 
							indexOfOktet3 = 0;
							indexOfOktet4 = 0;
							indexOfPrefixOktet5 = 0; //<-- indexy oktetov a prefixu sú nastavené na nulu 

							blockOktet3 = &hierarchia->emplaceSon(*blockOktet2, indexOfOktet3); //<-- vytvorenie oktetu 3 ako potomka od oktetu 2
							blockOktet3->data_ = Octet(oktet3); //<-- priradenie dát do oktetu 3
							indexOfOktet3++; //<-- inkrementacia indexu oktetu3 o +1

							blockOktet3 = hierarchia->accessSon(*blockOktet2, indexOfOktet3 - 1); //<-- nastavenie blockOktet3 na oktet 3 ktorý sme práve vytvorili
						}
						else //<-- ak sa už vytvorili nejaké vetvy oktetov a prefixov v aktuálnom oktete predka(blockOktet2)
						{
							for (size_t j = 0; j < velkostBlokuOktet2; j++) //<-- prejdeme for cyklom cez vsetky bloky oktetov 3 ktoré sa nachádzajú v oktete 2
							{
								//hierarchia->accessSon(*blockOktet1, j)->data_.octet == oktet2
								if (blockOktet2->sons_->access(j)->data_->data_.octet._Equal(oktet3)) //<-- ak sa v oktete 2 nachádza oktet 3 s rovnakou hodnotou ako je oktet 3 z IS
								{
									blockOktet3 = hierarchia->accessSon(*blockOktet2, j); //<-- nastavíme blockOktet3 na tento oktet 3
									foundOktet3 = true; //<-- nastavíme foundOktet3 na true
									break; //<-- ukončíme for cyklus
								}
								
							}
							if (!foundOktet3) //<-- inak ak este neexistuje oktet 3 s rovnakou hodnotou ako je oktet 3 z IS ale ine ano tak vytvoríme nový oktet 3
							{
								indexOfOktet3 = velkostBlokuOktet2; //<-- index oktetu 3 sa nastaví na veľkosť bloku oktetov 3 
								blockOktet3 = &hierarchia->emplaceSon(*blockOktet2, indexOfOktet3); //<-- vytvoríme nový oktet 3 ako syna od oktetu 2 a dam mu index
								blockOktet3->data_ = Octet(oktet3); //<-- priradíme dáta do oktetu 3

								//indexOfOktet3 = 0; //<-- index oktetu 3
								indexOfOktet4 = 0; //<-- index oktetu 4
								indexOfPrefixOktet5 = 0; //<-- index prefixu

								indexOfOktet3++; //<-- inkrementujeme index oktetu 3 o +1

								blockOktet3 = hierarchia->accessSon(*blockOktet2, indexOfOktet3 - 1); //<-- nastavíme referenciu blockOktet3 na oktet 3 ktorý sme práve vytvorili
							}
						}
					}
					else if (i == 4) //<-- ak sa jedná o štvrtý oktet
					{
						bool foundOktet4 = false;
						size_t velkostBlokuOktet3 = blockOktet3->sons_->size(); //<-- zistíme veľkosť bloku oktetov 3, preto lebo sa nachádzame v oktete 3 a chceme vedieť koľko oktetov 4 sa v ňom nachádza

						if (velkostBlokuOktet3 == 0) //<-- ak sa ešte nevytvorili žiadne vetvy oktetov alebo prefixov v aktuaálnom oktete predka(blockOktet3)
						{
							indexOfOktet4 = 0;
							indexOfPrefixOktet5 = 0; //<-- indexy oktetov a prefixu sú nastavené na nulu 

							blockOktet4 = &hierarchia->emplaceSon(*blockOktet3, indexOfOktet4); //<-- vytvorenie oktetu 4 ako potomka od oktetu 3
							blockOktet4->data_ = Octet(oktet4); //<-- priradenie dát do oktetu 4
							indexOfOktet4++; //<-- inkrementacia indexu oktetu4 o +1

							blockOktet4 = hierarchia->accessSon(*blockOktet3, indexOfOktet4 - 1); //<-- nastavenie blockOktet4 na oktet 4 ktorý sme práve vytvorili
						}
						else //<-- ak sa už vytvorili nejaké vetvy oktetov a prefixov v aktuálnom oktete predka(blockOktet3)
						{
							for (size_t j = 0; j < velkostBlokuOktet3; j++) //<-- prejdeme for cyklom cez vsetky bloky oktetov 4 ktoré sa nachádzajú v oktete 3
							{
								//hierarchia->accessSon(*blockOktet1, j)->data_.octet == oktet2
								if (blockOktet3->sons_->access(j)->data_->data_.octet._Equal(oktet4)) //<-- ak sa v oktete 3 nachádza oktet 4 s rovnakou hodnotou ako je oktet 4 z IS
								{
									blockOktet4 = hierarchia->accessSon(*blockOktet3, j); //<-- nastavíme blockOktet4 na tento oktet 4
									foundOktet4 = true; //<-- nastavíme foundOktet4 na true
									break; //<-- ukončíme for cyklus ked sme nasli zhodu
								}
							}
							if (!foundOktet4) //<-- inak ak este neexistuje oktet 4 s rovnakou hodnotou ako je oktet 4 z IS ale ine ano tak vytvoríme nový oktet 4
							{
								indexOfOktet4 = velkostBlokuOktet3; //<-- index oktetu 4 sa nastaví na veľkosť bloku oktetov 4 
								blockOktet4 = &hierarchia->emplaceSon(*blockOktet3, indexOfOktet4); //<-- vytvoríme nový oktet 4 ako syna od oktetu 3 a dam mu index
								blockOktet4->data_ = Octet(oktet4); //<-- priradíme dáta do oktetu 4

								indexOfPrefixOktet5 = 0;

								indexOfOktet4++; //<-- inkrementujeme index oktetu 4 o +1

								blockOktet4 = hierarchia->accessSon(*blockOktet3, indexOfOktet4 - 1); //<-- nastavíme blockOktet4 na oktet 4 ktorý sme práve vytvorili
							}
						}
					}
					else if (i == 5) //<-- ak sa jedná o prefix ktorý je spojený so záznamom z IS
					{
						bool foundPrefix = false;

						size_t velkostBlokuOktet4 = blockOktet4->sons_->size(); //<-- zistíme veľkosť bloku oktetov 4, preto lebo sa nachádzame v oktete 4 a chceme vedieť koľko prefixov sa v ňom nachádza

						if (velkostBlokuOktet4 == 0) //<-- ak sa ešte nevytvorili žiadne vetvy oktetov alebo prefixov v aktuaálnom oktete predka(blockOktet4)
						{
							indexOfPrefixOktet5 = 0; //<-- index prefixu sú nastavený na nulu

							blockOktet5 = &hierarchia->emplaceSon(*blockOktet4, indexOfPrefixOktet5); //<-- vytvorenie prefixu ako potomka od oktetu 4
							blockOktet5->data_ = Octet(prefix, sekvencia.accessLast()->data_); //<-- priradenie dát do prefixu
							indexOfPrefixOktet5++; //<-- inkrementacia indexu prefixu o +1

							blockOktet5 = hierarchia->accessSon(*blockOktet4, indexOfPrefixOktet5 - 1); //<-- nastavenie blockOktet5 na prefix ktorý sme práve vytvorili
						}
						else //<-- ak sa už vytvorili nejaké vetvy oktetov a prefixov v aktuálnom oktete predka(blockOktet4)
						{
							for (size_t j = 0; j < velkostBlokuOktet4; j++) //<-- prejdeme for cyklom cez vsetky bloky prefixov ktoré sa nachádzajú v oktete 4
							{
								if (blockOktet4->sons_->access(j)->data_->data_.octet._Equal(prefix)) //<-- ak sa v oktete 4 nenachádza oktet(prefix) 4 s rovnakou hodnotou ako je oktet 4 z IS
								{
									foundPrefix = true; //<-- nastavíme foundPrefix na true
								}
							}

							if (!foundPrefix) //<-- inak ak este neexistuje prefix s rovnakou hodnotou ako je prefix z IS ale ine ano tak vytvoríme nový prefix
							{
								indexOfPrefixOktet5 = velkostBlokuOktet4; //<-- index prefixu sa nastaví na veľkosť bloku prefixov (ak je nula tak sa nastaví na 0)
								blockOktet5 = &hierarchia->emplaceSon(*blockOktet4, indexOfPrefixOktet5); //<-- vytvoríme nový prefix ako syna od oktetu 4 a dam mu index
								blockOktet5->data_ = Octet(prefix, sekvencia.accessLast()->data_); //<-- priradíme dáta do prefixu
								indexOfPrefixOktet5++; //<-- inkrementujeme index prefixu o +1

								blockOktet5 = hierarchia->accessSon(*blockOktet4, indexOfPrefixOktet5 - 1); //<-- nastavíme blockOktet5 na prefix ktorý sme práve vytvorili
							}
						}
					}

				}

			}
		}
	}
	else
	{
		throw std::runtime_error("Súbor " + menoSuboru + " sa nepodarilo otvoriť, alebo neexistuje!");
	}
	csv.close();
}

void RoutingTable::loadDataToTable(ds::adt::HashTable<std::string, Route>* table, ds::amt::ImplicitSequence<Route> routes)
{
	auto it = routes.begin();
	auto end = routes.end();

	while (it != end)
	{
		table->insert((*it).getNextHopIpAdress(), *it);
		++it;
	}

	//for (size_t i = 0; i < routes.size(); i++)
	//{
	//	//std::cout << routes.access(i)->data_.getIpAdress() << std::endl;
	//	table->insert(routes.access(i)->data_.getNextHopIpAdress(), routes.access(i)->data_);
	//}
	std::cout << table->size() << std::endl;
}

RoutingTable::~RoutingTable()
{
}