import Common;
import std.core;
import Math;
import ECS;
import HamSTL;

using namespace ham;

int main(void)
{
    FixedVector<int, 10> vec;

    // �ʱ� ���� �׽�Ʈ
    ASSERT(vec.Empty());
    ASSERT(vec.Size() == 0);

    // PushBack �׽�Ʈ
    vec.PushBack(1);
    vec.PushBack(2);
    vec.PushBack(3);
    ASSERT(vec.Size() == 3);
    ASSERT(vec.Front() == 1);
    ASSERT(vec.Back() == 3);
    ASSERT(vec[1] == 2);

    // Insert �׽�Ʈ (index 1�� 10 ���� �� 1, 10, 2, 3)
    auto it = vec.Insert(vec.Begin() + 1, 10);
    ASSERT(*it == 10);
    ASSERT(vec.Size() == 4);
    ASSERT(vec[0] == 1 && vec[1] == 10 && vec[2] == 2 && vec[3] == 3);

    // Erase �׽�Ʈ (index 2 ���� �� 1, 10, 3)
    it = vec.Erase(vec.Begin() + 2);
    ASSERT(vec.Size() == 3);
    ASSERT(vec[0] == 1 && vec[1] == 10 && vec[2] == 3);

    // �߰� PushBack�� Ȯ�� ��, ���� Erase �׽�Ʈ
    vec.PushBack(4);
    vec.PushBack(5);
    // ���� ����: 1, 10, 3, 4, 5
    it = vec.Erase(vec.Begin() + 1, vec.Begin() + 3); // index 1~2 ���� �� 1, 4, 5
    ASSERT(vec.Size() == 3);
    ASSERT(vec[0] == 1 && vec[1] == 4 && vec[2] == 5);

    // PopBack �׽�Ʈ
    vec.PopBack(); // 5 ���� �� 1, 4
    ASSERT(vec.Size() == 2);
    ASSERT(vec.Back() == 4);

    // clear �׽�Ʈ
    vec.clear();
    ASSERT(vec.Empty());

    std::cout << "��� �׽�Ʈ�� ����߽��ϴ�.\n";
    return 0;
}

