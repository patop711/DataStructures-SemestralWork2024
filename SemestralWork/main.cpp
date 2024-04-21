#include "libds/heap_monitor.h"
#include "program.h"
#include <Windows.h>
#include <string>
#include "libds/amt/implicit_sequence.h"
#include "libds/amt/explicit_hierarchy.h"
#include "route.h"
#include "octet.h"
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

	
	Program* p = new Program();
	delete p;
	p = nullptr;

	/**
		 *         0
		 *    /         \
		 *    1         2
		 *  / | \       |
		 *  3 4 5       6
		 * 
		 */


	//ds::amt::MultiWayExplicitHierarchy<Octet> hierarchia;
	//hierarchia.emplaceRoot().data_ = Octet("KOREN");
	//auto koren = hierarchia.accessRoot(); //<-- koren
	//ds::amt::MultiWayExplicitHierarchyBlock<Octet>* oktet1 = hierarchia.accessRoot(); //<-- oktet1
	//ds::amt::MultiWayExplicitHierarchyBlock<Octet>* oktet2 = hierarchia.accessRoot(); //<-- oktet2
	//ds::amt::MultiWayExplicitHierarchyBlock<Octet>* oktet3 = hierarchia.accessRoot(); //<-- oktet3
	//ds::amt::MultiWayExplicitHierarchyBlock<Octet>* oktet4 = hierarchia.accessRoot(); //<-- oktet4
	//ds::amt::MultiWayExplicitHierarchyBlock<Octet>* oktet5 = hierarchia.accessRoot(); //<-- prefix

	//Route* r0 = new Route("1.0.0.0", "22", "128.223.51.1", "1h");
	//Route* r1 = new Route("1.0.0.0", "24", "128.223.51.1", "1h");
	//Route* r2 = new Route("1.0.0.1", "26", "128.223.51.1", "5h");
	//Route* r3 = new Route("1.0.0.1", "18", "128.223.51.1", "5h");

	//koren->data_ = Octet("KOREÒ");

	//oktet1 = &hierarchia.emplaceSon(*koren, 0);
	//oktet1->data_ = Octet("1");
	//oktet2 = &hierarchia.emplaceSon(*oktet1, 0);
	//oktet2->data_ = Octet("0");
	//oktet3 = &hierarchia.emplaceSon(*oktet2, 0);
	//oktet3->data_ = Octet("0");
	//oktet4 = &hierarchia.emplaceSon(*oktet3, 0);
	//oktet4->data_ = Octet("0");
	//oktet4 = &hierarchia.emplaceSon(*oktet3, 1);
	//oktet4->data_ = Octet("1");

	//oktet4 = hierarchia.accessSon(*oktet3, 0);
	//oktet5 = &hierarchia.emplaceSon(*oktet4, 0);
	//oktet5->data_ = Octet("/22", r0);
	//oktet5 = &hierarchia.emplaceSon(*oktet4, 1);
	//oktet5->data_ = Octet("/24", r1);

	//oktet4 = hierarchia.accessSon(*oktet3, 1);
	//oktet5 = &hierarchia.emplaceSon(*oktet4, 0);
	//oktet5->data_ = Octet("/26", r2);
	//oktet5 = &hierarchia.emplaceSon(*oktet4, 1);
	//oktet5->data_ = Octet("/18", r3);

	//for (int i = 0; i < 2; i++)
	//{
	//	std::cout << hierarchia.accessSon(*oktet4, i)->data_.octet << std::endl;
	//}
	//oktet4 = hierarchia.accessSon(*oktet3, 0);
	//for (int i = 0; i < 2; i++)
	//{
	//	std::cout << hierarchia.accessSon(*oktet4, i)->data_.octet << std::endl;
	//}




	return 0;
}
