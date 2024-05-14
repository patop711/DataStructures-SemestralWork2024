#pragma once
#include <string>
#include <iomanip>
#include <bitset>

/*
* Trieda reprezentujúca jeden záznam v tabu¾ke RT záznamov
* Obsahuje ip adresu, masku, next hop ip adresu a èasový údaj lifetime
*/
class Route
{
private:
	std::string ipAdress = "";
	std::string mask = "";
	std::string nextHopIpAdress = "";
	std::string time = "";
	long long lifeTimeSeconds = 0;

	std::string octet_1 = "";
	std::string octet_2 = "";
	std::string octet_3 = "";
	std::string octet_4 = "";

	//Oktety ip adresy
	std::bitset<8> ipAdressOctet1 = 0;
	std::bitset<8> ipAdressOctet2 = 0;
	std::bitset<8> ipAdressOctet3 = 0;
	std::bitset<8> ipAdressOctet4 = 0;

public:
	Route();
	Route(std::string ipAdress, std::string mask, std::string nextHopIpAdress, std::string time);
	bool operator==(const Route& other) const { return ipAdress == other.ipAdress && mask == other.mask && nextHopIpAdress == other.nextHopIpAdress && time == other.time; }
	bool operator!=(const Route& other) const { return !(*this == other); }
	Route(const Route& other) : ipAdress(other.ipAdress), mask(other.mask), nextHopIpAdress(other.nextHopIpAdress), time(other.time) {
		setIpAdressOctets(other.ipAdress);
		setLifeTimeToSeconds(other.time);
	}
	~Route();
public:
	bool isPartOfSubnet(std::string ipAdress) const;
	std::string getIpAdress() const;
	std::string getMask() const;
	std::string getNextHopIpAdress() const;
	std::string getTime() const;
	std::string getIpAdressOctets() const;
	void toString() const;
	size_t getLifeTimeSecondsInt() const;
	std::string getLifeTimeSeconds() const;
	size_t getPrefix() const;

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

bool Route::isPartOfSubnet(std::string targetIP) const
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
		lifeTimeSeconds = static_cast<size_t>(std::stoi(hour)) * 3600;

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

		lifeTimeSeconds = static_cast<size_t>(hoursInt) * 3600 + static_cast<unsigned long long>(minutesInt) * 60 + secondsInt;
	}
	//konverzia tyzdne a dni na sekundy
	if(time.find("w") != std::string::npos && time.find("d") != std::string::npos)
	{

		std::string weeks = time.substr(0, time.find("w"));

		time.erase(0, time.find("w") + 1);
		std::string days = time.substr(0, time.find("d"));


		int weeksInt = std::stoi(weeks);
		int daysInt = std::stoi(days);

		lifeTimeSeconds = static_cast<size_t>(weeksInt) * 604800 + static_cast<unsigned long long>(daysInt) * 86400;
	}
	//konverzia dni a hodin na sekundy
	if (time.find("d") != std::string::npos && time.find("h") != std::string::npos)
	{

		std::string days = time.substr(0, time.find("d"));

		time.erase(0, time.find("d") + 1);
		std::string hours = time.substr(0, time.find("h"));


		int daysInt = std::stoi(days);
		int hoursInt = std::stoi(hours);

		lifeTimeSeconds = static_cast<size_t>(daysInt) * 86400 + static_cast<unsigned long long>(hoursInt) * 3600;
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

void Route::toString() const
{
	std::cout << "IP: " << this->ipAdress << " MASKA: " << "/" << this->mask << " NEXT-HOP: " << this->nextHopIpAdress << " TTL: " << this->time << std::endl;
}

std::string Route::getLifeTimeSeconds() const
{
	return std::to_string(lifeTimeSeconds);
}

size_t Route::getLifeTimeSecondsInt() const
{
	return lifeTimeSeconds;
}

Route::~Route()
{
}