#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Base exception class for all stack operations.
 */
class StackException : public std::runtime_error {
public:
    explicit StackException(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @brief Exception thrown when popping or peeking into an empty stack.
 */
class EmptyStackException : public StackException {
public:
    explicit EmptyStackException(const std::string& message = "Cannot perform operation on an empty stack.")
        : StackException(message) {}
};

/**
 * @brief Exception thrown when pushing onto a full bounded stack.
 */
class StackOverflowException : public StackException {
public:
    explicit StackOverflowException(const std::string& message = "Stack overflow: maximum capacity reached.")
        : StackException(message) {}
};

/**
 * @brief Exception thrown when an invalid browser navigation is attempted.
 */
class NavigationException : public std::runtime_error {
public:
    explicit NavigationException(const std::string& message) : std::runtime_error(message) {}
};
