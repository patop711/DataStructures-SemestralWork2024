#pragma once
#include <string>
#include <bitset>

/*
* Trieda reprezentujúca jeden záznam v tabu¾ke RT záznamov
* Obsahuje ip adresu, masku, next hop ip adresu a èasový údaj lifetime
*/
class Route
{
private:
	std::string ipAdress;
	std::string mask;
	std::string nextHopIpAdress;
	std::string time;
	size_t lifeTimeSeconds = 0;

	//Oktety ip adresy
	std::bitset<8> ipAdressOctet1;
	std::bitset<8> ipAdressOctet2;
	std::bitset<8> ipAdressOctet3;
	std::bitset<8> ipAdressOctet4;

public:
	Route();
	Route(std::string ipAdress, std::string mask, std::string nextHopIpAdress, std::string time);
	bool isPartOfSubnet(std::string ipAdress);
	size_t getPrefix() const;
	std::string getIpAdress() const;
	std::string getMask() const;
	std::string getNextHopIpAdress() const;
	std::string getTime() const;
	std::string getIpAdressOctets() const;
	std::string toString() const;
	std::string getLifeTimeSeconds() const;
	~Route();

private:
	void setIpAdressOctets(std::string ipAdress);
	void setLifeTimeToSeconds(std::string time);
};

Route::Route()
{
}
Route::Route(std::string ipAdress, std::string mask, std::string nextHopIpAdress, std::string time) :
	ipAdress(ipAdress), mask(mask), nextHopIpAdress(nextHopIpAdress), time(time)
{
	setIpAdressOctets(ipAdress);
	setLifeTimeToSeconds(getTime());
}

void Route::setIpAdressOctets(std::string ipAdress)
{
	// Splitnutie ipAdress do 4 oktetov pomocou "." separatora
	std::string octet1 = ipAdress.substr(0, ipAdress.find("."));
	ipAdress.erase(0, ipAdress.find(".") + 1);
	std::string octet2 = ipAdress.substr(0, ipAdress.find("."));
	ipAdress.erase(0, ipAdress.find(".") + 1);
	std::string octet3 = ipAdress.substr(0, ipAdress.find("."));
	ipAdress.erase(0, ipAdress.find(".") + 1);
	std::string octet4 = ipAdress;

	// Konverzia oktetov na bitset
	ipAdressOctet1 = std::bitset<8>(std::stoi(octet1));
	ipAdressOctet2 = std::bitset<8>(std::stoi(octet2));
	ipAdressOctet3 = std::bitset<8>(std::stoi(octet3));
	ipAdressOctet4 = std::bitset<8>(std::stoi(octet4));
}

bool Route::isPartOfSubnet(std::string targetIP)
{
	std::string octet1 = targetIP.substr(0, targetIP.find("."));
	targetIP.erase(0, targetIP.find(".") + 1);
	std::string octet2 = targetIP.substr(0, targetIP.find("."));
	targetIP.erase(0, targetIP.find(".") + 1);
	std::string octet3 = targetIP.substr(0, targetIP.find("."));
	targetIP.erase(0, targetIP.find(".") + 1);
	std::string octet4 = targetIP;

	std::bitset<8> octet1b(std::stoi(octet1));
	std::bitset<8> octet2b(std::stoi(octet2));
	std::bitset<8> octet3b(std::stoi(octet3));
	std::bitset<8> octet4b(std::stoi(octet4));

	std::string targetIPBinary = octet1b.to_string() + octet2b.to_string() + octet3b.to_string() + octet4b.to_string();

	for (size_t i = 0; i < getPrefix(); i++) {
		if (targetIPBinary.at(i) != this->getIpAdressOctets().at(i)) {
			return false;
		}
	}
	return true;
}

void Route::setLifeTimeToSeconds(std::string time)
{
	if (time.length() == 2)
	{
		//konverzia hodiny na sekundy
		std::string hour = time.substr(0, 1);
		lifeTimeSeconds = std::stoi(hour) * 3600;

	}
	if (time.length() >= 7)
	{
		// Splitnutie casu do 3 casti pomocou ":" separatora
		std::string hours = time.substr(0, time.find(":"));
		time.erase(0, time.find(":") + 1);
		std::string minutes = time.substr(0, time.find(":"));
		time.erase(0, time.find(":") + 1);
		std::string seconds = time;

		// Konverzia casu na sekundy
		int hoursInt = std::stoi(hours);
		int minutesInt = std::stoi(minutes);
		int secondsInt = std::stoi(seconds);

		lifeTimeSeconds = hoursInt * 3600 + minutesInt * 60 + secondsInt;
	}
	//konverzia tyzdne a dni na sekundy
	if(time.find("w") != std::string::npos && time.find("d") != std::string::npos)
	{

		std::string weeks = time.substr(0, time.find("w"));

		time.erase(0, time.find("w") + 1);
		std::string days = time.substr(0, time.find("d"));


		int weeksInt = std::stoi(weeks);
		int daysInt = std::stoi(days);

		lifeTimeSeconds = weeksInt * 604800 + daysInt * 86400;
	}
	//konverzia dni a hodin na sekundy
	if (time.find("d") != std::string::npos && time.find("h") != std::string::npos)
	{

		std::string days = time.substr(0, time.find("d"));

		time.erase(0, time.find("d") + 1);
		std::string hours = time.substr(0, time.find("h"));


		int daysInt = std::stoi(days);
		int hoursInt = std::stoi(hours);

		lifeTimeSeconds = daysInt * 86400 + hoursInt * 3600;
	}
}

size_t Route::getPrefix() const
{
	return std::stoi(mask);
}

std::string Route::getIpAdressOctets() const
{
	return ipAdressOctet1.to_string() + ipAdressOctet2.to_string() + ipAdressOctet3.to_string() + ipAdressOctet4.to_string();
}

std::string Route::getIpAdress() const
{
	return this->ipAdress;
}

std::string Route::getMask() const
{
	return this->mask;
}

std::string Route::getNextHopIpAdress () const
{
	return this->nextHopIpAdress;
}

std::string Route::getTime() const
{
	return this->time;
}

std::string Route::toString() const
{
	return this->ipAdress + " " + this->mask + " " + this->nextHopIpAdress + " " + this->time;
}

std::string Route::getLifeTimeSeconds() const
{
	return std::to_string(lifeTimeSeconds);
}

Route::~Route()
{
}