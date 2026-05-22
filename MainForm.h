#pragma once

namespace ChessPlaceSolverGUI {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Threading;
    using namespace System::Threading::Tasks;
    using namespace System::Management;

    public ref class MainForm : public System::Windows::Forms::Form
    {
    private:
        CancellationTokenSource^ CancelSource;
        // UI state
        int BoardHeight;
        int BoardWidth;
        int PieceCount;
        int CurrentSolutionIndex;
        String^ PieceType;
        // Store Solutions as list of piece Positions
        System::Collections::Generic::List<System::Collections::Generic::List<Point>^>^ Solutions;
        // Fonts for drawing
        System::Drawing::Font^ font;
        System::Drawing::Font^ LabelFont;
        // Progress tracking
        int SolutionsFound;
        const int MAXSolutions = 1000;
        // Attack tracking for IsSafe
        array<bool>^ AttackedRows;
        array<bool>^ AttackedCols;
        array<bool>^ AttackedDiagSum;
        array<bool>^ AttackedDiagDiff;
        // FC+MRV support
        array<int>^ AvailableCount;
        array<int>^ CurrentBoard;
        String^ NoSolutionReason;

    private: System::Windows::Forms::Label^ label6;
    private: System::Windows::Forms::Label^ label5;
    private: System::Windows::Forms::Label^ label7;
    private: System::Windows::Forms::Label^ label8;
    private: System::Windows::Forms::NumericUpDown^ numericUpDown3;
    private: System::Windows::Forms::Label^ label9;
    private: System::Windows::Forms::TextBox^ textBox2;

    public:
        MainForm(void)
        {
            InitializeComponent();
            Solutions = gcnew System::Collections::Generic::List<System::Collections::Generic::List<Point>^>();
            CurrentSolutionIndex = 0;
            SolutionsFound = 0;
            this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::panel1_Paint);
            this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
            font = gcnew System::Drawing::Font(L"Times New Roman", 12, FontStyle::Bold);
            LabelFont = gcnew System::Drawing::Font(L"Times New Roman", 10);
        }

    protected:
        ~MainForm()
        {
            if (components) delete components;
            if (CancelSource) CancelSource = nullptr;
            if (font) delete font;
            if (LabelFont) delete LabelFont;
        }

    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::ComboBox^ comboBox1;
    private: System::Windows::Forms::Label^ label2;
    private: System::Windows::Forms::NumericUpDown^ numericUpDown1;
    private: System::Windows::Forms::Label^ label3;
    private: System::Windows::Forms::NumericUpDown^ numericUpDown2;
    private: System::Windows::Forms::Button^ button1;
    private: System::Windows::Forms::Button^ button2;
    private: System::Windows::Forms::Label^ label4;
    private: System::Windows::Forms::TextBox^ textBox1;
    private: System::Windows::Forms::Panel^ panel1;
    private: System::Windows::Forms::Button^ button3;
    private: System::Windows::Forms::Button^ button4;
    private: System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
           void InitializeComponent(void)
           {
               System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
               this->label1 = (gcnew System::Windows::Forms::Label());
               this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
               this->label2 = (gcnew System::Windows::Forms::Label());
               this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
               this->label3 = (gcnew System::Windows::Forms::Label());
               this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
               this->button1 = (gcnew System::Windows::Forms::Button());
               this->button2 = (gcnew System::Windows::Forms::Button());
               this->label4 = (gcnew System::Windows::Forms::Label());
               this->textBox1 = (gcnew System::Windows::Forms::TextBox());
               this->panel1 = (gcnew System::Windows::Forms::Panel());
               this->button3 = (gcnew System::Windows::Forms::Button());
               this->button4 = (gcnew System::Windows::Forms::Button());
               this->label6 = (gcnew System::Windows::Forms::Label());
               this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
               this->label5 = (gcnew System::Windows::Forms::Label());
               this->label7 = (gcnew System::Windows::Forms::Label());
               this->label8 = (gcnew System::Windows::Forms::Label());
               this->textBox2 = (gcnew System::Windows::Forms::TextBox());
               this->label9 = (gcnew System::Windows::Forms::Label());
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->BeginInit();
               this->SuspendLayout();
               // 
               // label1
               // 
               this->label1->AutoSize = true;
               this->label1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 19.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label1->ForeColor = System::Drawing::Color::Blue;
               this->label1->Location = System::Drawing::Point(12, 76);
               this->label1->Name = L"label1";
               this->label1->Size = System::Drawing::Size(176, 38);
               this->label1->TabIndex = 1;
               this->label1->Text = L"Piece Type:";
               // 
               // comboBox1
               // 
               this->comboBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->comboBox1->FormattingEnabled = true;
               this->comboBox1->Location = System::Drawing::Point(248, 81);
               this->comboBox1->Name = L"comboBox1";
               this->comboBox1->Size = System::Drawing::Size(105, 37);
               this->comboBox1->TabIndex = 3;
               this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::comboBox1_SelectedIndexChanged);
               // 
               // label2
               // 
               this->label2->AutoSize = true;
               this->label2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 19.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label2->ForeColor = System::Drawing::Color::Blue;
               this->label2->Location = System::Drawing::Point(12, 137);
               this->label2->Name = L"label2";
               this->label2->Size = System::Drawing::Size(179, 38);
               this->label2->TabIndex = 4;
               this->label2->Text = L"Piece Num:";
               // 
               // numericUpDown1
               // 
               this->numericUpDown1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->numericUpDown1->Location = System::Drawing::Point(248, 143);
               this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
               this->numericUpDown1->Name = L"numericUpDown1";
               this->numericUpDown1->Size = System::Drawing::Size(105, 34);
               this->numericUpDown1->TabIndex = 5;
               this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
               this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &MainForm::numericUpDown1_ValueChanged);
               // 
               // label3
               // 
               this->label3->AutoSize = true;
               this->label3->Font = (gcnew System::Drawing::Font(L"Times New Roman", 19.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label3->ForeColor = System::Drawing::Color::Blue;
               this->label3->Location = System::Drawing::Point(12, 196);
               this->label3->Name = L"label3";
               this->label3->Size = System::Drawing::Size(216, 38);
               this->label3->TabIndex = 6;
               this->label3->Text = L"Board Height:";
               // 
               // numericUpDown2
               // 
               this->numericUpDown2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->numericUpDown2->Location = System::Drawing::Point(248, 202);
               this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
               this->numericUpDown2->Name = L"numericUpDown2";
               this->numericUpDown2->Size = System::Drawing::Size(105, 34);
               this->numericUpDown2->TabIndex = 7;
               this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
               this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &MainForm::numericUpDown2_ValueChanged);
               // 
               // button1
               // 
               this->button1->BackColor = System::Drawing::Color::Red;
               this->button1->Font = (gcnew System::Drawing::Font(L"Bell MT", 19.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->button1->ForeColor = System::Drawing::Color::Black;
               this->button1->Location = System::Drawing::Point(19, 302);
               this->button1->Name = L"button1";
               this->button1->Size = System::Drawing::Size(334, 52);
               this->button1->TabIndex = 8;
               this->button1->Text = L"Solve";
               this->button1->UseVisualStyleBackColor = false;
               this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
               // 
               // button2
               // 
               this->button2->BackColor = System::Drawing::Color::Red;
               this->button2->Font = (gcnew System::Drawing::Font(L"Bell MT", 19.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->button2->ForeColor = System::Drawing::Color::Black;
               this->button2->Location = System::Drawing::Point(19, 360);
               this->button2->Name = L"button2";
               this->button2->Size = System::Drawing::Size(334, 52);
               this->button2->TabIndex = 9;
               this->button2->Text = L"Clear";
               this->button2->UseVisualStyleBackColor = false;
               this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
               // 
               // label4
               // 
               this->label4->AutoSize = true;
               this->label4->Font = (gcnew System::Drawing::Font(L"Times New Roman", 24, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label4->ForeColor = System::Drawing::Color::Crimson;
               this->label4->Location = System::Drawing::Point(11, 577);
               this->label4->Name = L"label4";
               this->label4->Size = System::Drawing::Size(298, 46);
               this->label4->TabIndex = 10;
               this->label4->Text = L"Solution Details:";
               // 
               // textBox1
               // 
               this->textBox1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->textBox1->Location = System::Drawing::Point(19, 626);
               this->textBox1->Multiline = true;
               this->textBox1->Name = L"textBox1";
               this->textBox1->ReadOnly = true;
               this->textBox1->Size = System::Drawing::Size(302, 87);
               this->textBox1->TabIndex = 11;
               // 
               // panel1
               // 
               this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                   | System::Windows::Forms::AnchorStyles::Left)
                   | System::Windows::Forms::AnchorStyles::Right));
               this->panel1->BackColor = System::Drawing::SystemColors::ControlDark;
               this->panel1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->panel1->Location = System::Drawing::Point(370, 81);
               this->panel1->Name = L"panel1";
               this->panel1->Size = System::Drawing::Size(800, 800);
               this->panel1->TabIndex = 12;
               // 
               // button3
               // 
               this->button3->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
               this->button3->Font = (gcnew System::Drawing::Font(L"Bell MT", 24, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->button3->ForeColor = System::Drawing::Color::Green;
               this->button3->Location = System::Drawing::Point(19, 495);
               this->button3->Name = L"button3";
               this->button3->Size = System::Drawing::Size(334, 52);
               this->button3->TabIndex = 13;
               this->button3->Text = L"Previous";
               this->button3->UseVisualStyleBackColor = false;
               this->button3->Click += gcnew System::EventHandler(this, &MainForm::button3_Click);
               // 
               // button4
               // 
               this->button4->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
               this->button4->Font = (gcnew System::Drawing::Font(L"Bell MT", 24, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->button4->ForeColor = System::Drawing::Color::Green;
               this->button4->Location = System::Drawing::Point(19, 437);
               this->button4->Name = L"button4";
               this->button4->Size = System::Drawing::Size(334, 52);
               this->button4->TabIndex = 14;
               this->button4->Text = L"Next";
               this->button4->UseVisualStyleBackColor = false;
               this->button4->Click += gcnew System::EventHandler(this, &MainForm::button4_Click);
               // 
               // label6
               // 
               this->label6->AutoSize = true;
               this->label6->Font = (gcnew System::Drawing::Font(L"Times New Roman", 19.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label6->ForeColor = System::Drawing::Color::Blue;
               this->label6->Location = System::Drawing::Point(12, 249);
               this->label6->Name = L"label6";
               this->label6->Size = System::Drawing::Size(202, 38);
               this->label6->TabIndex = 16;
               this->label6->Text = L"Board Width:";
               // 
               // numericUpDown3
               // 
               this->numericUpDown3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->numericUpDown3->Location = System::Drawing::Point(248, 255);
               this->numericUpDown3->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
               this->numericUpDown3->Name = L"numericUpDown3";
               this->numericUpDown3->Size = System::Drawing::Size(105, 34);
               this->numericUpDown3->TabIndex = 17;
               this->numericUpDown3->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
               this->numericUpDown3->ValueChanged += gcnew System::EventHandler(this, &MainForm::numericUpDown3_ValueChanged);
               // 
               // label5
               // 
               this->label5->AutoSize = true;
               this->label5->Font = (gcnew System::Drawing::Font(L"Times New Roman", 25.8F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label5->Location = System::Drawing::Point(12, 9);
               this->label5->Name = L"label5";
               this->label5->Size = System::Drawing::Size(361, 49);
               this->label5->TabIndex = 18;
               this->label5->Text = L"Chess Place Solver";
               // 
               // label7
               // 
               this->label7->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                   | System::Windows::Forms::AnchorStyles::Left)
                   | System::Windows::Forms::AnchorStyles::Right));
               this->label7->AutoSize = true;
               this->label7->Font = (gcnew System::Drawing::Font(L"Times New Roman", 16.2F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label7->ForeColor = System::Drawing::Color::DarkRed;
               this->label7->Location = System::Drawing::Point(663, 28);
               this->label7->Name = L"label7";
               this->label7->Size = System::Drawing::Size(77, 32);
               this->label7->TabIndex = 19;
               this->label7->Text = L"Note:";
               // 
               // label8
               // 
               this->label8->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                   | System::Windows::Forms::AnchorStyles::Left)
                   | System::Windows::Forms::AnchorStyles::Right));
               this->label8->AutoSize = true;
               this->label8->Font = (gcnew System::Drawing::Font(L"Times New Roman", 16.2F, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label8->Location = System::Drawing::Point(746, 28);
               this->label8->Name = L"label8";
               this->label8->Size = System::Drawing::Size(384, 32);
               this->label8->TabIndex = 20;
               this->label8->Text = L"Heavy Process can kill your PC!";
               // 
               // textBox2
               // 
               this->textBox2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->textBox2->Location = System::Drawing::Point(12, 781);
               this->textBox2->Multiline = true;
               this->textBox2->Name = L"textBox2";
               this->textBox2->ReadOnly = true;
               this->textBox2->Size = System::Drawing::Size(302, 100);
               this->textBox2->TabIndex = 21;
               this->textBox2->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox2_TextChanged);
               // 
               // label9
               // 
               this->label9->AutoSize = true;
               this->label9->Font = (gcnew System::Drawing::Font(L"Times New Roman", 24, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Italic)),
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->label9->ForeColor = System::Drawing::Color::Crimson;
               this->label9->Location = System::Drawing::Point(11, 716);
               this->label9->Name = L"label9";
               this->label9->Size = System::Drawing::Size(333, 46);
               this->label9->TabIndex = 22;
               this->label9->Text = L"Solution Positions:";
               this->label9->Click += gcnew System::EventHandler(this, &MainForm::label9_Click);
               // 
               // MainForm
               // 
               this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
               this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
               this->ClientSize = System::Drawing::Size(1182, 953);
               this->Controls->Add(this->label9);
               this->Controls->Add(this->textBox2);
               this->Controls->Add(this->label8);
               this->Controls->Add(this->label7);
               this->Controls->Add(this->label5);
               this->Controls->Add(this->numericUpDown3);
               this->Controls->Add(this->label6);
               this->Controls->Add(this->button4);
               this->Controls->Add(this->button3);
               this->Controls->Add(this->panel1);
               this->Controls->Add(this->textBox1);
               this->Controls->Add(this->label4);
               this->Controls->Add(this->button2);
               this->Controls->Add(this->button1);
               this->Controls->Add(this->numericUpDown2);
               this->Controls->Add(this->label3);
               this->Controls->Add(this->numericUpDown1);
               this->Controls->Add(this->label2);
               this->Controls->Add(this->comboBox1);
               this->Controls->Add(this->label1);
               this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
               this->Name = L"MainForm";
               this->Text = L"Chess Place Solver";
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->EndInit();
               this->ResumeLayout(false);
               this->PerformLayout();

           }
#pragma endregion

    private:
        bool ValidateInput(String^& ErrorMessage);
        int CalcMaxPieces();
        String^ GetNoSolutionReason();
        void Solve(String^ PieceType, int PieceCount, int Height, int Width);
        void PlaceQueens(array<int>^ Rows, int Row, int Remaining);
        bool IsSafeQueen(array<int>^ Rows, int Row, int Col);
        void PlacePieces(array<int>^ Board, int Index, int Remaining);
        bool IsSafe(array<int>^ Board, int Row, int Col);
        void MarkAttacks(int Row, int Col, bool Value);
        void PlaceWithFC_MRV(int Remaining);
        void PlaceKing_FC_MRV(array<int>^ Board, int Remaining);
        void PlaceWithFC_MRV_Symmetry(int Remaining);
        void PlaceKnight_FC_MRV(array<int>^ Board, int Remaining);
        void GenerateSymmetric(System::Collections::Generic::List<Point>^ Solution);
        int  CountKingMoves(array<int>^ Board, int Index);
        int  CountKnightMoves(array<int>^ Board, int Index);
        void SortByMRV(System::Collections::Generic::List<int>^ Candidates, bool IsKnight);
        void UpdateSolutionDisplay();
        void UpdateProgress();
        void RunSolverAsync();
        void ShowSolverError(String^ message);
        void OnSolverFinished(Task^ task);
        void OnSolverFinishedUI();
        String^ GetColumnLabel(int ColIndex);

    private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e);
    private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e);
    private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e);
    private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e);
    private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e);
    private: System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
    private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
    private: System::Void numericUpDown1_ValueChanged(System::Object^ sender, System::EventArgs^ e);
    private: System::Void numericUpDown2_ValueChanged(System::Object^ sender, System::EventArgs^ e);
    private: System::Void numericUpDown3_ValueChanged(System::Object^ sender, System::EventArgs^ e);
    private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e);
    private: System::Void label9_Click(System::Object^ sender, System::EventArgs^ e);
};
}