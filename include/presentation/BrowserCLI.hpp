#pragma once

#include "core/BrowserHistory.hpp"

/**
 * @brief Command Line Interface REPL controller.
 */
class BrowserCLI {
private:
    BrowserHistory browser;

    void handleVisit(const std::string& url);
    void handleBack();
    void handleForward();
    void handleStatus();
    void handlePeek();
    void handleLog();
    void handleClear();

public:
    explicit BrowserCLI(size_t capacity = 0);
    void run();
};
