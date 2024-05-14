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
				//IP adresa v prefixe prv�ho z�znamu je ni��ia ako IP adresa v prefixe druh�ho z�znamu.Ak s� IP adresy v prefixoch rovnak�, tak sa porovn�, �i je maska v prefixe prv�ho z�znamu men�ia ako maska v prefixe druh�ho z�znamu.
				uint32_t ipPrva = ipToInteger(prvyKontrolovany->getIpAdress());
				uint32_t ipDruha = ipToInteger(druhyKontrolovany->getIpAdress());
				if (ipPrva < ipDruha) {
					return true;  // IP adresa prv�ho prefixu je men�ia
				}
				else if (ipPrva == ipDruha) {
					return prvyKontrolovany->getPrefix() < druhyKontrolovany->getPrefix();  // Ak s� IP adresy rovnak�, porovn�me masky
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
				//�as pr�tomnosti prv�ho z�znamu je ni��� ako �as pr�tomnosti druh�ho z�znamu.

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

		// Konvertuje 4-oktetov� IP adresu na 32-bitov� ��slo
		uint32_t integerIp = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
		return integerIp;
	}
};