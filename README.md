# 🌐 CLI Browser History Simulator (C++17)

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Compiler](https://img.shields.io/badge/Clang%20%2F%20GCC-Passed-blue.svg?style=for-the-badge&logo=gnu-bash&logoColor=white)
![Data Structure](https://img.shields.io/badge/Data%20Structure-Pointer%20Linked%20Stack-brightgreen.svg?style=for-the-badge)
![Architecture](https://img.shields.io/badge/Architecture-3--Tier%20Layered-orange.svg?style=for-the-badge)
![Tests](https://img.shields.io/badge/Tests-19%20Passed-success.svg?style=for-the-badge)
![Build](https://img.shields.io/badge/Build-Makefile-purple.svg?style=for-the-badge)

<p align="center">
  A high-performance browser navigation simulator written in <b>Modern C++17</b>. Implemented from first principles using an encapsulated <b>Pointer-based Linked List Stack ADT</b>, dynamic memory management (RAII / Rule of Five), and a decoupled <b>3-Tier Layered Modular Architecture</b>.
</p>

</div>

---

## 📋 Implementation Plan: CLI Browser History Simulator

- **Project Title:** CLI Browser History Simulator
- **Course / Scope:** Data Structures Mini Project
- **Language / Standard:** C++17 (Native Compiled)
- **Architecture Pattern:** 3-Tier Layered Modular Architecture
- **Core Data Structure:** Pointer-based Linked List Stack Abstract Data Type (ADT)

---

## 🎯 Executive Summary & Objectives

The **CLI Browser History Simulator** models real-world web browser navigation (Google Chromium, Apple WebKit, Mozilla Gecko) using the **Pointer-based Linked List Stack Abstract Data Type (ADT)**.

```mermaid
flowchart TD
    %% ==========================================
    %% TIER 1: PRESENTATION LAYER
    %% ==========================================
    subgraph T1 ["🖥️ Tier 1: Presentation Layer (CLI)"]
        direction LR
        CLI["💻 <b>BrowserCLI</b><br/><i>Interactive REPL Loop</i>"]
        VIS["📊 <b>BrowserVisualizer</b><br/><i>ASCII Stack Diagrams</i>"]
        STY["🎨 <b>TerminalStyler</b><br/><i>ANSI Color Engine</i>"]
        CLI --> VIS
        VIS --> STY
    end

    %% ==========================================
    %% TIER 2: BUSINESS LOGIC LAYER
    %% ==========================================
    subgraph T2 ["⚙️ Tier 2: Business Logic Layer (Core Engine)"]
        direction LR
        BH["🧠 <b>BrowserHistory Engine</b><br/><i>Navigation Rules & Invariants</i>"]
        DTO["📦 <b>BrowserStateSummary</b><br/><i>Decoupled State Snapshot DTO</i>"]
        LOG["📜 <b>Chronological Audit Log</b><br/><i>Full Visit Timeline</i>"]
        BH --> DTO
        BH --> LOG
    end

    %% ==========================================
    %% TIER 3: DATA MANAGEMENT LAYER
    %% ==========================================
    subgraph T3 ["💾 Tier 3: Data Management Layer (Data Structures)"]
        direction LR
        BSTK[("⏮️ <b>Back Stack</b><br/><code>Stack&lt;Page&gt;</code>")]
        FSTK[("⏭️ <b>Forward Stack</b><br/><code>Stack&lt;Page&gt;</code>")]
        MOD["📄 <b>Page Model</b><br/><i>URL, Timestamp</i>"]
        EXC["🛡️ <b>Custom Exceptions</b><br/><i>EmptyStackException</i>"]
        BSTK ~~~ FSTK
        FSTK ~~~ MOD
        MOD ~~~ EXC
    end

    %% Direct Subgraph Flow (Forces Strict Vertical Stacking)
    T1 ==>|"Dispatches Navigation Commands (VISIT, BACK, FORWARD)"| T2
    T2 ==>|"LIFO Stack Operations (Push, Pop, Clear)"| T3

    %% Premium Modern Styling
    classDef presTier fill:#0f172a,stroke:#38bdf8,stroke-width:2px,color:#f8fafc;
    classDef bizTier fill:#1e1b4b,stroke:#818cf8,stroke-width:2px,color:#f8fafc;
    classDef stackTier fill:#064e3b,stroke:#34d399,stroke-width:2px,color:#ecfdf5;
    classDef auxTier fill:#1e293b,stroke:#64748b,stroke-width:1.5px,color:#cbd5e1;

    class CLI,VIS presTier;
    class STY auxTier;
    class BH,DTO,LOG bizTier;
    class BSTK,FSTK stackTier;
    class MOD,EXC auxTier;

    style T1 fill:#080d1a,stroke:#0284c7,stroke-width:2px,stroke-dasharray: 4 4,color:#38bdf8;
    style T2 fill:#0f091f,stroke:#7c3aed,stroke-width:2px,stroke-dasharray: 4 4,color:#c084fc;
    style T3 fill:#04140e,stroke:#059669,stroke-width:2px,stroke-dasharray: 4 4,color:#34d399;
```

### Key Technical Goals:
- **LIFO Paradigm**: Coordinate two pointer-linked stacks (`backStack` and `forwardStack`) to guarantee strict $\mathcal{O}(1)$ navigation.
- **Dynamic Memory Management & RAII**: Manual node allocation (`new`/`delete`) with full implementation of the **Rule of Five** (Destructor, Copy Constructor, Copy Assignment, Move Constructor, Move Assignment).
- **Layered Modular Architecture**: Strict separation of concerns between UI presentation, domain logic, and heap data management.
- **Robustness & Clean Code**: Zero external third-party dependencies, custom typed C++ exception handling, 100% automated test coverage.

---

## 🏛 System Architecture: 3-Tier Layered Design

```
+----------------------------------------------------------------------+
|                       PRESENTATION LAYER (CLI)                       |
|   - Interactive REPL Loop (`BrowserCLI::run()`)                      |
|   - Command Parser & Input Sanitization                              |
|   - ASCII Dual-Stack Visualizer (`BrowserVisualizer`)                |
+----------------------------------------------------------------------+
                                   │  (Invokes domain methods)
                                   ▼
+----------------------------------------------------------------------+
|                       BUSINESS LOGIC LAYER                           |
|   - Navigation Rules (`visit`, `back`, `forward`)                    |
|   - Invariant Enforcement (Purge forward stack on new visit)         |
|   - Navigation State Snapshots (`BrowserStateSummary`)               |
|   - Chronological Audit Trail (`auditLog`)                           |
+----------------------------------------------------------------------+
                                   │  (Invokes stack operations)
                                   ▼
+----------------------------------------------------------------------+
|                       DATA MANAGEMENT LAYER                          |
|   - Generic Pointer-based `Stack<T>` ADT (LIFO)                      |
|   - Domain Models (`Page`, `BrowserStateSummary`)                    |
|   - Custom Exceptions (`EmptyStackException`, `NavigationException`) |
+----------------------------------------------------------------------+
```

---

## 📦 Data Structure Specification (Pointer Linked Stack ADT)

The stack is implemented as a singly-linked list of dynamic nodes managed by a `topNode` pointer:

```cpp
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* nxt = nullptr) : data(val), next(nxt) {}
        Node(T&& val, Node* nxt = nullptr) : data(std::move(val)), next(nxt) {}
    };
    Node* topNode;
    size_t count;
    size_t capacity; // 0 = unbounded
    // ...
};
```

### Complexity Guarantees:

| Method | Signature | Description | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :---: | :---: |
| `push` | `void push(const T& val)` | Prepends dynamic Node to top of list | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ |
| `pop` | `T pop()` | Unlinks and `delete`s top Node, returns value | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ |
| `peek` | `const T& peek() const` | Returns const reference to top element | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ |
| `isEmpty` | `bool isEmpty() const noexcept` | Checks if `topNode == nullptr` | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ |
| `size` | `size_t size() const noexcept` | Returns active node count | $\mathcal{O}(1)$ | $\mathcal{O}(1)$ |
| `clear` | `void clear() noexcept` | Traverses and deallocates all linked nodes | $\mathcal{O}(N)$ | $\mathcal{O}(1)$ |

### Rule of Five Memory Safety:
- **Destructor (`~Stack`)**: Automatically deallocates every linked node preventing memory leaks.
- **Copy Constructor / Assignment**: Performs a deep copy of the linked list, allocating independent node chains.
- **Move Constructor / Assignment**: Transfers pointer ownership in $\mathcal{O}(1)$ time without copying.

---

## 🔄 State Transition Matrix

The table below documents how commands mutate the system's state variables:

| # | Initial State `(backStack, currentPage, forwardStack)` | Command | Resulting `backStack` | Resulting `currentPage` | Resulting `forwardStack` | Explanation |
| :-: | :--- | :--- | :---: | :---: | :---: | :--- |
| **1** | `([], None, [])` | `VISIT A` | `[]` | `"A"` | `[]` | First page opened; nothing pushed to back stack. |
| **2** | `(["A"], "B", [])` | `VISIT C` | `["A", "B"]` | `"C"` | `[]` | Current page `"B"` pushed to back stack; forward stack cleared. |
| **3** | `(["A"], "B", ["C"])` | `BACK` | `[]` | `"A"` | `["C", "B"]` | `"B"` pushed to forward stack; `"A"` popped to active page. |
| **4** | `(["A"], "B", ["C"])` | `FORWARD` | `["A", "B"]` | `"C"` | `[]` | `"B"` pushed to back stack; `"C"` popped to active page. |
| **5** | `(["A"], "B", ["C"])` | `VISIT D` | `["A", "B"]` | `"D"` | `[]` | Forward history `["C"]` is invalidated and purged. |

---

## 📁 Repository Structure

```text
ds_mini-project/
├── include/
│   ├── data_structures/
│   │   ├── Stack.hpp             # Generic Pointer-based Linked Stack ADT
│   │   └── Exceptions.hpp        # Typed Exception Hierarchy
│   ├── core/
│   │   ├── Page.hpp              # Page Domain Model with std::chrono
│   │   ├── BrowserStateSummary.hpp # Decoupled State Snapshot DTO
│   │   └── BrowserHistory.hpp    # Core Navigation Business Logic Engine
│   └── presentation/
│       ├── TerminalStyler.hpp    # ANSI Terminal Colors with TTY detection
│       ├── BrowserVisualizer.hpp # Real-time ASCII Stack Inspector
│       └── BrowserCLI.hpp        # Interactive REPL Controller
├── src/
│   ├── core/
│   │   └── BrowserHistory.cpp
│   ├── presentation/
│   │   ├── BrowserVisualizer.cpp
│   │   └── BrowserCLI.cpp
│   └── main.cpp                  # Application Entry Point
├── tests/
│   ├── test_stack.hpp            # Stack ADT unit test assertions
│   ├── test_browser.hpp          # Browser logic & matrix test assertions
│   └── test_runner.cpp           # Test suite driver executable
├── Makefile                      # Automated Build & Test orchestration
├── IMPLEMENTATION_PLAN.md        # Comprehensive Implementation Specification
├── README.md                     # Project Documentation
└── .gitignore                    # Git Ignore rules for binaries/objects
```

---

## 🚀 Building & Running

### Prerequisites
- Modern C++ Compiler: `clang++` (macOS default) or `g++` with **C++17** support.
- `make` utility.

### One-Command Build & Run:
```bash
# Build the application
make

# Launch the interactive simulator
make run
```

### Running the Automated Test Suite:
```bash
make test
```
**Sample Test Output:**
```text
================================================================
       🧪 RUNNING AUTOMATED UNIT TEST SUITE (C++17)
================================================================
Running Stack ADT tests... ✔ PASSED (10 test assertions)
Running Browser Navigation tests... ✔ PASSED (9 test assertions)
----------------------------------------------------------------
All 19 Unit Tests Passed Successfully! (0.76 ms)
================================================================
```

### Cleaning Build Artifacts:
```bash
make clean
```

---

## ⌨️ Command Reference

| Command | Syntax | Description |
| :--- | :--- | :--- |
| `VISIT` | `VISIT <url>` | Navigates to a new URL and invalidates forward history |
| `BACK` | `BACK` | Steps back to the previous page (LIFO back pop) |
| `FORWARD` | `FORWARD` | Steps forward to the next page (LIFO forward pop) |
| `STATUS` | `STATUS` | Displays real-time ASCII dual-stack diagram |
| `PEEK` | `PEEK` | Inspects adjacent pages without moving |
| `LOG` | `LOG` | Displays complete chronological audit trail |
| `CLEAR` | `CLEAR` | Resets back and forward stacks |
| `HELP` | `HELP` | Displays available commands |
| `QUIT` | `QUIT` / `EXIT` | Safely terminates the simulator |

---

## 📄 License
This project is licensed under the MIT License — open for academic learning and portfolio reference.
