# 📋 Detailed Implementation Plan: CLI Browser History Simulator

> **Project Title:** CLI Browser History Simulator  
> **Course / Category:** Data Structures Mini Project  
> **Architecture Pattern:** 3-Tier Layered Modular Architecture  
> **Core Data Structure:** Stack Abstract Data Type (ADT)  

---

## 🎯 Project Overview & Objectives

The goal of this project is to design, implement, and test an interactive Command-Line Interface (CLI) Browser History Simulator. The application simulates the navigation mechanics of real-world web browsers (Google Chrome, Mozilla Firefox, Safari) using the **Stack Abstract Data Type (ADT)**.

### Core Academic Goals:
1. **Mastery of LIFO Principles**: Demonstrate rigorous implementation of the Last-In, First-Out (LIFO) paradigm using two coordinated stacks: `back_stack` and `forward_stack`.
2. **Layered Modular Architecture**: Enforce strict decoupling across three tiers:
   - **Presentation Layer (CLI)**: Input reading, command parsing, ASCII visualizer.
   - **Business Logic Layer (Core)**: Browser navigation rules, state management, audit trail.
   - **Data Management Layer (Data Structures)**: Encapsulated Stack ADT and custom exceptions.
3. **Robustness & Clean Code**: $\mathcal{O}(1)$ time complexity for core operations, full type hinting, typed exception handling, and 100% automated test coverage.

---

## 🗓 Phase Breakdown

```
Phase 1: Architecture & Scaffolding
   │
   ▼
Phase 2: Data Management Layer (Stack ADT)
   │
   ▼
Phase 3: Domain Models & Business Logic (BrowserHistory)
   │
   ▼
Phase 4: Presentation Layer (ASCII Visualizer & REPL)
   │
   ▼
Phase 5: Automated Testing & Verification
   │
   ▼
Phase 6: Documentation & Viva Preparation
```

---

## 🔹 Phase 1: Environment & Directory Scaffolding

### Objective
Establish a clean, modular folder layout that enforces separation of concerns from day one.

### Action Items
1. **Verify Runtime**: Confirm Python 3.8+ is installed (`python3 --version`).
2. **Create Modular Directory Tree**:
   ```text
   ds mini project/
   ├── data_structures/     # Tier 3: Data Management
   ├── core/                # Tier 2: Business Logic
   ├── presentation/        # Tier 1: Presentation & UI
   ├── tests/               # Automated Unit Tests
   ├── main.py              # Application Entry Point
   ├── README.md            # Project Documentation
   └── IMPLEMENTATION_PLAN.md
   ```
3. **Initialize Package Namespaces**:
   - Create `__init__.py` files in `data_structures/`, `core/`, `presentation/`, and `tests/`.

### Deliverables & Verification
- [x] All directories created.
- [x] Python modules discoverable as packages.

---

## 🔹 Phase 2: Data Management Layer (Stack ADT)

### Objective
Implement an encapsulated, reusable, generic Stack Abstract Data Type without relying on raw, unconstrained list manipulations.

### Detailed Steps

#### Step 2.1: Custom Exception Hierarchy (`data_structures/exceptions.py`)
Define specific exceptions to prevent unhandled index errors:
- `StackError(Exception)`: Base error.
- `EmptyStackError(StackError)`: Raised when `pop()` or `peek()` is invoked on an empty stack.
- `StackOverflowError(StackError)`: Raised when pushing into a bounded stack at capacity.
- `NavigationError(Exception)`: Raised when invalid browser navigation is requested.

#### Step 2.2: Implement Generic Stack ADT (`data_structures/stack.py`)
Implement `Stack[T]` with complete type hinting and formal operations:
- `push(item: T) -> None`: Pushes element to top. $\mathcal{O}(1)$
- `pop() -> T`: Removes and returns top element; raises `EmptyStackError` if empty. $\mathcal{O}(1)$
- `peek() -> T`: Returns top element without removing; raises `EmptyStackError` if empty. $\mathcal{O}(1)$
- `is_empty() -> bool`: Returns `len(_items) == 0`. $\mathcal{O}(1)$
- `size() -> int`: Returns number of elements. $\mathcal{O}(1)$
- `clear() -> None`: Clears all elements. $\mathcal{O}(1)$
- `to_list() -> List[T]`: Returns items in `[bottom, ..., top]` order for read-only inspection.
- `to_reversed_list() -> List[T]`: Returns items in LIFO viewing order `[top, ..., bottom]`.

#### Step 2.3: Unit Test Stack ADT (`tests/test_stack.py`)
Write comprehensive unit tests verifying:
- Initial empty state.
- LIFO order invariance (first-in pops last).
- `EmptyStackError` on empty `pop()` and `peek()`.
- Bounded capacity overflow handling.

### Deliverables & Verification
- [x] `data_structures/stack.py` implemented.
- [x] `tests/test_stack.py` written and verified passing.

---

## 🔹 Phase 3: Domain Models & Business Logic Layer

### Objective
Implement core business rules of web navigation, coordinate the dual-stack mechanism, and maintain a chronological audit trail.

### Detailed Steps

#### Step 3.1: Domain Models (`core/models.py`)
- `Page`: Immutable dataclass representing a visited page (`url: str`, `timestamp: datetime`, `title: Optional[str]`).
- `BrowserStateSummary`: Data Transfer Object (DTO) holding a snapshot of:
  - `current_page: Optional[Page]`
  - `back_stack_items: List[Page]`
  - `forward_stack_items: List[Page]`
  - `total_history_count: int`

#### Step 3.2: Browser History Engine (`core/browser.py`)
Create `BrowserHistory` class managing:
- `self._back_stack: Stack[Page]`
- `self._forward_stack: Stack[Page]`
- `self._current_page: Optional[Page]`
- `self._audit_log: List[Page]`

Implement navigation logic adhering strictly to browser invariants:
1. **`visit(url: str) -> Page`**:
   - Sanitize URL (auto-prefix `https://` if protocol omitted).
   - If `current_page` exists, push it onto `_back_stack`.
   - Set `current_page = new_page`.
   - **Crucial Invariant**: Clear `_forward_stack` (`self._forward_stack.clear()`).
   - Append `new_page` to chronological `_audit_log`.
2. **`back() -> Page`**:
   - If `can_go_back()` is False, raise `NavigationError("Cannot go back: No history available.")`.
   - Push `current_page` onto `_forward_stack`.
   - Pop top of `_back_stack` and assign to `current_page`.
3. **`forward() -> Page`**:
   - If `can_go_forward()` is False, raise `NavigationError("Cannot go forward: No forward pages available.")`.
   - Push `current_page` onto `_back_stack`.
   - Pop top of `_forward_stack` and assign to `current_page`.
4. **Inspection & Utility**:
   - `peek_back()`, `peek_forward()`: Inspect adjacent pages without modifying state.
   - `get_state()`: Return `BrowserStateSummary` DTO for UI rendering.
   - `get_audit_log()`: Return complete chronological navigation history.

#### Step 3.3: Unit Test Navigation Rules (`tests/test_browser.py`)
Write unit tests verifying:
- Single and multiple visits.
- Back and forward transitions.
- Forward stack invalidation rule upon new page visit.
- Boundary conditions (navigating back/forward when stacks are empty).
- Complete validation of the **State Transition Matrix**.

### Deliverables & Verification
- [x] `core/models.py` and `core/browser.py` implemented.
- [x] `tests/test_browser.py` written and verified passing.

---

## 🔹 Phase 4: Presentation Layer (Visualizer & REPL)

### Objective
Create a rich, user-friendly terminal interface with ASCII diagrams and robust command parsing.

### Detailed Steps

#### Step 4.1: Terminal Styler & ASCII Visualizer (`presentation/visualizer.py`)
- `TerminalStyler`: Provides ANSI color codes (`CYAN`, `GREEN`, `YELLOW`, `RED`, `BOLD`, `DIM`) with automatic TTY detection.
- `BrowserVisualizer`:
  - `render_banner()`: Startup welcome header.
  - `render_help()`: Command syntax and documentation table.
  - `render_stack_diagram(state)`: Displays ASCII dual-stack inspector:
    - Forward stack (Top to Bottom with `▲ [Top]` pointer).
    - Active Page banner with visit timestamp.
    - Back stack (Top to Bottom with `▼ [Top]` pointer).
    - Summary metrics bar (back count, forward count, total visits).
  - `render_audit_log(log)`: Chronological numbered table of all visited URLs.
  - `render_peek(...)`: Shows next and previous pages.

#### Step 4.2: Interactive REPL Loop (`presentation/cli.py`)
- Prompt user with `[Browser] » `.
- Tokenize input into command (`cmd`) and arguments (`arg`).
- Case-insensitive routing:
  - `VISIT <url>`: Calls `browser.visit(url)`.
  - `BACK`: Calls `browser.back()`.
  - `FORWARD`: Calls `browser.forward()`.
  - `STATUS` / `HISTORY` / `STACKS`: Displays ASCII stack visualizer.
  - `PEEK`: Shows adjacent pages.
  - `LOG`: Displays chronological audit log.
  - `CLEAR`: Clears session stacks.
  - `HELP`: Shows available commands.
  - `QUIT` / `EXIT`: Gracefully exits.
- Handle `KeyboardInterrupt` (Ctrl+C) and `EOFError` (Ctrl+D) cleanly.

#### Step 4.3: Entry Point (`main.py`)
Create executable script that instantiates `BrowserCLI` and launches the REPL loop.

### Deliverables & Verification
- [x] `presentation/visualizer.py` and `presentation/cli.py` implemented.
- [x] `main.py` created and tested with interactive sessions.

---

## 🔹 Phase 5: Automated Testing & Verification

### Objective
Ensure zero regressions, correct algorithm execution, and verify every state transition.

### Test Matrix Overview
1. **Stack Tests (`test_stack.py`)**:
   - `test_initial_state`: Stack starts empty with size 0.
   - `test_push_and_peek`: Peek reflects most recently pushed element.
   - `test_lifo_order_pop`: Elements emerge in exact reverse order of insertion.
   - `test_empty_stack_exceptions`: Empty pops/peeks raise typed `EmptyStackError`.
   - `test_clear`: Clears all elements.
   - `test_to_list_and_iteration`: Verifies order-preserving copies.
   - `test_bounded_stack_capacity`: Bounded capacity raises `StackOverflowError`.
2. **Browser Tests (`test_browser.py`)**:
   - `test_initial_state`: Initial tab is blank, back/forward disabled.
   - `test_single_visit`: Sets active page, keeps stacks empty.
   - `test_multiple_visits`: Accumulates visited pages in back stack.
   - `test_back_navigation`: Correctly shifts pages between stacks.
   - `test_forward_navigation`: Correctly restores forward history.
   - `test_forward_invalidation_rule`: Validates that visiting a new URL purges forward stack.
   - `test_state_transition_matrix`: Tests all matrix scenarios end-to-end.
   - `test_boundary_empty_back_error`: Back when empty raises `NavigationError`.
   - `test_boundary_empty_forward_error`: Forward when empty raises `NavigationError`.
   - `test_audit_log`: Chronological history retains all visited pages.

### Test Execution Command
```bash
python3 -m unittest discover -s tests -v
```

---

## 🔹 Phase 6: Documentation, Deliverables & Viva Preparation

### Objective
Prepare polished project documentation and review key academic questions for project defense / viva.

### Key Viva Questions & Answers
1. **Q: Why are two stacks used instead of one?**  
   *A:* Moving forward and backward represents two opposite LIFO flows. When navigating backward, the current page must be remembered in reverse order of exploration, which is naturally modeled by a second stack (`forward_stack`).
2. **Q: Why is the forward stack cleared when visiting a new page?**  
   *A:* Visiting a new page creates a new, non-linear branch in browsing history. Retaining old forward pages would lead to ambiguous, branching history paths.
3. **Q: What are the time and space complexities?**  
   *A:* All core operations (`visit`, `back`, `forward`, `peek`) operate in $\mathcal{O}(1)$ time. Overall space is $\mathcal{O}(N)$ where $N$ is the number of stored history entries.
4. **Q: How does this design prevent tight coupling?**  
   *A:* The Presentation Layer only interacts with the Business Layer through clean method calls and Data Transfer Objects (`BrowserStateSummary`). The Business Layer never writes directly to stdout, and the Data Layer encapsulates data access behind the Stack ADT interface.
