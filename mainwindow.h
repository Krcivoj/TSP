#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QProgressBar>

#include <City.h>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    void setupSignalsAndSlots();

private slots:
    void loadClicked();
    void solveClicked();
    void drawingPath();

signals:
    void drawPath();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFile file;
    QGraphicsScene *scene;

    std::pair<double, std::vector<City>> TSP();
    void TSP_many();
};
#endif // MAINWINDOW_H
