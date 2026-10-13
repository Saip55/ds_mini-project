#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

/**
 * @brief Represents a visited web page with metadata.
 */
struct Page {
    std::string url;
    std::chrono::system_clock::time_point timestamp;
    std::string title;

    explicit Page(std::string pageUrl = "", std::string pageTitle = "")
        : url(std::move(pageUrl)),
          timestamp(std::chrono::system_clock::now()),
          title(std::move(pageTitle)) {}

    /**
     * @brief Formats timestamp into HH:MM:SS string.
     */
    std::string getFormattedTime() const {
        std::time_t timeT = std::chrono::system_clock::to_time_t(timestamp);
        std::tm tmBuffer{};
#if defined(_WIN32)
        localtime_s(&tmBuffer, &timeT);
#else
        localtime_r(&timeT, &tmBuffer);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tmBuffer, "%H:%M:%S");
        return oss.str();
    }

    bool operator==(const Page& other) const {
        return url == other.url;
    }

    bool operator!=(const Page& other) const {
        return !(*this == other);
    }
};
