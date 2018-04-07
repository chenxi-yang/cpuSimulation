#include"mainwindow.h"
#include"ui_mainwindow.h"
#include<QFile>
#include<QFileDialog>
#include<QDebug>
#include<QMenuBar>
#include<QMessageBox>
#include<QStatusBar>
#include<QToolBar>
#include<QDebug>
#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<fstream>
#include<map>
#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include"pipeline.h"
#include "mainwindow.h"
#include <QApplication>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QPalette palette_1(w.palette());
    palette_1.setColor(QPalette::Background,Qt::black);
    w.setPalette(palette_1);
    //Ui::MainWindow ui;
   // while(1){
    w.show();
   // }
    return a.exec();
}
