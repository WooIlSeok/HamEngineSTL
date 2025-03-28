module;

import std.core;
import Common.Memory;
import Common.Assert;

export module HamSTL.FixedQueue;

export namespace ham
{
    template <typename T, size_t N>
    class FixedQueue
    {
    public:

        //Constructor//
        FixedQueue() : mHead(0), mTail(0) {}
        FixedQueue(const FixedQueue& other) : mSize(other.mSize)
        {
            if (mHead < mTail)
            for (size_t i = ; i < mSize; ++i)
            {
                ::new(&GetArray()[i]) T(other[i]);
            }
        }
        FixedQueue(FixedQueue&& other) : mSize(other.mSize)
        {
            for (size_t i = 0; i < mSize; ++i)
            {
                ::new(&GetArray()[i]) T(std::move(other[i]));
                other[i].~T();
            }
            other.mHead = 0;
            other.mTail = 0;
        }
        ~FixedQueue()
        {
            clear();
        }
        FixedQueue& operator=(const FixedQueue& other)
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
        FixedQueue& operator=(FixedQueue&& other)
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
        size_t mHead;
        size_t mTail;

        alignas(T) unsigned char mArray[sizeof(T) * N];

    public:
        // Element Access //
        constexpr const T& Front() { return (reinterpret_cast<T*>(mArray))[mHead]; }
        constexpr const T& Back() { return (reinterpret_cast<T*>(mArray))[mTail]; }

        // Info //
        constexpr size_t Size() const { return (mHead <= mTail) ? (mTail - mHead) : N - (mHead - mTail); }
        constexpr size_t Capacity() const { return N; }
        constexpr bool Empty() const { return mSize == 0; }

        // Modifiers //
        constexpr void Push(const T& input);
        constexpr void Push(T&& input);
        constexpr void Pop();
        constexpr void clear();
    };
}

namespace ham
{
    template <typename T, size_t N>
    constexpr void FixedQueue<T, N>::Push(const T& input)
    {
        ::new(&GetArray()[mSize]) T(input);
    }

    template <typename T, size_t N>
    constexpr void FixedQueue<T, N>::Push(T&& input)
    {
        ASSERT(mSize < N);
        ::new(&GetArray()[mSize]) T(std::move(input));
        ++mSize;
    }

    template <typename T, size_t N>
    constexpr void FixedQueue<T, N>::Pop()
    {
        ASSERT(mSize > 0);
        --mSize;
        GetArray()[mSize].~T();
    }

    template <typename T, size_t N>
    constexpr FixedQueue<T, N>::Iterator FixedQueue<T, N>::Erase(const Iterator pos)
    {
        size_t eraseIdx = pos.mPtr - GetArray();
        ASSERT(pos.mPtr >= GetArray());
        ASSERT(eraseIdx < mSize);

        std::move(GetArray() + eraseIdx + 1, GetArray() + mSize, GetArray() + eraseIdx);

        GetArray()[mSize - 1].~T();
        --mSize;

        return Iterator(GetArray() + eraseIdx);
    }
