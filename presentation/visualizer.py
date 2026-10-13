"""
Visualizer and Formatter for the Presentation Layer.

Provides ASCII stack diagrams, colorized terminal styling, and formatted status tables.
"""

import sys
from typing import List, Optional
from core.models import Page, BrowserStateSummary


class TerminalStyler:
    """Handles ANSI color formatting with fallback for non-color terminals."""
    
    # ANSI escape sequences
    RESET = "\033[0m"
    BOLD = "\033[1m"
    DIM = "\033[2m"
    
    CYAN = "\033[96m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    RED = "\033[91m"
    MAGENTA = "\033[95m"
    BLUE = "\033[94m"

    @classmethod
    def _is_color_supported(cls) -> bool:
        return hasattr(sys.stdout, "isatty") and sys.stdout.isatty()

    @classmethod
    def style(cls, text: str, code: str) -> str:
        if cls._is_color_supported():
            return f"{code}{text}{cls.RESET}"
        return text

    @classmethod
    def bold(cls, text: str) -> str:
        return cls.style(text, cls.BOLD)

    @classmethod
    def green(cls, text: str) -> str:
        return cls.style(text, cls.GREEN)

    @classmethod
    def yellow(cls, text: str) -> str:
        return cls.style(text, cls.YELLOW)

    @classmethod
    def red(cls, text: str) -> str:
        return cls.style(text, cls.RED)

    @classmethod
    def cyan(cls, text: str) -> str:
        return cls.style(text, cls.CYAN)

    @classmethod
    def dim(cls, text: str) -> str:
        return cls.style(text, cls.DIM)


class BrowserVisualizer:
    """Renders formatted browser representations and ASCII stack diagrams."""

    @staticmethod
    def render_banner() -> str:
        """Returns the startup welcome banner."""
        ts = TerminalStyler
        banner = [
            ts.cyan("=" * 64),
            ts.bold(ts.cyan("     🌐 CLI BROWSER HISTORY SIMULATOR (DS MINI PROJECT)")),
            ts.dim("   Layered Architecture | Dual-Stack LIFO Engine | Audit Trail"),
            ts.cyan("=" * 64),
            "Type " + ts.green("HELP") + " to view available commands, or " + ts.yellow("VISIT <url>") + " to begin.",
        ]
        return "\n".join(banner)

    @staticmethod
    def render_help() -> str:
        """Returns the commands reference table."""
        ts = TerminalStyler
        lines = [
            "\n" + ts.bold("Available Commands:"),
            f"  {ts.green('VISIT <url>')}   : Navigate to a new address (clears forward stack)",
            f"  {ts.green('BACK')}          : Move backward through history (LIFO back pop)",
            f"  {ts.green('FORWARD')}       : Move forward through history (LIFO forward pop)",
            f"  {ts.green('STATUS')}        : Display visual stack inspection diagram",
            f"  {ts.green('PEEK')}          : View adjacent pages without moving",
            f"  {ts.green('LOG')}           : View full chronological navigation history",
            f"  {ts.green('CLEAR')}         : Clear back and forward stacks",
            f"  {ts.green('HELP')}          : Show this guide",
            f"  {ts.green('QUIT / EXIT')}   : Exit the simulator",
        ]
        return "\n".join(lines)

    @staticmethod
    def render_stack_diagram(state: BrowserStateSummary) -> str:
        """
        Generates an ASCII visualization of the browser's dual-stack state.
        
        Args:
            state: BrowserStateSummary snapshot.
        """
        ts = TerminalStyler
        lines = []
        divider = "─" * 60
        header = "═" * 60

        lines.append("\n" + ts.cyan(header))
        lines.append(ts.bold(ts.cyan("                 📊 BROWSER STACK INSPECTOR")))
        lines.append(ts.cyan(header))

        # Forward Stack: displayed from Top to Bottom
        forward_items = state.forward_stack_items  # [bottom, ..., top]
        lines.append(ts.bold("  [ FORWARD STACK ] (LIFO - Top to Bottom)"))
        if not forward_items:
            lines.append(ts.dim("     (empty - no forward pages)"))
        else:
            for i, page in enumerate(reversed(forward_items)):
                marker = "▲ [Top]   " if i == 0 else "│         "
                lines.append(f"    {ts.yellow(marker)} {page.url} {ts.dim('(' + page.formatted_time() + ')')}")

        # Current Active Page
        lines.append("  " + divider)
        if state.current_page:
            current_str = f"🌐 ACTIVE PAGE: {state.current_page.url}"
            lines.append(ts.bold(ts.green(f"  >>> {current_str} <<<")))
            lines.append(ts.dim(f"      Visited at: {state.current_page.formatted_time()}"))
        else:
            lines.append(ts.yellow("  >>> No page currently open (about:blank) <<<"))
        lines.append("  " + divider)

        # Back Stack: displayed from Top to Bottom
        back_items = state.back_stack_items  # [bottom, ..., top]
        lines.append(ts.bold("  [ BACK STACK ]    (LIFO - Top to Bottom)"))
        if not back_items:
            lines.append(ts.dim("     (empty - no back pages)"))
        else:
            for i, page in enumerate(reversed(back_items)):
                marker = "▼ [Top]   " if i == 0 else "│         "
                lines.append(f"    {ts.cyan(marker)} {page.url} {ts.dim('(' + page.formatted_time() + ')')}")

        lines.append(ts.cyan(header))
        summary = (
            f"  Back: {len(back_items)} item(s) | "
            f"Forward: {len(forward_items)} item(s) | "
            f"Total Visits: {state.total_history_count}"
        )
        lines.append(ts.dim(summary))
        lines.append(ts.cyan(header) + "\n")

        return "\n".join(lines)

    @staticmethod
    def render_audit_log(log: List[Page]) -> str:
        """Renders the chronological audit log of all visits."""
        ts = TerminalStyler
        if not log:
            return ts.dim("No pages have been visited yet.")

        lines = [
            "\n" + ts.bold("📜 Chronological Browsing History:"),
            ts.dim("Index  Time      URL"),
            ts.dim("-" * 50),
        ]
        for idx, page in enumerate(log, 1):
            lines.append(f"[{idx:02d}]   {page.formatted_time()}  {page.url}")
        lines.append(ts.dim("-" * 50) + "\n")
        return "\n".join(lines)

    @staticmethod
    def render_peek(peek_back: Optional[Page], peek_forward: Optional[Page]) -> str:
        """Renders peek information."""
        ts = TerminalStyler
        back_str = peek_back.url if peek_back else ts.dim("(None)")
        fwd_str = peek_forward.url if peek_forward else ts.dim("(None)")
        return (
            f"\n  ◀ Previous (Back) : {back_str}\n"
            f"  ▶ Next (Forward)  : {fwd_str}\n"
        )
