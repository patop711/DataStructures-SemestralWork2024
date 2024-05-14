#pragma once
#include "route.h"
#include "libds/adt/sorts.h"
#include "libds/amt/implicit_sequence.h"


class SortingAlgorithm
{
public:
	static void comparePrefix(ds::amt::ImplicitSequence<Route*>* routes)
	{
		ds::adt::QuickSort<Route*> quickSort;
		quickSort.sort(*routes, [](Route* prvyKontrolovany, Route* druhyKontrolovany)->bool
			{
				//IP adresa v prefixe prvého záznamu je nižšia ako IP adresa v prefixe druhého záznamu.Ak sú IP adresy v prefixoch rovnaké, tak sa porovná, èi je maska v prefixe prvého záznamu menšia ako maska v prefixe druhého záznamu.
				uint32_t ipPrva = ipToInteger(prvyKontrolovany->getIpAdress());
				uint32_t ipDruha = ipToInteger(druhyKontrolovany->getIpAdress());
				if (ipPrva < ipDruha) {
					return true;  // IP adresa prvého prefixu je menšia
				}
				else if (ipPrva == ipDruha) {
					return prvyKontrolovany->getPrefix() < druhyKontrolovany->getPrefix();  // Ak sú IP adresy rovnaké, porovnáme masky
				}
				return false;

				return ipPrva < ipDruha ? true : prvyKontrolovany->getPrefix() < druhyKontrolovany->getPrefix();
			});
	}

	static void compareTime(ds::amt::ImplicitSequence<Route*>* routes)
	{
		ds::adt::QuickSort<Route*> quickSort;
		quickSort.sort(*routes, [](Route* prvyKontrolovany, Route* druhyKontrolovany)->bool
			{
				//Èas prítomnosti prvého záznamu je nižší ako èas prítomnosti druhého záznamu.

				return prvyKontrolovany->getLifeTimeSecondsInt() != druhyKontrolovany->getLifeTimeSecondsInt() ? prvyKontrolovany->getLifeTimeSecondsInt() < druhyKontrolovany->getLifeTimeSecondsInt() : false;
			});
	}
private:
	static uint32_t ipToInteger(const std::string& ip)
	{
		std::vector<uint8_t> bytes;
		std::stringstream ss(ip);
		std::string byte;

		while (std::getline(ss, byte, '.')) {
			bytes.push_back(static_cast<uint8_t>(std::stoi(byte)));
		}

		// Konvertuje 4-oktetovú IP adresu na 32-bitové èíslo
		uint32_t integerIp = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
		return integerIp;
	}
};