#pragma once

#include <string>
#include <vector>
#include <optional>
#include "data_structures/Stack.hpp"
#include "data_structures/Exceptions.hpp"
#include "Page.hpp"
#include "BrowserStateSummary.hpp"

/**
 * @brief Core Business Logic Engine for Browser History Simulation.
 * 
 * Enforces browser invariants:
 *   1. When visiting a new URL, forward stack is purged.
 *   2. When going back, current page is pushed to forward stack,
 *      and back stack is popped to become current page.
 *   3. When going forward, current page is pushed to back stack,
 *      and forward stack is popped to become current page.
 *   4. An audit log maintains full chronological history of all visits.
 */
class BrowserHistory {
private:
    Stack<Page> backStack;
    Stack<Page> forwardStack;
    std::optional<Page> currentPage;
    std::vector<Page> auditLog;

public:
    /**
     * @brief Constructs a new BrowserHistory instance.
     * @param stackCapacity Maximum capacity for back and forward stacks (0 = unbounded).
     */
    explicit BrowserHistory(size_t stackCapacity = 0);

    /**
     * @brief Navigates to a new web page.
     * Pushes current page to back stack and clears forward stack.
     * @param rawUrl Raw URL entered by user.
     * @return Page The newly active page.
     */
    Page visit(const std::string& rawUrl);

    /**
     * @brief Navigates backward to the most recently visited page.
     * Pushes current page to forward stack and pops top of back stack.
     * @return Page The new active page.
     * @throws NavigationException If back stack is empty.
     */
    Page back();

    /**
     * @brief Navigates forward to the next page.
     * Pushes current page to back stack and pops top of forward stack.
     * @return Page The new active page.
     * @throws NavigationException If forward stack is empty.
     */
    Page forward();

    /**
     * @brief Checks if backward navigation is possible.
     */
    bool canGoBack() const noexcept;

    /**
     * @brief Checks if forward navigation is possible.
     */
    bool canGoForward() const noexcept;

    /**
     * @brief Inspects the previous page without mutating state.
     */
    std::optional<Page> peekBack() const;

    /**
     * @brief Inspects the forward page without mutating state.
     */
    std::optional<Page> peekForward() const;

    /**
     * @brief Returns the active page.
     */
    std::optional<Page> getCurrentPage() const noexcept;

    /**
     * @brief Clears back and forward session stacks.
     */
    void clear() noexcept;

    /**
     * @brief Generates an immutable state snapshot DTO.
     */
    BrowserStateSummary getState() const;

    /**
     * @brief Returns the complete chronological history of all visits.
     */
    std::vector<Page> getAuditLog() const;

    /**
     * @brief Sanitizes user URL (trims whitespace, adds https:// if missing).
     */
    static std::string sanitizeUrl(const std::string& rawUrl);
};
