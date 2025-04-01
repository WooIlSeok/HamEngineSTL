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
        using iterator = T*;
        using constIterator = const T*;

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
            Clear();
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

        //Iterator// (원시 포인터 반환)
        constexpr iterator Begin() { return GetArray(); }
        constexpr iterator End() { return GetArray() + mSize; }
        constexpr constIterator Begin() const { return GetArray(); }
        constexpr constIterator End() const { return GetArray() + mSize; }

        // Info //
        constexpr size_t Size() const { return mSize; }
        constexpr size_t Capacity() const { return N; }
        constexpr bool Empty() const { return mSize == 0; }

        // Modifiers //
        constexpr void PushBack(const T& input);
        constexpr void PushBack(T&& input);
        constexpr void PopBack();
        constexpr iterator Insert(iterator pos, const T& value);
        constexpr iterator Erase(iterator pos);
        constexpr iterator Erase(iterator start, iterator end);
        constexpr void Clear();
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
    constexpr FixedVector<T,N>::iterator FixedVector<T, N>::Insert(iterator pos, const T& value)
    {
        ASSERT(pos >= GetArray());
        size_t insertIdx = pos - GetArray();
        ASSERT(insertIdx <= mSize);
        ASSERT(mSize < N);

        if ((insertIdx) == mSize)
        {
            this->PushBack(value);
            return GetArray() + insertIdx;
        }

        ::new(&GetArray()[mSize]) T(std::move(GetArray()[mSize - 1]));

        std::move_backward(GetArray() + insertIdx, GetArray() + mSize - 1, GetArray() + mSize);

        GetArray()[insertIdx].~T();
        ::new(&GetArray()[insertIdx]) T(value);
        ++mSize;

        return GetArray() + insertIdx;
    }

    template <typename T, size_t N>
    constexpr FixedVector<T, N>::iterator FixedVector<T, N>::Erase(iterator pos)
    {
        size_t eraseIdx = pos - GetArray();
        ASSERT(pos >= GetArray());
        ASSERT(eraseIdx < mSize);

        std::move(GetArray() + eraseIdx + 1, GetArray() + mSize, GetArray() + eraseIdx);

        GetArray()[mSize - 1].~T();
        --mSize;

        return GetArray() + eraseIdx;
    }

    template <typename T, size_t N>
    constexpr FixedVector<T, N>::iterator FixedVector<T, N>::Erase(iterator start, iterator end)
    {
        size_t startIdx = start - GetArray();
        size_t endIdx = end - GetArray();
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
    constexpr void FixedVector<T, N>::Clear()
    {
        for (size_t i = 0; i < mSize; ++i)
        {
            GetArray()[i].~T();
        }
        mSize = 0;
    }
}
