# Chess Place Solver

A Windows desktop application that finds all valid placements of N chess pieces
on a custom board such that no piece attacks another on the next move.

## Problem Definition

Given a board of size H x W and a piece type with count N, find every arrangement
of N pieces such that no piece lies within the attack range of any other piece.
This is a generalization of the classic N-Queens problem extended to all standard
chess piece types on arbitrary rectangular boards.

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

| Piece  | Maximum Formula                  |
|--------|----------------------------------|
| Queen  | Min(H, W)                        |
| Rook   | Min(H, W)                        |
| Bishop | (H - 1) + (W - 1)                |
| King   | Ceil(H/2) * Ceil(W/2)            |
| Knight | Ceil(H * W / 2)                  |

## Algorithms

### Backtracking with Attack Maps (Queen, Rook, Bishop)
Maintains boolean arrays tracking occupied rows, columns, and diagonals.
Each safety check is O(1). The search tree collapses rapidly due to strong
pruning from linear attack patterns.

### Forward Checking (King, Knight)
Maintains an integer availability count per cell. When a piece is placed,
all cells it attacks have their count incremented. A cell is only a valid
candidate when its count is zero. When a piece is removed during backtracking,
counts are decremented. This eliminates attacked cells from consideration
immediately rather than discovering conflicts deeper in the search tree.

### Minimum Remaining Values - MRV (King, Knight)
At each placement step, candidate cells are sorted by the number of available
cells they can reach using the piece's attack pattern. Cells with fewer
reachable available cells are tried first. This is the fail-first principle:
the most constrained positions are resolved earliest, causing dead ends to
be detected and backtracked sooner.

### Symmetry Breaking (Knight, square boards only)
On square boards, every solution has up to 7 symmetric equivalents produced
by 4 rotations and 2 reflections. The solver generates all 8 symmetric copies
of each found solution automatically. Self-symmetric solutions that map to
themselves under a transform are detected and not duplicated.

## Board Constraints

- Minimum board size: 1 x 1
- Maximum board size: 20 x 20
- Piece count is capped at the theoretical maximum for the selected piece
  type and board dimensions, calculated dynamically

## No Solution Detection

When no valid placement exists, the application displays a detailed explanation
covering the technical reason based on the piece attack pattern, the theoretical
maximum for the given board size, and why the requested count exceeds what
the board geometry allows.

## Solution Navigation

The solver runs on a background thread and stores up to 1000 solutions.
Each solution can be browsed using the Next and Previous buttons.
The board panel renders the current solution with labeled rows and columns
using Excel-style column naming (A-Z, AA-AZ, BA-BZ, etc.) for boards
wider than 26 columns.

## Build Requirements

- Windows 10 or later
- Visual Studio 2019 or later
- .NET Framework 4.7.2 or later
- C++/CLI support enabled in the Visual Studio installer

## Building

1. Open ChessPlaceSolverGUI.sln in Visual Studio
2. Set configuration to Release and platform to x64
3. Build solution (Ctrl+Shift+B)
4. Executable is generated in x64/Release/

## Usage

1. Select a piece type from the dropdown
2. Set board height and width (1 to 20)
3. Set piece count (automatically capped at the theoretical maximum)
4. Click Solve
5. Use Next and Previous to browse all found solutions
6. Click Clear to reset the board

## License

See LICENSE file for terms of use.
This software is permitted for educational and personal non-commercial use only.

## Author

Omar
