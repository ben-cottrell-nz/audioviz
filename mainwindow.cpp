#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mPlotWidget(this)
{
    ui->setupUi(this);
//    ui->verticalLayout->addWidget(&mPlotWidget, 1);
    setCentralWidget(&mPlotWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

