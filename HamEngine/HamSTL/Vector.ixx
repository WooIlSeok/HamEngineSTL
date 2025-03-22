module;

import std.core;
import Common.Assert;

export module HamSTL.Vector;

export namespace ham
{
	template <typename T>
	using Vector = std::vector<T>;
	
	template <typename T, size_t N>
	class FixedVector
	{
	public:

		class Iterator
		{
		public:
			Iterator() : mPtr(nullptr) {};
			Iterator(T* ptr) : mPtr(ptr) {}

			T& operator*() const { return *mPtr; }
			Iterator& operator++() { ++mPtr; return *this; }
			Iterator operator++(int) { Iterator temp = *this; ++mPtr; return temp;}
			Iterator& operator--() { --mPtr; return *this; }
			Iterator operator--(int) { Iterator temp = *this; --mPtr;  return temp; }
			bool operator<(const Iterator& rhs) const { return mPtr < rhs.mPtr; };
			bool operator>(const Iterator& rhs) const { return mPtr > rhs.mPtr; };
			bool operator<=(const Iterator& rhs) const { return mPtr <= rhs.mPtr; };
			bool operator>=(const Iterator& rhs) const { return mPtr >= rhs.mPtr; };
			bool operator==(const Iterator& rhs) const { return mPtr == rhs.mPtr; };
			bool operator!=(const Iterator& rhs) const { return mPtr != rhs.mPtr; };
		private:
			T* mPtr;
			friend class FixedVector<T, N>;
		};

		FixedVector() : mSize(0) { std::memset(mArray, 0, sizeof(mArray)); }
		FixedVector(const FixedVector& other) : mSize(other.mSize) { std::memcpy(mArray, other.mArray, sizeof(mArray)); }
		FixedVector(FixedVector&& other) : mSize(other.mSize)
		{
			for (size_t i = 0; i < mSize; ++i) {
				mArray[i] = std::move(other.mArray[i]);
			}
			other.mSize = 0;
		}

	private:
		size_t mSize;
		T mArray[N];

	public:
		//Element Access//
		T& operator[](size_t idx) { ASSERT(idx < mSize); return mArray[idx]; }
		T& Front() { ASSERT(mSize > 0); return mArray[0]; }
		T& Back() { ASSERT(mSize > 0); return mArray[mSize-1]; }

		//Iterator//
		Iterator Begin() { return Iterator(&mArray[0]); }
		Iterator End() { return Iterator(&mArray[mSize - 1]); }

		//Modifiers//
		void PushBack(T input);
		void PopBack();
		Iterator Insert(const Iterator pos, const T& value);

		//Info//
		size_t Size() { return mSize; }
		size_t Capacity() { return N; }
		bool Empty() { return mSize == 0; }
	};
}


namespace ham
{

	template<typename T, size_t N>
	void FixedVector<T, N>::PushBack(T input) 
	{
		ASSERT(mSize <= N);

		mArray[mSize] = input;
		mSize++;
		return;
	}

	template<typename T, size_t N>
	void FixedVector<T, N>::PopBack()
	{
		ASSERT(mSize > 0);

		std::memset(&mArray[mSize-1], 0, sizeof(T));
		--mSize;
		return;
	}

	template<typename T, size_t N>
	FixedVector<T, N>::Iterator FixedVector<T, N>::Insert(const FixedVector<T, N>::Iterator iter, const T& value)
	{
		ASSERT(mSize < N);
		ptrdiff_t idx = iter.mPtr - &mArray[0];
		ASSERT((idx >= 0) && (idx <= mSize));
		
		for (size_t i = mSize; i > static_cast<size_t>(idx); --i)
		{
			mArray[i] = std::move(mArray[i - 1]);
		}
		mArray[idx] = value;
		mSize++;

		return Iterator(&mArray[idx]);
	}
}
