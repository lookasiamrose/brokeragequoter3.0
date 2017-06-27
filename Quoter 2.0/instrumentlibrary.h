#ifndef INSTRUMENTLIBRARY_H
#define INSTRUMENTLIBRARY_H

#include <QObject>
#include <QList>
#include <QTime>
#include "instrument.h"

class InstrumentElement;
class lastData;

class InstrumentLibrary : public Instrument
{
public:
    InstrumentLibrary();
    InstrumentLibrary(int id_temp);
    void addToHistory();
    void flush();
public:
    QList<InstrumentElement*> history;
    QString history_file;
    QList<lastData*> lastBuys;
    bool bought;
    bool sell_bought;
    quint8 opens;
};

class InstrumentElement : public QObject
{
    Q_OBJECT
public:
    InstrumentElement();
public:
    double buy;
    double sell;
    QTime time;
};

class lastData : public QObject
{
    Q_OBJECT
public:
    lastData();
public:
    double buy;
    double sell;
    double buy_diff;
    double sell_diff;
};

#endif // INSTRUMENTLIBRARY_H
