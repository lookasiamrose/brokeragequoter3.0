#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QDebug>

QMap<QString,Instrument*> MainWindow::instruments;
QMap<QString,QString> MainWindow::instructions;
QWebEnginePage* MainWindow::page = nullptr;
MainWindow* MainWindow::static_this = nullptr;
QString MainWindow::LOG = "";
qint32 MainWindow::log_counter = 0;

QString MainWindow::script_getData = "";
QString MainWindow::script_close_close = "";
QString MainWindow::script_open_sell = "";
QString MainWindow::script_open_buy = "";
QList<Proceed*> MainWindow::proceed;
QTimer* MainWindow::action_queue = nullptr;
qint32 MainWindow::tooMuchZeros = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->execCastButton->setEnabled(false);
    static_this = this;

    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath());
    settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,"config");

    page = new QWebEnginePage();
    view = new QWebEngineView();

    page->load( QUrl("https://app.plus500.com/trade/all-commodities") );

    sek = new QTimer(this);
    connect( sek, SIGNAL(timeout()), this, SLOT(sekTick()) );

    action_queue = new QTimer(this);
    connect( action_queue, SIGNAL(timeout()), this, SLOT(actionTick()) );
    action_queue->start(100);

    view->setPage(page);

    view->show();

    loadStatics();
}
QString MainWindow::time()
{
    QDateTime now;
    now = QDateTime::currentDateTime();
    QString temp = QString(" at ") + now.toString();
    return temp;
}
template<typename T>
QString MainWindow::toText(T arg)
{
    QString temp;
    temp.setNum(arg);
    return temp;
}
void MainWindow::sekTick()
{
    page->runJavaScript(script_getData,tickCallback);

    if(tooMuchZeros > 100)
    {
        LOG += QString("error - 100 sek no answer (only zeros)") + time() + QString("\n");
        tooMuchZeros = 0;
        view->reload();
    }
    page->runJavaScript(instructions["godemomode"]);
    page->runJavaScript(instructions["dialogclass"],dialogclass);
    page->runJavaScript(instructions["isloginscreen"],isloginscreen);

    if(log_counter > 60)
    {
        log_counter = 0;
        QFile file;
        file.setFileName( QApplication::applicationDirPath() + "\\LOG.txt" );
        if( file.open(QIODevice::Append | QIODevice::Text) )
        {
            QTextStream in(&file);
            in<<LOG;

        }
        file.close();
        LOG.clear();
    }
    log_counter++;
}
void MainWindow::actionTick()
{
    if( !proceed.isEmpty() )
    {
        Proceed* proc = *(proceed.rbegin());
        action_queue->stop();
        (*(proc->function))(proc->argument);
        proceed.pop_back();
        delete proc;
    }
}
void MainWindow::showInstruments()
{
    foreach(Instrument* item, instruments)
    {
        item->show();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
    delete sek;

    foreach(Instrument* item, instruments)
    {
        delete item;
    }
    instruments.clear();
}

void MainWindow::on_execCastButton_clicked()
{
    ui->execCastButton->setEnabled(false);
    sek->start(980);
}
void MainWindow::loadScripts()
{
    script_getData = "";
    QFile file;
    file.setFileName( QApplication::applicationDirPath() + "\\getAllData.txt" );
    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        while ( !in.atEnd() )
        {
            script_getData += in.readLine() + QString("\n");
        }

    }
    file.close();
    script_open_sell = "";
    file.setFileName( QApplication::applicationDirPath() + "\\open_sell.txt" );
    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        while ( !in.atEnd() )
        {
            script_open_sell += in.readLine() + QString("\n");
        }

    }
    script_open_sell.replace("#QUANTITY","{ 10: 250, 11: 10, 12: 1000, 123: 5000, 119: 250, 2265: 10, 1462: 5000, 1460: 50, 1461 : 100, 431 : 1250 }");
    file.close();
    script_open_buy = "";
    file.setFileName( QApplication::applicationDirPath() + "\\open_buy.txt" );
    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        while ( !in.atEnd() )
        {
            script_open_buy += in.readLine() + QString("\n");
        }

    }
    script_open_buy.replace("#QUANTITY","{ 10: 250, 11: 10, 12: 1000, 123: 5000, 119: 250, 2265: 10, 1462: 5000, 1460: 50, 1461 : 100, 431 : 1250 }");
    file.close();
    script_close_close = "";
    file.setFileName( QApplication::applicationDirPath() + "\\close_close.txt" );
    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        while ( !in.atEnd() )
        {
            script_close_close += in.readLine() + QString("\n");
        }

    }
    file.close();
    ui->execCastButton->setEnabled(true);
    LOG += QString("scripts loaded") + time() + QString("\n");
}
void MainWindow::tickCallback(const QVariant & result)
{
    if( result.canConvert<QVariantList>() )
    {
        foreach(QVariant item, result.toList() )
        {

            QStringList instrument = item.toStringList();
            if(instrument.size() != 6)
            {
                LOG += QString("error - weird instrument frame (data<6)") + time() + QString("\n");
                qDebug()<<"not enought instrument data";
                continue;
            }
            Instrument* ins_p = findInstrument( instrument[0] );
            if(ins_p != nullptr)
            {
                ins_p->disabled = getBool(instrument[1]);
                ins_p->name = instrument[2];
                QString sell_raw = instrument[3];
                QString buy_raw = instrument[4];

                sell_raw.remove( QChar(' ') );
                sell_raw.remove( 0xA0 );
                sell_raw.replace( QChar(','), QChar('.') );
                buy_raw.remove( QChar(' ') );
                buy_raw.remove( 0xA0 );
                buy_raw.replace( QChar(','), QChar('.') );

                ins_p->actual_sell = sell_raw.toDouble();
                ins_p->actual_buy = buy_raw.toDouble();

                ins_p->buy.push_front(ins_p->actual_buy);
                ins_p->sell.push_front(ins_p->actual_sell);

                if(ins_p->buy.size() > 1)
                {
                    ins_p->buy_change.push_front(  (*(ins_p->buy.begin()))-(*(++(ins_p->buy.begin()))) );
                    ins_p->sell_change.push_front( (*(ins_p->sell.begin()))-(*(++(ins_p->sell.begin()))) );
                }else
                {
                    ins_p->buy_change.push_front(0.0);
                    ins_p->sell_change.push_front(0.0);
                }

                if(ins_p->buy.size() > 128)
                {
                    ins_p->buy.pop_back();
                    ins_p->sell.pop_back();
                }

                ins_p->positions = instrument[5].toInt();
            }
        }
    }else qDebug()<<"cannot convert to QVariantList";
    if( !areZeros( *(instruments.begin()) ) )
    {
        tooMuchZeros = 0;
        foreach(Instrument* ins, instruments)
        {
            makeDecision(ins);
        }
    }else
        tooMuchZeros++;
}
void MainWindow::loadStatics()
{
    if( !instruments.isEmpty() )
    {
        foreach(Instrument* item, instruments) delete item;
        instruments.clear();
    }
    if( !instructions.isEmpty() ) instructions.clear();
    settings->sync();

    QStringList categories( settings->childGroups() );
    QStringList values( settings->allKeys() );

    foreach(QString category, categories)
    {
        if(category == "ID")
        {
            QStringList insts = values.filter("ID/");
            foreach(QString instName, insts)
            {
                Instrument* instTemp = new Instrument();
                instTemp->id = settings->value(instName).toString();

                instruments.insert(instTemp->id,instTemp);
            }
        }
        if(category == "Instructions")
        {
            QStringList inst = values.filter("Instructions/");
            foreach(QString instName, inst)
            {
                QString value = settings->value(instName).toString();
                instructions.insert(instName.remove("Instructions/"),value);
            }
        }
    }
    foreach(Instrument* item, instruments)
    {
        if( !categories.contains(item->id) )
        {
            instruments.remove(item->id);
            delete item;
        }
    }
    foreach(Instrument* item, instruments)
    {
        item->buymoment = settings->value(item->id + "/" + "buymoment").toDouble();
        item->buyeye = settings->value(item->id + "/" + "buyeye").toInt();
        item->sellmoment = settings->value(item->id + "/" + "sellmoment").toDouble();
        item->selleye = settings->value(item->id + "/" + "selleye").toInt();
        item->close = settings->value(item->id + "/" + "close").toDouble();
    }
    LOG += QString("static data loaded") + time() + QString("\n");
    loadScripts();
}
bool MainWindow::getBool(const QString& str_arg)
{
    if(str_arg == "true") return true;
        else if(str_arg == "false") return false;
            else qDebug()<<"wrong bool string";
    return true;
}
Instrument* MainWindow::findInstrument(const QString id_arg)
{
    Instrument* ins_p = nullptr;
    if( !instruments.isEmpty() )
    {
        foreach(Instrument* item, instruments)
        {
            if( item->id == id_arg )
            {
                ins_p = item;
            }
        }
    }
    return ins_p;
}
Instrument::Instrument()
{
    id = "undefined";
    name = "undefined";
    actual_buy = 0.0;
    actual_sell = 0.0;
    disabled = true;
    positions = 0;

    buymoment = 0.0;
    sellmoment = 0.0;
    buyeye = 0;
    selleye = 0;

    inopentime = 0;
    bought = false;
    sell_bought = false;
    candelay = 0;
    sub_budget = 0.0;

    close = 0.0;
    open_value = 0.0;
}
Instrument::Instrument(const QString& id_arg)
{
    id = id_arg;
    name = "undefined";
    actual_buy = 0.0;
    actual_sell = 0.0;
    disabled = true;
    positions = 0;

    buymoment = 0.0;
    sellmoment = 0.0;
    buyeye = 0;
    selleye = 0;

    inopentime = 0;
    bought = false;
    sell_bought = false;
    candelay = 0;
    sub_budget = 0.0;

    close = 0.0;
    open_value = 0.0;
}
void Instrument::show()
{
    qDebug()<<"id = "<<id;
    qDebug()<<"name = "<<name;
    qDebug()<<"actual_buy = "<<actual_buy;
    qDebug()<<"actual_sell = "<<actual_sell;
    qDebug()<<"disabled = "<<disabled;
    qDebug()<<"positions = "<<positions;

    qDebug()<<"buymoment = "<<buymoment;
    qDebug()<<"sellmoment = "<<sellmoment;
    qDebug()<<"buyeye = "<<buyeye;
    qDebug()<<"selleye = "<<selleye;

    qDebug()<<"inopentime = "<<inopentime;
    qDebug()<<"bought = "<<bought;
    qDebug()<<"sell_bought = "<<sell_bought;
}
void MainWindow::performAction(Action action_arg, Instrument* ins_arg)
{
    Proceed* temp = new Proceed;
    temp->argument = ins_arg;
    switch(action_arg)
    {
    case Action::OPEN_SELL:
        temp->function = performSell;
        break;
    case Action::OPEN_BUY:
        temp->function = performBuy;
        break;
    case Action::CLOSE_CLOSE:
        temp->function = performClose;
        break;
    default:
        qDebug()<<"Wrong action performed";
        break;
    }
    proceed.push_front(temp);
}
void MainWindow::performSell(Instrument* ins_arg)
{
    QString script = script_open_sell;
    script.replace("#ID",ins_arg->id);
    LOG += QString("open sell perform ") + ins_arg->id + time() + QString("\n");
    page->runJavaScript(script);
    QTimer::singleShot( 100, static_this, SLOT(castSellStatus()) );
}
void MainWindow::performBuy(Instrument* ins_arg)
{
    QString script = script_open_buy;
    script.replace("#ID",ins_arg->id);
    LOG += QString("open buy perform ") + ins_arg->id + time() + QString("\n");
    page->runJavaScript(script);
    QTimer::singleShot( 100, static_this, SLOT(castBuyStatus()) );
}
void MainWindow::performClose(Instrument* ins_arg)
{
    QString script = script_close_close;
    script.replace("#ID",ins_arg->id);
    LOG += QString("close perform ") + ins_arg->id + time() + QString("\n");
    page->runJavaScript(script);
    QTimer::singleShot( 100, static_this, SLOT(castCloseStatus()) );
}
void MainWindow::buyCallback(const QVariant & result)
{
    if( (result.toString() == "break")||(result.toString() == "timeout") )
    {
        page->runJavaScript("open_buy_log;",captureLog);
        if(result.toString() != "break") page->runJavaScript("open_buy_id;",clearInstrument);
        page->runJavaScript("open_buy_result = \"unknown\";");
        LOG += QString("buyCallback break or timeout") + time() + QString("\n");
        action_queue->start(100);
    }else
        if( result.toString() == "success" )
        {
            LOG += QString("buy success") + time() + QString("\n");
            //page->runJavaScript("open_buy_id;",clearInstrument);
            page->runJavaScript("open_buy_result = \"unknown\";");
            action_queue->start(100);
        }else
             QTimer::singleShot( 200, static_this, SLOT(castBuyStatus()) );
}
void MainWindow::sellCallback(const QVariant & result)
{
    if( (result.toString() == "break")||(result.toString() == "timeout") )
    {
        page->runJavaScript("open_sell_log;",captureLog);
        if(result.toString() != "break") page->runJavaScript("open_sell_id;",clearInstrument);
        page->runJavaScript("open_sell_result = \"unknown\";");
        LOG += QString("sellCallback break or timeout") + time() + QString("\n");
        action_queue->start(100);
    }else
        if( result.toString() == "success" )
        {
            LOG += QString("sell success") + time() + QString("\n");
            //page->runJavaScript("open_sell_id;",clearInstrument);
            page->runJavaScript("open_sell_result = \"unknown\";");
            action_queue->start(100);
        }else
             QTimer::singleShot( 200, static_this, SLOT(castSellStatus()) );
}
void MainWindow::closeCallback(const QVariant & result)
{
    if( (result.toString() == "break")||(result.toString() == "timeout") )
    {
        page->runJavaScript("close_log;",captureLog);
        if(result.toString() != "timeout") qDebug()<<".";//page->runJavaScript("close_id;",clearInstrument);
        else
            {
                LOG += QString("closeCallback timeout") + time() + QString("\n");
                page->runJavaScript("close_id;",repeatCloseForInstrument);
            }
        LOG += QString("closeCallback break or timeout") + time() + QString("\n");
        page->runJavaScript("close_result = \"unknown\";");
        action_queue->start(100);
    }else
        if( result.toString() == "success" )
        {
            LOG += QString("close success") + time() + QString("\n");
            //page->runJavaScript("close_id;",clearInstrument);
            page->runJavaScript("close_balance;",closeBalance);
            page->runJavaScript("close_result = \"unknown\";");
            action_queue->start(100);
        }else
             QTimer::singleShot( 200, static_this, SLOT(castCloseStatus()) );
}
void MainWindow::closeBalance(const QVariant & result)
{
    if( result.canConvert<QString>() ) LOG += QString("close balance ") + result.toString() + time() + QString("\n");
}
void MainWindow::clearInstrument(const QVariant & result)
{
    Instrument* temp = findInstrument( result.toString() );
    LOG += QString("clear ") + temp->id + QString(" buy ") + toText(temp->actual_buy) + QString(" sell ") + toText(temp->actual_sell) + time() + QString("\n");
    if( temp != nullptr )
    {
        temp->bought = false;
        temp->sell_bought = false;
    }
}
void MainWindow::repeatCloseForInstrument(const QVariant & result)
{
    Instrument* temp = findInstrument( result.toString() );
    if(temp->candelay < 3)
    {
        performAction(Action::CLOSE_CLOSE,temp);
        temp->candelay++;
    }else
    {
        temp->candelay = 0;
    }
}
void MainWindow::captureLog(const QVariant & result)
{
    QString logs = "";
    if( result.canConvert<QVariantList>() )
    {
        foreach( QVariant item, result.toList() )
        {
            logs += item.toString() + QString(" ");
            qDebug()<<item.toString();
        }
    }
    LOG += QString("log captured ") + logs + time() + QString("\n");
}
void MainWindow::castBuyStatus()
{
    page->runJavaScript("open_buy_result;",buyCallback);
}
void MainWindow::castSellStatus()
{
    page->runJavaScript("open_sell_result;",sellCallback);
}
void MainWindow::castCloseStatus()
{
    page->runJavaScript("close_result;",closeCallback);
}
void MainWindow::on_execPauseButton_clicked()
{
    performAction(Action::OPEN_BUY,findInstrument("12"));
    performAction(Action::OPEN_SELL,findInstrument("11"));
    performAction(Action::CLOSE_CLOSE,findInstrument("11"));
    performAction(Action::CLOSE_CLOSE,findInstrument("12"));
}
double MainWindow::sum_back(QList<double> &tab, const qint32 count)
{
    double sum = 0;
    QList<double>::iterator iter = tab.begin();
    for(qint32 i = 0;i<count;i++)
    {
        sum += (*iter);
        iter++;
    }
    return sum;
}
bool MainWindow::areZeros(Instrument* instrument) //True means bad
{
    QList<double>::iterator iter = instrument->buy.begin();
    qint32 limit = 100;

    if( instrument->buy.size() > 50 ) limit = 50;
        else limit = instrument->buy.size();
    for(qint32 i = 0;i<limit;i++)
    {
        if( (*iter) == 0.00 )
        {
            return true;
        }
        iter++;
    }
    return false;
}
void MainWindow::makeDecision(Instrument* instrument)
{
    if( instrument->buy.size() > 50)
    {
        double answer = 0;

        if(!(instrument->bought) && !(instrument->sell_bought))
        {
            answer = sum_back(instrument->buy_change,instrument->buyeye);
            if( answer > (instrument->buymoment)  )
            {
                performAction(Action::OPEN_BUY,instrument);

                instrument->bought = true;
                instrument->inopentime = 0;
                instrument->open_value = (instrument->actual_buy) * instrumentQuantity(instrument);
            }else
            {
                answer = sum_back(instrument->sell_change,instrument->buyeye);
                if( answer < -(instrument->buymoment)  )
                {
                    performAction(Action::OPEN_SELL,instrument);

                    instrument->sell_bought = true;
                    instrument->inopentime = 0;
                    instrument->open_value = (instrument->actual_sell) * instrumentQuantity(instrument);
                }
            }
        }else
            {
               /* if((instrument->sell_bought)) answer = sum_back(instrument->buy_change, instrument->selleye);
                    else
                        if((instrument->bought)) answer = sum_back(instrument->sell_change, instrument->selleye);
                */
                qint32 decision = 0;
                for(qint8 ii=0;ii<21;ii++)
                {
                    if( ii<(instrument->buy.size()) )
                    {
                        if(instrument->sell_bought)
                        {
                            if( (instrument->buy[ii] - instrument->buy[ii+1]) <= 0.0 )
                            {
                                decision++;
                            }
                        }else
                        {
                            if( (instrument->sell[ii] - instrument->sell[ii+1]) >= 0.0 )
                            {
                                decision++;
                            }
                        }
                    }
                }

                if(decision> (21*0.65) )
                {
                    if(instrument->bought) instrument->open_value = instrument->actual_buy * instrumentQuantity(instrument);
                      else instrument->open_value = instrument->actual_sell * instrumentQuantity(instrument);
                }

                if( (instrument->inopentime > 8) && (instrument->bought) && ((instrument->actual_sell)*instrumentQuantity(instrument)-(instrument->open_value) < -(instrument->close))  )
                {
                    performAction(Action::CLOSE_CLOSE,instrument);
                    instrument->bought = false;
                    instrument->sell_bought = false;
                }
                else
                    if( (instrument->inopentime > 8) && (instrument->sell_bought) && ((instrument->actual_buy)*instrumentQuantity(instrument)-(instrument->open_value) > (instrument->close))  )
                    {
                        performAction(Action::CLOSE_CLOSE,instrument);
                        instrument->bought = false;
                        instrument->sell_bought = false;
                    }
            }

        if( (*( instrument->buy.begin()) ) == 0.00 )
        {
        }

    }
    instrument->inopentime++;
}
qint32 MainWindow::instrumentQuantity(Instrument* temp_arg)
{
    qint32 temp = temp_arg->id.toInt();
    switch(temp)
    {
    case 10:
        return 250;
    case 11:
        return 10;
    case 12:
        return 1000;
    case 123:
        return 5000;
    case 119:
        return 250;
    case 2265:
        return 10;
    case 1462:
        return 5000;
    case 1460:
        return 50;
    case 1461:
        return 100;
    case 431:
        return 1250;
    default:
        qDebug()<<"no such instrument with quantity";
    }
    return 10000;
}

void MainWindow::dialogclass(const QVariant & result)
{
    if( result.toString().contains("visible") )
    {
        LOG += QString("dialog occured") + time() + QString("\n");
        page->runJavaScript(instructions["dialogtext"],dialogtext);
        page->runJavaScript(instructions["dialogbuttontext"],dialogbuttontext);
        page->runJavaScript(instructions["dialogbuttonclick"]);
    }
}
void MainWindow::dialogtext(const QVariant & result)
{
    LOG += QString("dialog content ") + result.toString() + time() + QString("\n");
    qDebug()<<result.toString();
}
void MainWindow::dialogbuttontext(const QVariant & result)
{
    LOG += QString("dialog button text") + result.toString() + time() + QString("\n");
    qDebug()<<" BUTTON: "<<result.toString();
}
void MainWindow::isloginscreen(const QVariant & result)
{
    if( result.toInt() > 0 )
    {
        qDebug()<<"standard login screen";
        LOG += QString("standard login screen occured") + time() + QString("\n");
        page->runJavaScript(instructions["submitlogin"]);
    }
}
