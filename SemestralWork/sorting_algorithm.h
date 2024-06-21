#pragma once
#include "route.h"
#include "libds/adt/sorts.h"
#include "libds/amt/implicit_sequence.h"

/*
* Trieda SortingAlgorithm obsahuje met�dy, ktor� sl��ia na zoradenie z�znamov v pod�a prefixu alebo pod�a �asu.
*/
class SortingAlgorithm
{
private:
	/*
	* Met�da compare porovn�va dva z�znamy pod�a zadan�ho kompar�tora.
	*/
	void compare(ds::amt::ImplicitSequence<Route*>* routes, std::function<bool(Route* prvyKontrolovany, Route* druhyKontrolovany)> comparator)
	{
		ds::adt::QuickSort<Route*> quickSort;
		quickSort.sort(*routes, comparator);
	}
	//////////////////////////////////KOMPARATORY/////////////////////////////////////////////////////
	/*
	* Kompar�tor comparePrefix porovn�va dva z�znamy pod�a prefixu.
	*/
	std::function<bool(Route* prvyKontrolovany, Route* druhyKontrolovany)> comparePrefix = [&](Route* prvyKontrolovany, Route* druhyKontrolovany)->bool
		{
			//IP adresa v prefixe prv�ho z�znamu je ni��ia ako IP adresa v prefixe druh�ho z�znamu.Ak s� IP adresy v prefixoch rovnak�, tak sa porovn�, �i je maska v prefixe prv�ho z�znamu men�ia ako maska v prefixe druh�ho z�znamu.
			uint32_t ipPrva = this->ipToInteger(prvyKontrolovany->getIpAdress());
			uint32_t ipDruha = this->ipToInteger(druhyKontrolovany->getIpAdress());
			if (ipPrva < ipDruha) {
				return true;  // IP adresa prv�ho prefixu je men�ia
			}
			else if (ipPrva == ipDruha) {
				return prvyKontrolovany->getPrefix() < druhyKontrolovany->getPrefix();  // Ak s� IP adresy rovnak�, porovn�me masky
			}
			return false;
		};
	/*
	* Kompar�tor compareTime porovn�va dva z�znamy pod�a �asu.
	*/
	std::function<bool(Route* prvyKontrolovany, Route* druhyKontrolovany)> compareTime = [&](Route* prvyKontrolovany, Route* druhyKontrolovany)
		{
			//�asov� �daj lifetime prv�ho z�znamu je men�� ako �asov� �daj lifetime druh�ho z�znamu.
			return prvyKontrolovany->getLifeTimeSecondsInt() < druhyKontrolovany->getLifeTimeSecondsInt() ? true : false;
		};
	//////////////////////////////////KOMPARATORY/////////////////////////////////////////////////////
	/*
	* Met�da ipToInteger konvertuje 4-oktetov� IP adresu na 32-bitov� ��slo.
	*/
	uint32_t ipToInteger(const std::string& ip)
	{
		//Met�da konvertuje 4-oktetov� IP adresu na 32-bitov� ��slo

		//TODO treba aby to bolo v prv�ch N cislach podla prefixu!!!
		std::vector<uint8_t> bytes;
		std::stringstream ss(ip);
		std::string byte;

		while (std::getline(ss, byte, '.')) {
			bytes.push_back(static_cast<uint8_t>(std::stoi(byte)));
		}


		uint32_t integerIp = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];


		return integerIp;
	}
public:
	/*
	* Met�da chooseSort zavol� met�du compare s kompar�torom pod�a zadan�ho typu zoradenia.
	*/
	void chooseSort(ds::amt::ImplicitSequence<Route*>* routes, int sortType)
	{
		if (sortType == 1) {
			this->compare(routes, comparePrefix);
		}
		else if (sortType == 2) {
			this->compare(routes, compareTime);
		}
		else {
			std::cout << "Zl� index pre sortovanie" << std::endl;
		}
	}
};