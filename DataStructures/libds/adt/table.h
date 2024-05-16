#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include <functional>
#include <limits>
#include <random>

namespace ds::adt {

	template <typename K, typename T>
	struct TableItem
	{
		K key_;
		T data_;

		bool operator==(const TableItem<K, T>& other) const
		{
			return key_ == other.key_ && data_ == other.data_;
		}

		bool operator!=(const TableItem<K, T>& other) const
		{
			return !(*this == other);
		}
	};

	//----------

	template <typename K, typename T>
	class Table :
		virtual public ADT
	{
	public:
		virtual void insert(const K& key, T data) = 0;
		virtual T& find(const K& key);
		virtual bool tryFind(const K& key, T*& data) const = 0;
		virtual bool contains(const K& key) const;
		virtual T remove(const K& key) = 0;

	protected:
		template<class TableT>
		bool areEqual(TableT& table1, const ADT& table2);
	};

	//----------

	template <typename K, typename T, typename SequenceType>
	class SequenceTable :
		public Table<K, T>,
		public ADS<TableItem<K, T>>
	{
	public:
		SequenceTable();
		SequenceTable(const SequenceTable& other);

		bool tryFind(const K& key, T*& data) const override;

	public:
		using BlockType = typename SequenceType::BlockType;
		using IteratorType = typename SequenceType::IteratorType;

		virtual BlockType* findBlockWithKey(const K& key) const = 0;

		IteratorType begin();
		IteratorType end();

	protected:
		SequenceType* getSequence() const;
	};

	//----------

	template <typename K, typename T, typename SequenceType>
	class UnsortedSequenceTable :
		public SequenceTable<K, T, SequenceType>
	{
	protected:
		using BlockType = typename SequenceType::BlockType;

		BlockType* findBlockWithKey(const K& key) const override;
	};

	template <typename K, typename T, typename SequenceType>
	using UnsortedSTab = UnsortedSequenceTable<K, T, SequenceType>;

	//----------

	template <typename K, typename T>
	class UnsortedImplicitSequenceTable :
		public UnsortedSequenceTable<K, T, amt::IS<TableItem<K, T>>>
	{
	public:
		void insert(const K& key, T data) override;
		T remove(const K& key) override;

		bool equals(const ADT& other) override;

	private:
		using BlockType = typename amt::IS<TableItem<K, T>>::BlockType;
	};

	template <typename K, typename T>
	using UnsortedISTab = UnsortedImplicitSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class UnsortedExplicitSequenceTable :
		public UnsortedSequenceTable<K, T, amt::SinglyLS<TableItem<K, T>>>
	{
	public:
		void insert(const K& key, T data) override;
		T remove(const K& key) override;

		bool equals(const ADT& other) override;

	private:
		using BlockType = typename amt::SinglyLS<TableItem<K, T>>::BlockType;
	};

	template <typename K, typename T>
	using UnsortedESTab = UnsortedExplicitSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class SortedSequenceTable :
		public SequenceTable<K, T, amt::IS<TableItem<K, T>>>
	{
	public:
		void insert(const K& key, T data) override;
		T remove(const K& key) override;
		bool equals(const ADT& other) override;

	protected:
		using BlockType = typename amt::IS<TableItem<K, T>>::BlockType;

		BlockType* findBlockWithKey(const K& key) const override;

	private:
		bool tryFindBlockWithKey(const K& key, size_t firstIndex, size_t lastIndex, BlockType*& lastBlock) const;
	};

	template <typename K, typename T>
	using SortedSTab = SortedSequenceTable<K, T>;

	//----------

	template <typename K, typename T>
	class HashTable :
		public Table<K, T>,
		public AUMS<TableItem<K, T>>
	{
	public:
		using HashFunctionType = std::function<size_t(const K&)>;

	public:
		HashTable();
		HashTable(const HashTable& other);
		HashTable(HashFunctionType hashFunction, size_t capacity);
		~HashTable() override;

		ADT& assign(const ADT& other) override;
		bool equals(const ADT& other) override;
		void clear() override;
		size_t size() const override;
		bool isEmpty() const override;

		void insert(const K& key, T data) override;
		bool tryFind(const K& key, T*& data) const override;
		T remove(const K& key) override;

	private:
		using SynonymTable = UnsortedESTab<K, T>;
		using SynonymTableIterator = typename SynonymTable::IteratorType;
		using PrimaryRegionIterator = typename amt::IS<SynonymTable*>::IteratorType;

	private:
		static const size_t CAPACITY = 100;

	private:
		amt::IS<SynonymTable*>* primaryRegion_;
		HashFunctionType hashFunction_;
		size_t size_;

	public:
		class HashTableIterator
		{
		public:
			HashTableIterator(PrimaryRegionIterator* tablesFirst, PrimaryRegionIterator* tablesLast);
			HashTableIterator(const HashTableIterator& other);
			~HashTableIterator();
			HashTableIterator& operator++();
			HashTableIterator operator++(int);
			bool operator==(const HashTableIterator& other) const;
			bool operator!=(const HashTableIterator& other) const;
			TableItem<K, T>& operator*();

		private:
			PrimaryRegionIterator* tablesCurrent_;
			PrimaryRegionIterator* tablesLast_;
			SynonymTableIterator* synonymIterator_;
		};

		HashTableIterator begin() const;
		HashTableIterator end() const;
	};

	//----------

	template <typename K, typename T, typename ItemType>
	class GeneralBinarySearchTree :
		public Table<K, T>,
		public ADS<ItemType>
	{
	public:
		using IteratorType = typename amt::BinaryEH<ItemType>::IteratorType;

	public:
		GeneralBinarySearchTree();
		GeneralBinarySearchTree(const GeneralBinarySearchTree& other);
		~GeneralBinarySearchTree() override;

		size_t size() const override;
		void clear() override;

		void insert(const K& key, T data) override;
		bool tryFind(const K& key, T*& data) const override;
		T remove(const K& key) override;

		IteratorType begin() const;
		IteratorType end() const;

	protected:
		using BSTNodeType = typename amt::BinaryEH<ItemType>::BlockType;

		amt::BinaryEH<ItemType>* getHierarchy() const;

		virtual void removeNode(BSTNodeType* node);
		virtual void balanceTree(BSTNodeType* node) { }

		bool tryFindNodeWithKey(const K& key, BSTNodeType*& node) const;

		void rotateLeft(BSTNodeType* node);
		void rotateRight(BSTNodeType* node);

	private:
		size_t size_;
	};

	//----------

	template <typename K, typename T>
	class BinarySearchTree :
		public GeneralBinarySearchTree<K, T, TableItem<K, T>>
	{
	public:
		bool equals(const ADT& other) override;
	};

	//----------

	template <typename K, typename T>
	struct TreapItem :
		public TableItem<K, T>
	{
		int priority_;
	};

	template <typename K, typename T>
	class Treap :
		public GeneralBinarySearchTree<K, T, TreapItem<K, T>>
	{
	public:
		Treap();
		bool equals(const ADT& other) override;

	protected:
		using BSTNodeType = typename GeneralBinarySearchTree<K, T, TreapItem<K, T>>::BSTNodeType;

		void removeNode(BSTNodeType* node) override;
		void balanceTree(BSTNodeType* node) override;

	private:
		std::default_random_engine rng_;
	};

	//----------

	template<typename K, typename T>
	template<class TableT>
	bool Table<K, T>::areEqual(TableT& table1, const ADT& table2)
	{
		if (&table1 == &table2)
		{
			return true;
		}

		const Table<K, T>* tab2 = dynamic_cast<const TableT*>(&table2);
		if (tab2 == nullptr)
		{
			return false;
		}

		if (table1.size() != table2.size())
		{
			return false;
		}

		for (const TableItem<K, T>& item : table1)
		{
			T* otherData = nullptr;
			if (!tab2->tryFind(item.key_, otherData) || *otherData != item.data_)
			{
				return false;
			}
		}
		return true;
	}

	//----------

	template<typename K, typename T>
	T& Table<K, T>::find(const K& key)
	{
		T* data = nullptr;
		if (!this->tryFind(key, data))
		{
			throw std::out_of_range("No such key!");
		}
		return *data;
	}

	template<typename K, typename T>
	bool Table<K, T>::contains(const K& key) const
	{
		T* data = nullptr;
		return this->tryFind(key, data);
	}

	//----------

	template<typename K, typename T, typename SequenceType>
	SequenceTable<K, T, SequenceType>::SequenceTable() :
		ADS<TableItem<K, T>>(new SequenceType())
	{
	}

	template<typename K, typename T, typename SequenceType>
	SequenceTable<K, T, SequenceType>::SequenceTable(const SequenceTable& other) :
		ADS<TableItem<K, T>>(new SequenceType(), other)
	{
	}

	template<typename K, typename T, typename SequenceType>
	bool SequenceTable<K, T, SequenceType>::tryFind(const K& key, T*& data) const
	{
		BlockType* blockWithKey = this->findBlockWithKey(key);
		if (blockWithKey == nullptr)
		{
			return false;
		}
		data = &blockWithKey->data_.data_;
		return true;
	}

	template <typename K, typename T, typename SequenceType>
	typename SequenceTable<K, T, SequenceType>::IteratorType SequenceTable<K, T, SequenceType>::begin()
	{
		return this->getSequence()->begin();
	}

	template <typename K, typename T, typename SequenceType>
	typename SequenceTable<K, T, SequenceType>::IteratorType SequenceTable<K, T, SequenceType>::end()
	{
		return this->getSequence()->end();
	}

	template<typename K, typename T, typename SequenceType>
	SequenceType* SequenceTable<K, T, SequenceType>::getSequence() const
	{
		return dynamic_cast<SequenceType*>(this->memoryStructure_);
	}

	//----------

	template<typename K, typename T, typename SequenceType>
	typename UnsortedSequenceTable<K, T, SequenceType>::BlockType* UnsortedSequenceTable<K, T, SequenceType>::findBlockWithKey(const K& key) const
	{
		return this->getSequence()->findBlockWithProperty([&key](BlockType* b) ->bool
			{
				return b->data_.key_ == key;
			});
	}

	//----------

	template<typename K, typename T>
	void UnsortedImplicitSequenceTable<K, T>::insert(const K& key, T data)
	{
		if (this->contains(key))
		{
			throw std::logic_error("Table already contains element associated with given key!");
		}

		TableItem<K, T>& tableItem = this->getSequence()->insertLast().data_;
		tableItem.key_ = key;
		tableItem.data_ = data;
	}

	template<typename K, typename T>
	T UnsortedImplicitSequenceTable<K, T>::remove(const K& key)
	{
		BlockType* blockWithKey = this->findBlockWithKey(key);

		if (blockWithKey == nullptr)
		{
			throw std::out_of_range("No such key!");
		}

		T result = blockWithKey->data_.data_;
		BlockType* lastBlock = this->getSequence()->accessLast();
		if (blockWithKey != lastBlock)
		{
			std::swap(blockWithKey->data_, lastBlock->data_);
		}
		this->getSequence()->removeLast();
		return result;
	}

	template<typename K, typename T>
	bool UnsortedImplicitSequenceTable<K, T>::equals(const ADT& other)
	{
		return Table<K, T>::areEqual(*this, other);
	}

	//----------

	template<typename K, typename T>
	void UnsortedExplicitSequenceTable<K, T>::insert(const K& key, T data)
	{
		if (this->contains(key))
		{
			throw std::logic_error("Table already contains element associated with given key!");
		}

		TableItem<K, T>& tableItem = this->getSequence()->insertFirst().data_;
		tableItem.key_ = key;
		tableItem.data_ = data;
	}

	template<typename K, typename T>
	T UnsortedExplicitSequenceTable<K, T>::remove(const K& key)
	{
		BlockType* blockWithKey = this->findBlockWithKey(key);

		if (blockWithKey == nullptr)
		{
			throw std::out_of_range("No such key!");
		}

		T result = blockWithKey->data_.data_;
		BlockType* firstBlock = this->getSequence()->accessFirst();
		if (blockWithKey != firstBlock)
		{
			using std::swap;
			swap(blockWithKey->data_, firstBlock->data_);
		}
		this->getSequence()->removeFirst();
		return result;
	}

	template<typename K, typename T>
	bool UnsortedExplicitSequenceTable<K, T>::equals(const ADT& other)
	{
		return Table<K, T>::areEqual(*this, other);
	}

	//----------

	template<typename K, typename T>
	void SortedSequenceTable<K, T>::insert(const K& key, T data)
	{
		TableItem<K, T>* tableItem;

		if (this->isEmpty())
		{
			tableItem = &this->getSequence()->insertFirst().data_;
		}
		else
		{
			BlockType* blok = nullptr;
			if (this->tryFindBlockWithKey(key, 0, this->size(), blok))
			{
				throw std::logic_error("Duplicate key!");
			}
			tableItem = key > blok->data_.key_
				? &this->getSequence()->insertAfter(*blok).data_
				: &this->getSequence()->insertBefore(*blok).data_;
		}

		tableItem->key_ = key;
		tableItem->data_ = data;
	}

	template<typename K, typename T>
	T SortedSequenceTable<K, T>::remove(const K& key)
	{
		BlockType* blockWithKey = nullptr;
		if (!this->tryFindBlockWithKey(key, 0, this->size(), blockWithKey))
		{
			throw std::out_of_range("No such key!");
		}

		T result = blockWithKey->data_.data_;
		if (this->getSequence()->accessFirst() == blockWithKey)
		{
			this->getSequence()->removeFirst();
		}
		else
		{
			this->getSequence()->removeNext(*this->getSequence()->accessPrevious(*blockWithKey));
		}
		return result;
	}

	template<typename K, typename T>
	typename SortedSequenceTable<K, T>::BlockType* SortedSequenceTable<K, T>::findBlockWithKey(const K& key) const
	{
		BlockType* blockWithKey = nullptr;
		return this->tryFindBlockWithKey(key, 0, this->size(), blockWithKey)
			? blockWithKey
			: nullptr;
	}

	template<typename K, typename T>
	bool SortedSequenceTable<K, T>::equals(const ADT& other)
	{
		return Table<K, T>::areEqual(*this, other);
	}

	template<typename K, typename T>
	bool SortedSequenceTable<K, T>::tryFindBlockWithKey(const K& key, size_t firstIndex, size_t lastIndex, BlockType*& lastBlock) const
	{
		if (this->isEmpty())
		{
			lastBlock = nullptr;
			return false;
		}

		size_t midIndex = firstIndex;
		while (firstIndex < lastIndex)
		{
			midIndex = firstIndex + (lastIndex - firstIndex) / 2;
			lastBlock = this->getSequence()->access(midIndex);

			if (lastBlock->data_.key_ < key)
			{
				firstIndex = midIndex + 1;
			}
			else if (lastBlock->data_.key_ > key)
			{
				lastIndex = midIndex;
			}
			else
			{
				break;
			}
		}

		lastBlock = this->getSequence()->access(midIndex);
		return lastBlock->data_.key_ == key;
	}

	//----------

	template<typename K, typename T>
	HashTable<K, T>::HashTable() :
		HashTable([](const K& key) { return std::hash<K>()(key); }, CAPACITY)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTable(const HashTable& other) :
		primaryRegion_(new amt::IS<SynonymTable*>(other.primaryRegion_->size(), true)),
		hashFunction_(other.hashFunction_),
		size_(0)
	{
		assign(other);
	}

	template<typename K, typename T>
	HashTable<K, T>::HashTable(HashFunctionType hashFunction, size_t capacity) :
		primaryRegion_(new amt::IS<SynonymTable*>(capacity, true)),
		hashFunction_(hashFunction),
		size_(0)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::~HashTable()
	{
		this->clear();
		delete primaryRegion_;
	}

	template <typename K, typename T>
	ADT& HashTable<K, T>::assign(const ADT& other)
	{
		if (this != &other)
		{
			const HashTable& otherTable = dynamic_cast<const HashTable&>(other);
			this->clear();
			for (TableItem<K, T>& otherItem : otherTable)
			{
				this->insert(otherItem.key_, otherItem.data_);
			}
		}

		return *this;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::equals(const ADT& other)
	{
		const HashTable& otherTable = dynamic_cast<const HashTable&>(other);

		if (this == &other)
		{
			return true;
		}
		if (this->size() != other.size())
		{
			return false;
		}

		for (TableItem<K, T>& otherItem : otherTable)
		{
			T* otherData = nullptr;
			if (!this->tryFind(otherItem.key_, otherData) || *otherData != otherItem.data_)
			{
				return false;
			}
		}
		return true;
	}

	template <typename K, typename T>
	void HashTable<K, T>::clear()
	{
		size_ = 0;
		primaryRegion_->processAllBlocksForward([](typename amt::IS<SynonymTable*>::BlockType* synonymBlock)
			{
				delete synonymBlock->data_;
				synonymBlock->data_ = nullptr;
			});
	}

	template <typename K, typename T>
	size_t HashTable<K, T>::size() const
	{
		return size_;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::isEmpty() const
	{
		return this->size() == 0;
	}

	template <typename K, typename T>
	void HashTable<K, T>::insert(const K& key, T data)
	{
		size_t index = hashFunction_(key) % primaryRegion_->size();
		SynonymTable* synonyms = primaryRegion_->access(index)->data_;
		if (!synonyms)
		{
			synonyms = new SynonymTable();
			primaryRegion_->access(index)->data_ = synonyms;
		}
		synonyms->insert(key, data);
		size_++;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::tryFind(const K& key, T*& data) const
	{
		size_t index = hashFunction_(key) % primaryRegion_->size();
		SynonymTable* synonyms = primaryRegion_->access(index)->data_;
		if (!synonyms)
		{
			return false;
		}
		return synonyms->tryFind(key, data);
	}

	template <typename K, typename T>
	T HashTable<K, T>::remove(const K& key)
	{
		size_t index = hashFunction_(key) % primaryRegion_->size();
		SynonymTable* synonyms = primaryRegion_->access(index)->data_;
		if (!synonyms)
		{
			throw std::invalid_argument("No such key present!");
		}
		T item = synonyms->remove(key);
		if (synonyms->isEmpty())
		{
			delete synonyms;
			primaryRegion_->access(index)->data_ = nullptr;
		}
		size_--;
		return item;
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::HashTableIterator
	(PrimaryRegionIterator* tablesFirst, PrimaryRegionIterator* tablesLast) :
		tablesCurrent_(tablesFirst),
		tablesLast_(tablesLast)
	{
		while (*tablesCurrent_ != *tablesLast_ && **tablesCurrent_ == nullptr)
		{
			++(*tablesCurrent_);
		}
		synonymIterator_ = *tablesCurrent_ != *tablesLast_
			? new SynonymTableIterator((**tablesCurrent_)->begin())
			: nullptr;
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::HashTableIterator
	(const HashTableIterator& other) :
		tablesCurrent_(new PrimaryRegionIterator(*other.tablesCurrent_)),
		tablesLast_(new PrimaryRegionIterator(*other.tablesLast_)),
		synonymIterator_(other.synonymIterator_ != nullptr
			? new SynonymTableIterator(*other.synonymIterator_)
			: nullptr
		)
	{
	}

	template <typename K, typename T>
	HashTable<K, T>::HashTableIterator::~HashTableIterator()
	{
		delete tablesCurrent_;
		delete tablesLast_;
		delete synonymIterator_;
	}

	template <typename K, typename T>
	typename HashTable<K, T>::HashTableIterator& HashTable<K, T>::HashTableIterator::operator++()
	{
		++(*synonymIterator_);
		SynonymTableIterator oldIt = *synonymIterator_;
		if (!(oldIt != (**tablesCurrent_)->end()))
		{
			do
			{
				++(*tablesCurrent_);
			} while (*tablesCurrent_ != *tablesLast_ && **tablesCurrent_ == nullptr);
			delete synonymIterator_;
			synonymIterator_ = nullptr;
			if (*tablesCurrent_ != *tablesLast_)
			{
				synonymIterator_ = new SynonymTableIterator((**tablesCurrent_)->begin());
			}
		}
		return *this;
	}

	template <typename K, typename T>
	typename HashTable<K, T>::HashTableIterator HashTable<K, T>::HashTableIterator::operator++(int)
	{
		HashTableIterator tmp(*this);
		this->operator++();
		return tmp;
	}

	template <typename K, typename T>
	bool HashTable<K, T>::HashTableIterator::operator==(const HashTableIterator& other) const
	{
		return synonymIterator_ == other.synonymIterator_ ||
			(synonymIterator_ != nullptr &&
				other.synonymIterator_ != nullptr &&
				*synonymIterator_ == *(other.synonymIterator_));
	}

	template <typename K, typename T>
	bool HashTable<K, T>::HashTableIterator::operator!=(const HashTableIterator& other) const
	{
		return !(*this == other);
	}

	template <typename K, typename T>
	TableItem<K, T>& HashTable<K, T>::HashTableIterator::operator*()
	{
		return (**synonymIterator_);
	}

	template <typename K, typename T>
	typename HashTable<K, T>::HashTableIterator HashTable<K, T>::begin() const
	{
		return HashTableIterator(
			new PrimaryRegionIterator(primaryRegion_->begin()),
			new PrimaryRegionIterator(primaryRegion_->end())
		);
	}

	template <typename K, typename T>
	typename HashTable<K, T>::HashTableIterator HashTable<K, T>::end() const
	{
		return HashTableIterator(
			new PrimaryRegionIterator(primaryRegion_->end()),
			new PrimaryRegionIterator(primaryRegion_->end())
		);
	}

	//----------

	template<typename K, typename T, typename ItemType>
	GeneralBinarySearchTree<K, T, ItemType>::GeneralBinarySearchTree() :
		ADS<ItemType>(new amt::BinaryEH<ItemType>()),
		size_(0)
	{
	}

	template<typename K, typename T, typename ItemType>
	GeneralBinarySearchTree<K, T, ItemType>::GeneralBinarySearchTree(const GeneralBinarySearchTree& other) :
		ADS<ItemType>(new amt::BinaryEH<ItemType>(), other),
		size_(other.size_)
	{
	}

	template<typename K, typename T, typename ItemType>
	GeneralBinarySearchTree<K, T, ItemType>::~GeneralBinarySearchTree()
	{
		size_ = 0;
	}

	template<typename K, typename T, typename ItemType>
	size_t GeneralBinarySearchTree<K, T, ItemType>::size() const
	{
		return size_;
	}

	template<typename K, typename T, typename ItemType>
	void GeneralBinarySearchTree<K, T, ItemType>::clear()
	{
		ADS<ItemType>::clear();
		size_ = 0;
	}

	template<typename K, typename T, typename ItemType>
	void GeneralBinarySearchTree<K, T, ItemType>::insert(const K& key, T data)
	{
		/*BSTNodeType* newNode = nullptr;

		if (this->isEmpty()) {
			newNode = &this->getHierarchy()->emplaceRoot();
		}
		else {
			BSTNodeType* parent = nullptr;
			if (tryFindNodeWithKey(key, parent)) {
				throw structure_error("Key already exists");
			}

			if (key > parent->data.key_) {
				newNode = &this->getHierarchy()->insertRightSon(*parent);
			}
			else {
				newNode = &this->getHierarchy()->insertLeftSon(*parent);
			}
		}
		newNode->data_.key_ = key;
		newNode->data_.data_ = data;
		++size_;
		balanceTree(newNode);*/
	}

	template<typename K, typename T, typename ItemType>
	bool GeneralBinarySearchTree<K, T, ItemType>::tryFind(const K& key, T*& data) const
	{
		BSTNodeType* nodeWithKey = nullptr;
		if (!tryFindNodeWithKey(key, nodeWithKey)) {
			return false;
		}
		data = &nodeWithKey->data_.data_;
		return true;
	}

	template<typename K, typename T, typename ItemType>
	T GeneralBinarySearchTree<K, T, ItemType>::remove(const K& key)
	{
		BSTNodeType* node = nullptr;
		if (!this->tryFindNodeWithKey(key, node)) {
			throw structure_error("No such key");
		}
		T result = node->data_.data_;
		removeNode(node);
		--size_;
		return result;
	}

	template <typename K, typename T, typename ItemType>
	typename GeneralBinarySearchTree<K, T, ItemType>::IteratorType GeneralBinarySearchTree<K, T, ItemType>::begin() const
	{
		return this->getHierarchy()->begin();
	}

	template <typename K, typename T, typename ItemType>
	typename GeneralBinarySearchTree<K, T, ItemType>::IteratorType GeneralBinarySearchTree<K, T, ItemType>::end() const
	{
		return this->getHierarchy()->end();
	}

	template<typename K, typename T, typename ItemType>
	amt::BinaryEH<ItemType>* GeneralBinarySearchTree<K, T, ItemType>::getHierarchy() const
	{
		return dynamic_cast<amt::BinaryEH<ItemType>*>(this->memoryStructure_);
	}

	template<typename K, typename T, typename ItemType>
	void GeneralBinarySearchTree<K, T, ItemType>::removeNode(BSTNodeType* node)
	{
		/*BSTNodeType* parent = static_cast<BSTNodeType*>(node->parent_);

		switch (this->getHierarchy()->degree(*node))
		{
		case 0:
			if (this->getHierarchy()->isRoot(*node)) this->getHierarchy()->clear();
			else {
				if (this->getHierarchy()->isLeftSon(*node)) this->getHierarchy()->removeLeftSon(*node);
				else this->getHierarchy()->removeRightSon(*node);
			}
			break;
		case 1:
			BSTNodeType * son = this->getHierarchy()->hasLeftSon(*node) ? node->left_ : node->right_;
			if (node->left_ == son) this->getHierarchy()->changeLeftSon(*node, nullptr);
			else this->getHierarchy()->changeRightSon(*node, nullptr);
			if (this->getHierarchy()->isRoot(*node)) {
				this->getHierarchy()->clear();
				this->getHierarchy()->changeRoot(son);
			}
			else {
				if (parent->left_ == node) {
					this->getHierarchy()->removeLeftSon(*parent);
					this->getHierarchy()->changeLeftSon(*parent, nullptr);
				}
				else {
					this->getHierarchy()->removeRightSon(*parent);
					parent->right_ = son;
					this->getHierarchy()->changeRightSon(*parent, nullptr);
				}
			}
			break;
		case 2:
			BSTNodeType* inOrderPredecessor = node->left_;
			while (this->getHierarchy()->hasRightSon(*inOrderPredecessor)) {
				inOrderPredecessor = inOrderPredecessor->right_;
			}
			swap(node->data_, inOrderPredecessor->data_);
			this->removeNode(inOrderPredecessor);
			break;
		default:
			break;
		}*/
	}

	template<typename K, typename T, typename ItemType>
	bool GeneralBinarySearchTree<K, T, ItemType>::tryFindNodeWithKey(const K& key, BSTNodeType*& node) const
	{
		if (this->isEmpty()) {
			return false;
		}
		node = this->getHierarchy()->accessRoot();
		while (node->data_.key_ != key && !this->getHierarchy()->isLeaf(*node)) {
			if (key < node->data_.key_) {
				if (node->left_ != nullptr) {
					node = node->left_;
				}
				else {
					return false;
				}
			}
			else {
				if (node->right_ != nullptr) {
					node = node->right_;
				}
				else {
					return false;
				}
			}
		}
		return node->data_.key_ = key;
	}

	template<typename K, typename T, typename ItemType>
	void GeneralBinarySearchTree<K, T, ItemType>::rotateLeft(BSTNodeType* node)
	{
		BSTNodeType* leftSon = node->left_;
		BSTNodeType* parent = getHierarchy()->accessParent(*node);
		BSTNodeType* grandParent = getHierarchy()->accessParent(*parent);
		getHierarchy()->changeRightSon(*parent, nullptr);
		getHierarchy()->changeLeftSon(*node, nullptr);

		if (grandParent)
		{
			if (grandParent->left_ == parent)
			{
				getHierarchy()->changeLeftSon(*grandParent, node);
			}
			else
			{
				getHierarchy()->changeRightSon(*grandParent, node);
			}
		}
		else
		{
			getHierarchy()->changeRoot(node);
		}

		getHierarchy()->changeRightSon(*parent, leftSon);
		getHierarchy()->changeLeftSon(*node, parent);
	}

	template<typename K, typename T, typename ItemType>
	void GeneralBinarySearchTree<K, T, ItemType>::rotateRight(BSTNodeType* node)
	{
		BSTNodeType* leftSon = node->left_;
		BSTNodeType* parent = getHierarchy()->accessParent(*node);
		BSTNodeType* grandParent = getHierarchy()->accessParent(*parent);
		getHierarchy()->changeLeftSon(*parent, nullptr);
		getHierarchy()->changeRightSon(*node, nullptr);

		if (grandParent)
		{
			if (grandParent->left_ == parent)
			{
				getHierarchy()->changeLeftSon(*grandParent, node);
			}
			else
			{
				getHierarchy()->changeRightSon(*grandParent, node);
			}
		}
		else
		{
			getHierarchy()->changeRoot(node);
		}

		getHierarchy()->changeLeftSon(*parent, leftSon);
		getHierarchy()->changeRightSon(*node, parent);
	}

	//----------

	template<typename K, typename T>
	bool BinarySearchTree<K, T>::equals(const ADT& other)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}

	//----------

	template<typename K, typename T>
	Treap<K, T>::Treap() :
		rng_(std::random_device()())
	{
	}

	template<typename K, typename T>
	void Treap<K, T>::removeNode(BSTNodeType* node)
	{
		node->data_.priority_ = rng_.max();
		while (this->getHierarchy()->degree(*node) == 2) {
			BSTNodeType* leftSon = node->left_;
			BSTNodeType* rightSon = node->right_;
			if (leftSon->data_.priority_ < rightSon->data_.priority_) {
				this->rotateRight(leftSon);
			}
			else {
				this->rotateLeft(rightSon);
			}
		}
		GeneralBinarySearchTree<K, T, TreapItem<K, T>>::removeNode(node);
	}

	template<typename K, typename T>
	void Treap<K, T>::balanceTree(BSTNodeType* node)
	{
		node->data_.priority_ = rng_();
		BSTNodeType* parent = static_cast<BSTNodeType*>(node->parent_);
		while (parent != nullptr && parent->data_.priority_ > node->data_.priority_) {
			if (parent->left_ == node) {
				this->rotateRight(node);
			}
			else {
				this->rotateLeft(node);
			}
			parent = static_cast<BSTNodeType*>(node->parent_);
		}
	}

	template<typename K, typename T>
	bool Treap<K, T>::equals(const ADT& other)
	{
		// TODO 11
		// po implementacii vymazte vyhodenie vynimky!
		throw std::runtime_error("Not implemented yet");
	}
}