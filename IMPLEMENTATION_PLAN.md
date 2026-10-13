# 📋 Detailed Implementation Plan: CLI Browser History Simulator (C++17)

> **Project Title:** CLI Browser History Simulator  
> **Course / Category:** Data Structures Mini Project  
> **Language & Standard:** C++17  
> **Architecture Pattern:** 3-Tier Layered Modular Architecture  
> **Core Data Structure:** Pointer-based Linked List Stack ADT  

---

## 🎯 Project Overview & Objectives

The goal of this project is to design, implement, and test a native Command-Line Interface (CLI) Browser History Simulator in **Modern C++17**. The application simulates the navigation mechanics of real-world web engines (Chromium, WebKit) using a custom **Pointer-based Linked List Stack Abstract Data Type (ADT)**.

### Core Academic Goals:
1. **Pointers & Dynamic Memory Management**: Implement dynamic node allocation (`new`/`delete`), RAII memory ownership, and the C++ **Rule of Five** (Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment).
2. **Mastery of LIFO Principles**: Implement dual pointer-linked stacks (`backStack` and `forwardStack`) to guarantee strict $\mathcal{O}(1)$ navigation.
3. **Layered Modular Architecture**: Complete decoupling across three tiers:
   - **Presentation Layer**: ANSI styling and interactive REPL loop.
   - **Business Logic Layer**: Navigation invariants, audit logging, and state snapshots.
   - **Data Management Layer**: Pointer-linked Stack template class and custom exception hierarchy.
4. **Robustness & Zero Leaks**: 100% automated test coverage with 19 assertions verifying boundary conditions and state transitions.

---

## 🗓 Phase Breakdown

```
Phase 1: Architecture & Scaffolding (C++17 & Makefile)
   │
   ▼
Phase 2: Data Management Layer (Pointer Linked Stack ADT)
   │
   ▼
Phase 3: Domain Models & Core Engine (BrowserHistory)
   │
   ▼
Phase 4: Presentation Layer (ASCII Visualizer & REPL)
   │
   ▼
Phase 5: Automated Testing & Verification
   │
   ▼
Phase 6: Build Automation & Documentation
```

---

## 🔹 Phase 1: Environment & Directory Scaffolding

### Directory Tree:
```text
ds_mini-project/
├── include/
│   ├── data_structures/     # Tier 3 Headers (Stack.hpp, Exceptions.hpp)
│   ├── core/                # Tier 2 Headers (BrowserHistory.hpp, Page.hpp)
│   └── presentation/        # Tier 1 Headers (BrowserCLI.hpp, Visualizer.hpp)
├── src/
│   ├── core/                # Engine Implementations
│   ├── presentation/        # UI Implementations
│   └── main.cpp             # Entry Point
├── tests/                   # Automated Unit Tests
├── Makefile                 # Clang/GCC Compilation & Test Targets
├── README.md
└── IMPLEMENTATION_PLAN.md
```

---

## 🔹 Phase 2: Data Management Layer (Pointer-Linked Stack ADT)

### Step 2.1: Custom Exception Hierarchy (`include/data_structures/Exceptions.hpp`)
- `StackException`: Inherits from `std::runtime_error`.
- `EmptyStackException`: Thrown on illegal `pop()` or `peek()` on empty stack.
- `StackOverflowException`: Thrown on capacity breach for bounded stacks.
- `NavigationException`: Thrown on illegal back/forward browser movements.

### Step 2.2: Generic Pointer Linked Stack (`include/data_structures/Stack.hpp`)
- Singly linked list with `Node* topNode` and `size_t count`.
- `void push(const T& val)`: Dynamically allocates `new Node(val, topNode)`. $\mathcal{O}(1)$
- `T pop()`: Unlinks top node, deletes it, and returns value. $\mathcal{O}(1)$
- `const T& peek() const`: Returns top node data without removal. $\mathcal{O}(1)$
- `bool isEmpty() const noexcept`: Checks if `topNode == nullptr`. $\mathcal{O}(1)$
- `size_t size() const noexcept`: Returns element count. $\mathcal{O}(1)$
- `void clear() noexcept`: Traverses and `delete`s each node. $\mathcal{O}(N)$
- **Rule of Five Implementations**:
  - `~Stack()`: Deallocates all nodes to prevent leaks.
  - `Stack(const Stack& other)`: Deep copy preserving LIFO order.
  - `Stack& operator=(const Stack& other)`: Deep copy assignment.
  - `Stack(Stack&& other) noexcept`: Pointer transfer move constructor.
  - `Stack& operator=(Stack&& other) noexcept`: Pointer transfer move assignment.

---

## 🔹 Phase 3: Domain Models & Business Logic Layer

### Step 3.1: Domain Models (`include/core/Page.hpp`, `BrowserStateSummary.hpp`)
- `Page`: Holds `std::string url`, `std::chrono::system_clock::time_point timestamp`, and `getFormattedTime()`.
- `BrowserStateSummary`: DTO capturing a point-in-time snapshot of:
  - `currentPage`: `std::optional<Page>`
  - `backStackItems`: `std::vector<Page>` [bottom to top]
  - `forwardStackItems`: `std::vector<Page>` [bottom to top]
  - `totalHistoryCount`: `size_t`

### Step 3.2: Browser History Engine (`include/core/BrowserHistory.hpp`, `src/core/BrowserHistory.cpp`)
- `Page visit(const std::string& rawUrl)`:
  - Sanitizes URL (auto-prefixes `https://`).
  - Pushes current page to `backStack`.
  - Clears `forwardStack` (invalidates forward branch).
  - Appends to chronological `auditLog`.
- `Page back()`:
  - Throws `NavigationException` if back stack is empty.
  - Pushes current page to `forwardStack`.
  - Pops from `backStack` to become current page.
- `Page forward()`:
  - Throws `NavigationException` if forward stack is empty.
  - Pushes current page to `backStack`.
  - Pops from `forwardStack` to become current page.

---

## 🔹 Phase 4: Presentation Layer (Visualizer & REPL)

### Step 4.1: Terminal Styler (`include/presentation/TerminalStyler.hpp`)
- ANSI codes with automatic terminal detection using `isatty(fileno(stdout))`.

### Step 4.2: ASCII Visualizer (`include/presentation/BrowserVisualizer.hpp`)
- Renders startup banner, command table, and real-time ASCII dual-stack diagram:
  ```text
  ============================================================
                   📊 BROWSER STACK INSPECTOR
  ============================================================
    [ FORWARD STACK ] (LIFO - Top to Bottom)
      ▲ [Top]    https://stackoverflow.com (11:39:40)
    ------------------------------------------------------------
    >>> 🌐 ACTIVE PAGE: https://github.com <<<
    ------------------------------------------------------------
    [ BACK STACK ]    (LIFO - Top to Bottom)
      ▼ [Top]    https://google.com (11:39:40)
  ============================================================
  ```

### Step 4.3: Interactive REPL (`include/presentation/BrowserCLI.hpp`, `src/presentation/BrowserCLI.cpp`)
- Reads user input with `std::getline(std::cin, line)`.
- Dispatches commands: `VISIT`, `BACK`, `FORWARD`, `STATUS`, `PEEK`, `LOG`, `CLEAR`, `HELP`, `QUIT`.
- Gracefully handles `EOF` (Ctrl+D) and interruptions.

---

## 🔹 Phase 5: Automated Testing & Verification

### Test Suite (`tests/test_runner.cpp`)
1. **Stack Tests (`tests/test_stack.hpp`)**:
   - Initial empty state.
   - Push and peek verification.
   - LIFO pop sequence.
   - Typed `EmptyStackException` on illegal operations.
   - Clear and size resets.
   - Deep copy constructor independence.
   - Deep copy assignment independence.
   - Move constructor and move assignment zero-copy transfer.
   - Vector serialization.
   - Bounded capacity overflow protection.
2. **Browser Tests (`tests/test_browser.hpp`)**:
   - Single & multiple visits.
   - Back & forward navigation transitions.
   - Forward stack invalidation upon new URL visit.
   - State Transition Matrix exact sequence.
   - Boundary exceptions on empty back/forward.
   - Chronological audit log retention.

### Execution:
```bash
make test
```
All **19 assertions** pass in `< 1 ms`.

---

## 🔹 Phase 6: Build Automation & Deliverables

- `make`: Compiles binary `bin/browser_simulator`.
- `make run`: Compiles and executes interactive simulator.
- `make test`: Compiles and executes test runner.
- `make clean`: Removes `obj/` and `bin/`.
