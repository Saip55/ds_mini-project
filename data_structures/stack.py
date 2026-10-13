"""
Stack Abstract Data Type (ADT) implementation.

A Stack is a linear data structure following the LIFO (Last-In, First-Out) principle.
All insertions and deletions take place at one end, called the 'top' of the stack.
"""

from typing import TypeVar, Generic, List, Optional, Iterator
from .exceptions import EmptyStackError, StackOverflowError

T = TypeVar("T")


class Stack(Generic[T]):
    """
    Generic Stack implementation using an encapsulated dynamic array.
    
    Time Complexities:
        - push:     O(1) amortized
        - pop:      O(1)
        - peek:     O(1)
        - is_empty: O(1)
        - size:     O(1)
        - clear:    O(1)
    
    Space Complexity:
        - O(N) where N is the number of elements stored.
    """

    def __init__(self, capacity: Optional[int] = None) -> None:
        """
        Initialize an empty stack.
        
        Args:
            capacity: Optional maximum number of items the stack can hold.
                      If None, the stack is unbounded.
        """
        self._items: List[T] = []
        self._capacity: Optional[int] = capacity

    def push(self, item: T) -> None:
        """
        Push an element onto the top of the stack.
        
        Args:
            item: The element to add.
            
        Raises:
            StackOverflowError: If the stack has reached its maximum capacity.
        """
        if self._capacity is not None and len(self._items) >= self._capacity:
            raise StackOverflowError(
                f"Stack overflow: maximum capacity of {self._capacity} reached."
            )
        self._items.append(item)

    def pop(self) -> T:
        """
        Remove and return the top element of the stack.
        
        Returns:
            The element at the top of the stack.
            
        Raises:
            EmptyStackError: If the stack is empty.
        """
        if self.is_empty():
            raise EmptyStackError("Cannot pop from an empty stack.")
        return self._items.pop()

    def peek(self) -> T:
        """
        Return the top element of the stack without removing it.
        
        Returns:
            The element at the top of the stack.
            
        Raises:
            EmptyStackError: If the stack is empty.
        """
        if self.is_empty():
            raise EmptyStackError("Cannot peek into an empty stack.")
        return self._items[-1]

    def is_empty(self) -> bool:
        """
        Check whether the stack contains any elements.
        
        Returns:
            True if empty, False otherwise.
        """
        return len(self._items) == 0

    def size(self) -> int:
        """
        Return the number of elements currently stored in the stack.
        
        Returns:
            Integer count of elements.
        """
        return len(self._items)

    def clear(self) -> None:
        """Remove all elements from the stack."""
        self._items.clear()

    def to_list(self) -> List[T]:
        """
        Return a copy of the underlying items ordered from bottom to top.
        Does not mutate the internal stack state.
        
        Returns:
            List of elements [bottom, ..., top].
        """
        return list(self._items)

    def to_reversed_list(self) -> List[T]:
        """
        Return a copy of items ordered from top to bottom (LIFO viewing order).
        
        Returns:
            List of elements [top, ..., bottom].
        """
        return list(reversed(self._items))

    def __len__(self) -> int:
        return self.size()

    def __bool__(self) -> bool:
        return not self.is_empty()

    def __iter__(self) -> Iterator[T]:
        """Iterates over items from top to bottom (LIFO order)."""
        return iter(reversed(self._items))

    def __repr__(self) -> str:
        return f"Stack({self._items!r}, capacity={self._capacity})"
