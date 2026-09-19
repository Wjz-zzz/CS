#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <string>

using I64 = long long;

std::string trim(std::string s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

bool isQuit(const std::string& s) {
    return s == "q" || s == "Q" || s == "quit" || s == "exit";
}

bool isHelp(const std::string& s) {
    return s == "h" || s == "H" || s == "help" || s == "?";
}

void printHelp() {
    std::cout
        << "用法：\n"
        << "  - 每次输入一个整数，或一行输入两个整数（空格分隔）\n"
        << "  - q / quit / exit 退出\n"
        << "  - h / help / ? 查看帮助\n";
}

std::optional<std::string> readLine(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string line;
    if (!std::getline(std::cin, line)) {
        return std::nullopt;
    }
    return trim(line);
}

std::optional<I64> parseInt(const std::string& token) {
    if (token.empty()) {
        return std::nullopt;
    }
    std::istringstream in(token);
    I64 value = 0;
    in >> value;
    if (!in || in.peek() != std::char_traits<char>::eof()) {
        return std::nullopt;
    }
    return value;
}

std::optional<I64> safeAdd(I64 a, I64 b) {
    constexpr I64 kMax = std::numeric_limits<I64>::max();
    constexpr I64 kMin = std::numeric_limits<I64>::min();
    if ((b > 0 && a > kMax - b) || (b < 0 && a < kMin - b)) {
        return std::nullopt;
    }
    return a + b;
}

enum class Status { Ok, Quit, Retry };

Status readNumber(const std::string& prompt, I64& out, std::optional<I64>& extra) {
    extra.reset();
    while (true) {
        auto line = readLine(prompt);
        if (!line) {
            return Status::Quit;
        }
        if (line->empty() || isHelp(*line)) {
            printHelp();
            continue;
        }
        if (isQuit(*line)) {
            return Status::Quit;
        }

        std::istringstream in(*line);
        std::string t1;
        std::string t2;
        std::string leftover;
        in >> t1 >> t2 >> leftover;

        auto n1 = parseInt(t1);
        if (!n1) {
            std::cerr << "输入无效，请输入整数。输入 h 查看帮助。\n";
            continue;
        }
        if (!t2.empty()) {
            auto n2 = parseInt(t2);
            if (!n2 || !leftover.empty()) {
                std::cerr << "一行最多两个整数，例如：3 5\n";
                continue;
            }
            extra = n2;
        }
        out = *n1;
        return Status::Ok;
    }
}

int main() {
    std::cout << "整数加法控制程序\n";
    printHelp();

    while (true) {
        I64 a = 0;
        I64 b = 0;
        std::optional<I64> extra;

        if (readNumber("请输入第一个整数（或两个整数）：", a, extra) == Status::Quit) {
            break;
        }

        if (extra) {
            b = *extra;
        } else if (readNumber("请输入第二个整数：", b, extra) == Status::Quit) {
            break;
        } else if (extra) {
            std::cerr << "第二个数只需一个整数。\n";
            continue;
        }

        if (auto sum = safeAdd(a, b)) {
            std::cout << a << " + " << b << " = " << *sum << "\n";
        } else {
            std::cerr << "结果超出 long long 范围，无法计算。\n";
        }

        auto again = readLine("继续？(回车继续，q 退出)：");
        if (!again || isQuit(*again)) {
            break;
        }
        if (isHelp(*again)) {
            printHelp();
        }
    }

    std::cout <<