#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <iostream>
#include <QIntValidator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _currentStep(_steps.begin())
    , _isSudokuSolved(false)
{
    ui->setupUi(this);
    this->resize(600, 600);

    this->setWindowTitle("Sudoku Resolver");
    initSudokuGrid();
    setDarkStyle();

    connect(ui->ResetLabel, &QPushButton::clicked, this, &MainWindow::resetSudokuGrid);
    connect(ui->solveButton, &QPushButton::clicked, this, &MainWindow::solveSudoku);

    // Usa una lambda function per la connessione, così possiamo aggiungere un messaggio di debug
    connect(ui->PreviousButton, &QPushButton::clicked, this, &MainWindow::previousStep);
    connect(ui->NextButton, &QPushButton::clicked, this, &MainWindow::nextStep);

    connect(ui->beginStep, &QPushButton::clicked, this, &MainWindow::beginStep);
    connect(ui->lastStep, &QPushButton::clicked, this, &MainWindow::lastStep);


    updateStepLabel();
    updateButtonStates();

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateMatrix(int row, int col, const QString &text) {

    if (text.isEmpty()) {
        // Se il QLineEdit è vuoto, imposta la matrice a 0
        _sudoku[row][col] = 0;

        // Azzera il testo del QLineEdit
        QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(row).arg(col));
        if (lineEdit) {
            lineEdit->setText("");
        }

        // Aggiorna il contenuto dei QLineEdit con la nuova matrice
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                QString text;
                if (_sudoku[r][c] != 0) {
                    text = QString::number(_sudoku[r][c]);
                }

                QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(r).arg(c));
                if (lineEdit) {
                    lineEdit->setText(text);
                }
            }
        }
    } else {
        // Altrimenti, converti il testo a un valore intero
        unsigned int value = text.toUInt();

        if (isCellValid(row, col, value)) {
            _sudoku[row][col] = value;

            ui->dialog->setText("");
            ui->dialog->setStyleSheet("");

            // Aggiorna il contenuto dei QLineEdit con la nuova matrice
            for (int r = 0; r < 9; ++r) {
                for (int c = 0; c < 9; ++c) {
                    QString text;
                    if (_sudoku[r][c] != 0) {
                        text = QString::number(_sudoku[r][c]);
                    }

                    QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(r).arg(c));
                    if (lineEdit) {
                        lineEdit->setText(text);
                    }
                }
            }
        } else {
            ui->dialog->setText("Warning: The number you are trying to enter cannot be placed in this position.");
            ui->dialog->setStyleSheet("QLabel { color : red; }");

            QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(row).arg(col));

            const QString emptyString = "";

            if (lineEdit) {
                lineEdit->setText(emptyString);
            }

            updateMatrix(row, col, emptyString);
        }
    }

    updateButtonStates();
    printMatrix();
}

void MainWindow::resetSudokuGrid() {
    // Resetta la matrice _sudoku a tutti zeri
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            _sudoku[row][col] = 0;
        }
    }

    // Resetta i QLineEdit
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(row).arg(col));
            if (lineEdit) {
                lineEdit->setReadOnly(false);
                lineEdit->setText("");
            }
        }
    }

    // Resetta le mosse
    _steps.clear();
    _currentStep = _steps.begin();
    updateStepLabel();

    // Disabilita il pulsante solveButton dopo il reset
    ui->solveButton->setEnabled(false);

    // Resetta il messaggio di dialogo
    ui->dialog->setText("");
    ui->dialog->setStyleSheet("");

    _isSudokuSolved = false;
    updateButtonStates();
}


void MainWindow::printMatrix() {
    std::cout << "Matrice sudoku aggiornata:" << std::endl;
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            std::cout << _sudoku[row][col] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void MainWindow::initSudokuGrid() {
    QIntValidator *validator = new QIntValidator(0, 9, this);
    QFont font("Arial", 18);

    // Impostazioni di stile base per i QLineEdit
    QString lineEditStyle = "QLineEdit {"
                            "font-size: 18px;"
                            "font-family: Arial;"
                            "text-align: center;"
                            "border: .5px solid black;"
                            "}";

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            _sudoku[row][col] = 0;

            QLineEdit *lineEdit = new QLineEdit(this);
            lineEdit->setObjectName(QString("lineEdit_%1_%2").arg(row).arg(col));
            lineEdit->setValidator(validator);
            lineEdit->setFont(font);

            // Applica lo stile base dei QLineEdit
            lineEdit->setStyleSheet(lineEditStyle);

            // Imposta l'allineamento del testo al centro
            lineEdit->setAlignment(Qt::AlignCenter);

            // Imposta la politica di dimensionamento per riempire lo spazio disponibile
            lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            // Aggiungi il widget al layout della griglia
            ui->sudokuGrid->addWidget(lineEdit, row, col);

            // Connessione del segnale textChanged per aggiornare la matrice
            connect(lineEdit, &QLineEdit::textChanged, this, [this, row, col](const QString &text) {
                updateMatrix(row, col, text);
            });

            // Disabilita il pulsante solveButton inizialmente
            ui->solveButton->setEnabled(false);
        }
    }
}



bool MainWindow::isCellValid(int row, int col, unsigned int num) {
    // Controllo della riga
    for (int c = 0; c < 9; ++c) {
        if (c != col && _sudoku[row][c] == num) {
            return false; // Il numero è già presente nella riga
        }
    }

    // Controllo della colonna
    for (int r = 0; r < 9; ++r) {
        if (r != row && _sudoku[r][col] == num) {
            return false; // Il numero è già presente nella colonna
        }
    }

    // Controllo del quadrato 3x3
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int r = startRow; r < startRow + 3; ++r) {
        for (int c = startCol; c < startCol + 3; ++c) {
            if (r != row && c != col && _sudoku[r][c] == num) {
                return false; // Il numero è già presente nel quadrato 3x3
            }
        }
    }

    return true; // Il numero può essere inserito
}


void MainWindow::updateButtonStates() {
    unsigned int countHints = 0;

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (_sudoku[row][col] != 0) {
                ++countHints;
            }
        }
    }

    bool shouldEnableSolve = countHints > 0 && !_isSudokuSolved;
    bool shouldEnableReset = countHints > 0;
    ui->solveButton->setEnabled(shouldEnableSolve);
    ui->ResetLabel->setEnabled(shouldEnableReset);

    // Stile per i pulsanti
    QString activeButtonStyle = "QPushButton { "
                                "   background-color: #4267B2; "
                                "   color: #FFFFFF; "
                                "   border: none; "
                                "   padding: 6px; "
                                "   font-weight: bold; "
                                "}"
                                "QPushButton:hover { background-color: #365899; }"
                                "QPushButton:pressed { background-color: #29487d; }";

    QString inactiveButtonStyle = "QPushButton { "
                                  "   background-color: #99A8BF; "
                                  "   color: #FFFFFF; "
                                  "   border: none; "
                                  "   padding: 6px; "
                                  "   font-weight: bold; "
                                  "}";

    ui->solveButton->setStyleSheet(shouldEnableSolve ? activeButtonStyle : inactiveButtonStyle);
    ui->ResetLabel->setStyleSheet(shouldEnableReset ? activeButtonStyle : inactiveButtonStyle);

    // Aggiorna lo stato e lo stile dei pulsanti Previous e Next
    bool shouldEnableNavButtons = _isSudokuSolved && !_steps.isEmpty();
    ui->PreviousButton->setEnabled(shouldEnableNavButtons && _currentStep != _steps.begin());
    ui->NextButton->setEnabled(shouldEnableNavButtons && _currentStep != _steps.end());
    ui->beginStep->setEnabled(shouldEnableNavButtons);
    ui->lastStep->setEnabled(shouldEnableNavButtons);

    ui->PreviousButton->setStyleSheet(shouldEnableNavButtons && _currentStep != _steps.begin() ? activeButtonStyle : inactiveButtonStyle);
    ui->NextButton->setStyleSheet(shouldEnableNavButtons && _currentStep != _steps.end() ? activeButtonStyle : inactiveButtonStyle);
    ui->beginStep->setStyleSheet(shouldEnableNavButtons ? activeButtonStyle : inactiveButtonStyle);
    ui->lastStep->setStyleSheet(shouldEnableNavButtons ? activeButtonStyle : inactiveButtonStyle);



    if (countHints > 0 && !_isSudokuSolved) {
        if (countHints < 17 && ui->dialog->text().isEmpty()) {
            ui->dialog->setText("Hint: Inserting 17 numbers ensures an existing and unique solution.");
            ui->dialog->setStyleSheet("QLabel { color : green; }");
        }
    } else {
        ui->dialog->setText("");
        ui->dialog->setStyleSheet("");
    }
}

bool MainWindow::findEmptyCell(unsigned int &row, unsigned int &col) {

    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            if (_sudoku[row][col] == 0) {
                return true;
            }
        }
    }

    return false;
}


bool MainWindow::solveSudoku() {
    unsigned int row = 0;
    unsigned int col = 0;

    if (!findEmptyCell(row, col)) {
        injectMatrixInUI();
        printSteps();  // Funzione separata per stampare gli step
        _currentStep = _steps.end();
        updateStepLabel();

        _isSudokuSolved = true;
        updateButtonStates();

        // Blocca l'editing di tutte le celle
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(r).arg(c));
                if (lineEdit) {
                    lineEdit->setReadOnly(true);
                }
            }
        }

        ui->dialog->setText("Sudoku solved successfully!");
        ui->dialog->setStyleSheet("QLabel { color : green; }");

        return true;  // Sudoku risolto
    }

    for (unsigned int num = 1; num <= 9; ++num) {
        if (isCellValid(row, col, num)) {
            _sudoku[row][col] = num;
            _steps.append(Step(row, col, num));  // Aggiungi lo step qui

            if (solveSudoku()) {
                return true;
            }

            // Se la soluzione non porta a una soluzione valida, backtrack
            _sudoku[row][col] = 0;
            _steps.removeLast();  // Rimuovi l'ultimo step in caso di backtrack
        }
    }

    ui->dialog->setText("The given Sudoku is unsolvable.");
    ui->dialog->setStyleSheet("QLabel { color : red; }");

    return false; // Nessuna soluzione trovata
}

void MainWindow::printSteps() {
    qDebug() << "Steps:";
    for (QVector<Step>::iterator it = _steps.begin(); it != _steps.end(); ++it) {
        const Step& step = *it;
        qDebug() << "Riga:" << step.x << "Colonna:" << step.y << "Valore:" << step.value;
    }
}

void MainWindow::injectMatrixInUI() {
    for (unsigned int row = 0; row < 9; ++row) {
        for (unsigned int col = 0; col < 9; ++col) {
            QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(row).arg(col));
            if (lineEdit) {
                if (_sudoku[row][col] != 0) {
                    lineEdit->setText(QString::number(_sudoku[row][col]));
                } else {
                    lineEdit->setText("");
                }
            }
        }
    }
}


void MainWindow::previousStep() {

    if (_currentStep != _steps.begin() ) {
        --_currentStep;
        removeStepFromMatrix(*_currentStep);
        injectMatrixInUI();
        updateStepLabel();
    }

}

void MainWindow::nextStep() {

    if (_currentStep != _steps.end()) {
        addStepToMatrix(*_currentStep);
        ++_currentStep;
        injectMatrixInUI();
        updateStepLabel();
    }
}

void MainWindow::removeStepFromMatrix(const Step& step) {
    _sudoku[step.x][step.y] = 0;
    printMatrix();
}

void MainWindow::addStepToMatrix(const Step& step) {
    _sudoku[step.x][step.y] = step.value;
    printMatrix();
}

void MainWindow::testButton() {
    std::cout << "Ciao!" << std::endl;
    qDebug() << "Ciao!";  // Questo apparirà anche nella console di debug di Qt
}


void MainWindow::updateStepLabel() {
    if (_steps.isEmpty()) {
        ui->StepLabel->setText("");
    } else {
        int currentStep = (_currentStep == _steps.end()) ? _steps.size() : (_currentStep - _steps.begin() + 1);
        int totalSteps = _steps.size();
        ui->StepLabel->setText(QString("Step: %1/%2").arg(currentStep).arg(totalSteps));
    }
}

void MainWindow::beginStep() {
    if (!_steps.isEmpty()) {
        _currentStep = _steps.begin();
        updateMatrixToStep(_currentStep);
        updateStepLabel();
        updateButtonStates();
    }
}

void MainWindow::lastStep() {
    if (!_steps.isEmpty()) {
        _currentStep = _steps.end() - 1;
        updateMatrixToStep(_currentStep);
        updateStepLabel();
        _currentStep = _steps.end();
        updateButtonStates();
    }
}


void MainWindow::updateMatrixToStep(QVector<Step>::const_iterator stepIt) {
    // Ripristina la matrice allo stato iniziale (quello inserito dall'utente)
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(r).arg(c));
            if (lineEdit) {
                QString text = lineEdit->text();
                _sudoku[r][c] = text.isEmpty() ? 0 : text.toUInt();
            } else {
                // Se per qualche motivo non troviamo il QLineEdit, impostiamo la cella a 0
                _sudoku[r][c] = 0;
            }
        }
    }

    // Applica tutti gli step fino a quello corrente
    for (auto it = _steps.begin(); it <= stepIt; ++it) {
        _sudoku[it->x][it->y] = it->value;
    }

    // per il primo numero (bug)
    if( _steps.begin() == stepIt ) {
        _sudoku[_steps.begin()->x][_steps.begin()->y] = 0;
    }

    // Se stepIt non è alla fine, azzera le celle successive
    if (stepIt != _steps.end()) {
        auto nextIt = stepIt + 1;
        for (; nextIt != _steps.end(); ++nextIt) {
            _sudoku[nextIt->x][nextIt->y] = 0;
        }
    }

    injectMatrixInUI();
}

void MainWindow::setDarkStyle()
{
    // Palette di colori
    QString mainBlue = "#4267B2";  // Colore blu principale
    QString lightBlue = "#E7F3FF";  // Azzurro chiaro per i QLineEdit
    QString white = "#FFFFFF";
    QString darkText = "#1C1E21";

    // Stile generale per i pulsanti
    QString buttonStyle = QString(
                              "QPushButton { "
                              "   background-color: %1; "
                              "   color: %2; "
                              "   border: none; "
                              "   padding: 6px; "
                              "   font-weight: bold; "
                              "}"
                              "QPushButton:hover { background-color: #365899; }"
                              "QPushButton:pressed { background-color: #29487d; }"
                              ).arg(mainBlue, white);

    // Applica lo stile ai pulsanti
    ui->PreviousButton->setStyleSheet(buttonStyle);
    ui->NextButton->setStyleSheet(buttonStyle);
    ui->solveButton->setStyleSheet(buttonStyle);
    ui->ResetLabel->setStyleSheet(buttonStyle);

    // Stile per la griglia del Sudoku
    QString lineEditStyleLight = QString(
                                     "QLineEdit {"
                                     "   font-size: 25px; "
                                     "   font-family: Arial; "
                                     "   text-align: center; "
                                     "   background-color: %1; "
                                     "   color: %2; "
                                     "   border: 1px solid #dddfe2; "
                                     "}"
                                     ).arg(lightBlue, mainBlue);

    QString lineEditStyleWhite = QString(
                                     "QLineEdit {"
                                     "   font-size: 25px; "
                                     "   font-family: Arial; "
                                     "   text-align: center; "
                                     "   background-color: %1; "
                                     "   color: %2; "
                                     "   border: 1px solid #dddfe2; "
                                     "}"
                                     ).arg(white, mainBlue);

    // Applica lo stile alternato ai QLineEdit della griglia
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QLineEdit *lineEdit = findChild<QLineEdit*>(QString("lineEdit_%1_%2").arg(row).arg(col));
            if (lineEdit) {
                if ((row / 3 + col / 3) % 2 == 0) {
                    lineEdit->setStyleSheet(lineEditStyleLight);
                } else {
                    lineEdit->setStyleSheet(lineEditStyleWhite);
                }
            }
        }
    }

    // Stile per il dialog label
    ui->dialog->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; }").arg(mainBlue));

    // Stile per il label degli step
    ui->StepLabel->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; }").arg(mainBlue));

}
