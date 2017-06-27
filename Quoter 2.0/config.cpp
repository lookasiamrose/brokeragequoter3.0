#include "config.h"
#include <QDebug>

Config::Config(QObject *parent) : QObject(parent)
{

}

QString Config::toInfo( qint32 id_arg )
{
    QString script = instructions["info"];
    QString id_string;
    script = script.replace( "#ID", id_string.setNum(id_arg));
    //plus500->page()->runJavaScript(script);
    return script;
}

QString Config::inputQuantity( qint32 qty_arg )
{
    QString script = instructions["inputquantity"];
    QString qty_string;
    script = script.replace( "#QTY", qty_string.setNum(qty_arg));
    //plus500->page()->runJavaScript(script);
    return script;
}

QString Config::prepareBuy()
{
    QString script;
    script = instructions["buyprepare"];
    //plus500->page()->runJavaScript(script);
    return script;
}

QString Config::prepareSell()
{
    QString script;
    script = instructions["sellprepare"];
    //plus500->page()->runJavaScript(script);
    return script;
}

QString Config::confirmTrade()
{
    QString script;
    script = instructions["tradeconfirm"];
    //plus500->page()->runJavaScript(script);
    return script;
}

QString Config::prepareClose( qint32 id_arg )
{
    QString script = instructions["closeprepare"];
    QString id_string;
    script = script.replace( "#ID", id_string.setNum(id_arg));
    //plus500->page()->runJavaScript(script);
    return script;
}

QString Config::confirmClose()
{
    QString script;
    script = instructions["closeconfirm"];
    //plus500->page()->runJavaScript(script);
    return script;
}

Config::Config( Browser* browser_temp )
{
    plus500 = browser_temp;

    settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,"GE");

    settings->sync();

    QStringList categories( settings->childGroups() );
    foreach(QString item, categories)
    {
        if( item == "accountManagement" )
        {
            instructions.insert("info", settings->value( item + "/" + "info").toString() );
            instructions.insert("buyprepare", settings->value( item + "/" + "buyprepare").toString() );
            instructions.insert("sellprepare", settings->value( item + "/" + "sellprepare").toString() );
            instructions.insert("inputquantity", settings->value( item + "/" + "inputquantity").toString() );
            instructions.insert("tradeconfirm", settings->value( item + "/" + "tradeconfirm").toString() );
            instructions.insert("closeprepare", settings->value( item + "/" + "closeprepare").toString() );
            instructions.insert("closeconfirm", settings->value( item + "/" + "closeconfirm").toString() );
        }
        if( item == "InstrumentsStats" )
        {
            instructions.insert("goldbuymomentum", settings->value( item + "/" + "goldbuymomentum").toString() );
            instructions.insert("goldsellmomentum", settings->value( item + "/" + "goldsellmomentum").toString() );
            instructions.insert("goldbuyeye", settings->value( item + "/" + "goldbuyeye").toString() );
            instructions.insert("goldselleye", settings->value( item + "/" + "goldselleye").toString() );
            instructions.insert("goldqty", settings->value( item + "/" + "goldqty").toString() );
            instructions.insert("goldid", settings->value( item + "/" + "goldid").toString() );

            instructions.insert("silverbuymomentum", settings->value( item + "/" + "silverbuymomentum").toString() );
            instructions.insert("silversellmomentum", settings->value( item + "/" + "silversellmomentum").toString() );
            instructions.insert("silverbuyeye", settings->value( item + "/" + "silverbuyeye").toString() );
            instructions.insert("silverselleye", settings->value( item + "/" + "silverselleye").toString() );
            instructions.insert("silverqty", settings->value( item + "/" + "silverqty").toString() );
            instructions.insert("silverid", settings->value( item + "/" + "silverid").toString() );

            instructions.insert("gasbuymomentum", settings->value( item + "/" + "gasbuymomentum").toString() );
            instructions.insert("gassellmomentum", settings->value( item + "/" + "gassellmomentum").toString() );
            instructions.insert("gasbuyeye", settings->value( item + "/" + "gasbuyeye").toString() );
            instructions.insert("gasselleye", settings->value( item + "/" + "gasselleye").toString() );
            instructions.insert("gasqty", settings->value( item + "/" + "gasqty").toString() );
            instructions.insert("gasid", settings->value( item + "/" + "gasid").toString() );

            instructions.insert("bawelnabuymomentum", settings->value( item + "/" + "bawelnabuymomentum").toString() );
            instructions.insert("bawelnasellmomentum", settings->value( item + "/" + "bawelnasellmomentum").toString() );
            instructions.insert("bawelnabuyeye", settings->value( item + "/" + "bawelnabuyeye").toString() );
            instructions.insert("bawelnaselleye", settings->value( item + "/" + "bawelnaselleye").toString() );
            instructions.insert("bawelnaqty", settings->value( item + "/" + "bawelnaqty").toString() );
            instructions.insert("bawelnaid", settings->value( item + "/" + "bawelnaid").toString() );

            instructions.insert("kakaobuymomentum", settings->value( item + "/" + "kakaobuymomentum").toString() );
            instructions.insert("kakaosellmomentum", settings->value( item + "/" + "kakaosellmomentum").toString() );
            instructions.insert("kakaobuyeye", settings->value( item + "/" + "kakaobuyeye").toString() );
            instructions.insert("kakaoselleye", settings->value( item + "/" + "kakaoselleye").toString() );
            instructions.insert("kakaoqty", settings->value( item + "/" + "kakaoqty").toString() );
            instructions.insert("kakaoid", settings->value( item + "/" + "kakaoid").toString() );

            instructions.insert("miedzbuymomentum", settings->value( item + "/" + "miedzbuymomentum").toString() );
            instructions.insert("miedzsellmomentum", settings->value( item + "/" + "miedzsellmomentum").toString() );
            instructions.insert("miedzbuyeye", settings->value( item + "/" + "miedzbuyeye").toString() );
            instructions.insert("miedzselleye", settings->value( item + "/" + "miedzselleye").toString() );
            instructions.insert("miedzqty", settings->value( item + "/" + "miedzqty").toString() );
            instructions.insert("miedzid", settings->value( item + "/" + "miedzid").toString() );
        }
    }
}

void Config::getBawelna(InstrumentStats* temp)
{
    temp->buy_momentum = instructions["bawelnabuymomentum"].toDouble();
    temp->sell_momentum = instructions["bawelnasellmomentum"].toDouble();
    temp->buy_eye = instructions["bawelnabuyeye"].toInt();
    temp->sell_eye = instructions["bawelnaselleye"].toInt();
    temp->qty = instructions["bawelnaqty"].toInt();
    temp->id = instructions["bawelnaid"].toInt();
}

void Config::getKakao(InstrumentStats* temp)
{
    temp->buy_momentum = instructions["kakaobuymomentum"].toDouble();
    temp->sell_momentum = instructions["kakaosellmomentum"].toDouble();
    temp->buy_eye = instructions["kakaobuyeye"].toInt();
    temp->sell_eye = instructions["kakaoselleye"].toInt();
    temp->qty = instructions["kakaoqty"].toInt();
    temp->id = instructions["kakaoid"].toInt();
}

void Config::getMiedz(InstrumentStats* temp)
{
    temp->buy_momentum = instructions["miedzbuymomentum"].toDouble();
    temp->sell_momentum = instructions["miedzsellmomentum"].toDouble();
    temp->buy_eye = instructions["miedzbuyeye"].toInt();
    temp->sell_eye = instructions["miedzselleye"].toInt();
    temp->qty = instructions["miedzqty"].toInt();
    temp->id = instructions["miedzid"].toInt();
}

void Config::getGold(InstrumentStats* temp)
{
    temp->buy_momentum = instructions["goldbuymomentum"].toDouble();
    temp->sell_momentum = instructions["goldsellmomentum"].toDouble();
    temp->buy_eye = instructions["goldbuyeye"].toInt();
    temp->sell_eye = instructions["goldselleye"].toInt();
    temp->qty = instructions["goldqty"].toInt();
    temp->id = instructions["goldid"].toInt();
}

void Config::getSilver(InstrumentStats* temp)
{
    temp->buy_momentum = instructions["silverbuymomentum"].toDouble();
    temp->sell_momentum = instructions["silversellmomentum"].toDouble();
    temp->buy_eye = instructions["silverbuyeye"].toInt();
    temp->sell_eye = instructions["silverselleye"].toInt();
    temp->qty = instructions["silverqty"].toInt();
    temp->id = instructions["silverid"].toInt();
}

void Config::getGas(InstrumentStats* temp)
{
    temp->buy_momentum = instructions["gasbuymomentum"].toDouble();
    temp->sell_momentum = instructions["gassellmomentum"].toDouble();
    temp->buy_eye = instructions["gasbuyeye"].toInt();
    temp->sell_eye = instructions["gasselleye"].toInt();
    temp->qty = instructions["gasqty"].toInt();
    temp->id = instructions["gasid"].toInt();
}


InstrumentStats::InstrumentStats()
{
    sell_momentum = 0.50;
    buy_momentum = 0.50;
    buy_eye = 3;
    sell_eye = 3;
    decimal = 0.01;
    qty = 1;
    id = 1;
}

void InstrumentStats::show()
{
    qDebug()<<"sell_momentum - "<<sell_momentum;
    qDebug()<<"buy_momentum - "<<buy_momentum;
    qDebug()<<"buy_eye - "<<buy_eye;
    qDebug()<<"sell_eye - "<<sell_eye;
    qDebug()<<"decimal - "<<decimal;
    qDebug()<<"qty - "<<qty;
    qDebug()<<"id - "<<id;
}

Config::~Config()
{
    delete settings;
}
