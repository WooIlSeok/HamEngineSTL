module;

import std.core;
import Common.Memory;
import Common.Assert;

export module HamSTL.FixedVector;

export namespace ham
{
    template <typename T, size_t N>
    class FixedVector
    {
    public:
        class Iterator
        {
        public:
            Iterator() : mPtr(nullptr) {}
            Iterator(T* ptr) : mPtr(ptr) {}

            T& operator*() { return *mPtr; }
            const T& operator*() const { return static_cast<const T>(*mPtr); }
            T* operator->() { return mPtr; }
            const T* operator->() const { return mPtr; }

            Iterator& operator++() { ++mPtr; return *this; }
            Iterator operator++(int) { Iterator temp = *this; ++mPtr; return temp; }
            Iterator& operator--() { --mPtr; return *this; }
            Iterator operator--(int) { Iterator temp = *this; --mPtr; return temp; }
            Iterator operator+(size_t idx) const { return Iterator(mPtr + idx); }
            Iterator operator-(size_t idx) const { return Iterator(mPtr - idx); }
            ptrdiff_t operator-(const Iterator& iter) const { return (mPtr - iter.mPtr); }

            void operator+=(size_t idx) { mPtr + idx;}
            void operator-=(size_t idx) { mPtr - idx;}
            bool operator<(const Iterator& rhs) const { return mPtr < rhs.mPtr; }
            bool operator>(const Iterator& rhs) const { return mPtr > rhs.mPtr; }
            bool operator<=(const Iterator& rhs) const { return mPtr <= rhs.mPtr; }
            bool operator>=(const Iterator& rhs) const { return mPtr >= rhs.mPtr; }
            bool operator==(const Iterator& rhs) const { return mPtr == rhs.mPtr; }
            bool operator!=(const Iterator& rhs) const { return mPtr != rhs.mPtr; }
        private:
            T* mPtr;
            friend class FixedVector<T, N>;
        };

        FixedVector() noexcept : mSize(0) {}
        FixedVector(const FixedVector& other) : mSize(other.mSize)
        {
            for (size_t i = 0; i < mSize; ++i)
            {
                ::new(&GetArray()[i]) T(other[i]);
            }
        }
        FixedVector(FixedVector&& other) noexcept : mSize(other.mSize)
        {
            for (size_t i = 0; i < mSize; ++i)
            {
                ::new(&GetArray()[i]) T(std::move(other[i]));
                other[i].~T();
            }
            other.mSize = 0;
        }
        ~FixedVector()
        {
            clear();
        }
        FixedVector& operator=(const FixedVector& other)
        {
            ASSERT(other.mSize <= N);
            if (this != &other)
            {
                size_t commonSize = (mSize < other.mSize) ? mSize : other.mSize;
                for (size_t i = 0; i < commonSize; ++i)
                {
                    GetArray()[i] = other[i];
                }
                for (size_t i = other.mSize; i < mSize; ++i)
                {
                    GetArray()[i].~T();
                }
                for (size_t i = mSize; i < other.mSize; ++i)
                {
                    ::new(&GetArray()[i]) T(other[i]);
                }
                mSize = other.mSize;
            }
            return *this;
        }

    private:
        size_t mSize;

        alignas(T) unsigned char mArray[sizeof(T) * N];

        T* GetArray() { return reinterpret_cast<T*>(mArray); }
        const T* GetArray() const { return reinterpret_cast<const T*>(mArray); }

    public:
        // Element Access //
        inline T& operator[](size_t idx) { ASSERT(idx < mSize); return GetArray()[idx]; }
        inline const T& operator[](size_t idx) const { ASSERT(idx < mSize); return GetArray()[idx]; }
        inline T& Front() { ASSERT(mSize > 0); return GetArray()[0]; }
        inline T& Back() { ASSERT(mSize > 0); return GetArray()[mSize - 1]; }

        // Iterator //
        inline Iterator Begin() { return Iterator(GetArray()); }
        inline Iterator End() { return Iterator(GetArray() + mSize); }

        // Info //
        inline size_t Size() const { return mSize; }
        inline size_t Capacity() const { return N; }
        inline bool Empty() const { return mSize == 0; }

        // Modifiers //
        void PushBack(const T& input);
        void PushBack(T&& input);
        void PopBack();
        Iterator Insert(const Iterator pos, const T& value);
        Iterator Erase(const Iterator pos);
        Iterator Erase(const Iterator start, const Iterator end);
        void clear();
    };
}

namespace ham
{
    template <typename T, size_t N>
    void FixedVector<T, N>::PushBack(const T& input)
    {
        ASSERT(mSize < N);
        ::new(&GetArray()[mSize]) T(input);
        ++mSize;
    }

    template <typename T, size_t N>
    void FixedVector<T, N>::PushBack(T&& input)
    {
        ASSERT(mSize < N);
        ::new(&GetArray()[mSize]) T(std::move(input));
        ++mSize;
    }

    template <typename T, size_t N>
    void FixedVector<T, N>::PopBack()
    {
        ASSERT(mSize > 0);
        --mSize;
        GetArray()[mSize].~T();
    }

    template <typename T, size_t N>
    FixedVector<T, N>::Iterator FixedVector<T, N>::Insert(const Iterator pos, const T& value)
    {
        size_t insertIdx = pos.mPtr - GetArray();
        ASSERT((insertIdx >= 0) && ((insertIdx) <= mSize));
        ASSERT(mSize < N);

        if ((insertIdx) == mSize)
        {
            this->PushBack(value);
            return Iterator(GetArray() + insertIdx);
        }

        ::new(&GetArray()[mSize]) T(std::move(GetArray()[mSize - 1]));
        for (size_t i = mSize; i > (insertIdx); --i)
        {
            GetArray()[i] = std::move(GetArray()[i - 1]);
        }
        ::new(&GetArray()[insertIdx]) T(value);
        ++mSize;

        return Iterator(GetArray() + insertIdx);
    }

    template <typename T, size_t N>
    FixedVector<T, N>::Iterator FixedVector<T, N>::Erase(const Iterator pos)
    {
        size_t eraseIdx = pos.mPtr - GetArray();
        ASSERT((eraseIdx >= 0) && (eraseIdx < mSize));

        for (size_t i = eraseIdx; i < mSize - 1; ++i)
        {
            GetArray()[i] = std::move(GetArray()[i + 1]);
        }

        GetArray()[mSize - 1].~T();
        --mSize;
        return Iterator(GetArray() + eraseIdx);
    }

    template <typename T, size_t N>
    FixedVector<T, N>::Iterator FixedVector<T, N>::Erase(const Iterator start, const Iterator end)
    {
        size_t startIdx = start.mPtr - GetArray();
        size_t endIdx = end.mPtr - GetArray();
        size_t numPull = mSize - (endIdx);
        ASSERT(startIdx <= endIdx);
        ASSERT((startIdx >= 0) && (endIdx < mSize));

        size_t i;
        for (i = 0; i < numPull; ++i)
        {
            GetArray()[startIdx + i] = GetArray()[endIdx + i];
        }

        for (; i < mSize; ++i)
        {
            GetArray()[startIdx + i].~T();
        }
        mSize = mSize - (endIdx - startIdx);
        return start;
    }

    template <typename T, size_t N>
    void FixedVector<T, N>::clear()
    {
        for (size_t i = 0; i < mSize; ++i)
        {
            GetArray()[i].~T();
        }
        mSize = 0;
    }
}
