#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class Instrument;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static void dataCapture(const QVariant & result);
    static void dialogclass(const QVariant & result);
    static void isloginscreen(const QVariant & result);
    ~MainWindow();

private slots:
    void on_execRun_clicked();
public slots:
    void sekTick();
private:
    Ui::MainWindow *ui;
    static QWebEnginePage* page;
    static QWebEngineView* view;
    QTimer* sek;
    static qint32 save60;
    static QMap<QString,Instrument*> instruments;
};

struct History
{
    QString buy;
    QString sell;
    QString time;
};

class Instrument
{
public:
    Instrument(QString id_arg);
    QString id;
    QList<History> history;
};

#endif // MAINWINDOW_H
