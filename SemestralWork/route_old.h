#pragma once
#include <string>
#include <bitset>
using USHORT = unsigned short int;
class Route
{
private:
	std::string ipAdress = "";
	std::string mask = "";
	std::string nextHopIpAdress = "";
	std::string time = "";
	//Oktety ip adresy
	std::bitset<8> ipAdressOctet1 = 0;
	std::bitset<8> ipAdressOctet2 = 0;
	std::bitset<8> ipAdressOctet3 = 0;
	std::bitset<8> ipAdressOctet4 = 0;
public:
	Route(std::string ipAdress, std::string mask, std::string nextHopIpAdress, std::string time) :
		ipAdress(ipAdress), mask(mask), nextHopIpAdress(nextHopIpAdress), time(time) 
	{
		setIpAdressOctets(ipAdress);
	}

	// Getters
	unsigned int getPrefix() const { return std::stoi(mask); }
	std::string getIpAdress() const { return ipAdress; }
	std::string getMask() const { return mask; }
	std::string getNextHopIpAdress() const { return nextHopIpAdress; }
	std::string getTime() const { return time; }
	std::bitset<8> getIpAdressOctet1() const { return ipAdressOctet1; }
	std::bitset<8> getIpAdressOctet2() const { return ipAdressOctet2; }
	std::bitset<8> getIpAdressOctet3() const { return ipAdressOctet3; }
	std::bitset<8> getIpAdressOctet4() const { return ipAdressOctet4; }
	std::string getIpAdressOctets() const { return ipAdressOctet1.to_string() + ipAdressOctet2.to_string() + ipAdressOctet3.to_string() + ipAdressOctet4.to_string(); }

	// Setters
	void setIpAdressOctets(std::string ipAdress) {
		// Split the ipAdress into 4 octets by the "." separator like 1.1.1.1
		std::string octet1 = ipAdress.substr(0, ipAdress.find("."));
		ipAdress.erase(0, ipAdress.find(".") + 1);
		std::string octet2 = ipAdress.substr(0, ipAdress.find("."));
		ipAdress.erase(0, ipAdress.find(".") + 1);
		std::string octet3 = ipAdress.substr(0, ipAdress.find("."));
		ipAdress.erase(0, ipAdress.find(".") + 1);
		std::string octet4 = ipAdress;

		// Convert the octets to bitsets
		ipAdressOctet1 = std::bitset<8>(std::stoi(octet1));
		ipAdressOctet2 = std::bitset<8>(std::stoi(octet2));
		ipAdressOctet3 = std::bitset<8>(std::stoi(octet3));
		ipAdressOctet4 = std::bitset<8>(std::stoi(octet4));
	}

	bool isPartOfSubnet(std::string ipAdress) {
		/*std::string octet1 = ipAdress.substr(0, ipAdress.find("."));
		ipAdress.erase(0, ipAdress.find(".") + 1);
		std::string octet2 = ipAdress.substr(0, ipAdress.find("."));
		ipAdress.erase(0, ipAdress.find(".") + 1);
		std::string octet3 = ipAdress.substr(0, ipAdress.find("."));
		ipAdress.erase(0, ipAdress.find(".") + 1);
		std::string octet4 = ipAdress;

		std::bitset<8> ipAdressOctet1 = std::bitset<8>(std::stoi(octet1));
		std::bitset<8> ipAdressOctet2 = std::bitset<8>(std::stoi(octet2));
		std::bitset<8> ipAdressOctet3 = std::bitset<8>(std::stoi(octet3));
		std::bitset<8> ipAdressOctet4 = std::bitset<8>(std::stoi(octet4));*/
		for (int i = 0; i < getPrefix(); i++) {
			//chceck if the "i" bit is the same in the ipAdress and the route ipAdress
			if (ipAdress.at(i) != getIpAdressOctets().at(i)) {
				return false;
			}
			//aj ked sa prve cislo rovna tak over aj ostatné aby sme zistili ci je to v danom rozsahu
		}
		return true;
	}
};

