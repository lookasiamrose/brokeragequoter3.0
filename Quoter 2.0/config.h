#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include <QFile>
#include <QXmlStreamReader>
#include <QString>
#include <QMap>
#include <browser.h>

static bool readXmlFile( QIODevice& device, QSettings::SettingsMap& map );
static bool writeXmlFile( QIODevice& device, const QSettings::SettingsMap& map );

class InstrumentStats;

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);
    Config( Browser* browser_temp );
    ~Config();

    QString toInfo( qint32 id_arg );
    QString prepareSell();
    QString prepareBuy();
    QString inputQuantity( qint32 qty_arg );
    QString confirmTrade();
    QString prepareClose( qint32 id_arg );
    QString confirmClose();
    void getGold(InstrumentStats* temp);
    void getSilver(InstrumentStats* temp);
    void getGas(InstrumentStats* temp);
    void getBawelna(InstrumentStats* temp);
    void getKakao(InstrumentStats* temp);
    void getMiedz(InstrumentStats* temp);

private:
    QSettings* settings;

    Browser* plus500;

    QMap<QString,QString> instructions;
signals:

public slots:
};

class InstrumentStats
{
public:
    InstrumentStats();
    void show();
public:
    double sell_momentum;
    double buy_momentum;
    qint32 buy_eye;
    qint32 sell_eye;
    double decimal;
    qint32 qty;
    qint32 id;
};

bool readXmlFile( QIODevice& device, QSettings::SettingsMap& map )
{
    QXmlStreamReader xmlReader( &device );

    QString currentElementName;
    while( !xmlReader.atEnd() )
    {
    xmlReader.readNext();
        while( xmlReader.isStartElement() )
        {
            if( xmlReader.name() == "SettingsMap" )
            {
                                xmlReader.readNext();
                continue;
            }

            if( !currentElementName.isEmpty() )
            {
                currentElementName += "/";
            }
            currentElementName += xmlReader.name().toString();
            xmlReader.readNext();
        }

        if( xmlReader.isEndElement() )
        {
            continue;
        }

        if( xmlReader.isCharacters() && !xmlReader.isWhitespace() )
        {
            QString key = currentElementName;
            QString value = xmlReader.text().toString();

            map[ key ] = value;

            currentElementName.clear();
        }
    }

     if( xmlReader.hasError() )
     {
        return false;
     }

    return true;
}

bool writeXmlFile( QIODevice& device, const QSettings::SettingsMap& map )
{
    QXmlStreamWriter xmlWriter( &device );
    xmlWriter.setAutoFormatting( true );

    xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement( "SettingsMap" );

    QSettings::SettingsMap::const_iterator mi = map.begin();
    for( mi; mi != map.end(); ++mi )
    {
        QStringList groups;
        groups = mi.key().split( "/" );
        foreach( QString groupName, groups )
        {
            xmlWriter.writeStartElement( groupName );
        }

        xmlWriter.writeCharacters( mi.value().toString() );

        foreach( QString groupName, groups )
        {
            xmlWriter.writeEndElement();
        }
    }

        xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return true;
}

#endif // CONFIG_H
