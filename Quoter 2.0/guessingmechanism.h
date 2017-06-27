#ifndef GUESSINGMECHANISM_H
#define GUESSINGMECHANISM_H

#include <QObject>
#include <QTimer>
#include <QTime>
//#include <QElapsedTimer>
#include "instrument.h"
#include "browser.h"
#include "instrumentswindow.h"
#include "instrumentlibrary.h"
#include "config.h"
#include "mainwindow.h"

class CaptureStruct;

class GuessingMechanism : public QObject
{
    Q_OBJECT
private:
    //static QElapsedTimer test;
    static QList<CaptureStruct*> commodities_pointers;
    QString jquery;
    static Browser* plus500_pointer;
    InstrumentsWindow* window;
    QTimer *tick;
    static QString js_result;
    static bool flag;
    QTime time_link;
    QTimer *queue_tick;
    quint32 id_click;
    bool back;
    quint32 time_me;
    Config* configs;
    MainWindow* mwindow;

    bool sell_bought;
    bool bought;
    qint32 time;
    AccountManager* account;
    QStringList queue;
    bool block;
    static QString log_string;

    ///////////////////////////////////////////////
    InstrumentStats* gold;
    InstrumentStats* chosen;
    InstrumentStats* silver;
    InstrumentStats* gas;
    InstrumentStats* bawelna;
    InstrumentStats* kakao;
    InstrumentStats* miedz;

    static quint8 gold_opens;
    static quint8 silver_opens;
    static quint8 gas_opens;
    static quint8 check_opens;
    static quint8 bawelna_opens;
    static quint8 kakao_opens;
    static quint8 miedz_opens;
public:
    QList<InstrumentLibrary*> commodities;
    ///////////////////////////////////////////////
    InstrumentLibrary* object;
    InstrumentLibrary* object_silver;
    InstrumentLibrary* object_gas;
    InstrumentLibrary* object_bawelna;
    InstrumentLibrary* object_kakao;
    InstrumentLibrary* object_miedz;
    static bool goldEnabled;
    static bool silverEnabled;
    static bool gasEnabled;
    static bool bawelnaEnabled;
    static bool kakaoEnabled;
    static bool miedzEnabled;
    static QTime actualmoment;

    static void goldPositions(const QVariant &result);
    static void silverPositions(const QVariant &result);
    static void gasPositions(const QVariant &result);
    static void bawelnaPositions(const QVariant &result);
    static void kakaoPositions(const QVariant &result);
    static void miedzPositions(const QVariant &result);

    static void isGoldDisabled(const QVariant &result);
    static void isSilverDisabled(const QVariant &result);
    static void isGasDisabled(const QVariant &result);
    static void isBawelnaDisabled(const QVariant &result);
    static void isKakaoDisabled(const QVariant &result);
    static void isMiedzDisabled(const QVariant &result);

    GuessingMechanism(QObject *parent = 0);
    void connectBrowser(Browser* browser_temp);
    void start();
    static void capture(const QVariant &result);
    static void logoutalert(const QVariant &result);
    static void choosedemo(const QVariant &result);
    static void dialogCapture(const QVariant &result);
    static void dialogContent(const QVariant &result);
    static void getValue(const QVariant &result);
    void connectWindow(InstrumentsWindow* window_temp);
    void connectParent(MainWindow* window_temp);
    void connectConfigs(Config* config_temp);
    bool areZeros();
    void makeDecision(const InstrumentStats* chosen_arg,InstrumentLibrary* object_arg);
    double sum_back(QList<lastData*> &tab, qint32 count,bool buy);
    void externalToQueue(QString temp);
    ~GuessingMechanism();
signals:
    void js_isup(bool);

public slots:
    void updateInstruments();
    void queue_move();
};

class CaptureStruct : public QObject
{
    Q_OBJECT
public:
    CaptureStruct(QObject *parent = 0) : QObject(parent) { commodity = nullptr; attribute = Attribute::NAME; }
    InstrumentLibrary* commodity;
    Attribute attribute;
};

#endif // GUESSINGMECHANISM_H
