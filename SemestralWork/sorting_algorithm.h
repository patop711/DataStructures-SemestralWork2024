#pragma once
#include "route.h"
#include "libds/adt/sorts.h"
#include "libds/amt/implicit_sequence.h"


class SortingAlgorithm
{
public:
	static void comparePrefix(ds::amt::ImplicitSequence<Route>* routes, std::function<bool(Route prvyKontrolovany, Route druhyKontrolovany)> comparator)
	{
		ds::adt::QuickSort<Route> quickSort;
		quickSort.sort(*routes, comparator);
	}

	static void compareTime(ds::amt::ImplicitSequence<Route>* routes, std::function<bool(Route prvyKontorlovany, Route druhyKontrolovany)> comparator)
	{
		ds::adt::QuickSort<Route> quickSort;
		quickSort.sort(*routes, comparator);
	}

	uint32_t ipToInteger(const std::string& ip)
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