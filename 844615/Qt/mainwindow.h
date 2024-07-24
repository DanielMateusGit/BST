#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void previousStep();
    void nextStep();

private slots:
    void printMatrix();
    void updateMatrix(int row, int col, const QString &text);
    void testButton();
    void beginStep();
    void lastStep();

private:

    Ui::MainWindow *ui;

    struct Step {
        unsigned int x;
        unsigned int y;
        unsigned int value;

        Step(unsigned int x, unsigned int y, unsigned int value) : x(x), y(y), value(value) {}
    };

    QVector<Step> _steps;
    QVector<Step>::const_iterator _currentStep;
    unsigned int _sudoku[9][9];
    bool _isSudokuSolved;

    // Questa inizializza il sudoku
    void initSudokuGrid();

    // Questa controlla che una cella segua le regole del sudoku:
    bool isCellValid(int row, int col, unsigned int num);

    // Questa controlla quando abilitare / disabilitare il pulsante di solve
    void updateButtonStates();

    // resetta la tabella
    void resetSudokuGrid();


    // Questo restituisce un booleano, e condividendo memoria con il chiamante
    // Inoltre => modifica con gli operatori con ++ i valori di row e col
    bool findEmptyCell(unsigned int &row, unsigned int &col);

    // risolve il sudoku
    bool solveSudoku();

    // inietta lo stato della matrice dentro la UI
    void injectMatrixInUI();

    // Gestione di previous e next
    void removeStepFromMatrix(const Step& step);
    void addStepToMatrix(const Step& step);

    // Scritta con le mosse correnti
    void updateStepLabel();

    void updateMatrixToStep(QVector<Step>::const_iterator stepIt);

    void printSteps() ;

    void setDarkStyle();
};
#endif // MAINWINDOW_H
