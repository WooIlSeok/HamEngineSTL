import Common;
import std.core;
import Math;
import ECS;
import HamSTL;

using namespace ham;

int main(void)
{   
    // 1. 기본 배열 생성 및 요소 초기화 테스트 (크기: 5)
    Array<int, 5> arr;
    Array<int, 8> other;
    for (size_t i = 0; i < arr.Size(); ++i) {
        arr[i] = static_cast<int>(i);  // 0, 1, 2, 3, 4
    }
    // operator[] 검증
    for (size_t i = 0; i < arr.Size(); ++i) {
        ASSERT(arr[i] == static_cast<int>(i));
    }
    // Front, Back 테스트 
    ASSERT(arr.Front() == 0);
    ASSERT(arr.Back() == 4);

    // 2. fill() 함수 테스트: 모든 요소를 7로 채움
    arr.Fill(7);
    for (size_t i = 0; i < arr.Size(); ++i) {
        ASSERT(arr[i] == 7);
    }
    
    // 3. 복사 생성자 테스트
    Array<int, 5> arrCopy(arr);
    for (size_t i = 0; i < arrCopy.Size(); ++i) {
        ASSERT(arrCopy[i] == 7);
    }
    
    // 4. 복사 대입 연산자 테스트
    Array<int, 5> arrAssign;
    arrAssign = arr;
    for (size_t i = 0; i < arrAssign.Size(); ++i) {
        ASSERT(arrAssign[i] == 7);
    }
    
    // 5. 비교 연산자 테스트
    Array<int, 5000> arr1;
    Array<int, 5000> arr2;
    // arr1, arr2를 각각 1,2,3,4,5로 초기화
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
    // 6. Iterator 테스트: arr1의 모든 요소의 합 계산 (1+2+3+4+5 = 15)
    int sum = 0;
    for (auto it = arr1.Begin(); it != arr1.End(); ++it) {
        sum += *it;
    }
    ASSERT(sum == 15);

    // 7. swap() 멤버 함수 테스트: arr1의 첫 요소와 마지막 요소 교환
    int firstElem = arr1[0];  // 1
    int lastElem = arr1[4];  // 5 (원래 arr1[4]였으나, arr2 변경 이후 arr1는 그대로 1,2,3,4,5)
    arr1.Swap(arr1[0], arr1[4]);
    ASSERT(arr1[0] == lastElem);
    ASSERT(arr1[4] == firstElem);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

