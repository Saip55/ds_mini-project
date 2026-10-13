#!/usr/bin/env python3
"""
Main Entry Point for the CLI Browser History Simulator.
A Data Structures Mini Project demonstrating Layered Architecture and Stack ADT.
"""

from presentation.cli import BrowserCLI


def main() -> None:
    """Launch the Browser History Simulator REPL."""
    cli = BrowserCLI()
    cli.run()


if __name__ == "__main__":
    main()
