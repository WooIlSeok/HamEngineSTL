import Common;
import std.core;
import Math;
import ECS;
import HamSTL;

using namespace ham;

int main(void)
{   
    FixedQueue<int, 5> q;
    ASSERT(q.Empty());
    ASSERT(q.Size() == 0);
    std::cout << "Initial queue empty, size: " << q.Size() << "\n";

    // 2. Push() 테스트: 요소 삽입
    q.Push(1);
    q.Push(2);
    q.Push(3);
    ASSERT(!q.Empty());
    ASSERT(q.Size() == 3);
    ASSERT(q.Front() == 1);
    ASSERT(q.Back() == 3);
    std::cout << "After Push 1,2,3: Front = " << q.Front() << ", Back = " << q.Back()
        << ", Size = " << q.Size() << "\n";

    // 3. Pop() 테스트: 요소 제거
    q.Pop(); // 1 제거
    ASSERT(q.Size() == 2);
    ASSERT(q.Front() == 2);
    std::cout << "After Pop: Front = " << q.Front() << ", Size = " << q.Size() << "\n";

    // 4. Wrap-Around 테스트
    q.Push(4); // {2,3,4}
    q.Push(5); // {2,3,4,5}
    q.Push(6); // {2,3,4,5,6} -> Capacity(5) 가득 찬 상태
    ASSERT(q.Size() == 5);
    std::cout << "After pushing to full: Size = " << q.Size() << "\n";

    std::cout << "Queue contents (wrap-around test): ";
    while (!q.Empty())
    {
        std::cout << q.Front() << " ";
        q.Pop();
    }
    std::cout << "\n";

    // 5. Clear() 테스트: 큐를 비워주는 함수
    q.Push(10);
    q.Push(20);
    q.Push(30);
    ASSERT(q.Size() == 3);
    q.Clear();
    ASSERT(q.Empty());
    std::cout << "After Clear(): Size = " << q.Size() << "\n";

    // 6. 복사 생성자 테스트
    FixedQueue<int, 5> qOriginal;
    qOriginal.Push(100);
    qOriginal.Push(200);
    qOriginal.Push(300);
    FixedQueue<int, 5> qCopy(qOriginal);
    ASSERT(qCopy.Size() == qOriginal.Size());
    std::cout << "Copied queue contents: ";
    while (!qCopy.Empty())
    {
        std::cout << qCopy.Front() << " ";
        qCopy.Pop();
    }
    std::cout << "\n";

    // 7. 이동 생성자 테스트
    FixedQueue<int, 5> qMove(std::move(qOriginal));
    ASSERT(qMove.Size() == 3);
    ASSERT(qOriginal.Empty());
    std::cout << "Moved queue contents: ";
    while (!qMove.Empty())
    {
        std::cout << qMove.Front() << " ";
        qMove.Pop();
    }
    std::cout << "\n";

    // 8. 복사 할당 연산자 테스트
    FixedQueue<int, 5> qA;
    qA.Push(7);
    qA.Push(8);
    FixedQueue<int, 5> qB;
    qB.Push(1);
    qB.Push(2);
    qB.Push(3);
    qB = qA; // qB에 qA 복사
    ASSERT(qB.Size() == qA.Size());
    std::cout << "After copy assignment, qB: ";
    while (!qB.Empty())
    {
        std::cout << qB.Front() << " ";
        qB.Pop();
    }
    std::cout << "\n";

    // 9. 이동 할당 연산자 테스트
    FixedQueue<int, 5> qC;
    qC.Push(10);
    qC.Push(20);
    FixedQueue<int, 5> qD;
    qD.Push(1000);
    qD.Push(2000);
    qD.Push(3000);
    qD = std::move(qC);  // qD로 이동
    ASSERT(qD.Size() == 2);
    ASSERT(qC.Empty());
    std::cout << "After move assignment, qD: ";
    while (!qD.Empty())
    {
        std::cout << qD.Front() << " ";
        qD.Pop();
    }
    std::cout << "\n";

    std::cout << "All tests passed." << std::endl;
    return 0;
}

