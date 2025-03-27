import Common;
import std.core;
import Math;
import ECS;
import HamSTL;

using namespace ham;

int main(void)
{   
    // 1. �⺻ �迭 ���� �� ��� �ʱ�ȭ �׽�Ʈ (ũ��: 5)
    Array<int, 5> arr;
    Array<int, 8> other;
    for (size_t i = 0; i < arr.Size(); ++i) {
        arr[i] = static_cast<int>(i);  // 0, 1, 2, 3, 4
    }
    // operator[] ����
    for (size_t i = 0; i < arr.Size(); ++i) {
        ASSERT(arr[i] == static_cast<int>(i));
    }
    // Front, Back �׽�Ʈ 
    ASSERT(arr.Front() == 0);
    ASSERT(arr.Back() == 4);

    // 2. fill() �Լ� �׽�Ʈ: ��� ��Ҹ� 7�� ä��
    arr.Fill(7);
    for (size_t i = 0; i < arr.Size(); ++i) {
        ASSERT(arr[i] == 7);
    }
    
    // 3. ���� ������ �׽�Ʈ
    Array<int, 5> arrCopy(arr);
    for (size_t i = 0; i < arrCopy.Size(); ++i) {
        ASSERT(arrCopy[i] == 7);
    }
    
    // 4. ���� ���� ������ �׽�Ʈ
    Array<int, 5> arrAssign;
    arrAssign = arr;
    for (size_t i = 0; i < arrAssign.Size(); ++i) {
        ASSERT(arrAssign[i] == 7);
    }
    
    // 5. �� ������ �׽�Ʈ
    Array<int, 5000> arr1;
    Array<int, 5000> arr2;
    // arr1, arr2�� ���� 1,2,3,4,5�� �ʱ�ȭ
    for (size_t i = 0; i < arr1.Size(); ++i) {
        arr1[i] = static_cast<int>(i + 1);
        arr2[i] = static_cast<int>(i + 1);
    }
    
    ASSERT(arr1 == arr2);
    arr2[4] = 6;  // arr1: 1,2,3,4,5 / arr2: 1,2,3,4,6
    ASSERT(arr1 != arr2);
    ASSERT(arr1 < arr2);
    ASSERT(arr2 > arr1);
    ASSERT(arr1 <= arr2);
    ASSERT(arr2 >= arr1);
    return 0;
    // 6. Iterator �׽�Ʈ: arr1�� ��� ����� �� ��� (1+2+3+4+5 = 15)
    int sum = 0;
    for (auto it = arr1.Begin(); it != arr1.End(); ++it) {
        sum += *it;
    }
    ASSERT(sum == 15);

    // 7. swap() ��� �Լ� �׽�Ʈ: arr1�� ù ��ҿ� ������ ��� ��ȯ
    int firstElem = arr1[0];  // 1
    int lastElem = arr1[4];  // 5 (���� arr1[4]������, arr2 ���� ���� arr1�� �״�� 1,2,3,4,5)
    arr1.Swap(arr1[0], arr1[4]);
    ASSERT(arr1[0] == lastElem);
    ASSERT(arr1[4] == firstElem);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

