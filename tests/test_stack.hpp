#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include "data_structures/Stack.hpp"
#include "data_structures/Exceptions.hpp"

class TestStack {
public:
    static void runAll() {
        testInitialState();
        testPushAndPeek();
        testLifoPopOrder();
        testEmptyStackExceptions();
        testClear();
        testDeepCopyConstructor();
        testCopyAssignment();
        testMoveSemantics();
        testToVectorSerialization();
        testBoundedCapacityOverflow();
    }

private:
    static void testInitialState() {
        Stack<int> s;
        assert(s.isEmpty());
        assert(s.size() == 0);
    }

    static void testPushAndPeek() {
        Stack<std::string> s;
        s.push("apple");
        assert(!s.isEmpty());
        assert(s.size() == 1);
        assert(s.peek() == "apple");

        s.push("banana");
        assert(s.size() == 2);
        assert(s.peek() == "banana");
    }

    static void testLifoPopOrder() {
        Stack<int> s;
        s.push(10);
        s.push(20);
        s.push(30);

        assert(s.pop() == 30);
        assert(s.pop() == 20);
        assert(s.pop() == 10);
        assert(s.isEmpty());
    }

    static void testEmptyStackExceptions() {
        Stack<double> s;
        bool caughtPop = false;
        try {
            s.pop();
        } catch (const EmptyStackException&) {
            caughtPop = true;
        }
        assert(caughtPop);

        bool caughtPeek = false;
        try {
            s.peek();
        } catch (const EmptyStackException&) {
            caughtPeek = true;
        }
        assert(caughtPeek);
    }

    static void testClear() {
        Stack<char> s;
        s.push('A');
        s.push('B');
        s.clear();
        assert(s.isEmpty());
        assert(s.size() == 0);
    }

    static void testDeepCopyConstructor() {
        Stack<int> original;
        original.push(1);
        original.push(2);
        original.push(3);

        Stack<int> copy(original); // Deep copy
        assert(copy.size() == 3);
        assert(copy.pop() == 3);
        assert(copy.pop() == 2);
        assert(copy.pop() == 1);

        // Verify original was not mutated
        assert(original.size() == 3);
        assert(original.peek() == 3);
    }

    static void testCopyAssignment() {
        Stack<std::string> s1;
        s1.push("first");
        s1.push("second");

        Stack<std::string> s2;
        s2.push("other");
        s2 = s1; // Deep copy assignment

        assert(s2.size() == 2);
        assert(s2.pop() == "second");
        assert(s2.pop() == "first");
        assert(s1.size() == 2);
    }

    static void testMoveSemantics() {
        Stack<std::string> s1;
        s1.push("alpha");
        s1.push("beta");

        Stack<std::string> s2 = std::move(s1);
        assert(s1.isEmpty());
        assert(s1.size() == 0);
        assert(s2.size() == 2);
        assert(s2.pop() == "beta");
        assert(s2.pop() == "alpha");
    }

    static void testToVectorSerialization() {
        Stack<int> s;
        s.push(1);
        s.push(2);
        s.push(3);

        auto vec = s.toVector(); // [bottom, ..., top]
        assert(vec.size() == 3);
        assert(vec[0] == 1);
        assert(vec[1] == 2);
        assert(vec[2] == 3);

        auto revVec = s.toReversedVector(); // [top, ..., bottom]
        assert(revVec[0] == 3);
        assert(revVec[1] == 2);
        assert(revVec[2] == 1);
    }

    static void testBoundedCapacityOverflow() {
        Stack<int> bounded(2);
        bounded.push(100);
        bounded.push(200);

        bool caughtOverflow = false;
        try {
            bounded.push(300);
        } catch (const StackOverflowException&) {
            caughtOverflow = true;
        }
        assert(caughtOverflow);
    }
};
