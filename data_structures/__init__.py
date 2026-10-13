"""
Data Structures module: contains custom implementations of core Abstract Data Types.
"""

from .stack import Stack
from .exceptions import StackError, EmptyStackError, StackOverflowError, NavigationError

__all__ = ["Stack", "StackError", "EmptyStackError", "StackOverflowError", "NavigationError"]
