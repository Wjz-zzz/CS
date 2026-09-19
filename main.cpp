#include <iostream>
#include <limits>
#include <optional>
#include <string>

void clearLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool readInt(const std::string& prompt, long long& value) {
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return true;
        }
        if (std::cin.eof()) {
            return false;
        }
        std::cerr << "输入无效，请重新输入一个整数。\n";
        clearLine();
    }
}

std::optional<long long> safeAdd(long long a, long long b) {
    if ((b > 0 && a > std::numeric_limits<long long>::max() - b) ||
        (b < 0 && a < std::numeric_limits<long long>::min() - b)) {
        return std::nullopt;
    }
    return a + b;
}

int main() {
    std::cout << "整数加法控制程序（输入 q 退出）\n";

    while (true) {
        long long a = 0;
        long long b = 0;

        if (!readInt("请输入第一个整数：", a)) {
            break;
        }
        if (!readInt("请输入第二个整数：", b)) {
            break;
        }

        if (auto sum = safeAdd(a, b)) {
            std::cout << a << " + " << b << " = " << *sum << "\n";
        } else {
            std::cerr << "结果超出 long long 范围，无法计算。\n";
        }

        std::cout << "继续？(回车继续，q 退出)：";
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "q" || cmd == "Q") {
            break;
        }
    }

    std::cout << "程序结束。\n";
    return 0;
}