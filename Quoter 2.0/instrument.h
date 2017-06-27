#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <QObject>
#include <QString>

enum class Attribute { NAME, BUY, SELL };

class Instrument : public QObject
{
    Q_OBJECT
public:
    qint32 id;
    QString name;
    double  buy;
    double sell;
public:
    Instrument(QObject *parent = 0);
    Instrument(qint32 id_temp, QString name_temp = "_BLANK", double buy_temp = 0.0, double sell_temp = 0.0);
    Instrument(const Instrument& temporary);
    Instrument& operator=(const Instrument& temporary);
    QString generateCode(Attribute element);
    void storeAttribute(QString attribute_temp, Attribute element);

signals:

public slots:
};

#endif // INSTRUMENT_H
