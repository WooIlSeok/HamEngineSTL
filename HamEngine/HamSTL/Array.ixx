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

        Array() {}
        Array(const Array& other)
        {
            for (size_t i = 0; i < N; ++i)
            {
                mArray[i] = other[i];
            }
        }
        Array(Array&& other)
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
        inline T& operator[](size_t idx) { ASSERT(idx < N); return mArray[idx]; }
        inline const T& operator[](size_t idx) const { ASSERT(idx < N); return mArray[idx]; }
        inline T& Front() { return mArray[0]; }
        inline T& Back() { return mArray[N-1]; }
        inline T* Data() { return mArray; }
        inline const T* Data() const { return mArray; }

        // Iterator //
        inline Iterator Begin() { return Iterator(mArray); }
        inline Iterator End() { return Iterator(mArray + N); }
        inline const Iterator Begin() const { return Iterator(const_cast<T*>(mArray)); }
        inline const Iterator End() const { return Iterator(const_cast<T*>(mArray + N)); }

        // Info //
        inline size_t Size() const { return N; }

        //operator//
        bool operator==(const Array& rhs) const { return std::equal(Begin(), End(), rhs.Begin()); }
        bool operator!=(const Array& rhs) const { return std::equal(Begin(), End(), rhs.Begin()) == false; }
        bool operator<(const Array& rhs) const { return std::lexicographical_compare(Begin(), End(), rhs.Begin(), rhs.End());}
        bool operator<=(const Array& rhs) const { return !(rhs < *this); }
        bool operator>(const Array& rhs) const { return rhs < *this ; }
        bool operator>=(const Array& rhs) const { return !(*this < rhs); }

        // Modifiers //
        inline void fill(const T& value)
        {
            for (size_t i = 0; i < N; ++i)
            {
                mArray[i] = value;
            }
        }
        inline void swap(T& element1, T& element2)
        {
            std::swap(element1, element2);
        }
    };
}


