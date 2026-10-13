"""
Custom exceptions for Data Structures and Browser Navigation.
"""


class StackError(Exception):
    """Base exception for stack operations."""
    pass


class EmptyStackError(StackError):
    """Raised when attempting to pop or peek from an empty stack."""
    pass


class StackOverflowError(StackError):
    """Raised when attempting to push onto a full bounded stack."""
    pass


class NavigationError(Exception):
    """Raised when an invalid browser navigation is attempted."""
    pass
