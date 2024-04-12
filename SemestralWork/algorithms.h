#pragma once
#include "route.h"
#include <functional>

/*
* Generická trieda Algorithms sluzi na vykonavanie operacii nad datovou strukturou a vyfiltruje zaznamy podla predikatu
* @param Iterator - iterator prechadzajuci zaznamy
* @param TypUdaju - typ zaznamu
* @param TypPredikatu - typ predikatu
*/
template <typename Iterator, typename TypUdaju, typename TypPredikatu>
class Algorithms
{
public:
	static void filter(Iterator it, Iterator end, std::function<bool(TypUdaju kontrolovany)> kontrolovany, std::function<void(TypUdaju akceptovany)> akceptovany)
	{
		while (it != end)
		{
			if (kontrolovany(*it))
			{
				akceptovany(*it);
			}
			it++;
		}
	}

	static bool matchWithAddress(TypUdaju kontrolovany, TypPredikatu& predikat)
	{
		return kontrolovany.isPartOfSubnet(predikat);
	}
	static bool matchLifetime(TypUdaju kontrolovany, TypPredikatu& predikat)
	{
		return kontrolovany.getLifeTimeSeconds()._Equal(predikat);
	}
};