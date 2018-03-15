#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <getmaterials.h>
#include <vector>
#include <string>
#include "connection.cpp"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadImage("/home/igor/MView/sphere-blue-acrylic.exr");
    populateCombo();
    ui->progressBar->hide();
    connect (ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateCombo(){
    GetMaterials getM("/home/igor/MView/Materials");
    getM.getFiles();
    QString val;
    vector<string>::iterator i;
    vector<string> items = getM.getMaterials();
    for(vector<string>::iterator i = items.begin();
             i != items.end();
             ++i) {
        val = QString::fromStdString(*i);
        ui->comboBox->addItem(val, QVariant::fromValue(val));
        ui->comboBox_2->addItem(val,QVariant::fromValue(val));
    }
}

void MainWindow::loadImage(QString path){
    QPixmap image(path);
    ui->label_3->setPixmap(image);
}

void MainWindow::pushButtonClicked(){
    Connection::renderizer(ui->comboBox->currentIndex()+1, ui->comboBox_2->currentIndex()+1);
    //Connection::makeConnection(ui->comboBox->currentText().toStdString());
    Connection::makeConnection("material_25");
    loadImage("simple.exr");
}
