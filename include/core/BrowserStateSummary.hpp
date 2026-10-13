#pragma once

#include <vector>
#include <optional>
#include "Page.hpp"

/**
 * @brief Data Transfer Object (DTO) capturing a point-in-time snapshot
 * of the browser's navigation state without coupling UI to engine internals.
 */
struct BrowserStateSummary {
    std::optional<Page> currentPage;
    std::vector<Page> backStackItems;      // [bottom, ..., top]
    std::vector<Page> forwardStackItems;   // [bottom, ..., top]
    size_t totalHistoryCount;

    BrowserStateSummary(std::optional<Page> current,
                        std::vector<Page> back,
                        std::vector<Page> forward,
                        size_t totalHistory)
        : currentPage(std::move(current)),
          backStackItems(std::move(back)),
          forwardStackItems(std::move(forward)),
          totalHistoryCount(totalHistory) {}
};
