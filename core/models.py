"""
Domain models for the Browser History Simulator.
"""

from dataclasses import dataclass, field
from datetime import datetime
from typing import List, Optional


@dataclass(frozen=True)
class Page:
    """Represents a visited web page with metadata."""
    url: str
    timestamp: datetime = field(default_factory=datetime.now)
    title: Optional[str] = None

    def formatted_time(self) -> str:
        """Returns time formatted as HH:MM:SS."""
        return self.timestamp.strftime("%H:%M:%S")

    def __str__(self) -> str:
        return self.url


@dataclass
class BrowserStateSummary:
    """
    Data Transfer Object (DTO) capturing a point-in-time snapshot of the browser state.
    Used to pass state from Business Logic to Presentation without coupling.
    """
    current_page: Optional[Page]
    back_stack_items: List[Page]       # [bottom, ..., top]
    forward_stack_items: List[Page]    # [bottom, ..., top]
    total_history_count: int
