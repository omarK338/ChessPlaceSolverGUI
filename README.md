# Chess Place Solver

A Windows desktop application that finds all valid placements of **N chess pieces**
on a custom board such that no piece attacks another on the next move.

Built in **C++/CLI** using **Windows Forms** as a learning project in:
- Algorithm Design
- Constraint Satisfaction Problems (CSP)
- Systems Programming

---

# Background

This project started as a generalization of the classic **N-Queens Problem**.

The original version solved only **8 Queens on an 8×8 board**.
It was progressively expanded to support:

- All standard chess piece types
- Arbitrary rectangular board dimensions
- Dynamic hardware-aware solution limits
- Multiple specialized algorithms selected automatically per piece type

The development process involved identifying and fixing real algorithmic problems:

- Replacing a naive `O(N)` safety check with `O(1)` attack maps
- Replacing a single generic solver with a portfolio of specialized algorithms
- Implementing:
  - Forward Checking
  - Minimum Remaining Values (MRV)
- Adding symmetry breaking for Knights on square boards

---

# Problem Definition

Given a board of size `H × W` and a piece count `N`,
find every arrangement of `N` pieces such that no piece lies within the attack
range of any other piece.

This is a **Constraint Satisfaction Problem (CSP)** on a chessboard.

The solver:
- Finds all valid arrangements
- Stops at a configurable maximum limit
- Allows browsing every solution on an interactive board display

---

# Supported Piece Types

| Piece | Attack Pattern | Algorithm Used |
|---|---|---|
| Queen | Row, column, both diagonals | Backtracking with `O(1)` attack maps |
| Rook | Row and column | Backtracking with `O(1)` attack maps |
| Bishop | Both diagonals | Backtracking with `O(1)` attack maps |
| King | 3×3 neighborhood (8 cells) | Forward Checking + MRV |
| Knight | L-shape (up to 8 cells) | Forward Checking + MRV + Symmetry Breaking |

---

# Maximum Piece Counts

The application calculates the theoretical maximum number of non-attacking pieces automatically.

| Piece | Maximum Formula | Reasoning |
|---|---|---|
| Queen | `Min(H, W)` | One per row and column |
| Rook | `Min(H, W)` | Same row/column constraint as Queens |
| Bishop | `(H - 1) + (W - 1)` | Independent diagonal sets |
| King | `Ceil(H/2) × Ceil(W/2)` | One King per `2×2` area |
| Knight | `Ceil(H × W / 2)` | Knights never attack same-color cells |

The piece count input is automatically capped whenever:
- Board size changes
- Piece type changes

This prevents impossible configurations before solving begins.

---

# Algorithms

## Algorithm Selection

Different piece types use different algorithms because attack patterns create
different pruning characteristics.

```txt
Queen, Rook, Bishop --> Backtracking with O(1) attack maps
King               --> Forward Checking + MRV
Knight             --> Forward Checking + MRV + Symmetry Breaking
```

---

## Backtracking with Attack Maps (Queen, Rook, Bishop)

Maintains boolean arrays tracking:
- Occupied rows
- Occupied columns
- Occupied diagonals

Each safety check is `O(1)`.

### Attack Structures

```txt
Queens  : AttackedCols[c] and diagonal arrays
Rooks   : AttackedRows[r] and AttackedCols[c]
Bishops : AttackedDiagSum[r+c] and AttackedDiagDiff[r-c+W]
```

The search tree collapses rapidly because placements eliminate:
- Entire rows
- Entire columns
- Entire diagonals

For Queens:
- One recursive call per row
- Guarantees one Queen per row
- Reduces search to permutations of column assignments

---

## Forward Checking (King, Knight)

Maintains an availability counter for every cell.

When a piece is placed:
- All attacked cells are incremented
- Invalid cells are removed immediately

When removed:
- Counts are decremented

This detects dead ends earlier than naive backtracking.

### Example

Without Forward Checking:
- A King only blocks placements during `IsSafe`

With Forward Checking:
- All 8 neighboring cells become unavailable immediately

---

## Minimum Remaining Values — MRV (King, Knight)

At each step:
- Candidate cells are sorted by remaining availability

The most constrained cells are tried first.

This implements the **fail-first principle**:
- Dead ends appear earlier
- Less wasted recursion

Especially powerful for Knights because:
- Board availability becomes highly uneven
- Isolated cells are detected quickly

---

## Symmetry Breaking (Knight, square boards only)

On square boards, every solution may have up to **7 symmetric equivalents**.

The solver automatically generates:
- Rotations
- Reflections

Duplicate self-symmetric solutions are ignored.

### Transformations

```txt
Original           : (r, c)
Rotate 90          : (c, N-1-r)
Rotate 180         : (N-1-r, N-1-c)
Rotate 270         : (N-1-c, r)
Reflect horizontal : (r, N-1-c)
Reflect + Rot 90   : (c, r)
Reflect + Rot 180  : (N-1-r, c)
Reflect + Rot 270  : (N-1-c, N-1-r)
```

This can improve effective throughput by up to **8×**.

---

# No Solution Detection

When no valid arrangement exists, the application explains:

- The attack pattern of the selected piece
- Why the geometry prevents a solution
- The theoretical maximum
- Mathematical reasoning behind the limit

The goal is to explain:
- Not only *that* no solution exists
- But *why*

---

# Hardware-Aware Solution Limit

The solver can estimate a safe solution limit based on:
- RAM availability
- CPU throughput

The user may still override the recommendation manually.

---

## RAM Constraint

```txt
MemoryPerSolution = PieceCount * 8 bytes
RAMBudget         = AvailableFreeRAM * 75%
MaxByRAM          = RAMBudget / MemoryPerSolution
```

---

## CPU Constraint

The solver runs a short benchmark using the actual solving code.

```txt
SolutionsPerSecond = SolutionsFoundIn200ms / 0.2
CPUBudget          = 30 seconds * 40% duty cycle
MaxByCPU           = SolutionsPerSecond * 12
```

Final recommendation:

```txt
RecommendedLimit = Min(MaxByRAM, MaxByCPU)
```

The result appears in a draggable floating panel.

---

# Board Display

The board automatically scales to the panel size.

## Labels

### Rows
```txt
1 → H (bottom-up)
```

### Columns
Excel-style notation:

```txt
0-25   : A-Z
26-51  : AA-AZ
52-77  : BA-BZ
```

Pieces are rendered as:
- Red circles
- Interactive solution previews

Navigation:
- Next
- Previous

---

# Board Constraints

```txt
Minimum board size : 1 × 1
Maximum board size : 20 × 20
Piece count        : 1 → theoretical maximum
Max solutions      : 1 → hardware-recommended limit
```

---

# Build Requirements

```txt
Windows 10 or later
Visual Studio 2019 or later
.NET Framework 4.7.2 or later
C++/CLI support enabled
Windows Management Instrumentation (WMI)
```

---

# Building

```txt
1. Open ChessPlaceSolverGUI.sln
2. Set:
   - Configuration: Release
   - Platform: x64
3. Build Solution (Ctrl + Shift + B)
4. Executable appears in:
   x64/Release/
```

---

# Usage

```txt
1. Select a piece type
2. Set board dimensions
3. Set piece count
4. Optionally calculate hardware limit
5. Click Solve
6. Browse solutions using Next/Previous
7. Click Clear to reset
```

---

# Project Structure

```txt
ChessPlaceSolverGUI.sln
    Visual Studio solution file

MainForm.h
    Form declarations
    UI control declarations
    InitializeComponent
    Method signatures

MainForm.cpp
    Solver algorithms
    Event handlers
    Hardware analysis
    Board rendering
```

---

# License

See the `LICENSE` file for usage terms.

This software is permitted for:
- Educational use
- Personal non-commercial use

The following are prohibited:
- Commercial use
- Resale
- Sublicensing

---

# Author

**Omar**
