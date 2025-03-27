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
        class Iterator {
        public:
            Iterator() : mPtr(nullptr) {}
            Iterator(T* ptr) : mPtr(ptr) {}

            T& operator*() { return *mPtr; }
            const T& operator*() const { return *mPtr; }
            T* operator->() { return mPtr; }
            const T* operator->() const { return mPtr; }

            Iterator& operator++() { ++mPtr; return *this; }
            Iterator operator++(int) { Iterator temp = *this; ++mPtr; return temp; }
            Iterator& operator--() { --mPtr; return *this; }
            Iterator operator--(int) { Iterator temp = *this; --mPtr; return temp; }
            Iterator operator+(size_t idx) const { return Iterator(mPtr + idx); }
            Iterator operator-(size_t idx) const { return Iterator(mPtr - idx); }
            ptrdiff_t operator-(const Iterator& iter) const { return mPtr - iter.mPtr; }

            Iterator& operator+=(size_t idx) { mPtr += idx; return *this; }
            Iterator& operator-=(size_t idx) { mPtr -= idx; return *this; }
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

        //Constructor//
        FixedVector() : mSize(0) {}
        FixedVector(const FixedVector& other) : mSize(other.mSize)
        {
            for (size_t i = 0; i < mSize; ++i)
            {
                ::new(&GetArray()[i]) T(other[i]);
            }
        }
        FixedVector(FixedVector&& other) : mSize(other.mSize)
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
        FixedVector& operator=(FixedVector&& other)
        {
            ASSERT(other.mSize <= N);
            if (this != &other)
            {
                size_t commonSize = (mSize < other.mSize) ? mSize : other.mSize;
                for (size_t i = 0; i < commonSize; ++i)
                {
                    GetArray()[i] = std::move(other[i]);
                }
                for (size_t i = other.mSize; i < mSize; ++i)
                {
                    GetArray()[i].~T();
                }
                for (size_t i = mSize; i < other.mSize; ++i)
                {
                    ::new(&GetArray()[i]) T(std::move(other[i]));
                }
                mSize = other.mSize;
                other.mSize = 0;
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
        constexpr T& operator[](size_t idx) { ASSERT(idx < mSize); return GetArray()[idx]; }
        constexpr const T& operator[](size_t idx) const { ASSERT(idx < mSize); return GetArray()[idx]; }
        constexpr T& Front() { ASSERT(mSize > 0); return GetArray()[0]; }
        constexpr T& Back() { ASSERT(mSize > 0); return GetArray()[mSize - 1]; }

        // Iterator //
        constexpr Iterator Begin() { return Iterator(GetArray()); }
        constexpr Iterator End() { return Iterator(GetArray() + mSize); }

        // Info //
        constexpr size_t Size() const { return mSize; }
        constexpr size_t Capacity() const { return N; }
        constexpr bool Empty() const { return mSize == 0; }

        // Modifiers //
        constexpr void PushBack(const T& input);
        constexpr void PushBack(T&& input);
        constexpr void PopBack();
        constexpr Iterator Insert(const Iterator pos, const T& value);
        constexpr Iterator Erase(const Iterator pos);
        constexpr Iterator Erase(const Iterator start, const Iterator end);
        constexpr void clear();
    };
}

namespace ham
{
    template <typename T, size_t N>
    constexpr void FixedVector<T, N>::PushBack(const T& input)
    {
        ASSERT(mSize < N);
        ::new(&GetArray()[mSize]) T(input);
        ++mSize;
    }

    template <typename T, size_t N>
    constexpr void FixedVector<T, N>::PushBack(T&& input)
    {
        ASSERT(mSize < N);
        ::new(&GetArray()[mSize]) T(std::move(input));
        ++mSize;
    }

    template <typename T, size_t N>
    constexpr void FixedVector<T, N>::PopBack()
    {
        ASSERT(mSize > 0);
        --mSize;
        GetArray()[mSize].~T();
    }

    template <typename T, size_t N>
    constexpr FixedVector<T, N>::Iterator FixedVector<T, N>::Insert(const Iterator pos, const T& value)
    {
        ASSERT(pos.mPtr >= GetArray());
        size_t insertIdx = pos.mPtr - GetArray();
        ASSERT(insertIdx <= mSize);
        ASSERT(mSize < N);

        if ((insertIdx) == mSize)
        {
            this->PushBack(value);
            return Iterator(GetArray() + insertIdx);
        }

        ::new(&GetArray()[mSize]) T(std::move(GetArray()[mSize - 1]));

        std::move_backward(GetArray() + insertIdx, GetArray() + mSize - 1, GetArray() + mSize);

        GetArray()[insertIdx].~T();
        ::new(&GetArray()[insertIdx]) T(value);
        ++mSize;

        return Iterator(GetArray() + insertIdx);
    }

    template <typename T, size_t N>
    constexpr FixedVector<T, N>::Iterator FixedVector<T, N>::Erase(const Iterator pos)
    {
        size_t eraseIdx = pos.mPtr - GetArray();
        ASSERT(pos.mPtr >= GetArray());
        ASSERT(eraseIdx < mSize);

        std::move(GetArray() + eraseIdx + 1, GetArray() + mSize, GetArray() + eraseIdx);

        GetArray()[mSize - 1].~T();
        --mSize;

        return Iterator(GetArray() + eraseIdx);
    }

    template <typename T, size_t N>
    constexpr FixedVector<T, N>::Iterator FixedVector<T, N>::Erase(const Iterator start, const Iterator end)
    {
        size_t startIdx = start.mPtr - GetArray();
        size_t endIdx = end.mPtr - GetArray();
        size_t numPull = mSize - (endIdx);
        size_t numElementsToRemove = endIdx - startIdx;

        ASSERT(startIdx <= endIdx);
        ASSERT((startIdx >= 0) && (endIdx <= mSize));

        std::move(GetArray() + endIdx, GetArray() + mSize, GetArray() + startIdx);

        for (size_t i = mSize - numElementsToRemove; i < mSize; ++i)
        {
            GetArray()[i].~T();
        }
        mSize -= numElementsToRemove;

        return start;
    }

    template <typename T, size_t N>
    constexpr void FixedVector<T, N>::clear()
    {
        for (size_t i = 0; i < mSize; ++i)
        {
            GetArray()[i].~T();
        }
        mSize = 0;
    }
}
