import Common;
import std.core;
import Math;
import ECS;
import HamSTL;

using namespace ham;

int main(void)
{   
    FixedStack<int, 10> fs;

    // �ʱ� ���� �׽�Ʈ
    if (fs.Empty()) {
        std::cout << "Initial: Stack is empty." << std::endl;
    }

    // Push �׽�Ʈ: 10, 20, 30, 40, 50 ����
    std::cout << "Pushing elements: ";
    for (int i = 1; i <= 5; ++i) {
        fs.Push(i * 10);
        std::cout << i * 10 << " ";
    }
    std::cout << "\nStack size after pushes: " << fs.Size() << std::endl;
    std::cout << "Top element: " << fs.Top() << std::endl;

    // Pop �׽�Ʈ: �ֻ�� ���� ����
    fs.Pop();
    std::cout << "After one pop, stack size: " << fs.Size() << std::endl;
    std::cout << "New top element: " << fs.Top() << std::endl;

    // Clear �׽�Ʈ
    fs.Clear();
    std::cout << "After clear, stack is " << (fs.Empty() ? "empty" : "not empty") << std::endl;

    // ���� ������ �׽�Ʈ
    // �ٽ� �� ���� ���� Push
    for (int i = 0; i < 3; ++i) {
        fs.Push(i + 1); // 1, 2, 3
    }
    std::cout << "Stack before copy, size: " << fs.Size()
        << ", top: " << fs.Top() << std::endl;

    FixedStack<int, 10> fs_copy(fs);
    std::cout << "Copied stack, size: " << fs_copy.Size()
        << ", top: " << fs_copy.Top() << std::endl;

    // �̵� ������ �׽�Ʈ
    FixedStack<int, 10> fs_move(std::move(fs));
    std::cout << "After move, moved stack size: " << fs_move.Size()
        << ", top: " << fs_move.Top() << std::endl;
    std::cout << "Original stack after move, size: " << fs.Size() << std::endl;

    // ���� ���� ������ �׽�Ʈ
    FixedStack<int, 10> fs_assign;
    fs_assign = fs_copy;
    std::cout << "After copy assignment, fs_assign size: " << fs_assign.Size()
        << ", top: " << fs_assign.Top() << std::endl;

    // �̵� ���� ������ �׽�Ʈ
    FixedStack<int, 10> fs_move_assign;
    fs_move_assign = std::move(fs_copy);
    std::cout << "After move assignment, fs_move_assign size: " << fs_move_assign.Size()
        << ", top: " << fs_move_assign.Top() << std::endl;
    std::cout << "Copied stack after move assignment, size: " << fs_copy.Size() << std::endl;

    return 0;
}

