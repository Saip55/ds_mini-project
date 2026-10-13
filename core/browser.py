"""
Core Browser Logic (Business Logic Layer).

Enforces navigation rules, coordinates back and forward Stacks,
maintains the global chronological audit log, and validates state transitions.
"""

from typing import Optional, List
from data_structures.stack import Stack
from data_structures.exceptions import EmptyStackError, NavigationError
from .models import Page, BrowserStateSummary


class BrowserHistory:
    """
    Manages browser navigation using two LIFO Stacks (Back and Forward)
    and an active current page pointer.
    
    Invariants:
        1. When visiting a new page, forward history is purged.
        2. When navigating back, current page is pushed to forward stack,
           and top of back stack becomes current page.
        3. When navigating forward, current page is pushed to back stack,
           and top of forward stack becomes current page.
    """

    def __init__(self, capacity: Optional[int] = None) -> None:
        """
        Initialize the browser history.
        
        Args:
            capacity: Optional maximum size for the back and forward stacks.
        """
        self._back_stack: Stack[Page] = Stack[Page](capacity=capacity)
        self._forward_stack: Stack[Page] = Stack[Page](capacity=capacity)
        self._current_page: Optional[Page] = None
        self._audit_log: List[Page] = []  # Chronological timeline of all visits

    @property
    def current_page(self) -> Optional[Page]:
        """Returns the currently active page."""
        return self._current_page

    def can_go_back(self) -> bool:
        """Check if back navigation is possible."""
        return not self._back_stack.is_empty()

    def can_go_forward(self) -> bool:
        """Check if forward navigation is possible."""
        return not self._forward_stack.is_empty()

    def peek_back(self) -> Optional[Page]:
        """Inspect the page that back() would return to, without moving."""
        if self._back_stack.is_empty():
            return None
        return self._back_stack.peek()

    def peek_forward(self) -> Optional[Page]:
        """Inspect the page that forward() would return to, without moving."""
        if self._forward_stack.is_empty():
            return None
        return self._forward_stack.peek()

    def visit(self, url: str) -> Page:
        """
        Navigate to a new URL.
        
        Business Logic Rules:
            1. If there is an existing current page, push it onto the back stack.
            2. Update current page to the new URL.
            3. Clear the forward stack completely (invalidates forward branch).
            4. Append the visit to the chronological audit log.
            
        Args:
            url: The destination web address.
            
        Returns:
            The newly created and active Page object.
        """
        sanitized_url = self._sanitize_url(url)
        new_page = Page(url=sanitized_url)

        if self._current_page is not None:
            self._back_stack.push(self._current_page)

        self._current_page = new_page
        self._forward_stack.clear()
        self._audit_log.append(new_page)

        return new_page

    def back(self) -> Page:
        """
        Navigate backward to the most recently visited page.
        
        Business Logic Rules:
            1. Validate that back stack is not empty.
            2. Push the current page onto the forward stack.
            3. Pop the top page from the back stack and set it as current page.
            
        Returns:
            The new active Page after navigating back.
            
        Raises:
            NavigationError: If there are no previous pages in the back stack.
        """
        if not self.can_go_back() or self._current_page is None:
            raise NavigationError("Cannot go back: No history available.")

        # Move current page to forward stack
        self._forward_stack.push(self._current_page)

        # Pop previous page from back stack
        try:
            self._current_page = self._back_stack.pop()
        except EmptyStackError as err:
            raise NavigationError("Cannot go back: Back stack is empty.") from err

        return self._current_page

    def forward(self) -> Page:
        """
        Navigate forward to the next page in the forward stack.
        
        Business Logic Rules:
            1. Validate that forward stack is not empty.
            2. Push the current page onto the back stack.
            3. Pop the top page from the forward stack and set it as current page.
            
        Returns:
            The new active Page after navigating forward.
            
        Raises:
            NavigationError: If there are no forward pages available.
        """
        if not self.can_go_forward() or self._current_page is None:
            raise NavigationError("Cannot go forward: No forward pages available.")

        # Move current page to back stack
        self._back_stack.push(self._current_page)

        # Pop next page from forward stack
        try:
            self._current_page = self._forward_stack.pop()
        except EmptyStackError as err:
            raise NavigationError("Cannot go forward: Forward stack is empty.") from err

        return self._current_page

    def clear(self) -> None:
        """Clear session stacks while preserving the current page."""
        self._back_stack.clear()
        self._forward_stack.clear()

    def get_state(self) -> BrowserStateSummary:
        """
        Create a decoupled snapshot of the browser's current state.
        
        Returns:
            BrowserStateSummary DTO for presentation consumption.
        """
        return BrowserStateSummary(
            current_page=self._current_page,
            back_stack_items=self._back_stack.to_list(),
            forward_stack_items=self._forward_stack.to_list(),
            total_history_count=len(self._audit_log),
        )

    def get_audit_log(self) -> List[Page]:
        """
        Return the full chronological audit trail of all visited pages.
        
        Returns:
            List of Page objects in historical order.
        """
        return list(self._audit_log)

    @staticmethod
    def _sanitize_url(raw_url: str) -> str:
        """Sanitizes user URL input."""
        url = raw_url.strip()
        if not url:
            return "about:blank"
        if not (url.startswith("http://") or url.startswith("https://") or url.startswith("about:")):
            return f"https://{url}"
        return url
