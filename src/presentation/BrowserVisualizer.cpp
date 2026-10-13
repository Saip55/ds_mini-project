#include "presentation/BrowserVisualizer.hpp"
#include "presentation/TerminalStyler.hpp"
#include <sstream>
#include <iomanip>

std::string BrowserVisualizer::renderBanner() {
    using TS = TerminalStyler;
    std::ostringstream oss;
    oss << TS::cyan("================================================================") << "\n"
        << TS::bold(TS::cyan("     🌐 CLI BROWSER HISTORY SIMULATOR (C++17 DS PROJECT)")) << "\n"
        << TS::dim("   3-Tier Architecture | Pointer Linked Stack | O(1) Operations") << "\n"
        << TS::cyan("================================================================") << "\n"
        << "Type " << TS::green("HELP") << " to view available commands, or "
        << TS::yellow("VISIT <url>") << " to begin.";
    return oss.str();
}

std::string BrowserVisualizer::renderHelp() {
    using TS = TerminalStyler;
    std::ostringstream oss;
    oss << "\n" << TS::bold("Available Commands:") << "\n"
        << "  " << TS::green("VISIT <url>") << "   : Navigate to a new address (clears forward stack)\n"
        << "  " << TS::green("BACK")        << "          : Move backward through history (LIFO back pop)\n"
        << "  " << TS::green("FORWARD")     << "       : Move forward through history (LIFO forward pop)\n"
        << "  " << TS::green("STATUS")      << "        : Display visual stack inspection diagram\n"
        << "  " << TS::green("PEEK")        << "          : View adjacent pages without moving\n"
        << "  " << TS::green("LOG")         << "           : View full chronological navigation history\n"
        << "  " << TS::green("CLEAR")       << "         : Clear back and forward stacks\n"
        << "  " << TS::green("HELP")        << "          : Show this guide\n"
        << "  " << TS::green("QUIT / EXIT") << "   : Exit the simulator";
    return oss.str();
}

std::string BrowserVisualizer::renderStackDiagram(const BrowserStateSummary& state) {
    using TS = TerminalStyler;
    std::ostringstream oss;
    std::string header(60, '=');
    std::string divider(60, '-');

    oss << "\n" << TS::cyan(header) << "\n"
        << TS::bold(TS::cyan("                 📊 BROWSER STACK INSPECTOR")) << "\n"
        << TS::cyan(header) << "\n";

    // Forward Stack (displayed Top to Bottom)
    oss << TS::bold("  [ FORWARD STACK ] (LIFO - Top to Bottom)\n");
    if (state.forwardStackItems.empty()) {
        oss << TS::dim("     (empty - no forward pages)\n");
    } else {
        // forwardStackItems are [bottom, ..., top]
        for (int i = static_cast<int>(state.forwardStackItems.size()) - 1; i >= 0; --i) {
            bool isTop = (i == static_cast<int>(state.forwardStackItems.size()) - 1);
            std::string marker = isTop ? "▲ [Top]   " : "│         ";
            const auto& page = state.forwardStackItems[static_cast<size_t>(i)];
            oss << "    " << TS::yellow(marker) << " " << page.url
                << " " << TS::dim("(" + page.getFormattedTime() + ")") << "\n";
        }
    }

    // Current Active Page
    oss << "  " << divider << "\n";
    if (state.currentPage.has_value()) {
        oss << TS::bold(TS::green("  >>> 🌐 ACTIVE PAGE: " + state.currentPage->url + " <<<\n"))
            << TS::dim("      Visited at: " + state.currentPage->getFormattedTime()) << "\n";
    } else {
        oss << TS::yellow("  >>> No page currently open (about:blank) <<<\n");
    }
    oss << "  " << divider << "\n";

    // Back Stack (displayed Top to Bottom)
    oss << TS::bold("  [ BACK STACK ]    (LIFO - Top to Bottom)\n");
    if (state.backStackItems.empty()) {
        oss << TS::dim("     (empty - no back pages)\n");
    } else {
        // backStackItems are [bottom, ..., top]
        for (int i = static_cast<int>(state.backStackItems.size()) - 1; i >= 0; --i) {
            bool isTop = (i == static_cast<int>(state.backStackItems.size()) - 1);
            std::string marker = isTop ? "▼ [Top]   " : "│         ";
            const auto& page = state.backStackItems[static_cast<size_t>(i)];
            oss << "    " << TS::cyan(marker) << " " << page.url
                << " " << TS::dim("(" + page.getFormattedTime() + ")") << "\n";
        }
    }

    oss << TS::cyan(header) << "\n"
        << TS::dim("  Back: " + std::to_string(state.backStackItems.size()) + " item(s) | " +
                   "Forward: " + std::to_string(state.forwardStackItems.size()) + " item(s) | " +
                   "Total Visits: " + std::to_string(state.totalHistoryCount)) << "\n"
        << TS::cyan(header) << "\n";

    return oss.str();
}

std::string BrowserVisualizer::renderAuditLog(const std::vector<Page>& log) {
    using TS = TerminalStyler;
    if (log.empty()) {
        return TS::dim("No pages have been visited yet.");
    }

    std::ostringstream oss;
    oss << "\n" << TS::bold("📜 Chronological Browsing History:\n")
        << TS::dim("Index  Time      URL\n")
        << TS::dim("--------------------------------------------------\n");

    for (size_t i = 0; i < log.size(); ++i) {
        oss << "[" << std::setw(2) << std::setfill('0') << (i + 1) << "]   "
            << log[i].getFormattedTime() << "  "
            << log[i].url << "\n";
    }
    oss << TS::dim("--------------------------------------------------\n");
    return oss.str();
}

std::string BrowserVisualizer::renderPeek(const std::optional<Page>& peekBack,
                                          const std::optional<Page>& peekForward) {
    using TS = TerminalStyler;
    std::ostringstream oss;
    std::string backStr = peekBack.has_value() ? peekBack->url : TS::dim("(None)");
    std::string fwdStr  = peekForward.has_value() ? peekForward->url : TS::dim("(None)");

    oss << "\n  ◀ Previous (Back) : " << backStr << "\n"
        << "  ▶ Next (Forward)  : " << fwdStr << "\n";
    return oss.str();
}
