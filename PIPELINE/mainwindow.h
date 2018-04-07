#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void cycle();
    void cycle_2();
    void openText(QString);
    void connectPipe(QString);
    void connectPipe_2(QString);
    void reset();
private slots:
    void on_File_clicked();
   // void changeButton();
    //void threadStop();
   // void threadStart();
   // void threadPR();

   // void on_pause_clicked();

    //void on_textBrowser_2_highlighted(const QString &arg1);

    void on_step_clicked();

    void on_reset_clicked();

    void on_run_clicked();

    void on_pause_clicked();

   // void on_horizontalSlider_actionTriggered(int action);

    void on_back_clicked();

    void on_file_2_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

