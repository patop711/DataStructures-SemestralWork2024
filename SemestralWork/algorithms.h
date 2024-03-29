//#include "route.h"
//#include <string>
//
//auto matchWithAddress = [](Route* address, std::string targetIP) {
//	std::string octet1 = targetIP.substr(0, targetIP.find("."));
//	targetIP.erase(0, targetIP.find(".") + 1);
//	std::string octet2 = targetIP.substr(0, targetIP.find("."));
//	targetIP.erase(0, targetIP.find(".") + 1);
//	std::string octet3 = targetIP.substr(0, targetIP.find("."));
//	targetIP.erase(0, targetIP.find(".") + 1);
//	std::string octet4 = targetIP;
//	std::bitset<8> octet1b(std::stoi(octet1));
//	std::bitset<8> octet2b(std::stoi(octet2));
//	std::bitset<8> octet3b(std::stoi(octet3));
//	std::bitset<8> octet4b(std::stoi(octet4));
//	std::string targetIPBinary = octet1b.to_string() + octet2b.to_string() + octet3b.to_string() + octet4b.to_string();
//	bool partOfSubnet = false;
//
//	for (size_t i = 0; i < address->getPrefix(); i++)
//	{
//		if (targetIPBinary.at(i) != address->getIpAdressOctets().at(i))
//		{
//			partOfSubnet = false;
//		}
//		else
//		{
//			partOfSubnet = true;
//		}
//	}
//	if (partOfSubnet == true)
//	{
//		return true;
//	}
//	return false;
//	};
//
//auto matchLifetime = [](Route* address, std::string lifetime)
//	{
//	if (address->getTime()._Equal(lifetime))
//	{
//		return true;
//	}
//	return false;
//	};