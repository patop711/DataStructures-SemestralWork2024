#pragma once
#include "route.h"
#include "libds/adt/sorts.h"
#include "libds/amt/implicit_sequence.h"

/*
* Trieda SortingAlgorithm obsahuje metódy, ktoré slúžia na zoradenie záznamov v pod¾a prefixu alebo pod¾a èasu.
*/
class SortingAlgorithm
{
private:
	/*
	* Metóda compare porovnáva dva záznamy pod¾a zadaného komparátora.
	*/
	void compare(ds::amt::ImplicitSequence<Route*>* routes, std::function<bool(Route* prvyKontrolovany, Route* druhyKontrolovany)> comparator)
	{
		ds::adt::QuickSort<Route*> quickSort;
		quickSort.sort(*routes, comparator);
	}
	//////////////////////////////////KOMPARATORY/////////////////////////////////////////////////////
	/*
	* Komparátor comparePrefix porovnáva dva záznamy pod¾a prefixu.
	*/
	std::function<bool(Route* prvyKontrolovany, Route* druhyKontrolovany)> comparePrefix = [&](Route* prvyKontrolovany, Route* druhyKontrolovany)->bool
		{
			//IP adresa v prefixe prvého záznamu je nižšia ako IP adresa v prefixe druhého záznamu.Ak sú IP adresy v prefixoch rovnaké, tak sa porovná, èi je maska v prefixe prvého záznamu menšia ako maska v prefixe druhého záznamu.
			uint32_t ipPrva = this->ipToInteger(prvyKontrolovany->getIpAdress());
			uint32_t ipDruha = this->ipToInteger(druhyKontrolovany->getIpAdress());
			if (ipPrva < ipDruha) {
				return true;  // IP adresa prvého prefixu je menšia
			}
			else if (ipPrva == ipDruha) {
				return prvyKontrolovany->getPrefix() < druhyKontrolovany->getPrefix();  // Ak sú IP adresy rovnaké, porovnáme masky
			}
			return false;
		};
	/*
	* Komparátor compareTime porovnáva dva záznamy pod¾a èasu.
	*/
	std::function<bool(Route* prvyKontrolovany, Route* druhyKontrolovany)> compareTime = [&](Route* prvyKontrolovany, Route* druhyKontrolovany)
		{
			//Èasový údaj lifetime prvého záznamu je menší ako èasový údaj lifetime druhého záznamu.
			return prvyKontrolovany->getLifeTimeSecondsInt() < druhyKontrolovany->getLifeTimeSecondsInt() ? true : false;
		};
	//////////////////////////////////KOMPARATORY/////////////////////////////////////////////////////
	/*
	* Metóda ipToInteger konvertuje 4-oktetovú IP adresu na 32-bitové èíslo.
	*/
	uint32_t ipToInteger(const std::string& ip)
	{
		//Metóda konvertuje 4-oktetovú IP adresu na 32-bitové èíslo

		//TODO treba aby to bolo v prvých N cislach podla prefixu!!!
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
	* Metóda chooseSort zavolá metódu compare s komparátorom pod¾a zadaného typu zoradenia.
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
			std::cout << "Zlý index pre sortovanie" << std::endl;
		}
	}
};