import Common;
import std.core;
import Math;
import ECS;
import HamSTL;

using namespace ham;

int main(void)
{
    FixedVector<int, 10> vec;

    // 초기 상태 테스트
    ASSERT(vec.Empty());
    ASSERT(vec.Size() == 0);

    // PushBack 테스트
    vec.PushBack(1);
    vec.PushBack(2);
    vec.PushBack(3);
    ASSERT(vec.Size() == 3);
    ASSERT(vec.Front() == 1);
    ASSERT(vec.Back() == 3);
    ASSERT(vec[1] == 2);

    // Insert 테스트 (index 1에 10 삽입 → 1, 10, 2, 3)
    auto it = vec.Insert(vec.Begin() + 1, 10);
    ASSERT(*it == 10);
    ASSERT(vec.Size() == 4);
    ASSERT(vec[0] == 1 && vec[1] == 10 && vec[2] == 2 && vec[3] == 3);

    // Erase 테스트 (index 2 제거 → 1, 10, 3)
    it = vec.Erase(vec.Begin() + 2);
    ASSERT(vec.Size() == 3);
    ASSERT(vec[0] == 1 && vec[1] == 10 && vec[2] == 3);

    // 추가 PushBack로 확장 후, 범위 Erase 테스트
    vec.PushBack(4);
    vec.PushBack(5);
    // 현재 벡터: 1, 10, 3, 4, 5
    it = vec.Erase(vec.Begin() + 1, vec.Begin() + 3); // index 1~2 제거 → 1, 4, 5
    ASSERT(vec.Size() == 3);
    ASSERT(vec[0] == 1 && vec[1] == 4 && vec[2] == 5);

    // PopBack 테스트
    vec.PopBack(); // 5 제거 → 1, 4
    ASSERT(vec.Size() == 2);
    ASSERT(vec.Back() == 4);

    // clear 테스트
    vec.clear();
    ASSERT(vec.Empty());

    std::cout << "모든 테스트를 통과했습니다.\n";
    return 0;
}

