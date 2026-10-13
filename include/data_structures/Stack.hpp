#pragma once

#include <cstddef>
#include <utility>
#include <vector>
#include <algorithm>
#include "Exceptions.hpp"

/**
 * @brief Generic Pointer-based Linked List Stack ADT.
 * 
 * Follows the LIFO (Last-In, First-Out) principle using dynamically
 * allocated linked nodes. Manages heap memory via RAII and implements
 * the full Rule of Five (copy/move constructors and assignment operators).
 * 
 * Time Complexity:
 *   - push():     O(1)
 *   - pop():      O(1)
 *   - peek():     O(1)
 *   - isEmpty():  O(1)
 *   - size():     O(1)
 *   - clear():    O(N)
 * 
 * Space Complexity:
 *   - O(N) where N is the number of stored elements.
 * 
 * @tparam T Type of elements stored in the stack.
 */
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& val, Node* nxt = nullptr) : data(val), next(nxt) {}
        Node(T&& val, Node* nxt = nullptr) : data(std::move(val)), next(nxt) {}
    };

    Node* topNode;
    size_t count;
    size_t capacity; // 0 indicates unbounded stack

    void copyFrom(const Stack& other) {
        if (!other.topNode) {
            topNode = nullptr;
            count = 0;
            return;
        }

        // Deep copy linked list preserving bottom-to-top order
        // Step 1: Collect nodes in temporary buffer to reverse
        std::vector<const Node*> nodes;
        nodes.reserve(other.count);
        for (Node* curr = other.topNode; curr != nullptr; curr = curr->next) {
            nodes.push_back(curr);
        }

        // Step 2: Push elements from bottom to top
        topNode = nullptr;
        count = 0;
        for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
            push((*it)->data);
        }
    }

public:
    /**
     * @brief Constructs an empty stack.
     * @param maxCapacity Maximum allowed capacity (0 = unbounded).
     */
    explicit Stack(size_t maxCapacity = 0)
        : topNode(nullptr), count(0), capacity(maxCapacity) {}

    /**
     * @brief Destructor. Deallocates all linked nodes.
     */
    ~Stack() {
        clear();
    }

    /**
     * @brief Copy Constructor (Deep Copy).
     */
    Stack(const Stack& other) : topNode(nullptr), count(0), capacity(other.capacity) {
        copyFrom(other);
    }

    /**
     * @brief Copy Assignment Operator (Strong Exception Safety).
     */
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            capacity = other.capacity;
            copyFrom(other);
        }
        return *this;
    }

    /**
     * @brief Move Constructor.
     */
    Stack(Stack&& other) noexcept
        : topNode(other.topNode), count(other.count), capacity(other.capacity) {
        other.topNode = nullptr;
        other.count = 0;
    }

    /**
     * @brief Move Assignment Operator.
     */
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            topNode = other.topNode;
            count = other.count;
            capacity = other.capacity;
            other.topNode = nullptr;
            other.count = 0;
        }
        return *this;
    }

    /**
     * @brief Pushes an element by const reference onto the top of the stack.
     * @param val Value to push.
     * @throws StackOverflowException If stack has reached capacity.
     */
    void push(const T& val) {
        if (capacity > 0 && count >= capacity) {
            throw StackOverflowException("Stack overflow: Maximum capacity reached.");
        }
        topNode = new Node(val, topNode);
        count++;
    }

    /**
     * @brief Pushes an element by move onto the top of the stack.
     * @param val Rvalue reference to value to push.
     * @throws StackOverflowException If stack has reached capacity.
     */
    void push(T&& val) {
        if (capacity > 0 && count >= capacity) {
            throw StackOverflowException("Stack overflow: Maximum capacity reached.");
        }
        topNode = new Node(std::move(val), topNode);
        count++;
    }

    /**
     * @brief Removes and returns the top element of the stack.
     * @return T The popped element.
     * @throws EmptyStackException If stack is empty.
     */
    T pop() {
        if (isEmpty()) {
            throw EmptyStackException("Cannot pop from an empty stack.");
        }
        Node* nodeToDelete = topNode;
        T val = std::move(nodeToDelete->data);
        topNode = topNode->next;
        delete nodeToDelete;
        count--;
        return val;
    }

    /**
     * @brief Returns a const reference to the top element without removing it.
     * @return const T& Top element reference.
     * @throws EmptyStackException If stack is empty.
     */
    const T& peek() const {
        if (isEmpty()) {
            throw EmptyStackException("Cannot peek into an empty stack.");
        }
        return topNode->data;
    }

    /**
     * @brief Returns a mutable reference to the top element without removing it.
     * @return T& Top element reference.
     * @throws EmptyStackException If stack is empty.
     */
    T& peek() {
        if (isEmpty()) {
            throw EmptyStackException("Cannot peek into an empty stack.");
        }
        return topNode->data;
    }

    /**
     * @brief Checks whether the stack contains any elements.
     * @return true if empty, false otherwise.
     */
    bool isEmpty() const noexcept {
        return topNode == nullptr;
    }

    /**
     * @brief Returns the number of elements currently stored.
     * @return size_t Element count.
     */
    size_t size() const noexcept {
        return count;
    }

    /**
     * @brief Deallocates all nodes and resets count to 0.
     */
    void clear() noexcept {
        while (topNode != nullptr) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
        count = 0;
    }

    /**
     * @brief Serializes elements into a std::vector ordered from Bottom to Top.
     * @return std::vector<T> [bottom, ..., top]
     */
    std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(count);
        for (Node* curr = topNode; curr != nullptr; curr = curr->next) {
            result.push_back(curr->data);
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    /**
     * @brief Serializes elements into a std::vector ordered from Top to Bottom (LIFO).
     * @return std::vector<T> [top, ..., bottom]
     */
    std::vector<T> toReversedVector() const {
        std::vector<T> result;
        result.reserve(count);
        for (Node* curr = topNode; curr != nullptr; curr = curr->next) {
            result.push_back(curr->data);
        }
        return result;
    }
};
