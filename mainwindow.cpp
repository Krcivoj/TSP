#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cfloat>

#include<QDebug>

#include "Ant.h"
#include "City.h"

#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QProgressBar>

using namespace std;

double nearestNeighbours(vector<City> cities)
{
    City c1 = cities.back();
    City c = c1;
    cities.pop_back();
    int n = cities.size();
    double l = 0;
    double min;
    int k;
    for(int i = 0; i < n; i++)
    {
        min = __DBL_MAX__;
        for(int j = 0; j < cities.size(); j++)
        {
            if(d(c,cities[j]) < min)
            {
                min = d(c,cities[j]);
                k = j;
            }
        }
        l += min;
        c = cities[k];
        cities.erase(cities.begin() + k);
    }
    l += d(c,c1);
    return l;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle("TSP solver");
    setupSignalsAndSlots();

    ui->fileName->setVisible(false);
    ui->numberCities->setVisible(false);
    ui->bestKnown->setVisible(false);
    ui->bestLength->setVisible(false);
    ui->accuracy->setVisible(false);
    file.setFileName("");

    ui->manyTest->setDisabled(true);

    scene = new QGraphicsScene(this);

    ui->graphArea->setScene(scene);

    // anti-aliasing
    ui->graphArea->setRenderHint(QPainter::Antialiasing);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    ui->progressBar_2->setVisible(false);

    coef = ui->coeff->value();
    flag = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSignalsAndSlots()
{
    connect(ui->load, &QPushButton::clicked, this, &MainWindow::loadClicked);
    connect(ui->solve, &QPushButton::clicked, this, &MainWindow::solveClicked);
    connect(ui->manyTest, &QPushButton::clicked, this, &MainWindow::TSP_many);
    connect(this, SIGNAL(drawPath()), this, SLOT(drawingPath()));
    connect(ui->coeff, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::drawCities);
}

void MainWindow::loadClicked()
{
    QString selfilter = tr("CSV (*.csv)");

    QString fileName = QFileDialog::getOpenFileName(this, "OpenFile", ".", tr("CSV (*.csv)"), &selfilter);
    if(fileName == ""){
       //qDebug() << "Canceled.";
    }
    else{
        file.setFileName(fileName);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::about(this,"Information","It is not possible to open required file!");
        }

        int val1;
        int val2;
        QTextStream stream(&file);
        Ant::cities.clear();
        Ant::pheromone.clear();
        scene->clear();
        coef = ui->coeff->value();
        while(!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList podaci = line.split(","); //pazi je li ide ili ne razmak
            if(podaci[0] == "best") {
                ui->bestKnown->setText(podaci[1]);
            }
            else {
                val1 = podaci[0].toDouble();
                val2 = podaci[1].toDouble();
                //qDebug() << val1 << " " << val2;
                Ant::cities.push_back( City(val1, val2));                
                scene->addEllipse( (double) val1*coef-10, (double) val2*coef-10, 15, 15, QPen(Qt::green), QBrush(Qt::green));
            }
        }

        file.close();

        ui->fileName->setVisible(true);

        QStringList names =  file.fileName().split("/");
        ui->fileName->setText(names.back());
        ui->numberCities->setVisible(true);

        ui->numberCities->setText(QString::number(Ant::cities.size()));

        ui->solve->setEnabled(true);
        ui->bestKnown->setVisible(false);
        ui->iterLabel->setVisible(false);
        ui->bestLength->setVisible(false);
        ui->accuracy->setVisible(false);
        ui->manyTest->setEnabled(true);
    }
}

void MainWindow::solveClicked()
{
   flag = true;
   ui->progressBar_2->setVisible(false);
   ui->load->setEnabled(false);
   ui->solve->setEnabled(false);
   ui->solve->repaint();
   ui->manyTest->setEnabled(false);
   TSP();
   ui->solve->setEnabled(true);
   ui->load->setEnabled(true);
   ui->manyTest->setEnabled(true);
   flag = false;
}

pair<pair<double,int>, vector<City>> MainWindow::TSP()
{
    coef = ui->coeff->value();
    Ant::pheromone.clear();

    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    vector<Ant> ants;
    vector<City> bestPath;
    double bestLength = DBL_MAX;
    int bestIteration = 0;
    int max_iterations =  ui->max_iterations_param->value();
    alpha = ui->alpha_param->value();
    beta_ = ui->beta_param->value();
    Q = ui->Q_param->value();
    rho = ui->rho_param->value();

    int m = Ant::cities.size();
    double C = nearestNeighbours(Ant::cities);
    for(int i = 0; i < m; i++)
    {
        vector<double> row;
        for(int j = 0; j < i; j++)
        {
            row.push_back(Q*m/C);
        }
        Ant::pheromone.push_back(row);
    }

    for( int i = 0; i < max_iterations ; i++)
    {
        int s = (int)((i+1)*100)/max_iterations;
        ui->progressBar->setValue(s);
        qApp->processEvents();
        update();

        //qDebug() << i;
        ants.clear();
        for (auto city : Ant::cities)
            ants.push_back(Ant(city));
        for( unsigned long long j = 0; j < Ant::cities.size() - 1; j++)
        {
            for( auto& ant : ants)
            {
                ant.next();
            }
        }
        for( auto& ant : ants)
        {
            ant.updatePheromone();
            if(ant.pathLength() < bestLength)
            {
                bestLength = ant.pathLength();
                bestPath = ant.visited;
                bestIteration = i;
            }
        }
        Ant::evaporatePheromone();
        if((i % 10 == 0 || i == (max_iterations-1))){
            emit drawPath();
        }
    }

    //za sad cu tu bacit drawing bestPatha da je ne moram slati kroz emit
    coef = ui->coeff->value();
    for(auto i = bestPath.begin(); next(i, 1) != bestPath.end(); i++)
        {
            QLineF l = QLineF(i->x*coef, i->y*coef, next(i,1)->x*coef, next(i,1)->y*coef);
            QPen p = QPen(Qt::green,Qt::RoundCap);
            p.setWidthF(3);
            scene->addLine(l,p);
            scene->addEllipse(i->x*coef-10, i->y*coef-10, 15, 15,
                              QPen(Qt::green), QBrush(Qt::green));
            scene->addEllipse(next(i,1)->x*coef-10, next(i,1)->y*coef-10, 15, 15,
                              QPen(Qt::green), QBrush(Qt::green));
        }
    //zatvaram ciklus
    QLineF l = QLineF(bestPath.begin()->x*coef, bestPath.begin()->y*coef,
                      next(bestPath.end(),-1)->x*coef, next(bestPath.end(),-1)->y*coef);
    QPen p = QPen(Qt::green,Qt::RoundCap);
    p.setWidthF(3);
    scene->addLine(l,p);
    scene->update();
    scene->addEllipse(bestPath.begin()->x*coef-10, bestPath.begin()->y*coef-10, 15, 15,
                      QPen(Qt::green), QBrush(Qt::green));
    scene->addEllipse(next(bestPath.end(),-1)->x*coef-10, next(bestPath.end(),-1)->y*coef-10, 15, 15,
                      QPen(Qt::green), QBrush(Qt::green));


    ui->graphArea->repaint();
    ui->bestLength->setVisible(true);
    ui->bestLength->setText(QString::number(bestLength));
    ui->bestKnown->setVisible(true);
    ui->iterLabel->setVisible(true);
    ui->iterLabel->setText(QString::number(bestIteration));
    ui->accuracy->setVisible(true);
    float acc = (ui->bestKnown->text().toFloat() < bestLength) ?
                ui->bestKnown->text().toFloat()/bestLength : bestLength/ ui->bestKnown->text().toFloat();
    ui->accuracy->setText(QString::number(acc));
    pair<double, int> best = pair(bestLength, bestIteration);
    return pair(best, bestPath);
}

void MainWindow::TSP_many()
{
    ui->load->setEnabled(false);
    ui->solve->setEnabled(false);
    ui->manyTest->setEnabled(false);

    int N = ui->numberTest->value();

    ui->progressBar_2->setMinimum(0);
    ui->progressBar_2->setMaximum(N);
    ui->progressBar_2->setValue(0);
    ui->progressBar_2->setVisible(true);

    double bestLength = DBL_MAX;
    double sum = 0;
    vector<City> bestPath;
    QString name = file.fileName();
    name = name.split("/").back();
    QFile outFile;

    QString selfilter = tr("TXT (*.txt)");

    QString fileName = QFileDialog::getSaveFileName(this, "Save as file", ".", selfilter, &selfilter);
    if(fileName == ""){
       set();
       return;
    }
    outFile.setFileName(fileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        set();
        return;
    }

    QTextStream out(&outFile);
    out << name << '\n';
    out << "iterations: " << ui->max_iterations_param->value() <<'\n';
    out << "alpha: " << ui->alpha_param->value() << '\n';
    out << "beta: " << ui->beta_param->value() << '\n';
    out << "Q: " << ui->Q_param->value() << '\n';
    out << "rho " << ui->rho_param->value() << '\n';

    for(int i = 0; i < N; i++)
    {

        pair<pair<double,int>, vector<City>> p = TSP();
        out << p.first.first << "\t";
        out << " iteration: " << p.first.second << '\n';
        sum += p.first.first;
        if(p.first.first < bestLength)
        {
            bestLength = p.first.first;
            bestPath = p.second;
        }
        ui->progressBar_2->setValue(i+1);
    }
    out << "Best found: " << bestLength << '\n';
    out << "Average: "<< sum/N << '\n';
    out << "Best known: "<< ui->bestKnown->text();

    outFile.close();
    set();
}
void MainWindow::set(){
    ui->load->setEnabled(true);
    ui->solve->setEnabled(true);
    ui->manyTest->setEnabled(true);
}

void MainWindow::drawingPath()
{
    coef = ui->coeff->value();
    scene->clear();
    for(int j = 0; j < (int) Ant::pheromone.size(); j++)
        for(int i = j+1; i < (int) Ant::pheromone.size(); i++)
        {
            if(ui->pherChoice->checkState() == Qt::Checked)
            {
                QLineF l = QLineF(Ant::cities[i].x*coef, Ant::cities[i].y*coef, Ant::cities[j].x*coef, Ant::cities[j].y*coef);
                QPen p = QPen(Qt::red,Qt::RoundCap);
                p.setWidthF(std::min(Ant::pheromone[i][j]/10, 10.0));
                scene->addLine(l,p);
            }
            scene->addEllipse(Ant::cities[i].x*coef-10, Ant::cities[i].y*coef-10, 15, 15,
                              QPen(Qt::green), QBrush(Qt::green));
            scene->addEllipse(Ant::cities[j].x*coef-10, Ant::cities[j].y*coef-10, 15, 15,
                              QPen(Qt::green), QBrush(Qt::green));
        }

    scene->update();
    ui->graphArea->repaint();
    qApp->processEvents();
    update();
}

void MainWindow::drawCities()
{
    if(flag) return;
    scene->clear();
    coef = ui->coeff->value();
    for(auto city : Ant::cities)
    {
        scene->addEllipse(city.x*coef-10, city.y*coef-10, 15, 15,
                          QPen(Qt::green), QBrush(Qt::green));
    }
    scene->update();
    ui->graphArea->repaint();
    qApp->processEvents();
    update();
}

