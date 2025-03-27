module;

import std.core;
import Common.Memory;
import Common.Assert;

export module HamSTL.Array;

export namespace ham
{
    template <typename T, size_t N>
    class Array
    {
    public:
        class Iterator
        {
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
            ptrdiff_t operator-(const Iterator& iter) const { return (mPtr - iter.mPtr); }

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
            friend class Array<T, N>;
        };

        constexpr Array() = default;
        constexpr Array(const Array& other)
        {
            for (size_t i = 0; i < N; ++i)
            {
                mArray[i] = other[i];
            }
        }
        constexpr Array(Array&& other)
        {
            for (size_t i = 0; i < N; ++i)
            {
                mArray[i] = std::move(other[i]);
            }
        }

        Array& operator=(const Array& other)
        {
            for (size_t i = 0; i < N; ++i)
            {
                mArray[i] = other[i];
            }
            return *this;
        }

        ~Array() = default;

    private:
        T mArray[N];

    public:
        // Element Access //
        constexpr T& operator[](size_t idx) { ASSERT(idx < N); return mArray[idx]; }
        constexpr const T& operator[](size_t idx) const { ASSERT(idx < N); return mArray[idx]; }
        constexpr T& Front() { return mArray[0]; }
        constexpr T& Back() { return mArray[N-1]; }
        constexpr T* Data() { return mArray; }
        constexpr const T* Data() const { return mArray; }

        // Iterator //
        constexpr Iterator Begin() { return Iterator(mArray); }
        constexpr Iterator End() { return Iterator(mArray + N); }
        constexpr const Iterator Begin() const { return Iterator(const_cast<T*>(mArray)); }
        constexpr const Iterator End() const { return Iterator(const_cast<T*>(mArray + N)); }

        // Info //
        constexpr size_t Size() const { return N; }

        //operator//
        bool operator==(const Array& rhs) const 
        { 
            Iterator lhsIter = this->Begin(), rhsIter = rhs.Begin(); 
            for (; lhsIter < this->End(); ++lhsIter, ++rhsIter)
            {
                if (*lhsIter != *rhsIter)
                    return false;
            }
            return true;
        }
        bool operator!=(const Array& rhs) const { return (*this == rhs) == false; }
        bool operator<(const Array& rhs) const 
        { 
            Iterator lhsIter = this->Begin(), rhsIter = rhs.Begin();
            for (; lhsIter < this->End(); ++lhsIter, ++rhsIter)
            {
                if (*lhsIter < *rhsIter)
                    return true;
                else if (*lhsIter > *rhsIter)
                    return false;
            }
            return false;
        }
        bool operator<=(const Array& rhs) const { return !(rhs < *this); }
        bool operator>(const Array& rhs) const { return rhs < *this ; }
        bool operator>=(const Array& rhs) const { return !(*this < rhs); }

        // Modifiers //
        void Fill(const T value)
        {
            for (size_t i = 0; i < N; ++i)
            {
                mArray[i] = value;
            }
        }
        void Swap(T& element1, T& element2)
        {
            std::swap(element1, element2);
        }
    };
}


