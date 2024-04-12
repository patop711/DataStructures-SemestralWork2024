#include "libds/heap_monitor.h"
#include "program.h"
#include <Windows.h>
#include <string>
#include "libds/amt/implicit_sequence.h"
//#include "libds/mm/memory_manager.h"

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

	std::string menoSuboru;
	std::cout << "Zadajte nazov CSV suboru s routovacou tabulkou: ";
	std::cin >> menoSuboru;
	Program* p = new Program(menoSuboru);
	delete p;
	p = nullptr;
	return 0;
}
