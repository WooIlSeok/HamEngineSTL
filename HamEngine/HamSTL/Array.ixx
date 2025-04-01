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

        using iterator = T*;
        using constIterator = const T*;

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

        // Iterator //
        constexpr iterator Begin() { return mArray; }
        constexpr iterator End() { return mArray + N; }
        constexpr constIterator Begin() const { return mArray; }
        constexpr constIterator End() const { return mArray + N; }

        // Info //
        constexpr size_t Size() const { return N; }

        //operator//
        bool operator==(const Array& rhs) const 
        { 
            T* lhsIter = this->Begin(), rhsIter = rhs.Begin(); 
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
            T* lhsIter = this->Begin(), rhsIter = rhs.Begin();
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


