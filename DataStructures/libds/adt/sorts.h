#pragma once

#include <libds/amt/implicit_sequence.h>
#include <libds/adt/queue.h>
#include <libds/adt/array.h>
#include <functional>
#include <cmath>
#include <algorithm>
#include <type_traits>

namespace ds::adt
{
	template <typename T>
	struct Sort
	{
	public:
		virtual void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) = 0;
		void sort(amt::ImplicitSequence<T>& is) { sort(is, [](const T& a, const T& b)->bool {return a < b; }); }
	};

	//----------

	template <typename T>
	class SelectSort :
		public Sort<T>
	{
	public:
		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
	};

	//----------

	template <typename T>
	class InsertSort :
		public Sort<T>
	{
	public:
		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
	};

	//----------

	template <typename T>
	class BubbleSort :
		public Sort<T>
	{
	public:
		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
	};

	//----------

	template <typename T>
	class QuickSort :
		public Sort<T>
	{
	public:
		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

	private:
		void quick(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t min, size_t max);
	};

	//----------

	template <typename T>
	class HeapSort :
		public Sort<T>
	{
	public:
		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;
	};

	//----------

	template <typename T>
	class ShellSort :
		public Sort<T>
	{
	public:
		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

	private:
		void shell(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t k);
	};

	//----------

	template <typename Key, typename T>
	class RadixSort :
		public Sort<T>
	{
		static_assert(std::is_integral_v<Key>, "Radix sort supports only integral types.");

	public:
		RadixSort();
		RadixSort(std::function<Key(const T&)> getKey);

		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

	private:
		std::function<Key(const T&)> getKey_;
	};

	//----------

	template <typename T>
	class MergeSort :
		public Sort<T>
	{
	public:
		void sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare) override;

	private:
		void split(size_t n);
		void merge(std::function<bool(const T&, const T&)> compare, size_t n);

	private:
		ImplicitQueue<T>* queue1_{ nullptr };
		ImplicitQueue<T>* queue2_{ nullptr };
		ImplicitQueue<T>* mergeQueue_{ nullptr };
	};

	//----------

	template<typename T>
	void SelectSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		for (size_t i = 0; i < (is.size() - 1); i++)
		{
			size_t min = i;
			for (size_t j = i + 1; j < is.size(); j++)
			{
				if (compare(is.access(j)->data_, is.access(min)->data_))
				{
					min = j;
				}
			}
			std::swap(is.access(i)->data_, is.access(min)->data_);
		}

	}

	template<typename T>
	void InsertSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		for (size_t i = 1; i < is.size(); i++)
		{
			T tmp = is.access(i)->data_;
			size_t j = i;
			while (j > 0 && compare(tmp, is.access(j - 1)->data_))
			{
				is.access(j)->data_ = is.access(j - 1)->data_;
				--j;
			}
			is.access(j)->data_ = tmp;
		}
	}

	template<typename T>
	void BubbleSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		bool swapped;
		do
		{
			swapped = false;
			for (size_t i = 0; i < is.size() - 1; ++i)
			{
				if (compare(is.access(i + 1)->data_, is.access(i)->data_))
				{
					using std::swap;
					swap(is.access(i + 1)->data_, is.access(i)->data_);
					swapped = true;
				}
			}
		} while (swapped);
	}

	template<typename T>
	void QuickSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		if (!is.isEmpty())
		{
			quick(is, compare, 0, is.size() - 1);
		}
	}

	template<typename T>
	void QuickSort<T>::quick(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t min, size_t max)
	{
		T pivot = is.access(min + (max - min) / 2)->data_;
		size_t left = min;
		size_t right = max;
		while (left <= right) {
			while (compare(is.access(left)->data_, pivot))
			{
				++left;
			}
			while (right > 0 && compare(pivot, is.access(right)->data_))
			{
				--right;
			}
			if (left <= right)
			{
				std::swap(is.access(left)->data_, is.access(right)->data_);
				++left;
				if (right > 0)
				{
					--right;
				}
			}
		}
		if (min < right)
		{
			this->quick(is, compare, min, right);
		}
		if (left < max)
		{
			this->quick(is, compare, left, max);
		}
	}

	template<typename T>
	void HeapSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		for (size_t i = 1; i < (is.size() - 1); i++)
		{
			bool swapped;
			size_t actual = i;
			do
			{
				swapped = false;
				size_t parent = (actual - 1) / 2;
				if (actual > 0 && compare(is.access(parent)->data_, is.access(actual)->data_))
				{
					std::swap(is.access(actual)->data_, is.access(parent)->data_);
					actual = parent;
					swapped = true;
				}
			} while (swapped);
		}
		for (size_t i = is.size() - 1; i > 0; i--)
		{
			std::swap(is.access(0)->data_, is.access(i)->data_);
			bool swapped;
			size_t actual = 0;
			do
			{
				swapped = false;
				size_t left = 2 * actual + 1;
				size_t right = 2 * actual + 2;
				size_t max;
				if (left < i && right < i)
				{
					if (compare(is.access(right)->data_, is.access(left)->data_))
					{
						max = left;
					}
					else
					{
						max = right;
					}
				}
				else
				{
					if (left < i)
					{
						max = left;
					}
					else
					{
						max = right;
					}
				}
				if (max < i && compare(is.access(actual)->data_, is.access(max)->data_))
				{
					std::swap(is.access(actual)->data_, is.access(max)->data_);
					actual = max;
					swapped = true;
				}
			} while (swapped);
		}
	}

	template<typename T>
	void ShellSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		shell(is, compare, static_cast<size_t>(std::ceil(std::log10(is.size()))));
	}

	template<typename T>
	void ShellSort<T>::shell(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare, size_t k)
	{
		for (size_t d = 0; d < k; d++)
		{
			for (size_t i = d; i <= is.size() - 1; i += k) {
				size_t j = i;
				while (j >= k && j - k >= d && compare(is.access(j)->data_, is.access(j - k)->data_))
				{
					std::swap(is.access(j)->data_, is.access(j - k)->data_);
					j = j - k;
				}
			}
		}
		if (k > 1)
		{
			shell(is, compare, k - 1);
		}
	}

	template<typename Key, typename T>
	RadixSort<Key, T>::RadixSort() :
		getKey_([](auto const& x) { return x; })
	{
	}

	template<typename Key, typename T>
	RadixSort<Key, T>::RadixSort(std::function<Key(const T&)> getKey) :
		getKey_(getKey)
	{
	}

	template<typename Key, typename T>
	void RadixSort<Key, T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		{
			Array<ExplicitQueue<T>*> compartments = 10;

			for (size_t i = 0; i < 10; i++)
			{
				compartments.set(new ExplicitQueue<T>(), i);
			}

			size_t component = 1;
			bool nextComponentExists = true;

			while (nextComponentExists)
			{
				nextComponentExists = false;
				for (T element : is)
				{
					Key key = getKey_(element);
					compartments.access((key / component) % 10)->push(element);

					if (element / (component * 10) > 0)
					{
						nextComponentExists = true;
					}
				}

				component = component * 10;
				size_t index = 0;

				for (size_t i = 0; i < 10; i++)
				{
					ExplicitQueue<T>* compartment = compartments.access(i);
					while (!compartment->isEmpty())
					{
						is.access(index)->data_ = compartment->pop();
						++index;
					}
				}
			}
			for (size_t i = 0; i < (compartments.size() - 1); i++)
			{
				delete compartments.access(i);
			}
		}
	}

	template<typename T>
	void MergeSort<T>::sort(amt::ImplicitSequence<T>& is, std::function<bool(const T&, const T&)> compare)
	{
		this->queue1_ = new ImplicitQueue<T>(is.size());
		this->queue2_ = new ImplicitQueue<T>(is.size());
		this->mergeQueue_ = new ImplicitQueue<T>(is.size());

		for (T element : is)
		{
			this->mergeQueue_->push(element);
		}

		size_t i = 1;
		while (i < is.size())
		{
			this->split(i);
			this->merge(compare, i);
			i = i * 2;
		}

		this->split(i);
		this->merge(compare, i);

		for (size_t i = 0; i < is.size(); i++)
		{
			is.access(i)->data_ = this->mergeQueue_->pop();
		}

		delete this->queue1_;
		delete this->queue2_;
		delete this->mergeQueue_;
		this->queue1_ = nullptr;
		this->queue2_ = nullptr;
		this->mergeQueue_ = nullptr;
	}

	template<typename T>
	void MergeSort<T>::split(size_t n)
	{
		size_t count = 0;
		bool isFirst = true;

		while (!this->mergeQueue_->isEmpty())
		{
			if (count % n == 0)
			{
				count = 0;
				isFirst = !isFirst;
			}

			isFirst ? this->queue1_->push(this->mergeQueue_->pop()) : this->queue2_->push(this->mergeQueue_->pop());
			++count;
		}
	}

	template<typename T>
	void MergeSort<T>::merge(std::function<bool(const T&, const T&)> compare, size_t n)
	{
		size_t firstCount = 0;
		size_t secondCount = 0;

		do
		{
			if (firstCount == 0 && secondCount == 0)
			{
				firstCount = std::min<int>(n, this->queue1_->size());
				secondCount = std::min<int>(n, this->queue2_->size());
			}

			T* key1 = firstCount > 0 ? &(this->queue1_->peek()) : nullptr;
			T* key2 = secondCount > 0 ? &(this->queue2_->peek()) : nullptr;

			if (key1 != nullptr && key2 != nullptr)
			{
				if (compare(*key1, *key2))
				{
					--firstCount;
					this->mergeQueue_->push(this->queue1_->pop());
				}
				else
				{
					--secondCount;
					this->mergeQueue_->push(this->queue2_->pop());
				}
			}
			else
			{
				if (key1 != nullptr)
				{
					--firstCount;
					this->mergeQueue_->push(this->queue1_->pop());
				}
				else if (key2 != nullptr)
				{
					--secondCount;
					this->mergeQueue_->push(this->queue2_->pop());
				}
			}
		} while (!this->queue1_->isEmpty() || !this->queue2_->isEmpty());
	}
}