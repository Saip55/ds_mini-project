"""
Unit tests for the Stack Abstract Data Type (ADT).
"""

import unittest
from data_structures.stack import Stack
from data_structures.exceptions import EmptyStackError, StackOverflowError


class TestStack(unittest.TestCase):
    """Test suite verifying LIFO stack operations and constraints."""

    def setUp(self) -> None:
        self.stack: Stack[str] = Stack[str]()

    def test_initial_state(self) -> None:
        self.assertTrue(self.stack.is_empty())
        self.assertEqual(self.stack.size(), 0)
        self.assertEqual(len(self.stack), 0)
        self.assertFalse(bool(self.stack))

    def test_push_and_peek(self) -> None:
        self.stack.push("apple")
        self.assertFalse(self.stack.is_empty())
        self.assertEqual(self.stack.size(), 1)
        self.assertEqual(self.stack.peek(), "apple")

        self.stack.push("banana")
        self.assertEqual(self.stack.size(), 2)
        self.assertEqual(self.stack.peek(), "banana")

    def test_lifo_order_pop(self) -> None:
        items = ["first", "second", "third"]
        for item in items:
            self.stack.push(item)

        # LIFO verification: items should pop in reverse order
        self.assertEqual(self.stack.pop(), "third")
        self.assertEqual(self.stack.pop(), "second")
        self.assertEqual(self.stack.pop(), "first")
        self.assertTrue(self.stack.is_empty())

    def test_empty_stack_exceptions(self) -> None:
        with self.assertRaises(EmptyStackError):
            self.stack.pop()

        with self.assertRaises(EmptyStackError):
            self.stack.peek()

    def test_clear(self) -> None:
        self.stack.push("a")
        self.stack.push("b")
        self.stack.clear()
        self.assertTrue(self.stack.is_empty())
        self.assertEqual(self.stack.size(), 0)

    def test_to_list_and_iteration(self) -> None:
        self.stack.push("1")
        self.stack.push("2")
        self.stack.push("3")

        # Bottom to top list representation
        self.assertEqual(self.stack.to_list(), ["1", "2", "3"])
        # Top to bottom list representation (LIFO order)
        self.assertEqual(self.stack.to_reversed_list(), ["3", "2", "1"])
        # Direct iteration yields LIFO order
        self.assertEqual(list(self.stack), ["3", "2", "1"])

    def test_bounded_stack_capacity(self) -> None:
        bounded: Stack[int] = Stack[int](capacity=2)
        bounded.push(10)
        bounded.push(20)
        with self.assertRaises(StackOverflowError):
            bounded.push(30)


if __name__ == "__main__":
    unittest.main()
