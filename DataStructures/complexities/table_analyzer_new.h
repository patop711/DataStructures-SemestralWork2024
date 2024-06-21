#pragma once
#include <complexities/complexity_analyzer.h>
#include <vector>
#include <map>
#include "libds/adt/table.h"
#include <string>
#include <iterator>
#include <random>
#include <unordered_set>
namespace ds::utils
{
	template<class Table>
	class TableAnalyzer : public ComplexityAnalyzer<Table>
	{
	public:
		//void analyze() override;
	protected:
		explicit TableAnalyzer(const std::string& name);
	protected:
		void growToSize(Table& structure, size_t size) override;

		std::string getNewRandomKey() const;
		std::string getExistingRandomKey() const;
		int getRandomData() const;
	protected:
		std::default_random_engine rngData_;
		std::default_random_engine rngShuffle_;

		std::vector<std::string> keys_;
		std::vector<std::string> inserted_keys_;

		size_t index_pointer_ = 0;
		size_t sets_counter_ = 0;
		int data_;

		std::string new_key_;
		std::string existing_key_;

	};
	//----------INSERT TEST---------
	template<class Table>
	class TableInsertAnalyzer : public TableAnalyzer<Table>
	{
	public:
		explicit TableInsertAnalyzer(const std::string& name);

	protected:
		void executeOperation(Table& structure) override;
	};
	//----------INSERT TEST---------
	//----------REMOVE TEST---------
	template<class Table>
	class TableRemoveAnalyzer : public TableAnalyzer<Table>
	{
	public:
		explicit TableRemoveAnalyzer(const std::string& name);

	protected:
		void executeOperation(Table& structure) override;
	};
	//-----------REMOVE TEST---------
	//----------FIND TEST------------
	template<class Table>
	class TableFindAnalyzer : public TableAnalyzer<Table>
	{
	public:
		explicit TableFindAnalyzer(const std::string& name);
	protected:
		void executeOperation(Table& structure) override;
	};
	/**
	 * @brief Container for all table analyzers.
	 */
	class TablesAnalyzer : public CompositeAnalyzer
	{
	public:
		TablesAnalyzer();
	};
	//-----------
	template<class Table>
	TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
		ComplexityAnalyzer<Table>(name),
		rngData_(std::random_device()()),
		rngShuffle_(std::random_device()())
	{
		std::ifstream csv("RT.csv");
		std::string line;
		std::string ipAdress;
		std::string mask;
		std::string nextHopIpAdress;
		std::string time;

		if (csv.is_open())
		{
			//Skipnutie headeru
			std::getline(csv, line);
			while (std::getline(csv, line)) {
				// Splitnutie udajov pomocou ";"
				std::vector<std::string> tokens;
				std::stringstream ss(line);
				while (std::getline(ss, line, ';')) {
					tokens.push_back(line);
				}

				//Kontrola ci sa v riadku nachadzaju vsetky potrebne udaje
				if (tokens.size() >= 5) {
					ipAdress = tokens[1];
					mask = ipAdress.substr(ipAdress.find("/") + 1);
					ipAdress = ipAdress.substr(0, ipAdress.find("/"));
					nextHopIpAdress = tokens[3].substr(tokens[3].find(" ") + 1);
					time = tokens[4];

					//sekvencia->insertLast().data_ = new Route(ipAdress, mask, nextHopIpAdress, time);
					this->keys_.push_back(nextHopIpAdress);
					tokens.clear();
					ss.clear();
				}
			}
		}
		else
		{
			throw std::runtime_error("S˙bor sa nepodarilo otvoriù, alebo neexistuje!");
		}
		csv.close();

		std::unordered_set uniqueSet(keys_.begin(), keys_.end());
		keys_.assign(uniqueSet.begin(), uniqueSet.end());

		std::shuffle(std::begin(keys_), std::end(keys_), rngShuffle_);

		ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
			{
				std::uniform_int_distribution<size_t> indexDist(0, table.size() - 1);
				data_ = rngData_();
				new_key_ = keys_[index_pointer_];
				existing_key_ = inserted_keys_[index_pointer_];
			});

		ComplexityAnalyzer<Table>::registerAfterOperation([this](Table& table)
			{
				++index_pointer_;
				if (index_pointer_ == keys_.size())
				{
					index_pointer_ = 0;
					++sets_counter_;
					std::shuffle(keys_.begin(), keys_.end(), rngShuffle_);
				}
			});
	}

	template <class Table>
	void TableAnalyzer<Table>::growToSize(Table& table, size_t size)
	{

		for (size_t i = 0; i < size; ++i)
		{
			std::cout << "Inserting: " << keys_[index_pointer_] + "_" + std::to_string(sets_counter_) << std::endl;
			table.insert(keys_[index_pointer_] + "_" + std::to_string(sets_counter_), rngData_());
			inserted_keys_.push_back(keys_[index_pointer_] + "_" + std::to_string(sets_counter_));

			++index_pointer_;
			if (index_pointer_ == keys_.size())
			{
				++sets_counter_;
				index_pointer_ = 0;
				std::shuffle(std::begin(keys_), std::end(keys_), rngShuffle_);
			}

		}
	}

	template<class Table>
	std::string TableAnalyzer<Table>::getNewRandomKey() const
	{
		return new_key_;
	}

	template<class Table>
	std::string TableAnalyzer<Table>::getExistingRandomKey() const
	{
		return existing_key_;
	}

	template<class List>
	int TableAnalyzer<List>::getRandomData() const
	{
		return data_;
	}
	//-----------
	template<class Table>
	TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) : TableAnalyzer<Table>(name)
	{
	}
	template<class Table>
	void TableInsertAnalyzer<Table>::executeOperation(Table& table)
	{
		auto data = this->getRandomData();
		auto key = this->getNewRandomKey();
		table.insert(key, data);
	}
	//----------
	template<class Table>
	TableRemoveAnalyzer<Table>::TableRemoveAnalyzer(const std::string& name) : TableAnalyzer<Table>(name)
	{
	}
	template<class Table>
	void TableRemoveAnalyzer<Table>::executeOperation(Table& table)
	{
		auto key = this->getExistingRandomKey();
		table.remove(key);
	}
	//----------
	template<class Table>
	TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name) : TableAnalyzer<Table>(name)
	{
	}

	template<class Table>
	void TableFindAnalyzer<Table>::executeOperation(Table& table)
	{
		auto key = this->getExistingRandomKey();
		table.find(key);
	}

	//----------
	/*template<class Table>
	void TableAnalyzer<Table>::analyze()
	{
		auto hashFunction = [](const std::string& kluc)
			{
				static size_t randomizer = 0;
				size_t hash = 0;
				for (char cislo : kluc) {
					hash = hash * 2 + cislo;
				}
				hash += randomizer;
				randomizer += 5;
				return hash;
			};
		this->resetSuccess();

		std::map<size_t, std::vector<std::chrono::nanoseconds>> data;

		for (size_t r = 0; r < this->getReplicationCount(); ++r)
		{
			inserted_keys_.clear();
			auto* table = new ds::adt::HashTable<std::string, size_t>(hashFunctionAsciiSum, 100000);

			for (size_t step = 0; step < this->getStepCount(); ++step)
			{
				const size_t expectedSize = (step + 1) * this->getStepSize();
				const size_t insertCount = expectedSize - table->size();
				this->growToSize(*table, insertCount);
				beforeOperation_(*table);
				auto start = std::chrono::high_resolution_clock::now();
				this->executeOperation(*table);
				auto end = std::chrono::high_resolution_clock::now();
				afterOperation_(*table);
				auto duration = std::chrono::duration_cast<duration_t>(end - start);
				data[expectedSize].push_back(duration);
			}

			delete table;
		}

		this->saveToCsvFile(data);
		this->setSuccess();

	}*/

	inline TablesAnalyzer::TablesAnalyzer() :
		CompositeAnalyzer("Tables")
	{
		//this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::UnsortedImplicitSequenceTable<std::string, int>>>("unsorted-imp-tab-insert"));
		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::UnsortedImplicitSequenceTable<std::string, int>>>("unsorted-imp-tab-remove"));
		////find
		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::UnsortedImplicitSequenceTable<std::string, int>>>("unsorted-imp-tab-find"));

		//this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::UnsortedExplicitSequenceTable<std::string, int>>>("unsorted-exp-seq-tab-insert"));
		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::UnsortedExplicitSequenceTable<std::string, int>>>("unsorted-exp-seq-tab-remove"));
		////find
		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::UnsortedExplicitSequenceTable<std::string, int>>>("unsorted-exp-seq-tab-find"));

		//this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<std::string, int>>>("sorted-tab-insert"));
		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::SortedSequenceTable<std::string, int>>>("sorted-tab-remove"));
		////find
		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::SortedSequenceTable<std::string, int>>>("sorted-tab-find"));

		this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::HashTable<std::string, int>>>("hash-tab-insert"));
		this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::HashTable<std::string, int>>>("hash-tab-remove"));
		//find
		this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::HashTable<std::string, int>>>("hash-tab-find"));

		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::BinarySearchTree<std::string, int>>>("bst-tab-insert"));
		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::BinarySearchTree<std::string, int>>>("bst-tab-remove"));
		////find
		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::BinarySearchTree<std::string, int>>>("bst-tab-find"));

		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::Treap<std::string,int>>>("treap-tab-insert"));
		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::Treap<std::string, int>>>("treap-tab-remove"));
		////find
		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::Treap<std::string, int>>>("treap-tab-find"));


	}

};