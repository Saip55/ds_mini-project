#include <iostream>
#include <chrono>
#include "test_stack.hpp"
#include "test_browser.hpp"
#include "presentation/TerminalStyler.hpp"

int main() {
    using TS = TerminalStyler;
    std::cout << TS::cyan("================================================================") << "\n";
    std::cout << TS::bold(TS::cyan("       🧪 RUNNING AUTOMATED UNIT TEST SUITE (C++17)")) << "\n";
    std::cout << TS::cyan("================================================================") << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    try {
        std::cout << "Running Stack ADT tests... ";
        TestStack::runAll();
        std::cout << TS::green("✔ PASSED") << " (10 test assertions)\n";

        std::cout << "Running Browser Navigation tests... ";
        TestBrowser::runAll();
        std::cout << TS::green("✔ PASSED") << " (9 test assertions)\n";

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << TS::cyan("----------------------------------------------------------------") << "\n";
        std::cout << TS::bold(TS::green("All 19 Unit Tests Passed Successfully!")) << " ("
                  << elapsed.count() << " ms)\n";
        std::cout << TS::cyan("================================================================") << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << TS::bold(TS::red("✖ Test Failed with Exception: ")) << e.what() << "\n";
        return 1;
    }
}
