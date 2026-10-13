"""
Unit tests for BrowserHistory navigation logic and state transitions.
"""

import unittest
from core.browser import BrowserHistory
from data_structures.exceptions import NavigationError


class TestBrowserHistory(unittest.TestCase):
    """Test suite verifying browser navigation rules, invariants, and state transitions."""

    def setUp(self) -> None:
        self.browser = BrowserHistory()

    def test_initial_state(self) -> None:
        state = self.browser.get_state()
        self.assertIsNone(state.current_page)
        self.assertEqual(state.back_stack_items, [])
        self.assertEqual(state.forward_stack_items, [])
        self.assertFalse(self.browser.can_go_back())
        self.assertFalse(self.browser.can_go_forward())

    def test_single_visit(self) -> None:
        page = self.browser.visit("example.com")
        self.assertEqual(page.url, "https://example.com")
        self.assertEqual(self.browser.current_page.url, "https://example.com")
        self.assertFalse(self.browser.can_go_back())
        self.assertFalse(self.browser.can_go_forward())

    def test_multiple_visits(self) -> None:
        self.browser.visit("a.com")
        self.browser.visit("b.com")
        self.browser.visit("c.com")

        state = self.browser.get_state()
        self.assertEqual(state.current_page.url, "https://c.com")
        self.assertEqual([p.url for p in state.back_stack_items], ["https://a.com", "https://b.com"])
        self.assertEqual(state.forward_stack_items, [])
        self.assertTrue(self.browser.can_go_back())
        self.assertFalse(self.browser.can_go_forward())

    def test_back_navigation(self) -> None:
        self.browser.visit("a.com")
        self.browser.visit("b.com")
        self.browser.visit("c.com")

        # Go back from C -> B
        back_page = self.browser.back()
        self.assertEqual(back_page.url, "https://b.com")
        
        state = self.browser.get_state()
        self.assertEqual(state.current_page.url, "https://b.com")
        self.assertEqual([p.url for p in state.back_stack_items], ["https://a.com"])
        self.assertEqual([p.url for p in state.forward_stack_items], ["https://c.com"])

        # Go back from B -> A
        back_page = self.browser.back()
        self.assertEqual(back_page.url, "https://a.com")
        
        state = self.browser.get_state()
        self.assertEqual(state.current_page.url, "https://a.com")
        self.assertEqual(state.back_stack_items, [])
        self.assertEqual([p.url for p in state.forward_stack_items], ["https://c.com", "https://b.com"])

    def test_forward_navigation(self) -> None:
        self.browser.visit("a.com")
        self.browser.visit("b.com")
        self.browser.back()  # Now at a.com, forward has b.com

        forward_page = self.browser.forward()
        self.assertEqual(forward_page.url, "https://b.com")

        state = self.browser.get_state()
        self.assertEqual(state.current_page.url, "https://b.com")
        self.assertEqual([p.url for p in state.back_stack_items], ["https://a.com"])
        self.assertEqual(state.forward_stack_items, [])

    def test_forward_invalidation_rule(self) -> None:
        """Crucial browser invariant: Visiting a new URL purges the forward stack."""
        self.browser.visit("a.com")
        self.browser.visit("b.com")
        self.browser.visit("c.com")

        self.browser.back()  # At b.com, forward has c.com
        self.assertEqual([p.url for p in self.browser.get_state().forward_stack_items], ["https://c.com"])

        # Visit brand new page D -> forward stack MUST be purged
        self.browser.visit("d.com")
        state = self.browser.get_state()
        self.assertEqual(state.current_page.url, "https://d.com")
        self.assertEqual([p.url for p in state.back_stack_items], ["https://a.com", "https://b.com"])
        self.assertEqual(state.forward_stack_items, [])
        self.assertFalse(self.browser.can_go_forward())

    def test_state_transition_matrix(self) -> None:
        """
        Validates all transitions from the architectural State Transition Matrix:
        1. None -> VISIT A -> back=[], current=A, fwd=[]
        2. [A], B, [] -> VISIT C -> back=[A, B], current=C, fwd=[]
        3. [A], B, [C] -> BACK -> back=[], current=A, fwd=[C, B]
        4. [A], B, [C] -> FORWARD -> back=[A, B], current=C, fwd=[]
        """
        # Step 1: None -> VISIT A
        self.browser.visit("a.com")
        s = self.browser.get_state()
        self.assertEqual(s.back_stack_items, [])
        self.assertEqual(s.current_page.url, "https://a.com")
        self.assertEqual(s.forward_stack_items, [])

        # Step 2: VISIT B, then VISIT C -> back=[A, B], current=C, fwd=[]
        self.browser.visit("b.com")
        self.browser.visit("c.com")
        s = self.browser.get_state()
        self.assertEqual([p.url for p in s.back_stack_items], ["https://a.com", "https://b.com"])
        self.assertEqual(s.current_page.url, "https://c.com")
        self.assertEqual(s.forward_stack_items, [])

        # Set up state: back=[A], current=B, fwd=[C]
        self.browser.back()
        s = self.browser.get_state()
        self.assertEqual([p.url for p in s.back_stack_items], ["https://a.com"])
        self.assertEqual(s.current_page.url, "https://b.com")
        self.assertEqual([p.url for p in s.forward_stack_items], ["https://c.com"])

        # Step 3: From ([A], B, [C]), execute BACK -> back=[], current=A, fwd=[C, B]
        self.browser.back()
        s = self.browser.get_state()
        self.assertEqual(s.back_stack_items, [])
        self.assertEqual(s.current_page.url, "https://a.com")
        self.assertEqual([p.url for p in s.forward_stack_items], ["https://c.com", "https://b.com"])

        # Reset to ([A], B, [C]) to test Step 4: FORWARD
        self.browser.forward()  # Now at B, back=[A], fwd=[C]
        s = self.browser.get_state()
        self.assertEqual([p.url for p in s.back_stack_items], ["https://a.com"])
        self.assertEqual(s.current_page.url, "https://b.com")
        self.assertEqual([p.url for p in s.forward_stack_items], ["https://c.com"])

        # Step 4: From ([A], B, [C]), execute FORWARD -> back=[A, B], current=C, fwd=[]
        self.browser.forward()
        s = self.browser.get_state()
        self.assertEqual([p.url for p in s.back_stack_items], ["https://a.com", "https://b.com"])
        self.assertEqual(s.current_page.url, "https://c.com")
        self.assertEqual(s.forward_stack_items, [])

    def test_boundary_empty_back_error(self) -> None:
        with self.assertRaises(NavigationError):
            self.browser.back()

        self.browser.visit("a.com")
        with self.assertRaises(NavigationError):
            self.browser.back()

    def test_boundary_empty_forward_error(self) -> None:
        with self.assertRaises(NavigationError):
            self.browser.forward()

        self.browser.visit("a.com")
        with self.assertRaises(NavigationError):
            self.browser.forward()

    def test_audit_log(self) -> None:
        self.browser.visit("first.com")
        self.browser.visit("second.com")
        self.browser.visit("third.com")
        log = self.browser.get_audit_log()
        self.assertEqual(len(log), 3)
        self.assertEqual([p.url for p in log], ["https://first.com", "https://second.com", "https://third.com"])


if __name__ == "__main__":
    unittest.main()
