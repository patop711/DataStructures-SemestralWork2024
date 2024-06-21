//#pragma once
//#include <complexities/complexity_analyzer.h>
//#include <vector>
//#include <map>
//#include "libds/adt/table.h"
//#include <string>
//#include <iterator>
//#include <random>
//#include <unordered_set>
//namespace ds::utils
//{
//	class TableAnalyzer : public ComplexityAnalyzer<ds::adt::HashTable<std::string, int>>
//	{
//	public:
//		void analyze() override;
//	protected:
//		explicit TableAnalyzer(const std::string& name);
//	protected:
//		void growToSize(ds::adt::HashTable<std::string, int>& structure, size_t size) override;
//
//		int getNewRandomKey() const;
//		int getExistingRandomKey() const;
//		int getRandomData() const;
//	protected:
//		std::default_random_engine rngData_;
//		std::default_random_engine rngShuffle_;
//		std::vector<std::string> keys_;
//		std::vector<std::string> inserted_keys_;
//
//		size_t index_pointer_ = 0;
//		size_t sets_counter_ = 0;
//		int data_;
//		int new_key_;
//		int existing_key_;
//
//	};
//	//----------INSERT TEST---------
//	class TableInsertAnalyzer : public TableAnalyzer
//	{
//	public:
//		explicit TableInsertAnalyzer(const std::string& name);
//
//	protected:
//		void executeOperation(ds::adt::HashTable<std::string, int>& structure) override;
//	};
//	//----------INSERT TEST---------
//	//----------REMOVE TEST---------
//	class TableRemoveAnalyzer : public TableAnalyzer
//	{
//	public:
//		explicit TableRemoveAnalyzer(const std::string& name);
//
//	protected:
//		void executeOperation(ds::adt::HashTable<std::string, int>& structure) override;
//	};
//	//-----------REMOVE TEST---------
//	//----------FIND TEST------------
//	class TableFindAnalyzer : public TableAnalyzer
//	{
//	public:
//		explicit TableFindAnalyzer(const std::string& name);
//	protected:
//		void executeOperation(ds::adt::HashTable<std::string, int>& structure) override;
//	};
//	/**
//	 * @brief Container for all table analyzers.
//	 */
//	class TablesAnalyzer : public CompositeAnalyzer
//	{
//	public:
//		TablesAnalyzer();
//	};
//	//-----------
//	TableAnalyzer::TableAnalyzer(const std::string& name) :
//		ComplexityAnalyzer(name),
//		rngData_(std::random_device()()),
//		rngShuffle_(std::random_device()())
//	{
//		std::ifstream csv("RT.csv");
//		std::string line;
//		std::string ipAdress;
//		std::string mask;
//		std::string nextHopIpAdress;
//		std::string time;
//
//		if (csv.is_open())
//		{
//			//Skipnutie headeru
//			std::getline(csv, line);
//			while (std::getline(csv, line)) {
//				// Splitnutie udajov pomocou ";"
//				std::vector<std::string> tokens;
//				std::stringstream ss(line);
//				while (std::getline(ss, line, ';')) {
//					tokens.push_back(line);
//				}
//
//				//Kontrola ci sa v riadku nachadzaju vsetky potrebne udaje
//				if (tokens.size() >= 5) {
//					ipAdress = tokens[1];
//					mask = ipAdress.substr(ipAdress.find("/") + 1);
//					ipAdress = ipAdress.substr(0, ipAdress.find("/"));
//					nextHopIpAdress = tokens[3].substr(tokens[3].find(" ") + 1);
//					time = tokens[4];
//
//					//sekvencia->insertLast().data_ = new Route(ipAdress, mask, nextHopIpAdress, time);
//					this->keys_.push_back(nextHopIpAdress);
//					tokens.clear();
//					ss.clear();
//				}
//			}
//		}
//		else
//		{
//			throw std::runtime_error("S˙bor sa nepodarilo otvoriù, alebo neexistuje!");
//		}
//		csv.close();
//
//		std::unordered_set uniqueSet(keys_.begin(), keys_.end());
//		keys_.assign(uniqueSet.begin(), uniqueSet.end());
//
//		std::shuffle(std::begin(keys_), std::end(keys_), rngShuffle_);
//
//		ComplexityAnalyzer::registerBeforeOperation([this](ds::adt::HashTable<std::string, int>& table)
//			{
//				std::uniform_int_distribution<int> indexDist(0, table.size() - 1);
//				data_ = rngData_();
//				new_key_ = keys_[index_pointer_];
//				existing_key_ = inserted_keys_[index_pointer_];
//			});
//
//		ComplexityAnalyzer::registerAfterOperation([this](ds::adt::HashTable<std::string, int>& table)
//			{
//				++index_pointer_;
//				if (index_pointer_ == keys_.size())
//				{
//					index_pointer_ = 0;
//					++sets_counter_;
//					std::shuffle(keys_.begin(), keys_.end(), rngShuffle_);
//				}
//			});
//	}
//	void TableAnalyzer::growToSize(ds::adt::HashTable<std::string, int>& table, size_t size)
//	{
//
//		for (size_t i = 0; i < n; ++i)
//		{
//			std::cout << "Inserting: " << keys_[index_pointer_] + "_" + std::to_string(sets_counter_) << std::endl;
//			table.insert(keys_[index_pointer_] + "_" + std::to_string(sets_counter_), rngData_());
//			inserted_keys_.push_back(keys_[index_pointer_] + "_" + std::to_string(sets_counter_));
//
//			++index_pointer_;
//			if (index_pointer_ == keys_.size())
//			{
//				++sets_counter_;
//				index_pointer_ = 0;
//				std::shuffle(std::begin(keys_), std::end(keys_), rngShuffle_);
//			}
//
//		}
//	}
//
//	int TableAnalyzer::getNewRandomKey() const
//	{
//		return new_key_;
//	}
//
//	int TableAnalyzer::getExistingRandomKey() const
//	{
//		return existing_key_;
//	}
//
//	int TableAnalyzer::getRandomData() const
//	{
//		return data_;
//	}
//	//-----------
//	TableInsertAnalyzer::TableInsertAnalyzer(const std::string& name) : TableAnalyzer(name)
//	{
//	}
//	void TableInsertAnalyzer::executeOperation(ds::adt::HashTable<std::string, int>& table)
//	{
//		auto data = this->getRandomData();
//		auto key = this->getNewRandomKey();
//		table.insert(key, data);
//	}
//	//----------
//	TableRemoveAnalyzer::TableRemoveAnalyzer(const std::string& name) : TableAnalyzer(name)
//	{
//	}
//	void TableRemoveAnalyzer::executeOperation(ds::adt::HashTable<std::string, int>& table)
//	{
//		auto key = this->getExistingRandomKey();
//		table.remove(key);
//	}
//	//----------
//	TableFindAnalyzer::TableFindAnalyzer(const std::string& name) : TableAnalyzer(name)
//	{
//	}
//
//	void TableFindAnalyzer::executeOperation(ds::adt::HashTable<std::string, int>& table)
//	{
//		auto key = this->getExistingRandomKey();
//		table.find(key);
//	}
//
//	//----------
//	void TableAnalyzer::analyze()
//	{
//		auto hashFunction = [](const std::string& kluc)
//			{
//				static size_t randomizer = 0;
//				size_t hash = 0;
//				for (char cislo : kluc) {
//					hash = hash * 2 + cislo;
//				}
//				hash += randomizer;
//				randomizer += 5;
//				return hash;
//			};
//		this->resetSuccess();
//
//		std::map<size_t, std::vector<std::chrono::nanoseconds>> data;
//
//		for (size_t r = 0; r < this->getReplicationCount(); ++r)
//		{
//			inserted_keys_.clear();
//			auto* table = new ds::adt::HashTable<std::string, size_t>(hashFunctionAsciiSum, 100000);
//
//			for (size_t step = 0; step < this->getStepCount(); ++step)
//			{
//				const size_t expectedSize = (step + 1) * this->getStepSize();
//				const size_t insertCount = expectedSize - table->size();
//				this->growToSize(*table, insertCount);
//				beforeOperation_(*table);
//				auto start = std::chrono::high_resolution_clock::now();
//				this->executeOperation(*table);
//				auto end = std::chrono::high_resolution_clock::now();
//				afterOperation_(*table);
//				auto duration = std::chrono::duration_cast<duration_t>(end - start);
//				data[expectedSize].push_back(duration);
//			}
//
//			delete table;
//		}
//
//		this->saveToCsvFile(data);
//		this->setSuccess();
//
//	}
//
//	inline TablesAnalyzer::TablesAnalyzer() :
//		CompositeAnalyzer("Tables")
//	{
//		this->addAnalyzer(std::make_unique<TableInsertAnalyzer>("hash-insert"));
//		this->addAnalyzer(std::make_unique<TableFindAnalyzer>("hash-find"));
//
//		//this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::UnsortedImplicitSequenceTable<std::string, int>>>("unsorted-imp-tab-insert"));
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::UnsortedImplicitSequenceTable<std::string, int>>>("unsorted-imp-tab-remove"));
//		////find
//		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::UnsortedImplicitSequenceTable<std::string, int>>>("unsorted-imp-tab-find"));
//
//		//this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::UnsortedExplicitSequenceTable<std::string, int>>>("unsorted-exp-seq-tab-insert"));
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::UnsortedExplicitSequenceTable<std::string, int>>>("unsorted-exp-seq-tab-remove"));
//		////find
//		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::UnsortedExplicitSequenceTable<std::string, int>>>("unsorted-exp-seq-tab-find"));
//
//		//this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::SortedSequenceTable<std::string, int>>>("sorted-tab-insert"));
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::SortedSequenceTable<std::string, int>>>("sorted-tab-remove"));
//		////find
//		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::SortedSequenceTable<std::string, int>>>("sorted-tab-find"));
//
//		//this->addAnalyzer(std::make_unique<TableInsertAnalyzer<ds::adt::HashTable<std::string, int>>>("hash-tab-insert"));
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::HashTable<std::string, int>>>("hash-tab-remove"));
//		////find
//		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::HashTable<std::string, int>>>("hash-tab-find"));
//
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::BinarySearchTree<std::string, int>>>("bst-tab-insert"));
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::BinarySearchTree<std::string, int>>>("bst-tab-remove"));
//		////find
//		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::BinarySearchTree<std::string, int>>>("bst-tab-find"));
//
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::Treap<std::string,int>>>("treap-tab-insert"));
//		//this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<ds::adt::Treap<std::string, int>>>("treap-tab-remove"));
//		////find
//		//this->addAnalyzer(std::make_unique<TableFindAnalyzer<ds::adt::Treap<std::string, int>>>("treap-tab-find"));
//
//
//	}
//
//};