#pragma once
#include "route.h"
#include <functional>
#include <string>
#include <bitset>
#include "libds/amt/explicit_hierarchy.h"

/*
* Generick· trieda Algorithms sluzi na vykonavanie operacii nad datovou strukturou a vyfiltruje zaznamy podla predikatu
* @param Iterator - iterator prechadzajuci zaznamy
* @param TypUdaju - typ zaznamu
* @param TypPredikatu - typ predikatu
*/
template <typename Iterator,typename TypUdaju, typename TypPredikatu>
class Algorithms
{
private:
	/*
	* MetÛda isPartOfThisSubnet porovn·va ci je zadan· ip adresa s˙Ëasùou danej podsiete s prefixom
	*/
	static bool isPartOfThisSubnet(std::string subnetIP, size_t prefix , std::string targetIP)
	{
		std::string subnetIPBinary = ipToBits(subnetIP);
		std::string targetIPBinary = ipToBits(targetIP);

		for (size_t i = 0; i < prefix; i++) {
			if (targetIPBinary.at(i) != subnetIPBinary.at(i)) {
				return false;
			}
		}
		return true;
	}

	/*
	* MetÛda ipToBits konvertuje 4-oktetov˙ IP adresu na 32-bitov˝ string
	*/
	static std::string ipToBits(std::string ip)
	{
		std::string octet1 = ip.substr(0, ip.find("."));
		ip.erase(0, ip.find(".") + 1);
		std::string octet2 = ip.substr(0, ip.find("."));
		ip.erase(0, ip.find(".") + 1);
		std::string octet3 = ip.substr(0, ip.find("."));
		ip.erase(0, ip.find(".") + 1);
		std::string octet4 = ip;

		std::bitset<8> octet1b(std::stoi(octet1));
		std::bitset<8> octet2b(std::stoi(octet2));
		std::bitset<8> octet3b(std::stoi(octet3));
		std::bitset<8> octet4b(std::stoi(octet4));

		return octet1b.to_string() + octet2b.to_string() + octet3b.to_string() + octet4b.to_string();
	}
public:
	static void filter(Iterator it, Iterator end, std::function<bool(TypUdaju kontrolovany)> kontrolovany, std::function<void(TypUdaju vykonaj)> vykonaj)
	{
		while (it != end)
		{
			if (kontrolovany(*it))
			{
				vykonaj(*it);
			}
			++it;
		}
	}

	static bool matchWithAddress(TypUdaju kontrolovany, TypPredikatu& predikat)
	{
		return isPartOfThisSubnet(kontrolovany->getIpAdress(), kontrolovany->getPrefix(), predikat);
	}
	static bool matchLifetime(TypUdaju kontrolovany, TypPredikatu& predikat)
	{
		return kontrolovany->getLifeTimeSecondsInt() == std::stoi(predikat);
	}
};