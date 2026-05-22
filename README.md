```md
# Chess Place Solver

A Windows desktop application that finds all valid placements of N chess pieces
on a custom board such that no piece attacks another on the next move.
Built in C++/CLI using Windows Forms as a learning project in algorithm design,
constraint satisfaction, and systems programming.

## Background

This project started as a generalization of the classic N-Queens problem.
The original version solved only 8 Queens on an 8x8 board.
It was progressively expanded to support all standard chess piece types,
arbitrary rectangular board dimensions, dynamic hardware-aware solution limits,
and multiple specialized algorithms selected automatically based on piece type.

The development process involved identifying and fixing real algorithmic problems:
replacing a naive O(N) safety check with O(1) attack maps, replacing a single
generic solver with a portfolio of algorithms matched to each piece type,
implementing Forward Checking and Minimum Remaining Values heuristics for
weakly-pruned piece types, and adding symmetry breaking for Knights on square boards.

## Problem Definition

Given a board of size H x W and a piece type with count N, find every arrangement
of N pieces such that no piece lies within the attack range of any other piece.
This is a Constraint Satisfaction Problem (CSP) on a chessboard.
The solver finds all valid arrangements up to a configurable maximum limit
and allows the user to browse each solution on an interactive board display.

## Supported Piece Types

| Piece  | Attack Pattern                        | Algorithm Used                              |
|--------|---------------------------------------|---------------------------------------------|
| Queen  | Row, column, both diagonals           | Backtracking with O(1) attack maps          |
| Rook   | Row and column                        | Backtracking with O(1) attack maps          |
| Bishop | Both diagonals                        | Backtracking with O(1) attack maps          |
| King   | 3x3 neighborhood (8 cells)            | Forward Checking + MRV                      |
| Knight | L-shape (up to 8 cells)               | Forward Checking + MRV + Symmetry Breaking  |

## Maximum Piece Counts

The application calculates the theoretical maximum non-attacking pieces
for any board size automatically using these formulas:

| Piece  | Maximum Formula                  | Reasoning                                              |
|--------|----------------------------------|--------------------------------------------------------|
| Queen  | Min(H, W)                        | One per row, one per column, limited by smaller dim    |
| Rook   | Min(H, W)                        | Same row/column constraint as Queens                   |
| Bishop | (H - 1) + (W - 1)                | Independent white/black diagonal sets                  |
| King   | Ceil(H/2) * Ceil(W/2)            | Grid pattern, one King per 2x2 area                    |
| Knight | Ceil(H * W / 2)                  | All cells of majority color, Knights cannot attack same color |

The piece count input is automatically capped at the theoretical maximum
whenever the board size or piece type changes. This prevents the user from
requesting an impossible configuration before solving begins.

## Algorithms

### Algorithm Selection

The solver uses a portfolio approach: different algorithms are selected
automatically based on the piece type being placed. This is because different
attack patterns have fundamentally different pruning characteristics.
A single algorithm cannot be optimal for all piece types simultaneously.

```
Queen, Rook, Bishop --> Backtracking with O(1) attack maps
King               --> Forward Checking + MRV
Knight             --> Forward Checking + MRV + Symmetry Breaking
```

### Backtracking with Attack Maps (Queen, Rook, Bishop)

Maintains boolean arrays tracking occupied rows, columns, and diagonals.
Each safety check is O(1). When a piece is placed, its attacked rows,
columns, or diagonals are marked in constant time. When it is removed
during backtracking, those marks are cleared.

```
Queens  : AttackedCols[c] and diagonal arrays
Rooks   : AttackedRows[r] and AttackedCols[c]
Bishops : AttackedDiagSum[r+c] and AttackedDiagDiff[r-c+W]
```

The search tree collapses rapidly for these piece types because each
placement eliminates entire rows, columns, or diagonals from consideration.
For Queens specifically, the solver advances one row per recursive call,
guaranteeing one Queen per row and reducing the effective search space
to permutations of column assignments rather than all cell combinations.

### Forward Checking (King, Knight)

Maintains an integer availability count per cell. When a piece is placed,
all cells it attacks have their count incremented. A cell is only a valid
candidate when its count is zero. When a piece is removed during backtracking,
counts are decremented. This eliminates attacked cells from consideration
immediately rather than discovering conflicts deeper in the search tree.

For naive backtracking without forward checking, a King placed at position X
only prevents another King from being placed at X during the IsSafe check.
With forward checking, placing a King at X immediately removes all 8 neighbors
from the candidate list before the next recursive call begins. Dead ends are
detected one level earlier at every step of the search.

### Minimum Remaining Values - MRV (King, Knight)

At each placement step, candidate cells are sorted by the number of available
cells they can reach using the piece attack pattern. Cells with fewer reachable
available cells are tried first. This is the fail-first principle: the most
constrained positions are resolved earliest, causing dead ends to be detected
and backtracked sooner rather than after many wasted recursive calls.

For Knights this is especially powerful because Knight placement creates
highly unequal availability across the board. Some cells become isolated
quickly with zero reachable neighbors. MRV detects these isolated cells
and tries them first, immediately pruning entire subtrees that would
otherwise require deep exploration before the conflict is discovered.

### Symmetry Breaking (Knight, square boards only)

On square boards every solution has up to 7 symmetric equivalents produced
by 4 rotations and 2 reflections of the board. The solver generates all
8 symmetric copies of each found solution automatically after finding
the original. Self-symmetric solutions that map to themselves under a
transform are detected and not duplicated.

This effectively multiplies the number of solutions found per unit of
search work by up to 8x on square boards, since finding one solution
in the canonical orientation immediately yields up to 7 more without
any additional search.

The 8 transforms for a square board of size N are:

```
Original           : (r, c)
Rotate 90          : (c, N-1-r)
Rotate 180         : (N-1-r, N-1-c)
Rotate 270         : (N-1-c, r)
Reflect horizontal : (r, N-1-c)
Reflect + Rot 90   : (c, r)
Reflect + Rot 180  : (N-1-r, c)
Reflect + Rot 270  : (N-1-c, N-1-r)
```

## No Solution Detection

When no valid placement exists the application displays a detailed explanation
specific to the piece type. The explanation covers:

- The exact attack pattern of the piece type
- Why that pattern creates the constraint
- The theoretical maximum for the given board dimensions
- The mathematical reasoning behind that maximum
- Why the requested count exceeds what the board geometry allows

This gives the user enough information to understand not just that no solution
exists but why the geometry of the problem makes it impossible.

## Hardware-Aware Solution Limit

The maximum number of solutions to find is configurable by the user.
A Calculate Limit button analyzes the current machine and recommends
a safe maximum based on two hardware constraints:

### RAM constraint
```
MemoryPerSolution = PieceCount * 8 bytes
RAMBudget         = AvailableFreeRAM * 75%
MaxByRAM          = RAMBudget / MemoryPerSolution
```

### CPU constraint
The solver runs a 200ms sample of the actual solver code on a background
thread and counts how many solutions are found in that time. This gives
a real measured throughput on the actual machine rather than an estimate.

```
SolutionsPerSecond = SolutionsFoundIn200ms / 0.2
CPUBudget          = 30 seconds * 40% duty cycle = 12 seconds of work
MaxByCPU           = SolutionsPerSecond * 12
```

The recommended limit is the minimum of MaxByRAM and MaxByCPU.
The analysis result is shown in a floating draggable panel that the user
can position anywhere on the screen. The panel appears when Calculate Limit
is clicked and hides automatically when Solve is clicked.

The user retains full control: the recommended limit is applied to the
Max Solutions input but the user can adjust it manually before solving.
The limit is a usability cap, not a hardware protection mechanism, since
the solver's actual memory usage is well under 1 MB for all supported
board sizes and piece counts.

## Board Display

The board is rendered in a resizable panel that scales automatically to
the panel dimensions. Row labels use numeric notation (1 to H from bottom).
Column labels use Excel-style alphabetic notation:

```
Columns 0-25   : A through Z
Columns 26-51  : AA through AZ
Columns 52-77  : BA through BZ
...
```

Pieces are shown as red circles on the board. The current solution index
and total count are displayed above the board. The user can navigate through
all found solutions using Next and Previous buttons.

## Board Constraints

```
Minimum board size : 1 x 1
Maximum board size : 20 x 20
Piece count        : 1 to theoretical maximum for piece type and board size
Max solutions      : 1 to hardware-recommended limit (default 1000)
```

## Build Requirements

```
Windows 10 or later
Visual Studio 2019 or later
.NET Framework 4.7.2 or later
C++/CLI support enabled in the Visual Studio installer
Windows Management Instrumentation (WMI) - standard on all Windows installs
```

## Building

```
1. Open ChessPlaceSolverGUI.sln in Visual Studio
2. Set configuration to Release and platform to x64
3. Build solution (Ctrl+Shift+B)
4. Executable is generated in x64/Release/
```

## Usage

```
1. Select a piece type from the dropdown
2. Set board height and width (1 to 20)
3. Set piece count (automatically capped at theoretical maximum)
4. Optionally click Calculate Limit to get a hardware-based recommendation
   for the Max Solutions input, then adjust manually if desired
5. Click Solve
6. Use Next and Previous to browse all found solutions
7. Click Clear to reset the board and results
```

## Project Structure

```
ChessPlaceSolverGUI.sln    - Visual Studio solution file
MainForm.h                 - Form class declaration, fields, UI control declarations,
                             InitializeComponent, all method signatures
MainForm.cpp               - All method implementations including solver algorithms,
                             UI event handlers, hardware analysis, board rendering
```

## License

See LICENSE file for terms of use.
This software is permitted for educational and personal non-commercial use only.
Commercial use, resale, and sublicensing are prohibited.

## Author

Omar
```
