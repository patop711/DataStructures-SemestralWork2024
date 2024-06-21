#include "libds/heap_monitor.h"
#include "program.h"
#include <Windows.h>
#include <string>
#include "libds/amt/implicit_sequence.h"
#include "libds/amt/explicit_hierarchy.h"
#include "route.h"
#include "octet.h"

#include "libds/adt/array.h"
#include "libds/adt/priority_queue.h"


int main()
{
	/*
	* Inicialilizácia monitoru haldy
	*/
	initHeapMonitor();

	/*
	* Nastavenie kódovania výstupu na Windows-1250
	*/
	SetConsoleOutputCP(1250);

	/*
	* Spustenie programu
	*/
	Program* p = new Program();
	delete p;
	p = nullptr;


	return 0;
}
