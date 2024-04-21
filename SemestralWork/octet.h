#pragma once
#include <string>
#include "route.h"

struct Octet
{
	Octet() = default;
	Octet(std::string octet, Route* referenceToRoute = nullptr) : octet(octet), referenceToRoute(referenceToRoute) {}
	/*~Octet() { 
		delete referenceToRoute;
		referenceToRoute = nullptr; 
	}*/
	bool operator==(const Octet& other) const
	{
		return octet == other.octet && referenceToRoute == other.referenceToRoute;
	}
	std::string octet;
	Route* referenceToRoute = nullptr;
};