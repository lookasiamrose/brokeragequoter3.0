#include "instrumentlibrary.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

InstrumentLibrary::InstrumentLibrary()
{
    opens = 0;
}

lastData::lastData()
{
    buy = 0;
    sell = 0;
    sell_diff = 0;
    buy_diff = 0;
}

InstrumentLibrary::InstrumentLibrary(int id_temp) : Instrument(id_temp)
{
    bought = false;
    sell_bought = false;
}

InstrumentElement::InstrumentElement()
{

}

void InstrumentLibrary::addToHistory()
{
    InstrumentElement* temp = new InstrumentElement();
    temp->buy = buy;
    temp->sell = sell;
    temp->time = QTime::currentTime();
    history.push_back(temp);
}

void InstrumentLibrary::flush()
{
    QFile handler;
    QString file_name;
    QString id_name;
    id_name.setNum(id);
    file_name = "C:\\Users\\lukasz.jamroz\\Documents\\Qt Projects\\NoRegrets\\build-NoRegrets-Desktop_Qt_5_7_0_MSVC2013_64bit-Debug\\release\\Data\\" + id_name + ".hs";
    handler.setFileName(file_name);
    handler.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&handler);
    //qDebug()<<handler.size();
    foreach(InstrumentElement* item, history)
    {
        out.setRealNumberPrecision(6);
        QTime* temp = new QTime(0,0);
        if( (*temp) == (item->time) )
        {
            QDate date = QDate::currentDate();
            out<<date.toString()<<"\n";
        }
        out<<item->buy<<" "<<item->sell<<" "<<item->time.toString()<<"\n";
        delete item;
    }

    history.clear();
    handler.close();
}
