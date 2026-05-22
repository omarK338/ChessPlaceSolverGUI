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
        CancellationTokenSource^ cancelSource;
        // Variables
        int boardHeight;
        int boardWidth;
        int pieceCount;
        int currentSolutionIndex = 0;
        String^ pieceType;
        // Store solutions as list of piece Positions (row, Col) to save memory
        System::Collections::Generic::List<System::Collections::Generic::List<Point>^>^ solutions =
            gcnew System::Collections::Generic::List<System::Collections::Generic::List<Point>^>();
        // Fonts for drawing pieces and labels
        System::Drawing::Font^ font;
        System::Drawing::Font^ LabelFont;

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
            this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::panel1_Paint);
            this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
            // Initialize fonts
            font = gcnew System::Drawing::Font(L"Times New Roman", 12, FontStyle::Bold);
            LabelFont = gcnew System::Drawing::Font(L"Times New Roman", 10);
        }

    protected:
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
            if (cancelSource)
            {
                cancelSource = nullptr;
            }
            if (font)
            {
                delete font;
                font = nullptr;
            }
            if (LabelFont)
            {
                delete LabelFont;
                LabelFont = nullptr;
            }
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
               this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
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
               this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
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
               this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
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

    private: bool CheckSystemCapability(long long estimatedOps, String^& reason) 
    {
        int cpuCores = Environment::ProcessorCount;
        unsigned long long ramInMB = 0;
        try 
        {
            ManagementObjectSearcher^ searcher = gcnew ManagementObjectSearcher("SELECT Capacity FROM Win32_PhysicalMemory");
            for each (ManagementObject ^ obj in searcher->Get())
                ramInMB += Convert::ToUInt64(obj["Capacity"]) / (1024 * 1024);
        }
        catch (Exception^ ex) 
        {
            ramInMB = 4096; // Fallback to 4 GB if retrieval fails
            System::IO::File::AppendAllText("debug_log.txt", String::Format("{0}: RAM retrieval failed - {1}\n", DateTime::Now, ex->Message));
        }

        // Improved operation estimation: combinatorial approach with logarithmic cap
        long long spaces = (long long)boardHeight * boardWidth;
        long long maxOps = 1e18; // Cap to avoid overflow
        long long cappedSpaces = (long long)Math::Min((double)spaces, 10000.0);
        long long cappedPieceCount = (long long)Math::Min((double)pieceCount, 1000.0);
        if (cappedPieceCount > 0 && cappedSpaces > 0) 
        {
            double logBase = Math::Log((double)cappedSpaces);
            double tempSpaces = (double)spaces;
            double tempPieceCount = (double)pieceCount;
            long long cappedSpaces = (long long)(tempSpaces < 10000.0 ? tempSpaces : 10000.0);
            long long cappedPieceCount = (long long)(tempPieceCount < 1000.0 ? tempPieceCount : 1000.0);

            if (cappedPieceCount > 0 && cappedSpaces > 0) 
            {
                double logBase = Math::Log((double)cappedSpaces);
                double innerMin = (double)cappedPieceCount < 100.0 ?
                    (double)cappedPieceCount : 100.0;
                double tempResult = Math::Round(Math::Exp(logBase * innerMin));
                if (tempResult > (double)maxOps)
                    estimatedOps = maxOps;
                else
                    estimatedOps = (long long)tempResult;
            }
            else 
            {
                estimatedOps = 1;
            }


        }
        else 
        {
            estimatedOps = 1;
        }

        // Refined system requirements based on operations
        String^ reqMessage = "";
        if (estimatedOps > 1e15) 
        {
            reqMessage = "Recommended: 64 GB RAM, 16-core CPU (Infeasible, consider reducing input)";
        }
        else if (estimatedOps > 1e12) 
        {
            reqMessage = "Recommended: 32 GB RAM, 8-core CPU";
        }
        else if (estimatedOps > 1e9) 
        {
            reqMessage = "Recommended: 16 GB RAM, 4-core CPU";
        }
        else 
        {
            reqMessage = "Recommended: 8 GB RAM, 2-core CPU";
        }

        // Dynamic RAM requirement: base + solution storage
        long long ramRequiredMB = ((long long)boardHeight * boardWidth * sizeof(int) / (1024 * 1024)) + // Board array
            ((long long)pieceCount * solutions->Count * sizeof(Point) / (1024 * 1024)) + 512; // Solutions + buffer
        bool enoughRam = ramInMB > ramRequiredMB;

        // CPU budget: 2 billion operations per core (updated estimate)
        long long cpuBudget = (long long)cpuCores * 2'000'000 * 1000;

        /*
        // Debug output
        String^ debugMessage = String::Format(
            "Debug:\nEstimated Ops: {0:N0}\nCPU Cores: {1}\nCPU Budget: {2:N0}\nRAM: {3:N0} MB\nRequired RAM: {4:N0} MB\nEnough RAM: {5}\n",
            estimatedOps, cpuCores, cpuBudget, ramInMB, ramRequiredMB, enoughRam);
        MessageBox::Show(debugMessage, "Debug Info");
        */
        if (estimatedOps <= cpuBudget && enoughRam)
            return true;

        reason = String::Format(
            "{0}\n\nEstimated Ops: {1:N0}\n"
            "CPU Cores: {2}\n"
            "RAM: {3:N0} MB (Requires ~{4:N0} MB)\n"
            "Expected Time: ~{5:F2} sec\n"
            "Suggestion: Reduce board size or piece count if possible.",
            reqMessage, estimatedOps, cpuCores, ramInMB, ramRequiredMB, (double)estimatedOps / (cpuCores * 2'000'000));
        return false;
    }

    private: bool ValidateInput(String^& errorMessage) 
    {
        if (comboBox1->SelectedItem == nullptr) 
        {
            errorMessage = "Please select a piece type.";
            return false;
        }
        if (boardHeight <= 0 || boardWidth <= 0 || pieceCount <= 0) 
        {
            errorMessage = "Board dimensions and piece count must be greater than 0.";
            return false;
        }
        if (boardHeight > 10000 || boardWidth > 10000 || pieceCount > 10000) 
        {
            errorMessage = "Board dimensions and piece count cannot exceed 10,000.";
            return false;
        }
        if (pieceCount > boardHeight * boardWidth) 
        {
            errorMessage = "Piece count cannot exceed board size.";
            return false;
        }
        if (pieceType == "Queen" && pieceCount > Math::Min(boardHeight, boardWidth)) 
        {
            errorMessage = "Too many Queens for the board size.";
            return false;
        }
        return true;
    }

    private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
    {
        boardHeight = (int)numericUpDown2->Value;
        boardWidth = (int)numericUpDown3->Value;
        pieceCount = (int)numericUpDown1->Value;

        String^ errorMessage;
        if (!ValidateInput(errorMessage)) 
        {
            MessageBox::Show(errorMessage, "Invalid Input", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        long long estimatedOps = 1;
        int spaces = boardHeight * boardWidth;
        for (int i = 0; i < pieceCount; ++i)
            estimatedOps *= (spaces - i);

        String^ reason;
        if (!CheckSystemCapability(estimatedOps, reason)) 
        {
            auto result = MessageBox::Show(
                "Estimated operations: " + estimatedOps.ToString() +
                "\n\nWarning: " + reason +
                "\n\nDo you want to continue?",
                "⚠️ Heavy Process Warning",
                MessageBoxButtons::YesNo,
                MessageBoxIcon::Warning);
            if (result == Windows::Forms::DialogResult::No)
                return;
        }

        solutions->Clear();
        currentSolutionIndex = 0;
        panel1->Invalidate();
        pieceType = comboBox1->SelectedItem->ToString();
        button1->Enabled = false;
        button3->Enabled = false;
        button4->Enabled = false;
        textBox1->Text = "Solving...";
        textBox2->Text = "";

        cancelSource = gcnew CancellationTokenSource();
        Task::Run(gcnew Action(this, &MainForm::RunSolverAsync), cancelSource->Token);
    }

    private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (cancelSource != nullptr) 
        {
            cancelSource->Cancel();
            cancelSource = nullptr;
        }
        solutions->Clear();
        currentSolutionIndex = 0;
        textBox1->Clear();
        textBox2->Clear();
        panel1->Invalidate();
        button1->Enabled = true;
        button3->Enabled = true;
        button4->Enabled = true;
    }

    private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (cancelSource != nullptr && !cancelSource->IsCancellationRequested)
            return; // Don't change solution while solving
        if (solutions->Count == 0)
            return;
        currentSolutionIndex = (currentSolutionIndex - 1 + solutions->Count) % solutions->Count;
        panel1->Invalidate();
        UpdateSolutionDisplay();
    }

    private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e)
    {
        if (cancelSource != nullptr && !cancelSource->IsCancellationRequested)
            return; // Don't change solution while solving
        if (solutions->Count == 0)
            return;
        currentSolutionIndex = (currentSolutionIndex + 1) % solutions->Count;
        panel1->Invalidate();
        UpdateSolutionDisplay();
    }

    private: void Solve(String^ pieceType, int pieceCount, int height, int width)
    {
        this->pieceType = pieceType;
        this->pieceCount = pieceCount;
        this->boardHeight = height;
        this->boardWidth = width;
        RunSolver();
    }

    private: void RunSolver()
    {
        try 
        {
            if (pieceType == "Queen") 
            {
                array<int>^ board = gcnew array<int>(boardHeight * boardWidth);
                PlacePices(board, 0, pieceCount);
            }
            else 
            {
                array<int>^ board = gcnew array<int>(boardHeight * boardWidth);
                PlacePieces(board, 0, pieceCount);
            }
        }
        catch (OperationCanceledException^) 
        {
            // Handled by cancellation token
        }
    }

    private: void PlacePices(array<int>^ board, int row, int remaining)
    {
        if (cancelSource != nullptr && cancelSource->IsCancellationRequested)
            return;
        if (remaining == 0) {
            // Save solution as list of piece Positions
            auto solution = gcnew System::Collections::Generic::List<Point>();
            for (int i = 0; i < boardHeight * boardWidth; i++) 
            {
                if (board[i] == 1) 
                {
                    int r = i / boardWidth;
                    int c = i % boardWidth;
                    solution->Add(Point(r, c));
                }
            }
            solutions->Add(solution);
            return;
        }
        if (row >= boardHeight)
            return;

        for (int Col = 0; Col < boardWidth; Col++) 
        {
            if (IsSafe(board, row, Col)) 
            {
                board[row * boardWidth + Col] = 1;
                PlacePices(board, row + 1, remaining - 1);
                board[row * boardWidth + Col] = 0;
            }
        }
    }

    private: void PlacePieces(array<int>^ board, int index, int remaining)
    {
        if (cancelSource != nullptr && cancelSource->IsCancellationRequested)
            return;
        if (remaining == 0) 
        {
            // Save solution as list of piece Positions
            auto solution = gcnew System::Collections::Generic::List<Point>();
            for (int i = 0; i < boardHeight * boardWidth; i++) 
            {
                if (board[i] == 1) 
                {
                    int r = i / boardWidth;
                    int c = i % boardWidth;
                    solution->Add(Point(r, c));
                }
            }
            solutions->Add(solution);
            return;
        }
        if (index >= boardHeight * boardWidth)
            return;

        int row = index / boardWidth;
        int Col = index % boardWidth;

        if (IsSafe(board, row, Col)) 
        {
            board[index] = 1;
            PlacePieces(board, index + 1, remaining - 1);
            board[index] = 0;
        }
        PlacePieces(board, index + 1, remaining);
    }

    private: bool IsSafe(array<int>^ board, int row, int Col)
    {
        int index = row * boardWidth + Col;
        for (int i = 0; i < boardHeight * boardWidth; i++) 
        {
            if (board[i] == 1) 
            {
                int r = i / boardWidth;
                int c = i % boardWidth;
                if (pieceType == "King") 
                {
                    if (Math::Abs(row - r) <= 1 && Math::Abs(Col - c) <= 1)
                        return false;
                }
                else if (pieceType == "Rook") 
                {
                    if (row == r || Col == c)
                        return false;
                }
                else if (pieceType == "Bishop") 
                {
                    if (Math::Abs(row - r) == Math::Abs(Col - c))
                        return false;
                }
                else if (pieceType == "Queen") 
                {
                    if (row == r || Col == c || Math::Abs(row - r) == Math::Abs(Col - c))
                        return false;
                }
                else if (pieceType == "Knight") 
                {
                    int dr = Math::Abs(row - r);
                    int dc = Math::Abs(Col - c);
                    if ((dr == 2 && dc == 1) || (dr == 1 && dc == 2))
                        return false;
                }
            }
        }
        return true;
    }

    private: void UpdateSolutionDisplay()
    {
        if (solutions->Count == 0) 
        {
            textBox1->Text = "No solutions found.";
            textBox2->Text = "";
            return;
        }
        textBox1->Text = String::Format("Solution {0} of {1}", currentSolutionIndex + 1, solutions->Count);
        String^ Positions = "";
        for each (Point p in solutions[currentSolutionIndex]) 
        {
            Positions += String::Format("({0}, {1}) \n", p.X + 1, p.Y + 1);
        }
        textBox2->Text = Positions;
    }

    private: void RunSolverAsync()
    {
        try 
        {
            RunSolver();
            this->Invoke(gcnew Action(this, &MainForm::OnSolverFinished));
        }
        catch (OperationCanceledException^) 
        {
            // Cancellation handle
        }
        catch (OutOfMemoryException^ ex) {
            this->Invoke(gcnew Action<String^>(this, &MainForm::ShowSolverError), "Out of memory: " + ex->Message);
        }
        catch (Exception^ ex) {
            this->Invoke(gcnew Action<String^>(this, &MainForm::ShowSolverError), ex->Message);
        }
        finally 
        {
            if (cancelSource != nullptr) 
            {
                cancelSource = nullptr;
            }
        }
    }

    private: void ShowSolverError(String^ message)
    {
        MessageBox::Show("Error while solving: " + message, "Solver Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        button1->Enabled = true;
        button3->Enabled = true;
        button4->Enabled = true;
        textBox1->Text = "Solver failed.";
        textBox2->Text = "";
    }

    private: void OnSolverFinished()
    {
        button1->Enabled = true;
        button3->Enabled = true;
        button4->Enabled = true;
        if (solutions->Count > 0) 
        {
            currentSolutionIndex = 0;
            UpdateSolutionDisplay();
            panel1->Invalidate();
        }
        else 
        {
            textBox1->Text = "No solutions found.";
            textBox2->Text = "";
        }
    }

    private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) 
    {
        comboBox1->Items->AddRange(gcnew cli::array<Object^>{"Queen", "King", "Rook", "Bishop", "Knight"});
        comboBox1->SelectedIndex = 0;
        numericUpDown1->Value = 8;
        numericUpDown2->Value = 8;
        numericUpDown3->Value = 8;
    }

    private: System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
    {
        if (solutions->Count == 0 || currentSolutionIndex >= solutions->Count)
            return;

        Graphics^ g = e->Graphics;
        Pen^ pen = gcnew Pen(Color::Black, 2);
        Brush^ WhiteBrush = Brushes::White;
        Brush^ BlackBrush = Brushes::Gray;
        Brush^ pieceBrush = Brushes::Red;

        int cellSize = Math::Min(panel1->Width / (boardWidth + 1), panel1->Height / (boardHeight + 1));
        int offsetX = (panel1->Width - boardWidth * cellSize) / 2;
        int offsetY = (panel1->Height - boardHeight * cellSize) / 2;

        // Draw row labels (1 to 8 from bottom to top)
        for (int i = 0; i < boardHeight; i++) 
        {
            g->DrawString((boardHeight - i).ToString(), LabelFont, Brushes::Black, offsetX - 20, offsetY + (boardHeight - 1 - i) * cellSize + cellSize / 2 - 5);
        }
        // Draw Column labels (A to H from left to right)
        for (int j = 0; j < boardWidth; j++) 
        {
            String^ ColLabel = String::Format("{0}", (char)('A' + j));
            g->DrawString(ColLabel, LabelFont, Brushes::Black, offsetX + j * cellSize + cellSize / 2 - 5, offsetY - 20);
        }

        // Draw board
        for (int i = 0; i < boardHeight; i++) 
        {
            for (int j = 0; j < boardWidth; j++) 
            {
                int x = offsetX + j * cellSize;
                int y = offsetY + (boardHeight - 1 - i) * cellSize; // Flip y-axis for correct orientation
                bool IsWhite = (i + j) % 2 == 0;
                g->FillRectangle(IsWhite ? WhiteBrush : BlackBrush, x, y, cellSize, cellSize);
                g->DrawRectangle(pen, x, y, cellSize, cellSize);
            }
        }

        // Draw red dots for pieces (75% of cellSize)
        int ballSize = (int)(cellSize * 0.75);
        int ballOffset = ballSize / 2;
        for each (Point p in solutions[currentSolutionIndex]) 
        {
            int x = offsetX + p.Y * cellSize + cellSize / 2;
            int y = offsetY + (boardHeight - 1 - p.X) * cellSize + cellSize / 2; // Flip y-axis
            g->FillEllipse(pieceBrush, x - ballOffset, y - ballOffset, ballSize, ballSize); // Draw larger red circle
        }
    }

    private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
    {
        if (comboBox1->SelectedItem != nullptr) {
            pieceType = comboBox1->SelectedItem->ToString();
        }
    }

    private: System::Void numericUpDown1_ValueChanged(System::Object^ sender, System::EventArgs^ e) 
    {
        pieceCount = (int)numericUpDown1->Value;
    }

    private: System::Void numericUpDown2_ValueChanged(System::Object^ sender, System::EventArgs^ e) 
    {
        boardHeight = (int)numericUpDown2->Value;
    }

    private: System::Void numericUpDown3_ValueChanged(System::Object^ sender, System::EventArgs^ e) 
    {
        boardWidth = (int)numericUpDown3->Value;
    }

    private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {}
    private: System::Void label9_Click(System::Object^ sender, System::EventArgs^ e) {}
};
}