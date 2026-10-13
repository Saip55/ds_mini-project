"""
Command Line Interface (CLI) REPL for the Presentation Layer.

Handles user input reading, command parsing, dispatching to the core logic,
and presenting formatted feedback.
"""

import sys
from typing import Optional
from core.browser import BrowserHistory
from data_structures.exceptions import NavigationError, StackError
from .visualizer import BrowserVisualizer, TerminalStyler


class BrowserCLI:
    """CLI Controller and REPL runner."""

    def __init__(self, browser: Optional[BrowserHistory] = None) -> None:
        self.browser = browser if browser is not None else BrowserHistory()
        self.visualizer = BrowserVisualizer()
        self.styler = TerminalStyler()

    def run(self) -> None:
        """Starts the interactive REPL loop."""
        print(self.visualizer.render_banner())

        while True:
            try:
                # Prompt user for input
                prompt = self.styler.bold("\n[Browser] ") + "» "
                raw_input = input(prompt).strip()

                if not raw_input:
                    continue

                # Parse into command and argument
                parts = raw_input.split(" ", 1)
                cmd = parts[0].upper()
                arg = parts[1].strip() if len(parts) > 1 else ""

                # Route command
                if cmd in ("QUIT", "EXIT", "Q"):
                    print(self.styler.yellow("Exiting Browser History Simulator. Goodbye!"))
                    break

                elif cmd == "VISIT":
                    self._handle_visit(arg)

                elif cmd == "BACK":
                    self._handle_back()

                elif cmd == "FORWARD":
                    self._handle_forward()

                elif cmd in ("STATUS", "HISTORY", "STACKS"):
                    self._handle_status()

                elif cmd == "PEEK":
                    self._handle_peek()

                elif cmd == "LOG":
                    self._handle_log()

                elif cmd == "CLEAR":
                    self._handle_clear()

                elif cmd in ("HELP", "?"):
                    print(self.visualizer.render_help())

                else:
                    print(self.styler.red(f"Unknown command: '{cmd}'. Type HELP for options."))

            except (KeyboardInterrupt, EOFError):
                print(self.styler.yellow("\nSession terminated. Goodbye!"))
                break
            except Exception as e:
                print(self.styler.red(f"Unexpected error: {e}"))

    def _handle_visit(self, url: str) -> None:
        if not url:
            print(self.styler.red("Usage: VISIT <url> (e.g. VISIT google.com)"))
            return
        page = self.browser.visit(url)
        print(self.styler.green(f"✔ Navigated to: {page.url}"))
        print(self.visualizer.render_stack_diagram(self.browser.get_state()))

    def _handle_back(self) -> None:
        try:
            page = self.browser.back()
            print(self.styler.green(f"◀ Back to: {page.url}"))
            print(self.visualizer.render_stack_diagram(self.browser.get_state()))
        except NavigationError as e:
            print(self.styler.red(f"✖ {e}"))

    def _handle_forward(self) -> None:
        try:
            page = self.browser.forward()
            print(self.styler.green(f"▶ Forward to: {page.url}"))
            print(self.visualizer.render_stack_diagram(self.browser.get_state()))
        except NavigationError as e:
            print(self.styler.red(f"✖ {e}"))

    def _handle_status(self) -> None:
        print(self.visualizer.render_stack_diagram(self.browser.get_state()))

    def _handle_peek(self) -> None:
        peek_back = self.browser.peek_back()
        peek_forward = self.browser.peek_forward()
        print(self.visualizer.render_peek(peek_back, peek_forward))

    def _handle_log(self) -> None:
        print(self.visualizer.render_audit_log(self.browser.get_audit_log()))

    def _handle_clear(self) -> None:
        self.browser.clear()
        print(self.styler.yellow("✔ Browser session stacks cleared."))
