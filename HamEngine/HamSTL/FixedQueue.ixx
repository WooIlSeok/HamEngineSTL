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
        FixedQueue(const FixedQueue& other) : mHead(other.mHead), mTail(other.mTail)
        {
            for (size_t i = 0; i < Size(); ++i) {
                size_t idx = (mHead + i) % (N + 1);
                ::new(GetArbitraryPtr(idx)) T(*(other.GetArbitraryPtr(idx)));
            }
        }

        FixedQueue(FixedQueue&& other) : mHead(other.mHead), mTail(other.mTail)
        {
            for (size_t i = 0; i < Size(); ++i) {
                size_t idx = (mHead + i) % (N + 1);
                ::new(GetArbitraryPtr(idx)) T(std::move(*(other.GetArbitraryPtr(idx))));
                other.GetArbitraryPtr(idx)->~T();
            }
            other.mHead = 0;
            other.mTail = 0;
        }

        ~FixedQueue()
        {
            Clear();
        }

        FixedQueue& operator=(const FixedQueue& other)
        {
            if (this != &other)
            {
                Clear();
                mHead = other.mHead;
                mTail = other.mTail;
                for (size_t i = 0; i < Size(); ++i) {
                    size_t idx = (mHead + i) % (N + 1);
                    ::new(GetArbitraryPtr(idx)) T(*(other.GetArbitraryPtr(idx)));
                }
            }
            return *this;
        }

        FixedQueue& operator=(FixedQueue&& other)
        {
            if (this != &other)
            {
                Clear();
                mHead = other.mHead;
                mTail = other.mTail;
                for (size_t i = 0; i < Size(); ++i) {
                    size_t idx = (mHead + i) % (N + 1);
                    ::new(GetArbitraryPtr(idx)) T(std::move(*(other.GetArbitraryPtr(idx))));
                    other.GetArbitraryPtr(idx)->~T();
                }
                other.mHead = 0;
                other.mTail = 0;
            }
            return *this;
        }

    private:
        size_t mHead;
        size_t mTail;

        alignas(T) unsigned char mArray[sizeof(T) * (N + 1)]; //(N+1)개의 공간. 실제 Capacity는 N
        constexpr T* GetArbitraryPtr(size_t idx) { return reinterpret_cast<T*>(mArray) + idx; }
        constexpr const T* GetArbitraryPtr(size_t idx) const { return reinterpret_cast<const T*>(mArray) + idx; } //이동, 대입 생성자에만 사용
        constexpr T* GetHeadPtr() { return reinterpret_cast<T*>(mArray) + mHead; }
        constexpr T* GetTailPtr() { return reinterpret_cast<T*>(mArray) + mTail; }

    public:
        // Element Access //
        constexpr const T& Front() { return (reinterpret_cast<T*>(mArray))[mHead]; }
        constexpr const T& Back() { return (reinterpret_cast<T*>(mArray))[mTail == 0 ? N - 1 : mTail - 1]; }

        // Info //
        constexpr size_t Size() const { return (mHead <= mTail) ? (mTail - mHead) : (N + 1) - (mHead - mTail); }
        constexpr size_t Capacity() const { return N; }
        constexpr bool Empty() const { return mHead == mTail; }

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
    constexpr void FixedQueue<T, N>::Push(const T& input)
    {
        ASSERT(Size() < N);
        ::new(GetTailPtr()) T(input);
        mTail = (mTail < N) ? mTail + 1 : 0;
    }

    template <typename T, size_t N>
    constexpr void FixedQueue<T, N>::Push(T&& input)
    {
        ASSERT(Size() < N);
        ::new(GetTailPtr()) T(std::move(input));
        mTail = (mTail < N) ? mTail + 1 : 0;
    }

    template <typename T, size_t N>
    constexpr void FixedQueue<T, N>::Pop()
    {
        ASSERT(mHead != mTail);
        GetHeadPtr()->~T();
        mHead = (mHead < N) ? mHead + 1 : 0;
    }

    template <typename T, size_t N>
    constexpr void FixedQueue<T, N>::Clear()
    {
        while (mHead != mTail)
            Pop();
    }
}
