#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QSettings>
#include <QTimer>

namespace Ui {
class MainWindow;
}

enum class Action{ OPEN_BUY, OPEN_SELL, CLOSE_CLOSE };

class Instrument;

typedef void (*Queue)(Instrument*);
struct Proceed
{
    Queue function;
    Instrument* argument;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    static MainWindow* static_this;
    QSettings* settings;
    static QWebEnginePage* page;
    QWebEngineView* view;
    QTimer* sek;
    static QTimer* action_queue;
    static QMap<QString,QString> instructions;
    static QMap<QString,Instrument*> instruments;
    static qint32 tooMuchZeros;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadStatics();
    static Instrument* findInstrument(const QString id_arg);
    static bool getBool(const QString& str_arg);
    static void showInstruments();
    static void performAction(Action action_arg, Instrument* ins_arg);
    static void performSell(Instrument* ins_arg);
    static void performBuy(Instrument* ins_arg);
    static void performClose(Instrument* ins_arg);
    void loadScripts();
    static void repeatCloseForInstrument(const QVariant & result);
    static void captureLog(const QVariant & result);
    static void clearInstrument(const QVariant & result);
    static void buyCallback(const QVariant & result);
    static void sellCallback(const QVariant & result);
    static void closeCallback(const QVariant & result);
    static void closeBalance(const QVariant & result);
    static QString time();
    template<typename T>
    static QString toText(T arg);
    static QString LOG;
    static qint32 log_counter;

    static void tickCallback(const QVariant & result);
    static QString script_getData;
    static QString script_close_close;
    static QString script_open_sell;
    static QString script_open_buy;
    static QList<Proceed*> proceed;
    static double sum_back(QList<double> &tab, const qint32 count);
    static bool areZeros(Instrument* instrument);
    static void makeDecision(Instrument* instrument);

    static void dialogclass(const QVariant & result);
    static void dialogtext(const QVariant & result);
    static void dialogbuttontext(const QVariant & result);
    static void isloginscreen(const QVariant & result);
    static qint32 instrumentQuantity(Instrument* temp_arg);
private slots:
    void on_execCastButton_clicked();
    void sekTick();
    void actionTick();

    static void castBuyStatus();
    static void castSellStatus();
    static void castCloseStatus();

    void on_execPauseButton_clicked();

private:
    Ui::MainWindow *ui;
};

class Instrument
{
public:
    Instrument();
    Instrument(const QString&);
    void show();
    QString id;
    QString name;
    double actual_buy;
    double actual_sell;
    bool disabled;
    int positions;

    QList<double> buy;
    QList<double> sell;
    QList<double> buy_change;
    QList<double> sell_change;

    double buymoment;
    double sellmoment;
    double buyeye;
    double selleye;
    double close;
    double open_value;

    qint32 inopentime;
    bool bought;
    bool sell_bought;

    qint32 candelay;
    double sub_budget;
};

#endif // MAINWINDOW_H
