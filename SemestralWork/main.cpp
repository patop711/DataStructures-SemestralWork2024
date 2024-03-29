#include "libds/heap_monitor.h"
#include "program.h"


int main()
{
	//RoutingTable rt("C:/Users/Patrik Pavlík/Desktop/AUS/AUS-VS2022/SemestralWork/RT.csv");
	//rt.printTable();
	//rt.matchWithAddress("5.83.124.13");
	//rt.matchLifetime(rt.getRoutes(), "6:33");

	initHeapMonitor();

	Program* p = new Program();
	delete p;

	return 0;
}
