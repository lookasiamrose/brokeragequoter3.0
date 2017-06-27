#include "instrument.h"
#include <QDebug>

Instrument::Instrument(QObject *parent) : QObject(parent)
{
    id = 0;
    name = "_BLANK";
    buy = 0.0;
    sell = 0.0;
}

Instrument::Instrument(qint32 id_temp, QString name_temp, double buy_temp, double sell_temp)
{
    id = id_temp;
    name = name_temp;
    buy = buy_temp;
    sell = sell_temp;
}

Instrument::Instrument(const Instrument& temporary)
{
    id = temporary.id;
    name = temporary.name;
    buy = temporary.buy;
    sell = temporary.sell;
}

Instrument& Instrument::operator=(const Instrument& temporary)
{
    id = temporary.id;
    name = temporary.name;
    buy = temporary.buy;
    sell = temporary.sell;
    return *this;
}

QString Instrument::generateCode(Attribute element)
{
    QString js_code = QString::number(id);

    switch( element )
    {
    case Attribute::NAME:
        js_code = "$(\"div[data-instrument-id='" + js_code + "']\").find('.name').find('span').text()";
        break;

    case Attribute::BUY:
        js_code = "$(\"div[data-instrument-id='" + js_code + "']\").find('.buy').text()";
        break;

    case Attribute::SELL:
        js_code = "$(\"div[data-instrument-id='" + js_code + "']\").find('.sell').text()";
        break;

    default:
        js_code = "document.title";
        break;
    }
    return js_code;
}

void Instrument::storeAttribute(QString attribute_temp, Attribute element)
{
    bool* error_flag = new bool(true);

    attribute_temp.remove( QChar(' ') );
    attribute_temp.remove( 0xA0 );
    attribute_temp.replace( QChar(','), QChar('.') );

    switch( element )
    {
    case Attribute::NAME:
        name  = attribute_temp;
        break;

    case Attribute::BUY:
        buy  = attribute_temp.toDouble(error_flag);
        break;

    case Attribute::SELL:
        sell  = attribute_temp.toDouble(error_flag);
        break;

    default:
        qDebug()<<"INSTRUMENT CLASS: ERROR - WRONG ATTRIBUTE ENUM";
        break;
    }
    if( !(*error_flag) ) qDebug()<<"ATTRIBUTE_CONVERSION_ERROR"<<" >>FOR>> "<<attribute_temp;
}
