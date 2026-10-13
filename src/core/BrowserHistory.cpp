#include "core/BrowserHistory.hpp"
#include <algorithm>
#include <cctype>

BrowserHistory::BrowserHistory(size_t stackCapacity)
    : backStack(stackCapacity),
      forwardStack(stackCapacity),
      currentPage(std::nullopt) {}

Page BrowserHistory::visit(const std::string& rawUrl) {
    std::string sanitized = sanitizeUrl(rawUrl);
    Page newPage(sanitized);

    if (currentPage.has_value()) {
        backStack.push(*currentPage);
    }

    currentPage = newPage;
    forwardStack.clear();
    auditLog.push_back(newPage);

    return newPage;
}

Page BrowserHistory::back() {
    if (!canGoBack() || !currentPage.has_value()) {
        throw NavigationException("Cannot go back: No history available.");
    }

    // Move current page to forward stack
    forwardStack.push(*currentPage);

    // Pop previous page from back stack
    try {
        currentPage = backStack.pop();
    } catch (const EmptyStackException& e) {
        throw NavigationException(std::string("Cannot go back: ") + e.what());
    }

    return *currentPage;
}

Page BrowserHistory::forward() {
    if (!canGoForward() || !currentPage.has_value()) {
        throw NavigationException("Cannot go forward: No forward pages available.");
    }

    // Move current page to back stack
    backStack.push(*currentPage);

    // Pop next page from forward stack
    try {
        currentPage = forwardStack.pop();
    } catch (const EmptyStackException& e) {
        throw NavigationException(std::string("Cannot go forward: ") + e.what());
    }

    return *currentPage;
}

bool BrowserHistory::canGoBack() const noexcept {
    return !backStack.isEmpty();
}

bool BrowserHistory::canGoForward() const noexcept {
    return !forwardStack.isEmpty();
}

std::optional<Page> BrowserHistory::peekBack() const {
    if (backStack.isEmpty()) {
        return std::nullopt;
    }
    return backStack.peek();
}

std::optional<Page> BrowserHistory::peekForward() const {
    if (forwardStack.isEmpty()) {
        return std::nullopt;
    }
    return forwardStack.peek();
}

std::optional<Page> BrowserHistory::getCurrentPage() const noexcept {
    return currentPage;
}

void BrowserHistory::clear() noexcept {
    backStack.clear();
    forwardStack.clear();
}

BrowserStateSummary BrowserHistory::getState() const {
    return BrowserStateSummary(
        currentPage,
        backStack.toVector(),
        forwardStack.toVector(),
        auditLog.size()
    );
}

std::vector<Page> BrowserHistory::getAuditLog() const {
    return auditLog;
}

std::string BrowserHistory::sanitizeUrl(const std::string& rawUrl) {
    // Trim leading whitespace
    auto start = std::find_if_not(rawUrl.begin(), rawUrl.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    // Trim trailing whitespace
    auto end = std::find_if_not(rawUrl.rbegin(), rawUrl.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();

    if (start >= end) {
        return "about:blank";
    }

    std::string trimmed(start, end);

    if (trimmed.rfind("http://", 0) == 0 ||
        trimmed.rfind("https://", 0) == 0 ||
        trimmed.rfind("about:", 0) == 0) {
        return trimmed;
    }

    return "https://" + trimmed;
}
