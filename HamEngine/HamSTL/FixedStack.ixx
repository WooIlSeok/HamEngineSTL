module;

import std.core;
import Common.Memory;
import Common.Assert;

export module HamSTL.FixedStack;

export namespace ham
{
    template <typename T, size_t N>
    class FixedStack
    {
    public:

        //Constructor//
        FixedStack() : mNextFreeIdx(0) {}
        FixedStack(const FixedStack& other) : mNextFreeIdx(other.mNextFreeIdx)
        {
            for (size_t i = 0; i < Size(); ++i) {
                ::new(GetArbitraryPtr(i)) T(*(other.GetArbitraryPtr(i)));
            }
        }

        FixedStack(FixedStack&& other) : mNextFreeIdx(other.mNextFreeIdx)
        {
            for (size_t i = 0; i < Size(); ++i) {
                ::new(GetArbitraryPtr(i)) T(std::move(*(other.GetArbitraryPtr(i))));
                other.GetArbitraryPtr(i)->~T();
            }
            other.mNextFreeIdx = 0;
        }

        ~FixedStack()
        {
            Clear();
        }

        FixedStack& operator=(const FixedStack& other)
        {
            if (this != &other)
            {
                Clear();
                mNextFreeIdx = other.mNextFreeIdx;
                for (size_t i = 0; i < Size(); ++i) {
                    ::new(GetArbitraryPtr(i)) T(*(other.GetArbitraryPtr(i)));
                }
            }
            return *this;
        }

        FixedStack& operator=(FixedStack&& other)
        {
            if (this != &other)
            {
                Clear();
                mNextFreeIdx = other.mNextFreeIdx;
                for (size_t i = 0; i < Size(); ++i) {
                    ::new(GetArbitraryPtr(i)) T(std::move(*(other.GetArbitraryPtr(i))));
                    other.GetArbitraryPtr(i)->~T();
                }
                other.mNextFreeIdx = 0;
            }
            return *this;
        }

    private:
        size_t mNextFreeIdx; //다음에 삽입될 인덱스(최소 0), 마지막 값은 mNextFreeIdx - 1
        alignas(T) unsigned char mArray[sizeof(T) * N]; 

        constexpr T* GetArbitraryPtr(size_t idx) { return reinterpret_cast<T*>(mArray) + idx; }
        constexpr const T* GetArbitraryPtr(size_t idx) const { return reinterpret_cast<const T*>(mArray) + idx; } //이동, 대입 생성자에만 사용
        constexpr T* GetTopPtr() { ASSERT(mNextFreeIdx > 0);  return reinterpret_cast<T*>(mArray) + (mNextFreeIdx - 1); }
        constexpr T* GetNextFreePtr() { return reinterpret_cast<T*>(mArray) + mNextFreeIdx; }

    public:
        // Element Access //
        constexpr const T& Top() { ASSERT(mNextFreeIdx > 0); return (reinterpret_cast<T*>(mArray))[mNextFreeIdx - 1]; }

        // Info //
        constexpr size_t Size() const { return mNextFreeIdx; }
        constexpr size_t Capacity() const { return N; }
        constexpr bool Empty() const { return mNextFreeIdx == 0; }

        // Modifiers //
        constexpr void Push(const T& input);
        constexpr void Push(T&& input);
        constexpr void Pop();
        constexpr void Clear();
    };
}

namespace ham
{
    template <typename T, size_t N>
    constexpr void FixedStack<T, N>::Push(const T& input)
    {
        ASSERT(mNextFreeIdx < N);
        ::new(GetNextFreePtr()) T(input);
        ++mNextFreeIdx;
    }

    template <typename T, size_t N>
    constexpr void FixedStack<T, N>::Push(T&& input)
    {
        ASSERT(mNextFreeIdx < N);
        ::new(GetNextFreePtr()) T(std::move(input));
        ++mNextFreeIdx;
    }

    template <typename T, size_t N>
    constexpr void FixedStack<T, N>::Pop()
    {
        ASSERT(mNextFreeIdx > 0);
        GetTopPtr()->~T();
        --mNextFreeIdx;
    }

    template <typename T, size_t N>
    constexpr void FixedStack<T, N>::Clear()
    {
        while (mNextFreeIdx > 0)
            Pop();
    }
}
