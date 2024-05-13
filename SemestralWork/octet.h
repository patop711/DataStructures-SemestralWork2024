#pragma once
#include <string>
#include "route.h"

struct Octet
{
	std::string octet;
	Route* referenceToRoute = nullptr;

	Octet() = default;
	Octet(std::string octet, Route* referenceToRoute = nullptr) : octet(octet), referenceToRoute(referenceToRoute) {}
	bool operator==(const Octet& other) const
	{
		return octet == other.octet && referenceToRoute == other.referenceToRoute;
	}
	bool operator!=(const Octet& other) const
	{
		return !(*this == other);
	}
};