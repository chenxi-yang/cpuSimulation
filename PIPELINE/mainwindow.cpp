#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QFileDialog>
#include<QDebug>
#include<QMenuBar>
#include<QMessageBox>
#include<QStatusBar>
#include<QThread>
#include<QMutex>
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

static total cpu;
//static MyThread *myThread;
//static MyThread *oneThread;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //textBrowser
    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->textBrowser_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QPalette palette=ui->textBrowser->palette();
    palette.setColor(QPalette::Base,QColor(Qt::black));
    palette.setColor(QPalette::Text,QColor(Qt::white));
    ui->textBrowser->setPalette(palette);
    QPalette palette_2=ui->textBrowser_2->palette();
    palette_2.setColor(QPalette::Base,QColor(Qt::black));
    palette_2.setColor(QPalette::Text,QColor(Qt::white));
    ui->textBrowser_2->setPalette(palette_2);
   // myThread=new MyThread;
   // oneThread=new MyThread;
   // connect(ui->pause,SIGNAL(clicked()),this,SLOT(changeButton()));
   // connect(ui->start,SIGNAL(clicked()),this,SLOT(threadStart()));
   // connect(ui->pause,SIGNAL(clicked()),this,SLOT(threadPR()));
   // connect(ui->stop,SIGNAL(clicked()),this,SLOT(threadStop()));
}

MainWindow::~MainWindow()
{
        delete ui;
}
void MainWindow::cycle_2()
{
    //cpu.prepare();
   // cpu.read_in();
    while(cpu.Stat==SAOK){
        sleep(1);
        QString s;
        //cpu.show_register();

        //show_variable();
        cpu.Control();
        cpu.Send();

        cpu.Write();
        cpu.Memory();
        cpu.Execute();
        cpu.Decode();
        cpu.Fetch();


        //refresh.update();

        //***show registers**
        ui->eax->setText("0x"+QString("%1").arg(cpu.Reg[0],8,16,QChar('0')).right(8));
        ui->ecx->setText("0x"+QString("%1").arg(cpu.Reg[1],8,16,QChar('0')).right(8));
        ui->edx->setText("0x"+QString("%1").arg(cpu.Reg[2],8,16,QChar('0')).right(8));
        ui->ebx->setText("0x"+QString("%1").arg(cpu.Reg[3],8,16,QChar('0')).right(8));
        ui->esp->setText("0x"+QString("%1").arg(cpu.Reg[4],8,16,QChar('0')).right(8));
        ui->ebp->setText("0x"+QString("%1").arg(cpu.Reg[5],8,16,QChar('0')).right(8));
        ui->esi->setText("0x"+QString("%1").arg(cpu.Reg[6],8,16,QChar('0')).right(8));
        ui->edi->setText("0x"+QString("%1").arg(cpu.Reg[7],8,16,QChar('0')).right(8));
        //***END show registers*****

        //***show variable***
        //cpu.show_vairable();QString("%1").arg(cpu.D_valC,8,16,QChar('0'))
        ui->CPI->setText(QString::number(cpu.CPI,10,2));
        ui->instructions->setText(QString("%1").arg(cpu.instructions,3,10,QChar('0')));
        ui->F_predPC->setText("0x"+QString("%1").arg(cpu.F_predPC,8,16,QChar('0')));
        ui->D_stat->setText(&cpu.select_stat(cpu.D_stat)[0]);
        ui->D_icode->setText(&cpu.select_icode(cpu.D_icode)[0]);
        ui->D_ifun->setText(s.number(cpu.D_ifun));
        ui->D_rA->setText(&cpu.select_reg(cpu.D_rA)[0]);
        ui->D_rB->setText(&cpu.select_reg(cpu.D_rB)[0]);
        ui->D_valC->setText("0x"+QString("%1").arg(cpu.D_valC,8,16,QChar('0')).right(8));
        ui->D_valP->setText("0x"+QString("%1").arg(cpu.D_valP,8,16,QChar('0')).right(8));
        ui->E_stat->setText(&cpu.select_stat(cpu.E_stat)[0]);
        ui->E_icode->setText(&cpu.select_icode(cpu.E_icode)[0]);
        ui->E_ifun->setText(s.number(cpu.E_ifun));
        ui->E_valC->setText("0x"+QString("%1").arg(cpu.E_valC,8,16,QChar('0')).right(8));
        ui->E_valA->setText("0x"+QString("%1").arg(cpu.E_valA,8,16,QChar('0')).right(8));
        ui->E_valB->setText("0x"+QString("%1").arg(cpu.E_valB,8,16,QChar('0')).right(8));
        ui->E_dstE->setText(&cpu.select_reg(cpu.E_dstE)[0]);
        ui->E_dstM->setText(&cpu.select_reg(cpu.E_dstM)[0]);
        ui->E_srcA->setText(&cpu.select_reg(cpu.E_srcA)[0]);
        ui->E_srcB->setText(&cpu.select_reg(cpu.E_srcB)[0]);
        ui->M_stat->setText(&cpu.select_stat(cpu.M_stat)[0]);
        ui->M_icode->setText(&cpu.select_icode(cpu.M_icode)[0]);
        ui->M_valE->setText("0x"+QString("%1").arg(cpu.M_valE,8,16,QChar('0')).right(8));
        ui->M_valA->setText("0x"+QString("%1").arg(cpu.M_valA,8,16,QChar('0')).right(8));
        ui->M_dstE->setText(&cpu.select_reg(cpu.M_dstE)[0]);
        ui->M_dstM->setText(&cpu.select_reg(cpu.M_dstM)[0]);
        ui->W_stat->setText(&cpu.select_stat(cpu.W_stat)[0]);
        ui->W_icode->setText(&cpu.select_icode(cpu.W_icode)[0]);
        ui->W_valE->setText("0x"+QString("%1").arg(cpu.W_valE,8,16,QChar('0')).right(8));
        ui->W_valM->setText("0x"+QString("%1").arg(cpu.W_valM,8,16,QChar('0')).right(8));
        ui->W_dstE->setText(&cpu.select_reg(cpu.W_dstE)[0]);
        ui->W_dstM->setText(&cpu.select_reg(cpu.W_dstM)[0]);
        ui->ZF->setText(s.number(cpu.ZF));
        ui->SF->setText(s.number(cpu.SF));
        ui->OF->setText(s.number(cpu.OF));
        ui->clock->setText(QString("%1").arg(cpu.circle_time,3,10,QChar('0')));
        //ui.textBrowser->set()
        //w.show();
        //***END show variable***
        qApp->processEvents();
        if(cpu.W_stat)
            cpu.instructions++;
        if(cpu.instructions==0)
            cpu.CPI=1;
        else
            cpu.CPI=double(cpu.circle_time)/cpu.instructions;
        cpu.circle_time++;
    }
}

void MainWindow::cycle()
{
    //cpu.prepare();
   // cpu.read_in();
    if(cpu.Stat==SAOK){
        sleep(0.5);
        QString s;
        //cpu.show_register();

        //show_variable();
        cpu.Control();
        cpu.Send();

        cpu.Write();
        cpu.Memory();
        cpu.Execute();
        cpu.Decode();
        cpu.Fetch();


        //refresh.update();

        //***show registers**
        ui->eax->setText("0x"+QString("%1").arg(cpu.Reg[0],8,16,QChar('0')).right(8));
        ui->ecx->setText("0x"+QString("%1").arg(cpu.Reg[1],8,16,QChar('0')).right(8));
        ui->edx->setText("0x"+QString("%1").arg(cpu.Reg[2],8,16,QChar('0')).right(8));
        ui->ebx->setText("0x"+QString("%1").arg(cpu.Reg[3],8,16,QChar('0')).right(8));
        ui->esp->setText("0x"+QString("%1").arg(cpu.Reg[4],8,16,QChar('0')).right(8));
        ui->ebp->setText("0x"+QString("%1").arg(cpu.Reg[5],8,16,QChar('0')).right(8));
        ui->esi->setText("0x"+QString("%1").arg(cpu.Reg[6],8,16,QChar('0')).right(8));
        ui->edi->setText("0x"+QString("%1").arg(cpu.Reg[7],8,16,QChar('0')).right(8));
        //***END show registers*****

        //***show variable***
        //cpu.show_vairable();QString("%1").arg(cpu.D_valC,8,16,QChar('0'))
        ui->CPI->setText(QString::number(cpu.CPI,10,2));
        ui->instructions->setText(QString("%1").arg(cpu.instructions,3,10,QChar('0')));
        ui->F_predPC->setText("0x"+QString("%1").arg(cpu.F_predPC,8,16,QChar('0')));
        ui->D_stat->setText(&cpu.select_stat(cpu.D_stat)[0]);
        ui->D_icode->setText(&cpu.select_icode(cpu.D_icode)[0]);
        ui->D_ifun->setText(s.number(cpu.D_ifun));
        ui->D_rA->setText(&cpu.select_reg(cpu.D_rA)[0]);
        ui->D_rB->setText(&cpu.select_reg(cpu.D_rB)[0]);
        ui->D_valC->setText("0x"+QString("%1").arg(cpu.D_valC,8,16,QChar('0')).right(8));
        ui->D_valP->setText("0x"+QString("%1").arg(cpu.D_valP,8,16,QChar('0')).right(8));
        ui->E_stat->setText(&cpu.select_stat(cpu.E_stat)[0]);
        ui->E_icode->setText(&cpu.select_icode(cpu.E_icode)[0]);
        ui->E_ifun->setText(s.number(cpu.E_ifun));
        ui->E_valC->setText("0x"+QString("%1").arg(cpu.E_valC,8,16,QChar('0')).right(8));
        ui->E_valA->setText("0x"+QString("%1").arg(cpu.E_valA,8,16,QChar('0')).right(8));
        ui->E_valB->setText("0x"+QString("%1").arg(cpu.E_valB,8,16,QChar('0')).right(8));
        ui->E_dstE->setText(&cpu.select_reg(cpu.E_dstE)[0]);
        ui->E_dstM->setText(&cpu.select_reg(cpu.E_dstM)[0]);
        ui->E_srcA->setText(&cpu.select_reg(cpu.E_srcA)[0]);
        ui->E_srcB->setText(&cpu.select_reg(cpu.E_srcB)[0]);
        ui->M_stat->setText(&cpu.select_stat(cpu.M_stat)[0]);
        ui->M_icode->setText(&cpu.select_icode(cpu.M_icode)[0]);
        ui->M_valE->setText("0x"+QString("%1").arg(cpu.M_valE,8,16,QChar('0')).right(8));
        ui->M_valA->setText("0x"+QString("%1").arg(cpu.M_valA,8,16,QChar('0')).right(8));
        ui->M_dstE->setText(&cpu.select_reg(cpu.M_dstE)[0]);
        ui->M_dstM->setText(&cpu.select_reg(cpu.M_dstM)[0]);
        ui->W_stat->setText(&cpu.select_stat(cpu.W_stat)[0]);
        ui->W_icode->setText(&cpu.select_icode(cpu.W_icode)[0]);
        ui->W_valE->setText("0x"+QString("%1").arg(cpu.W_valE,8,16,QChar('0')).right(8));
        ui->W_valM->setText("0x"+QString("%1").arg(cpu.W_valM,8,16,QChar('0')).right(8));
        ui->W_dstE->setText(&cpu.select_reg(cpu.W_dstE)[0]);
        ui->W_dstM->setText(&cpu.select_reg(cpu.W_dstM)[0]);
        ui->ZF->setText(s.number(cpu.ZF));
        ui->SF->setText(s.number(cpu.SF));
        ui->OF->setText(s.number(cpu.OF));
        ui->clock->setText(QString("%1").arg(cpu.circle_time,3,10,QChar('0')));
        //ui.textBrowser->set()
        //w.show();
        //***END show variable***
        qApp->processEvents();
        if(cpu.W_stat)
            cpu.instructions++;
        if(cpu.instructions==0)
            cpu.CPI=1;
        else
            cpu.CPI=double(cpu.circle_time)/cpu.instructions;
        cpu.circle_time++;
    }
}
void MainWindow::openText(QString f)
{
    QFile file(f);
    if(!file.open(QFile::ReadOnly|QFile::Text))
        qDebug()<<"Can not open";
    QTextStream in(&file);
    ui->textBrowser_2->setText(in.readAll());
}
//void pipelineDialog::on_actionOpen_o_triggered()
//{
 //   QString str=QFileDialog::getOpenFileName(this,"./home/y86-code","asm(*.yo,*.txt,*.s)");
 //   openText(str);
 //   connectPipe(str);
//}
void MainWindow::connectPipe(QString str)
{
    char *ch;
    QByteArray ba=str.toLatin1();
    ch=ba.data();
    cpu.prepare();
    cpu.read_in(ch);
    ui->textBrowser->setText(cpu.ch_use);
    cycle();
}
void MainWindow::connectPipe_2(QString str)
{
    char *ch;
    QByteArray ba=str.toLatin1();
    ch=ba.data();
    cpu.prepare();
    cpu.read_in_2(ch);
    ui->textBrowser->setText(cpu.ch_use);
    cycle();
}
void MainWindow::reset(){
    memset(cpu.ch_code,0,sizeof(char));
    memset(cpu.ch_use,0,sizeof(char));
    cpu.Stat=1;
    cpu.M_stat=cpu.W_stat=cpu.D_stat=cpu.E_stat=0;
    cpu.m_stat=cpu.d_stat=cpu.e_stat=0;
    cpu.Reg[0]=0;cpu.Reg[1]=0;cpu.Reg[2]=0;cpu.Reg[3]=0;cpu.Reg[4]=0;cpu.Reg[5]=0;cpu.Reg[6]=0;cpu.Reg[7]=0;
    cpu.f_icode=cpu.f_ifun=cpu.f_rA=cpu.f_rB=cpu.f_valC=cpu.f_valP=cpu.f_pc=cpu.f_predPC=0;
    cpu.d_icode=cpu.d_ifun=cpu.d_valC=cpu.d_valA=cpu.d_valB=cpu.d_dstE=cpu.d_dstM=cpu.d_srcA=cpu.d_srcB=cpu.d_rvalA=cpu.d_rvalB=0;
    cpu.e_stat=cpu.e_icode=cpu.e_Cnd=cpu.e_valE=cpu.e_valA=cpu.e_dstE=cpu.e_dstM=cpu.e_aluA=cpu.e_aluB=cpu.e_alufun=0;
    cpu.m_stat=cpu.m_icode=cpu.m_mem_addr=cpu.m_mem_data=cpu.m_valE=cpu.m_valM=cpu.m_dstE=cpu.m_dstM=0;
    cpu.circle_time=0;
    cpu.CPI=1;
    cpu.prepare();
}
void MainWindow::on_File_clicked()
{
   // QString str=QFileDialog::getOpenFileName(this,"./home/y86-code","asm(*.yo,*.txt,*.s)");//my pc
    QString str=QFileDialog::getOpenFileName(this,"./home/aaron/Desktop","asm(*.yo,*.txt,*.s)");
    openText(str);
    connectPipe(str);
    reset();
}
void MainWindow::on_step_clicked()
{
    cycle();
}

void MainWindow::on_reset_clicked()
{
    reset();
    cycle();
}

void MainWindow::on_run_clicked()
{
    cycle_2();
}

void MainWindow::on_pause_clicked()
{
    //sleep(1);
}


//void MainWindow::on_horizontalSlider_actionTriggered(int action)
//{

//}

void MainWindow::on_back_clicked()
{
    cpu.temp_circle_time=cpu.circle_time;
    reset();
    while(cpu.Stat==SAOK&&cpu.circle_time+1<cpu.temp_circle_time){
        QString s;
        //cpu.show_register();

        //show_variable();
        cpu.Control();
        cpu.Send();

        cpu.Write();
        cpu.Memory();
        cpu.Execute();
        cpu.Decode();
        cpu.Fetch();


        //refresh.update();

        //***show registers**
        ui->eax->setText("0x"+QString("%1").arg(cpu.Reg[0],8,16,QChar('0')).right(8));
        ui->ecx->setText("0x"+QString("%1").arg(cpu.Reg[1],8,16,QChar('0')).right(8));
        ui->edx->setText("0x"+QString("%1").arg(cpu.Reg[2],8,16,QChar('0')).right(8));
        ui->ebx->setText("0x"+QString("%1").arg(cpu.Reg[3],8,16,QChar('0')).right(8));
        ui->esp->setText("0x"+QString("%1").arg(cpu.Reg[4],8,16,QChar('0')).right(8));
        ui->ebp->setText("0x"+QString("%1").arg(cpu.Reg[5],8,16,QChar('0')).right(8));
        ui->esi->setText("0x"+QString("%1").arg(cpu.Reg[6],8,16,QChar('0')).right(8));
        ui->edi->setText("0x"+QString("%1").arg(cpu.Reg[7],8,16,QChar('0')).right(8));
        //***END show registers*****

        //***show variable***
        //cpu.show_vairable();QString("%1").arg(cpu.D_valC,8,16,QChar('0'))
        ui->CPI->setText(QString::number(cpu.CPI,10,2));
        ui->instructions->setText(QString("%1").arg(cpu.instructions,3,10,QChar('0')));
        ui->F_predPC->setText("0x"+QString("%1").arg(cpu.F_predPC,8,16,QChar('0')));
        ui->D_stat->setText(&cpu.select_stat(cpu.D_stat)[0]);
        ui->D_icode->setText(&cpu.select_icode(cpu.D_icode)[0]);
        ui->D_ifun->setText(s.number(cpu.D_ifun));
        ui->D_rA->setText(&cpu.select_reg(cpu.D_rA)[0]);
        ui->D_rB->setText(&cpu.select_reg(cpu.D_rB)[0]);
        ui->D_valC->setText("0x"+QString("%1").arg(cpu.D_valC,8,16,QChar('0')).right(8));
        ui->D_valP->setText("0x"+QString("%1").arg(cpu.D_valP,8,16,QChar('0')).right(8));
        ui->E_stat->setText(&cpu.select_stat(cpu.E_stat)[0]);
        ui->E_icode->setText(&cpu.select_icode(cpu.E_icode)[0]);
        ui->E_ifun->setText(s.number(cpu.E_ifun));
        ui->E_valC->setText("0x"+QString("%1").arg(cpu.E_valC,8,16,QChar('0')).right(8));
        ui->E_valA->setText("0x"+QString("%1").arg(cpu.E_valA,8,16,QChar('0')).right(8));
        ui->E_valB->setText("0x"+QString("%1").arg(cpu.E_valB,8,16,QChar('0')).right(8));
        ui->E_dstE->setText(&cpu.select_reg(cpu.E_dstE)[0]);
        ui->E_dstM->setText(&cpu.select_reg(cpu.E_dstM)[0]);
        ui->E_srcA->setText(&cpu.select_reg(cpu.E_srcA)[0]);
        ui->E_srcB->setText(&cpu.select_reg(cpu.E_srcB)[0]);
        ui->M_stat->setText(&cpu.select_stat(cpu.M_stat)[0]);
        ui->M_icode->setText(&cpu.select_icode(cpu.M_icode)[0]);
        ui->M_valE->setText("0x"+QString("%1").arg(cpu.M_valE,8,16,QChar('0')).right(8));
        ui->M_valA->setText("0x"+QString("%1").arg(cpu.M_valA,8,16,QChar('0')).right(8));
        ui->M_dstE->setText(&cpu.select_reg(cpu.M_dstE)[0]);
        ui->M_dstM->setText(&cpu.select_reg(cpu.M_dstM)[0]);
        ui->W_stat->setText(&cpu.select_stat(cpu.W_stat)[0]);
        ui->W_icode->setText(&cpu.select_icode(cpu.W_icode)[0]);
        ui->W_valE->setText("0x"+QString("%1").arg(cpu.W_valE,8,16,QChar('0')).right(8));
        ui->W_valM->setText("0x"+QString("%1").arg(cpu.W_valM,8,16,QChar('0')).right(8));
        ui->W_dstE->setText(&cpu.select_reg(cpu.W_dstE)[0]);
        ui->W_dstM->setText(&cpu.select_reg(cpu.W_dstM)[0]);
        ui->ZF->setText(s.number(cpu.ZF));
        ui->SF->setText(s.number(cpu.SF));
        ui->OF->setText(s.number(cpu.OF));
        ui->clock->setText(QString("%1").arg(cpu.circle_time,3,10,QChar('0')));
        //ui.textBrowser->set()
        //w.show();
        //***END show variable***
        //qApp->processEvents();
        if(cpu.W_stat)
            cpu.instructions++;
        if(cpu.instructions==0)
            cpu.CPI=1;
        else
            cpu.CPI=double(cpu.circle_time)/cpu.instructions;
        cpu.circle_time++;
    }
}

void MainWindow::on_file_2_clicked()
{
       // QString str=QFileDialog::getOpenFileName(this,"./home/y86-code","asm(*.yo,*.txt,*.s)");//my pc
        QString str=QFileDialog::getOpenFileName(this,"./home/aaron/Desktop","asm(*.yo,*.txt,*.s)");//HZK's pc
        openText(str);
        connectPipe_2(str);
        reset();
}
