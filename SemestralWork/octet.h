#pragma once
#include <string>
#include "route.h"
/*
* 
* Trieda Octet sluzi na reprezentaciu jedneho oktetu IP adresy a referenciu na trasu v poslednom oktete ktorý je prefix v hierarchii
* 
*/
struct Octet
{
	std::string octet;
	Route* referenceToRoute = nullptr;

	Octet() = default;
	Octet(std::string octet, Route* referenceToRoute = nullptr) : octet(octet), referenceToRoute(referenceToRoute) {}
	~Octet() { if (referenceToRoute != nullptr) { delete referenceToRoute; } }
	bool operator==(const Octet& other) const
	{
		return octet == other.octet && referenceToRoute == other.referenceToRoute;
	}
	bool operator!=(const Octet& other) const
	{
		return !(*this == other);
	}
};