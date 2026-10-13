#pragma once

#include <string>
#include <vector>
#include <optional>
#include "core/Page.hpp"
#include "core/BrowserStateSummary.hpp"

/**
 * @brief Renders formatted ASCII diagrams, tables, and banners for the CLI.
 */
class BrowserVisualizer {
public:
    static std::string renderBanner();
    static std::string renderHelp();
    static std::string renderStackDiagram(const BrowserStateSummary& state);
    static std::string renderAuditLog(const std::vector<Page>& log);
    static std::string renderPeek(const std::optional<Page>& peekBack,
                                  const std::optional<Page>& peekForward);
};
