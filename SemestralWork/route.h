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
	//POTREBNE DATA
	std::string ipAdress = "";
	std::string mask = "";
	std::string nextHopIpAdress = "";
	std::string time = "";
	//POTREBNE DATA
	long long lifeTimeSeconds = 0;

public:
	Route();
	Route(std::string ipAdress, std::string mask, std::string nextHopIpAdress, std::string time);
	bool operator==(const Route& other) const { return ipAdress == other.ipAdress && mask == other.mask && nextHopIpAdress == other.nextHopIpAdress && time == other.time; }
	bool operator!=(const Route& other) const { return !(*this == other); }
	Route(const Route& other) : ipAdress(other.ipAdress), mask(other.mask), nextHopIpAdress(other.nextHopIpAdress), time(other.time) {
		setLifeTimeToSeconds(other.time);
	}
	~Route();
public:
	std::string getIpAdress() const;
	std::string getMask() const;
	std::string getNextHopIpAdress() const;
	std::string getTime() const;
	void toString() const;
	size_t getLifeTimeSecondsInt() const;
	size_t getPrefix() const;

private:
	void setLifeTimeToSeconds(std::string time);
};

Route::Route()
{
}

/*
* Konstruktor triedy Route
*/
Route::Route(std::string ipAdress, std::string mask, std::string nextHopIpAdress, std::string time) :
	ipAdress(ipAdress), mask(mask), nextHopIpAdress(nextHopIpAdress), time(time)
{
	setLifeTimeToSeconds(getTime());
}

/*
* Nastavenie casu na sekundy
*/
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

/*
* Vrati prefix
*/
size_t Route::getPrefix() const
{
	return std::stoi(mask);
}

/*
* Vrati ip adresu
*/
std::string Route::getIpAdress() const
{
	return this->ipAdress;
}

/*
* Vrati masku
*/
std::string Route::getMask() const
{
	return this->mask;
}

/*
* Vrati next hop ip adresu
*/
std::string Route::getNextHopIpAdress () const
{
	return this->nextHopIpAdress;
}

/*
* Vrati lifetime zaznamu
*/
std::string Route::getTime() const
{
	return this->time;
}

/*
* Vypise zaznam
*/
void Route::toString() const
{
	std::cout << "IP: " << this->ipAdress << " MASKA: " << "/" << this->mask << " NEXT-HOP: " << this->nextHopIpAdress << " TTL: " << this->time << std::endl;
}

/*
* Vrati lifetime v sekundach
*/
size_t Route::getLifeTimeSecondsInt() const
{
	return lifeTimeSeconds;
}

/*
* Destruktor triedy Route
*/
Route::~Route()
{
}