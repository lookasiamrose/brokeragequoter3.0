#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QElapsedTimer>
#include "browser.h"
#include "instrumentswindow.h"
#include "config.h"
#include "analysiswindow.h"

namespace Ui
{
    class MainWindow;
}

class GuessingMechanism;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Ui::MainWindow* ui;
    void showLog(QString text);
    static void captureTest(const QVariant &result);
    ~MainWindow();

private:
    static QElapsedTimer test;

    AnalysisWindow* analysis;
    InstrumentsWindow* instruments;
    Browser* plus500;
    GuessingMechanism* machine;
    Browser* plus500_A;
    Config* configs;

private slots:
    void webpageLoaded(bool);
    void on_startMachineButton_clicked();
    void on_viewButton_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
};

#endif // MAINWINDOW_H
