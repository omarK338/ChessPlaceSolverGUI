#include "MainForm.h"
#include <bitset>
#include <cmath>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Management;

namespace ChessPlaceSolverGUI {

    bool MainForm::ValidateInput(String^& ErrorMessage)
    {
        if (comboBox1->SelectedItem == nullptr)
        {
            ErrorMessage = "Please select a piece type.";
            return false;
        }
        if (BoardHeight <= 0 || BoardWidth <= 0 || PieceCount <= 0)
        {
            ErrorMessage = "Board dimensions and piece count must be greater than 0.";
            return false;
        }
        if (BoardHeight > 20 || BoardWidth > 20)
        {
            ErrorMessage = "Board dimensions cannot exceed 20.";
            return false;
        }
        int MaxPieces = CalcMaxPieces();
        if (PieceCount > MaxPieces)
        {
            ErrorMessage = String::Format(
                "Too many pieces for this board.\n"
                "Maximum {0} {1}s on a {2}x{3} board.",
                MaxPieces, PieceType, BoardHeight, BoardWidth);
            return false;
        }
        return true;
    }

    int MainForm::CalcMaxPieces()
    {
        if (PieceType == "Queen" || PieceType == "Rook")
            return Math::Min(BoardHeight, BoardWidth);

        if (PieceType == "Bishop")
            return Math::Max(1, (BoardHeight - 1) + (BoardWidth - 1));

        if (PieceType == "King")
            return (int)Math::Ceiling(BoardHeight / 2.0) *
            (int)Math::Ceiling(BoardWidth / 2.0);

        if (PieceType == "Knight")
            return (int)Math::Ceiling((BoardHeight * BoardWidth) / 2.0);

        return BoardHeight * BoardWidth;
    }

    String^ MainForm::GetNoSolutionReason()
    {
        int MaxPieces = CalcMaxPieces();

        if (PieceType == "Queen")
        {
            return String::Format(
                "No solution found for {0} Queens on a {1}x{2} board.\n\n"
                "Technical reason:\n"
                "A Queen attacks every cell in its row, column, and both diagonals.\n"
                "This means no two Queens can share any row, column, or diagonal.\n"
                "On a {1}x{2} board, the maximum number of non-attacking Queens\n"
                "is {3} (one per row and one per column).\n\n"
                "Logical reason:\n"
                "With {0} Queens and {4} rows, the algorithm exhausted every\n"
                "possible arrangement and found no valid placement.\n"
                "This can happen when piece count is close to the theoretical\n"
                "maximum and board proportions make diagonal conflicts unavoidable.",
                PieceCount, BoardHeight, BoardWidth, MaxPieces,
                BoardHeight);
        }

        if (PieceType == "Rook")
        {
            return String::Format(
                "No solution found for {0} Rooks on a {1}x{2} board.\n\n"
                "Technical reason:\n"
                "A Rook attacks every cell in its row and column.\n"
                "This means no two Rooks can share any row or column.\n"
                "On a {1}x{2} board, the maximum number of non-attacking Rooks\n"
                "is {3} (limited by the smaller dimension).\n\n"
                "Logical reason:\n"
                "With {0} Rooks, the algorithm needs {0} distinct rows and\n"
                "{0} distinct columns simultaneously.\n"
                "On this board that is {4}.",
                PieceCount, BoardHeight, BoardWidth, MaxPieces,
                PieceCount <= MaxPieces ? "theoretically possible but no arrangement exists" :
                "impossible because the board is too small");
        }

        if (PieceType == "Bishop")
        {
            return String::Format(
                "No solution found for {0} Bishops on a {1}x{2} board.\n\n"
                "Technical reason:\n"
                "A Bishop attacks every cell on both its diagonals.\n"
                "Bishops on white squares only attack other white-square cells.\n"
                "Bishops on black squares only attack other black-square cells.\n"
                "On a {1}x{2} board there are {5} white diagonals and {6} black diagonals.\n"
                "The maximum non-attacking Bishops is {3}.\n\n"
                "Logical reason:\n"
                "The algorithm exhausted all diagonal arrangements.\n"
                "With {0} Bishops, conflicts on shared diagonals were unavoidable\n"
                "in every arrangement tried.",
                PieceCount, BoardHeight, BoardWidth, MaxPieces,
                0,
                BoardHeight + BoardWidth - 1,
                BoardHeight + BoardWidth - 1);
        }

        if (PieceType == "King")
        {
            return String::Format(
                "No solution found for {0} Kings on a {1}x{2} board.\n\n"
                "Technical reason:\n"
                "A King attacks all 8 surrounding cells (3x3 neighborhood).\n"
                "Non-attacking Kings must be placed at least 2 cells apart\n"
                "in every direction (horizontal, vertical, diagonal).\n"
                "This creates a grid pattern where Kings occupy every other\n"
                "cell in both dimensions.\n"
                "On a {1}x{2} board the maximum is {3} Kings\n"
                "(Ceil({1}/2) x Ceil({2}/2) = {4} x {5}).\n\n"
                "Logical reason:\n"
                "With {0} Kings the algorithm could not find enough\n"
                "cells that are mutually at least 2 steps apart.\n"
                "The board density required exceeds what this board allows.",
                PieceCount, BoardHeight, BoardWidth, MaxPieces,
                (int)Math::Ceiling(BoardHeight / 2.0),
                (int)Math::Ceiling(BoardWidth / 2.0));
        }

        if (PieceType == "Knight")
        {
            return String::Format(
                "No solution found for {0} Knights on a {1}x{2} board.\n\n"
                "Technical reason:\n"
                "A Knight attacks up to 8 cells in an L-shape pattern\n"
                "(2 squares in one direction, 1 square perpendicular).\n"
                "Knights on white squares only attack black squares and vice versa.\n"
                "This means all Knights of the same color are mutually non-attacking.\n"
                "On a {1}x{2} board ({3} total cells), the maximum is {4} Knights\n"
                "(all cells of the majority color = Ceil({3}/2)).\n\n"
                "Logical reason:\n"
                "With {0} Knights the algorithm explored all arrangements\n"
                "including symmetry-reduced search space.\n"
                "No valid placement exists for this configuration.\n"
                "Note: if piece count equals the theoretical maximum,\n"
                "solutions exist by filling all cells of one color.",
                PieceCount, BoardHeight, BoardWidth,
                BoardHeight * BoardWidth, MaxPieces);
        }

        return String::Format(
            "No solution found for {0} {1}s on a {2}x{3} board.\n"
            "The algorithm exhausted all possible arrangements.",
            PieceCount, PieceType, BoardHeight, BoardWidth);
    }

    void MainForm::button1_Click(System::Object^ Sender, System::EventArgs^ E)
    {
        BoardHeight = static_cast<int>(numericUpDown2->Value);
        BoardWidth = static_cast<int>(numericUpDown3->Value);
        PieceCount = static_cast<int>(numericUpDown1->Value);
        PieceType = comboBox1->SelectedItem->ToString();

        String^ ErrorMessage;
        if (!ValidateInput(ErrorMessage))
        {
            MessageBox::Show(ErrorMessage, "Invalid Input",
                MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        Solutions->Clear();
        CurrentSolutionIndex = 0;
        SolutionsFound = 0;
        NoSolutionReason = "";
        panel1->Invalidate();

        button1->Enabled = false;
        button3->Enabled = false;
        button4->Enabled = false;
        textBox1->Text = "Solving...";
        textBox2->Text = "";

        CancelSource = gcnew CancellationTokenSource();
        Task::Run(gcnew Action(this, &MainForm::RunSolverAsync))
            ->ContinueWith(gcnew Action<Task^>(this, &MainForm::OnSolverFinished));
    }

    void MainForm::button2_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (CancelSource != nullptr)
        {
            CancelSource->Cancel();
            CancelSource = nullptr;
        }
        Solutions->Clear();
        CurrentSolutionIndex = 0;
        SolutionsFound = 0;
        textBox1->Clear();
        textBox2->Clear();
        panel1->Invalidate();
        button1->Enabled = true;
        button3->Enabled = true;
        button4->Enabled = true;
    }

    void MainForm::button3_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (CancelSource != nullptr && !CancelSource->IsCancellationRequested)
            return;
        if (Solutions->Count == 0)
            return;
        CurrentSolutionIndex = (CurrentSolutionIndex - 1 + Solutions->Count) % Solutions->Count;
        panel1->Invalidate();
        UpdateSolutionDisplay();
    }

    void MainForm::button4_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (CancelSource != nullptr && !CancelSource->IsCancellationRequested)
            return;
        if (Solutions->Count == 0)
            return;
        CurrentSolutionIndex = (CurrentSolutionIndex + 1) % Solutions->Count;
        panel1->Invalidate();
        UpdateSolutionDisplay();
    }

    void MainForm::Solve(String^ Type, int Count, int Height, int Width)
    {
        PieceType = Type;
        PieceCount = Count;
        BoardHeight = Height;
        BoardWidth = Width;
        NoSolutionReason = "";

        if (PieceType == "Queen")
        {
            array<int>^ Rows = gcnew array<int>(Height);
            for (int i = 0; i < Height; i++) Rows[i] = -1;
            PlaceQueens(Rows, 0, Count);
        }
        else if (PieceType == "Rook" || PieceType == "Bishop")
        {
            array<int>^ Board = gcnew array<int>(Height * Width);
            AttackedRows = gcnew array<bool>(Height);
            AttackedCols = gcnew array<bool>(Width);
            AttackedDiagSum = gcnew array<bool>(Height + Width);
            AttackedDiagDiff = gcnew array<bool>(Height + Width);
            PlacePieces(Board, 0, Count);
        }
        else if (PieceType == "King")
        {
            PlaceWithFC_MRV(Count);
        }
        else if (PieceType == "Knight")
        {
            PlaceWithFC_MRV_Symmetry(Count);
        }
    }

    void MainForm::PlaceQueens(array<int>^ Rows, int Row, int Remaining)
    {
        if (CancelSource != nullptr && CancelSource->IsCancellationRequested)
            return;
        if (SolutionsFound >= MAXSolutions)
            return;
        if (Remaining == 0)
        {
            auto solution = gcnew System::Collections::Generic::List<Point>();
            for (int r = 0; r < BoardHeight; r++)
            {
                if (Rows[r] >= 0)
                    solution->Add(Point(r, Rows[r]));
            }
            Solutions->Add(solution);
            SolutionsFound++;
            UpdateProgress();
            return;
        }
        if (Row >= BoardHeight || Remaining > BoardHeight - Row)
            return;

        for (int Col = 0; Col < BoardWidth; Col++)
        {
            if (IsSafeQueen(Rows, Row, Col))
            {
                Rows[Row] = Col;
                PlaceQueens(Rows, Row + 1, Remaining - 1);
                Rows[Row] = -1;
            }
        }
    }

    bool MainForm::IsSafeQueen(array<int>^ Rows, int Row, int Col)
    {
        for (int PrevRow = 0; PrevRow < Row; PrevRow++)
        {
            int PrevCol = Rows[PrevRow];
            if (PrevCol < 0)
                continue;
            if (PrevCol == Col || Math::Abs(PrevRow - Row) == Math::Abs(PrevCol - Col))
                return false;
        }
        return true;
    }

    void MainForm::PlacePieces(array<int>^ Board, int Index, int Remaining)
    {
        if (CancelSource != nullptr && CancelSource->IsCancellationRequested) return;
        if (SolutionsFound >= MAXSolutions) return;

        if (Remaining == 0)
        {
            auto Solution = gcnew System::Collections::Generic::List<Point>();
            for (int i = 0; i < BoardHeight * BoardWidth; i++)
                if (Board[i] == 1)
                    Solution->Add(Point(i / BoardWidth, i % BoardWidth));
            Solutions->Add(Solution);
            SolutionsFound++;
            UpdateProgress();
            return;
        }

        if (Index >= BoardHeight * BoardWidth || Remaining > BoardHeight * BoardWidth - Index)
            return;

        int Row = Index / BoardWidth;
        int Col = Index % BoardWidth;

        if (IsSafe(Board, Row, Col))
        {
            Board[Index] = 1;
            MarkAttacks(Row, Col, true);
            PlacePieces(Board, Index + 1, Remaining - 1);
            Board[Index] = 0;
            MarkAttacks(Row, Col, false);
        }
        PlacePieces(Board, Index + 1, Remaining);
    }

    bool MainForm::IsSafe(array<int>^ Board, int Row, int Col)
    {
        if (PieceType == "Rook")
            return !AttackedRows[Row] && !AttackedCols[Col];

        if (PieceType == "Bishop")
            return !AttackedDiagSum[Row + Col] && !AttackedDiagDiff[Row - Col + BoardWidth];

        if (PieceType == "Knight")
        {
            int Moves[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1} };
            for (int M = 0; M < 8; M++)
            {
                int R = Row + Moves[M][0];
                int C = Col + Moves[M][1];
                if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                    if (Board[R * BoardWidth + C] == 1) return false;
            }
            return true;
        }

        if (PieceType == "King")
        {
            for (int Dr = -1; Dr <= 1; Dr++)
                for (int Dc = -1; Dc <= 1; Dc++)
                {
                    if (Dr == 0 && Dc == 0) continue;
                    int R = Row + Dr;
                    int C = Col + Dc;
                    if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                        if (Board[R * BoardWidth + C] == 1) return false;
                }
            return true;
        }

        return true;
    }

    void MainForm::MarkAttacks(int Row, int Col, bool Value)
    {
        if (PieceType == "Rook")
        {
            AttackedRows[Row] = Value;
            AttackedCols[Col] = Value;
        }
        else if (PieceType == "Bishop")
        {
            AttackedDiagSum[Row + Col] = Value;
            AttackedDiagDiff[Row - Col + BoardWidth] = Value;
        }
    }

    void MainForm::PlaceWithFC_MRV(int Remaining)
    {
        AvailableCount = gcnew array<int>(BoardHeight * BoardWidth);
        array<int>^ Board = gcnew array<int>(BoardHeight * BoardWidth);
        PlaceKing_FC_MRV(Board, Remaining);
    }

    void MainForm::PlaceKing_FC_MRV(array<int>^ Board, int Remaining)
    {
        if (CancelSource != nullptr && CancelSource->IsCancellationRequested) return;
        if (SolutionsFound >= MAXSolutions) return;

        if (Remaining == 0)
        {
            auto Solution = gcnew System::Collections::Generic::List<Point>();
            for (int i = 0; i < BoardHeight * BoardWidth; i++)
                if (Board[i] == 1)
                    Solution->Add(Point(i / BoardWidth, i % BoardWidth));
            Solutions->Add(Solution);
            SolutionsFound++;
            UpdateProgress();
            return;
        }

        // Collect available cells
        System::Collections::Generic::List<int>^ Candidates =
            gcnew System::Collections::Generic::List<int>();

        for (int i = 0; i < BoardHeight * BoardWidth; i++)
            if (Board[i] == 0 && AvailableCount[i] == 0)
                Candidates->Add(i);

        // Store board reference for sorting
        CurrentBoard = Board;
        SortByMRV(Candidates, false);

        for (int Idx = 0; Idx < Candidates->Count; Idx++)
        {
            if (CancelSource != nullptr && CancelSource->IsCancellationRequested) return;
            if (SolutionsFound >= MAXSolutions) return;

            int Index = Candidates[Idx];
            int Row = Index / BoardWidth;
            int Col = Index % BoardWidth;

            Board[Index] = 1;
            for (int Dr = -1; Dr <= 1; Dr++)
                for (int Dc = -1; Dc <= 1; Dc++)
                {
                    if (Dr == 0 && Dc == 0) continue;
                    int R = Row + Dr;
                    int C = Col + Dc;
                    if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                        AvailableCount[R * BoardWidth + C]++;
                }

            PlaceKing_FC_MRV(Board, Remaining - 1);

            Board[Index] = 0;
            for (int Dr = -1; Dr <= 1; Dr++)
                for (int Dc = -1; Dc <= 1; Dc++)
                {
                    if (Dr == 0 && Dc == 0) continue;
                    int R = Row + Dr;
                    int C = Col + Dc;
                    if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                        AvailableCount[R * BoardWidth + C]--;
                }
        }
    }

    void MainForm::PlaceWithFC_MRV_Symmetry(int Remaining)
    {
        AvailableCount = gcnew array<int>(BoardHeight * BoardWidth);
        array<int>^ Board = gcnew array<int>(BoardHeight * BoardWidth);
        PlaceKnight_FC_MRV(Board, Remaining);
    }

    void MainForm::PlaceKnight_FC_MRV(array<int>^ Board, int Remaining)
    {
        if (CancelSource != nullptr && CancelSource->IsCancellationRequested) return;
        if (SolutionsFound >= MAXSolutions) return;

        if (Remaining == 0)
        {
            auto Solution = gcnew System::Collections::Generic::List<Point>();
            for (int i = 0; i < BoardHeight * BoardWidth; i++)
                if (Board[i] == 1)
                    Solution->Add(Point(i / BoardWidth, i % BoardWidth));
            Solutions->Add(Solution);
            SolutionsFound++;
            UpdateProgress();

            if (BoardHeight == BoardWidth && SolutionsFound < MAXSolutions)
                GenerateSymmetric(Solution);

            return;
        }

        System::Collections::Generic::List<int>^ Candidates =
            gcnew System::Collections::Generic::List<int>();

        for (int i = 0; i < BoardHeight * BoardWidth; i++)
            if (Board[i] == 0 && AvailableCount[i] == 0)
                Candidates->Add(i);

        CurrentBoard = Board;
        SortByMRV(Candidates, true);

        int KnightMoves[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1} };

        for (int Idx = 0; Idx < Candidates->Count; Idx++)
        {
            if (CancelSource != nullptr && CancelSource->IsCancellationRequested) return;
            if (SolutionsFound >= MAXSolutions) return;

            int Index = Candidates[Idx];
            int Row = Index / BoardWidth;
            int Col = Index % BoardWidth;

            Board[Index] = 1;
            for (int M = 0; M < 8; M++)
            {
                int R = Row + KnightMoves[M][0];
                int C = Col + KnightMoves[M][1];
                if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                    AvailableCount[R * BoardWidth + C]++;
            }

            PlaceKnight_FC_MRV(Board, Remaining - 1);

            Board[Index] = 0;
            for (int M = 0; M < 8; M++)
            {
                int R = Row + KnightMoves[M][0];
                int C = Col + KnightMoves[M][1];
                if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                    AvailableCount[R * BoardWidth + C]--;
            }
        }
    }

    void MainForm::GenerateSymmetric(System::Collections::Generic::List<Point>^ Solution)
    {
        int N = BoardHeight;

        // Store all 8 transforms as separate solutions
        // Transform definitions for square board of size N:
        // Original          : (r, c)
        // Rotate 90         : (c, N-1-r)
        // Rotate 180        : (N-1-r, N-1-c)
        // Rotate 270        : (N-1-c, r)
        // Reflect horizontal: (r, N-1-c)
        // Reflect + Rot 90  : (c, r)
        // Reflect + Rot 180 : (N-1-r, c)
        // Reflect + Rot 270 : (N-1-c, N-1-r)

        for (int T = 1; T <= 7; T++)
        {
            if (SolutionsFound >= MAXSolutions) return;

            auto Symmetric = gcnew System::Collections::Generic::List<Point>();
            bool IsDuplicate = false;

            for each(Point P in Solution)
            {
                int R = P.X;
                int C = P.Y;
                Point Transformed;

                switch (T)
                {
                case 1: Transformed = Point(C, N - 1 - R); break;         // Rotate 90
                case 2: Transformed = Point(N - 1 - R, N - 1 - C); break; // Rotate 180
                case 3: Transformed = Point(N - 1 - C, R);     break;     // Rotate 270
                case 4: Transformed = Point(R, N - 1 - C); break;         // Reflect horizontal
                case 5: Transformed = Point(C, R);     break;             // Reflect + Rot 90
                case 6: Transformed = Point(N - 1 - R, C);     break;     // Reflect + Rot 180
                case 7: Transformed = Point(N - 1 - C, N - 1 - R); break; // Reflect + Rot 270
                default: Transformed = P; break;
                }
                Symmetric->Add(Transformed);
            }

            // Check if this symmetric solution is identical to the original
            bool SameAsOriginal = true;
            for (int I = 0; I < Solution->Count; I++)
            {
                if (Symmetric[I].X != Solution[I].X || Symmetric[I].Y != Solution[I].Y)
                {
                    SameAsOriginal = false;
                    break;
                }
            }

            if (!SameAsOriginal)
            {
                Solutions->Add(Symmetric);
                SolutionsFound++;
                UpdateProgress();
            }
        }
    }

    int MainForm::CountKingMoves(array<int>^ Board, int Index)
    {
        int Row = Index / BoardWidth;
        int Col = Index % BoardWidth;
        int Count = 0;

        for (int Dr = -1; Dr <= 1; Dr++)
            for (int Dc = -1; Dc <= 1; Dc++)
            {
                if (Dr == 0 && Dc == 0) continue;
                int R = Row + Dr;
                int C = Col + Dc;
                if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                    if (Board[R * BoardWidth + C] == 0 && AvailableCount[R * BoardWidth + C] == 0)
                        Count++;
            }
        return Count;
    }

    int MainForm::CountKnightMoves(array<int>^ Board, int Index)
    {
        int KnightMoves[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1} };
        int Row = Index / BoardWidth;
        int Col = Index % BoardWidth;
        int Count = 0;

        for (int M = 0; M < 8; M++)
        {
            int R = Row + KnightMoves[M][0];
            int C = Col + KnightMoves[M][1];
            if (R >= 0 && R < BoardHeight && C >= 0 && C < BoardWidth)
                if (Board[R * BoardWidth + C] == 0 && AvailableCount[R * BoardWidth + C] == 0)
                    Count++;
        }
        return Count;
    }

    void MainForm::SortByMRV(System::Collections::Generic::List<int>^ Candidates, bool IsKnight)
    {    
		// Sort candidates by MRV
        for (int I = 1; I < Candidates->Count; I++)
        {
            int Key = Candidates[I];
            int KeyCount = IsKnight
                ? CountKnightMoves(CurrentBoard, Key)
                : CountKingMoves(CurrentBoard, Key);
            int J = I - 1;

            while (J >= 0)
            {
                int JCount = IsKnight
                    ? CountKnightMoves(CurrentBoard, Candidates[J])
                    : CountKingMoves(CurrentBoard, Candidates[J]);

                if (JCount <= KeyCount) break;
                Candidates[J + 1] = Candidates[J];
                J--;
            }
            Candidates[J + 1] = Key;
        }
    }
    
    void MainForm::UpdateSolutionDisplay()
    {
        if (Solutions->Count == 0)
        {
            textBox1->Text = "No solution found.";
            textBox2->Text = (NoSolutionReason->Length > 0)
                ? NoSolutionReason
                : "No valid placement exists for this configuration.";
            return;
        }

        textBox1->Text = String::Format(
            "Solution {0} of {1}",
            CurrentSolutionIndex + 1,
            Solutions->Count);

        String^ Positions = "";
        for each(Point P in Solutions[CurrentSolutionIndex])
            Positions += String::Format(
                "({0}, {1})\n",
                P.X + 1,
                P.Y + 1);

        textBox2->Text = Positions;
    }

    void MainForm::UpdateProgress()
    {
        if (SolutionsFound % 100 == 0)
            this->Invoke(gcnew Action(this, &MainForm::UpdateSolutionDisplay));
    }

    void MainForm::RunSolverAsync()
    {
        try
        {
            Solve(PieceType, PieceCount, BoardHeight, BoardWidth);
        }
        catch (OperationCanceledException^)
        {
            // Handled by cancellation
        }
        catch (OutOfMemoryException^ ex)
        {
            this->Invoke(gcnew Action<String^>(this, &MainForm::ShowSolverError), "Out of memory: " + ex->Message);
        }
        catch (Exception^ ex)
        {
            this->Invoke(gcnew Action<String^>(this, &MainForm::ShowSolverError), ex->Message);
        }
    }

    void MainForm::ShowSolverError(String^ message)
    {
        MessageBox::Show("Error while solving: " + message, "Solver Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        button1->Enabled = true;
        button3->Enabled = true;
        button4->Enabled = true;
        textBox1->Text = "Solver failed.";
        textBox2->Text = "";
    }

    void MainForm::OnSolverFinished(Task^ task)
    {
        this->Invoke(gcnew Action(this, &MainForm::OnSolverFinishedUI));
    }

    void MainForm::OnSolverFinishedUI()
    {
        button1->Enabled = true;
        button3->Enabled = true;
        button4->Enabled = true;

        if (Solutions->Count > 0)
        {
            CurrentSolutionIndex = 0;
            UpdateSolutionDisplay();
            panel1->Invalidate();
        }
        else
        {
            NoSolutionReason = GetNoSolutionReason();
            UpdateSolutionDisplay();
        }

        CancelSource = nullptr;
    }

    void MainForm::MainForm_Load(System::Object^ Sender, System::EventArgs^ E)
    {
        comboBox1->Items->AddRange(
            gcnew cli::array<Object^>{"Queen", "King", "Rook", "Bishop", "Knight"});
        comboBox1->SelectedIndex = 0;

        numericUpDown2->Maximum = 20;
        numericUpDown3->Maximum = 20;
        numericUpDown2->Value = 8;
        numericUpDown3->Value = 8;

        PieceType = "Queen";
        BoardHeight = 8;
        BoardWidth = 8;

        int MaxPieces = CalcMaxPieces();
        numericUpDown1->Maximum = MaxPieces;
        numericUpDown1->Value = 8;
    }

    String^ MainForm::GetColumnLabel(int ColIndex)
    {
        String^ Label = "";
        int Index = ColIndex;
        do
        {
            Label = String::Format("{0}", static_cast<wchar_t>('A' + (Index % 26))) + Label;
            Index = (Index / 26) - 1;
        } while (Index >= 0);
        return Label;
    }

    void MainForm::panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
    {
        if (Solutions->Count == 0 || CurrentSolutionIndex >= Solutions->Count)
            return;

        Graphics^ g = e->Graphics;
        Pen^ pen = gcnew Pen(Color::Black, 2);
        Brush^ WhiteBrush = Brushes::White;
        Brush^ BlackBrush = Brushes::Gray;
        Brush^ PieceBrush = Brushes::Red;

        int CellSize = Math::Min(
            static_cast<int>(panel1->Width / static_cast<float>(BoardWidth + 1)),
            static_cast<int>(panel1->Height / static_cast<float>(BoardHeight + 1)));
        int OffsetX = (panel1->Width - BoardWidth * CellSize) / 2;
        int OffsetY = (panel1->Height - BoardHeight * CellSize) / 2;

        // Draw row labels
        for (int i = 0; i < BoardHeight; i++)
        {
            String^ RowLabel = (BoardHeight - i).ToString();
            g->DrawString(
                RowLabel,
                LabelFont,
                Brushes::Black,
                static_cast<float>(OffsetX - 30),
                static_cast<float>(OffsetY + (BoardHeight - 1 - i) * CellSize + CellSize / 2 - 7));
        }

        // Draw column labels using Excel-style naming
        for (int j = 0; j < BoardWidth; j++)
        {
            String^ ColLabel = GetColumnLabel(j);
            g->DrawString(
                ColLabel,
                LabelFont,
                Brushes::Black,
                static_cast<float>(OffsetX + j * CellSize + CellSize / 2 - 5),
                static_cast<float>(OffsetY - 20));
        }

        // Draw board
        for (int i = 0; i < BoardHeight; i++)
        {
            for (int j = 0; j < BoardWidth; j++)
            {
                int X = OffsetX + j * CellSize;
                int Y = OffsetY + (BoardHeight - 1 - i) * CellSize;
                bool IsWhite = (i + j) % 2 == 0;
                g->FillRectangle(IsWhite ? WhiteBrush : BlackBrush, X, Y, CellSize, CellSize);
                g->DrawRectangle(pen, X, Y, CellSize, CellSize);
            }
        }

        // Draw pieces
        int BallSize = static_cast<int>(CellSize * 0.75);
        int BallOffset = BallSize / 2;
        for each(Point p in Solutions[CurrentSolutionIndex])
        {
            int X = OffsetX + p.Y * CellSize + CellSize / 2;
            int Y = OffsetY + (BoardHeight - 1 - p.X) * CellSize + CellSize / 2;
            g->FillEllipse(PieceBrush, X - BallOffset, Y - BallOffset, BallSize, BallSize);
        }
    }

    void MainForm::comboBox1_SelectedIndexChanged(System::Object^ Sender, System::EventArgs^ E)
    {
        if (comboBox1->SelectedItem == nullptr) return;
        PieceType = comboBox1->SelectedItem->ToString();

        int MaxPieces = CalcMaxPieces();
        numericUpDown1->Maximum = MaxPieces;
        if (numericUpDown1->Value > MaxPieces)
            numericUpDown1->Value = MaxPieces;
    }

    void MainForm::numericUpDown1_ValueChanged(System::Object^ sender, System::EventArgs^ e)
    {
        PieceCount = static_cast<int>(numericUpDown1->Value);
    }

    void MainForm::numericUpDown2_ValueChanged(System::Object^ Sender, System::EventArgs^ E)
    {
        BoardHeight = static_cast<int>(numericUpDown2->Value);
        int MaxPieces = CalcMaxPieces();
        numericUpDown1->Maximum = MaxPieces;
        if (numericUpDown1->Value > MaxPieces)
            numericUpDown1->Value = MaxPieces;
    }

    void MainForm::numericUpDown3_ValueChanged(System::Object^ Sender, System::EventArgs^ E)
    {
        BoardWidth = static_cast<int>(numericUpDown3->Value);
        int MaxPieces = CalcMaxPieces();
        numericUpDown1->Maximum = MaxPieces;
        if (numericUpDown1->Value > MaxPieces)
            numericUpDown1->Value = MaxPieces;
    }

    void MainForm::textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {}
    void MainForm::label9_Click(System::Object^ sender, System::EventArgs^ e) {}

    [STAThreadAttribute]
    int main(array<String^>^ args)
    {
        Application::EnableVisualStyles();
        Application::SetCompatibleTextRenderingDefault(false);
        Application::Run(gcnew ChessPlaceSolverGUI::MainForm());
        return 0;
    }
}