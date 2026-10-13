#pragma once

#include <string>
#include <unistd.h>

/**
 * @brief Handles ANSI terminal styling with automatic TTY detection.
 */
class TerminalStyler {
public:
    static inline const std::string RESET   = "\033[0m";
    static inline const std::string BOLD    = "\033[1m";
    static inline const std::string DIM     = "\033[2m";
    static inline const std::string CYAN    = "\033[96m";
    static inline const std::string GREEN   = "\033[92m";
    static inline const std::string YELLOW  = "\033[93m";
    static inline const std::string RED     = "\033[91m";
    static inline const std::string MAGENTA = "\033[95m";
    static inline const std::string BLUE    = "\033[94m";

    static bool isColorSupported() {
        return isatty(fileno(stdout));
    }

    static std::string style(const std::string& text, const std::string& code) {
        if (isColorSupported()) {
            return code + text + RESET;
        }
        return text;
    }

    static std::string bold(const std::string& text)   { return style(text, BOLD); }
    static std::string green(const std::string& text)  { return style(text, GREEN); }
    static std::string yellow(const std::string& text) { return style(text, YELLOW); }
    static std::string red(const std::string& text)    { return style(text, RED); }
    static std::string cyan(const std::string& text)   { return style(text, CYAN); }
    static std::string dim(const std::string& text)    { return style(text, DIM); }
};
