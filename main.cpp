#include <iostream>

int main() {
    int a = 0;
    int b = 0;

    std::cout << "请输入两个整数：";
    if (!(std::cin >> a >> b)) {
        std::cerr << "输入无效，请输入两个整数。\n";
        return 1;
    }

    int sum = a + b;
    std::cout << "两数之和为：" << sum << std::endl;
    return 0;
}
