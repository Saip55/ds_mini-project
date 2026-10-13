#include "presentation/BrowserCLI.hpp"
#include "presentation/BrowserVisualizer.hpp"
#include "presentation/TerminalStyler.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

BrowserCLI::BrowserCLI(size_t capacity)
    : browser(capacity) {}

void BrowserCLI::run() {
    using TS = TerminalStyler;
    std::cout << BrowserVisualizer::renderBanner() << "\n";

    std::string line;
    while (true) {
        std::cout << "\n" << TS::bold("[Browser] ") << "» ";
        if (!std::getline(std::cin, line)) {
            // EOF encountered (Ctrl+D)
            std::cout << TS::yellow("\nSession terminated. Goodbye!\n");
            break;
        }

        // Trim leading and trailing whitespace
        auto start = std::find_if_not(line.begin(), line.end(), [](unsigned char ch) {
            return std::isspace(ch);
        });
        auto end = std::find_if_not(line.rbegin(), line.rend(), [](unsigned char ch) {
            return std::isspace(ch);
        }).base();

        if (start >= end) {
            continue; // Empty input
        }

        std::string trimmedLine(start, end);
        std::string cmd;
        std::string arg;

        auto firstSpace = trimmedLine.find(' ');
        if (firstSpace == std::string::npos) {
            cmd = trimmedLine;
        } else {
            cmd = trimmedLine.substr(0, firstSpace);
            auto argStart = trimmedLine.find_first_not_of(" \t", firstSpace);
            if (argStart != std::string::npos) {
                arg = trimmedLine.substr(argStart);
            }
        }

        // Convert command to uppercase
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        });

        if (cmd == "QUIT" || cmd == "EXIT" || cmd == "Q") {
            std::cout << TS::yellow("Exiting Browser History Simulator. Goodbye!\n");
            break;
        } else if (cmd == "VISIT") {
            handleVisit(arg);
        } else if (cmd == "BACK") {
            handleBack();
        } else if (cmd == "FORWARD") {
            handleForward();
        } else if (cmd == "STATUS" || cmd == "HISTORY" || cmd == "STACKS") {
            handleStatus();
        } else if (cmd == "PEEK") {
            handlePeek();
        } else if (cmd == "LOG") {
            handleLog();
        } else if (cmd == "CLEAR") {
            handleClear();
        } else if (cmd == "HELP" || cmd == "?") {
            std::cout << BrowserVisualizer::renderHelp() << "\n";
        } else {
            std::cout << TS::red("Unknown command: '" + cmd + "'. Type HELP for options.\n");
        }
    }
}

void BrowserCLI::handleVisit(const std::string& url) {
    using TS = TerminalStyler;
    if (url.empty()) {
        std::cout << TS::red("Usage: VISIT <url> (e.g. VISIT google.com)\n");
        return;
    }
    try {
        Page page = browser.visit(url);
        std::cout << TS::green("✔ Navigated to: " + page.url) << "\n";
        std::cout << BrowserVisualizer::renderStackDiagram(browser.getState());
    } catch (const std::exception& e) {
        std::cout << TS::red(std::string("✖ Error: ") + e.what()) << "\n";
    }
}

void BrowserCLI::handleBack() {
    using TS = TerminalStyler;
    try {
        Page page = browser.back();
        std::cout << TS::green("◀ Back to: " + page.url) << "\n";
        std::cout << BrowserVisualizer::renderStackDiagram(browser.getState());
    } catch (const NavigationException& e) {
        std::cout << TS::red(std::string("✖ ") + e.what()) << "\n";
    }
}

void BrowserCLI::handleForward() {
    using TS = TerminalStyler;
    try {
        Page page = browser.forward();
        std::cout << TS::green("▶ Forward to: " + page.url) << "\n";
        std::cout << BrowserVisualizer::renderStackDiagram(browser.getState());
    } catch (const NavigationException& e) {
        std::cout << TS::red(std::string("✖ ") + e.what()) << "\n";
    }
}

void BrowserCLI::handleStatus() {
    std::cout << BrowserVisualizer::renderStackDiagram(browser.getState());
}

void BrowserCLI::handlePeek() {
    std::cout << BrowserVisualizer::renderPeek(browser.peekBack(), browser.peekForward());
}

void BrowserCLI::handleLog() {
    std::cout << BrowserVisualizer::renderAuditLog(browser.getAuditLog());
}

void BrowserCLI::handleClear() {
    using TS = TerminalStyler;
    browser.clear();
    std::cout << TS::yellow("✔ Browser session stacks cleared.\n");
}
