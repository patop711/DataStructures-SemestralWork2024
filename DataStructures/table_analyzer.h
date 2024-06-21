#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <vector>

namespace ds::utils
{
    /**
     * @brief Common base for list analyzers.
     */
    template<class Table>
    class TableAnalyzer : public ComplexityAnalyzer<Table>
    {
    protected:
        explicit TableAnalyzer(const std::string& name);

    protected:
        void growToSize(Table& structure, size_t size) override;
        int getRandomKey();
        int getRandomData();
    protected:
        /*ds::amt::ImplicitSequence<int> keysSequence_;*/
        std::vector<int> keysSequence_;
        int key_;
        int data_;
        int randomIndex;

        std::default_random_engine rngData_;
        std::default_random_engine rngKey_;
    };

    /*
    * @brief Analyzes complexity of an erasure at the beginning.
    */
    template<class Table>
    class TableRemoveAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableRemoveAnalyzer(const std::string& name);
    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Analyzes complexity of an insertion at the beginning.
     */
    template<class Table>
    class TableInsertAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Analyzes complexity of an find at the beginning.
     */
    template<class Table>
    class TableFindAnalyzer : public TableAnalyzer<Table>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name);

    protected:
        void executeOperation(Table& structure) override;
    };

    /**
     * @brief Container for all list analyzers.
     */
    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer();
    };

    //----------Base class ----------

    template<class Table>
    TableAnalyzer<Table>::TableAnalyzer(const std::string& name) :
        ComplexityAnalyzer<Table>(name),
        rngData_(144),
        rngKey_(144),
        key_(0),
        data_(0),
        randomIndex(0),
        keysSequence_()
    {
        ComplexityAnalyzer<Table>::registerAfterOperation([this](Table& structure)
			{
                //Pre kazde testovanie zadefinujeme ze sa ma po kazdej replikacii vycistit
                this->keysSequence_.clear();
			});
    }

    template <class Table>
    void TableAnalyzer<Table>::growToSize(Table& structure, size_t size)
    {
        const size_t toInsert = size - structure.size();

        for (size_t i = 0; i < toInsert; i++)
        {
            std::uniform_int_distribution indexDist(0, INT_MAX);
            int key = indexDist(rngKey_);
            int data = rngData_();

            while (structure.contains(key))
            {
                key = indexDist(rngKey_);
            }
            //this->keysSequence_.insertLast().data_ = key;
            this->keysSequence_.push_back(key);
            structure.insert(key, data);
        }
    }

    template<class Table>
    int TableAnalyzer<Table>::getRandomKey()
	{
		return key_;
	}

    template<class Table>
    int TableAnalyzer<Table>::getRandomData()
    {
        return data_;
	}

    //---------- Base class ----------
    // --------- Table remove analyzer ---------
    //TODO 11 - Dorobit analyzer doma - DONE
    template <class Table>
    TableRemoveAnalyzer<Table>::TableRemoveAnalyzer(const std::string& name) :
		TableAnalyzer<Table>(name)
	{
        ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& structure)
			{
                //registracia operacie pre remove analyzu
                std::uniform_int_distribution<int> indexDist(0, this->keysSequence_.size() - 1);
                this->randomIndex = indexDist(this->rngKey_);
                this->key_ = this->keysSequence_[randomIndex];
                std::swap(this->keysSequence_[randomIndex], this->keysSequence_.back());
                this->keysSequence_.pop_back();

			});
	}

    template <class Table>
    void TableRemoveAnalyzer<Table>::executeOperation(Table& structure)
    {
        /*static size_t repCount = 1;
        std::cout << "Replikacia remove nr. " << repCount << std::endl;
        repCount++;*/
        structure.remove(this->key_);
    }

    //---------- Table remove analyzer ---------
    //---------- Table insert analyzer ---------

    template <class Table>
    TableInsertAnalyzer<Table>::TableInsertAnalyzer(const std::string& name) :
        TableAnalyzer<Table>(name)
    {
        ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& structure)
			{
                //registracia operacie pre insert analyzu
				std::uniform_int_distribution indexDist(0, INT_MAX);
				this->key_ = indexDist(this->rngKey_);
				this->data_ = rngData_();

				while (structure.contains(this->key_))
				{
					this->key_ = indexDist(this->rngKey_);
				}
			});
    }

    template <class Table>
    void TableInsertAnalyzer<Table>::executeOperation(Table& structure)
    {
        /*system("cls");
        static size_t repCount = 1;
        std::cout << "Replikacia insert nr. " << repCount << std::endl;
        repCount++;*/
        structure.insert(this->getRandomKey(), this->getRandomData());
    }

    //---------- Table insert analyzer ---------
    //---------- Table find analyzer ---------

    template <class Table>
    TableFindAnalyzer<Table>::TableFindAnalyzer(const std::string& name) :
		TableAnalyzer<Table>(name)
	{
        ComplexityAnalyzer<Table>::registerBeforeOperation([this](Table& table)
            {
                std::uniform_int_distribution<int> dist(0, this->keysSequence_.size() - 1);
                this->randomIndex = dist(this->rngKey_);
                this->key_ = this->keysSequence_.at(this->randomIndex);
            });
	}

    template <class Table>
    void TableFindAnalyzer<Table>::executeOperation(Table& structure)
	{
        /*system("cls");
        static size_t repCount = 1;
        std::cout << "Replikacia find nr. " << repCount << std::endl;
        repCount++;*/
		structure.find(this->getRandomKey());
	}
    //----------------- Table find analyzer ---------

    inline TablesAnalyzer::TablesAnalyzer() :
        CompositeAnalyzer("Tables")
    {
        //TODO 11 - Dorobit analyzer remove doma a pridat ich sem
        //TODO - Vykonajte analyzu casovej zlozitosty operacie UnsortedImplicitSequenceTable::remove(K key)
        //TODO - Vykonajte analyzu casovej zlozitosty operacie UnsortedExplicitSequenceTable::remove(K key)
        this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<adt::UnsortedImplicitSequenceTable<int, int>>>("unsorted-implicit-tab-remove")); //DONE
        this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<adt::UnsortedExplicitSequenceTable<int, int>>>("unsorted-explicit-tab-remove")); //DONE
        //Semestralka analyza

        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<adt::HashTable<int, int>>>("hash-tab-insert"));
        this->addAnalyzer(std::make_unique<TableFindAnalyzer<adt::HashTable<int, int>>>("hash-tab-find"));
        this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<adt::HashTable<int, int>>>("hash-tab-remove"));

        this->addAnalyzer(std::make_unique<TableInsertAnalyzer<adt::ModifiedHashTable<int, int>>>("m-hash-tab-insert"));
        this->addAnalyzer(std::make_unique<TableFindAnalyzer<adt::ModifiedHashTable<int, int>>>("m-hash-tab-find"));
        this->addAnalyzer(std::make_unique<TableRemoveAnalyzer<adt::ModifiedHashTable<int, int>>>("m-hash-tab-remove"));
    }
}