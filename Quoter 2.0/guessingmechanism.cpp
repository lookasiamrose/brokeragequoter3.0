#include "guessingmechanism.h"
#include <QFile>
#include "instrument.h"

QString GuessingMechanism::js_result;
bool GuessingMechanism::flag;
Browser* GuessingMechanism::plus500_pointer;
QList<CaptureStruct*> GuessingMechanism::commodities_pointers;
//QElapsedTimer GuessingMechanism::test;
//////////////////////////////////////////////////////////
bool GuessingMechanism::goldEnabled = true;
bool GuessingMechanism::silverEnabled = true;
bool GuessingMechanism::gasEnabled = true;
bool GuessingMechanism::bawelnaEnabled = true;
bool GuessingMechanism::kakaoEnabled = true;
bool GuessingMechanism::miedzEnabled = true;

QString GuessingMechanism::log_string;
QTime GuessingMechanism::actualmoment;

quint8 GuessingMechanism::gold_opens = 0;
quint8 GuessingMechanism::silver_opens = 0;
quint8 GuessingMechanism::gas_opens = 0;
quint8 GuessingMechanism::check_opens = 0;
quint8 GuessingMechanism::bawelna_opens = 0;
quint8 GuessingMechanism::kakao_opens = 0;
quint8 GuessingMechanism::miedz_opens = 0;

GuessingMechanism::GuessingMechanism(QObject *parent) : QObject(parent)
{
    {   //Loading JQuery library
        QFile jquery_file;
        jquery_file.setFileName(":/JQuery/jquery-3.0.0.min.js");
        jquery_file.open(QIODevice::ReadOnly);
        jquery = jquery_file.readAll();
        jquery_file.close();
    }


    {
        QFile instrument_id_file;

        instrument_id_file.setFileName("C:\\Users\\lukasz.jamroz\\Documents\\Qt Projects\\NoRegrets\\build-NoRegrets-Desktop_Qt_5_7_0_MSVC2013_64bit-Release\\release\\Data\\instruments.id");
        instrument_id_file.open(QIODevice::ReadOnly | QIODevice::Text);
        qDebug()<<"t";
        while(!instrument_id_file.atEnd())
        {
            QString instrument_id = instrument_id_file.readLine();
            instrument_id.remove("\n");
            InstrumentLibrary* commodity = new InstrumentLibrary( instrument_id.toInt() );
            ///////////////////////////////////////////////////////////////////
           /* if( instrument_id.toInt() == 11 ) object = commodity;
            if( instrument_id.toInt() == 12 ) object_silver = commodity;
            if( instrument_id.toInt() == 123 ) object_gas = commodity;
            if( instrument_id.toInt() == 119 ) object_bawelna = commodity;
            if( instrument_id.toInt() == 2265 ) object_kakao = commodity;
            if( instrument_id.toInt() == 1462 ) object_miedz = commodity;*/
            qDebug()<<instrument_id.toInt();
            //qDebug()<<commodity.buy;
            //window->connectInstrument( &commodity );



            commodities.push_back(commodity);

        }
        window->connectInstrument( &commodities );

        instrument_id_file.close();

    }

    id_click = 10;
    back = false;
    time_me = 0;

    account = new AccountManager(1000,100);

    //bought = false;
    //sell_bought = false;
    block = false;

    queue_tick = new QTimer(this);
    connect(queue_tick, SIGNAL(timeout()), this, SLOT(queue_move()));
    queue_tick->start(750);
    ///////////////////////////////////////////////
    goldEnabled = true;
    silverEnabled = true;
    gasEnabled = true;
    bawelnaEnabled = true;
    kakaoEnabled = true;
    miedzEnabled = true;
}

GuessingMechanism::~GuessingMechanism()
{
    delete tick;
    delete queue_tick;
}

void GuessingMechanism::connectBrowser(Browser* browser_temp)
{
    plus500_pointer = browser_temp;
}

void GuessingMechanism::connectConfigs(Config* config_temp)
{
    ///////////////////////////////////////////////
    configs = config_temp;
    gold = new InstrumentStats();
    configs->getGold(gold);

    silver = new InstrumentStats();
    configs->getSilver(silver);

    gas = new InstrumentStats();
    configs->getGas(gas);

    bawelna = new InstrumentStats();
    configs->getBawelna(bawelna);

    kakao = new InstrumentStats();
    configs->getKakao(kakao);

    miedz = new InstrumentStats();
    configs->getMiedz(miedz);

}


void GuessingMechanism::start()
{
    chosen = gold;

    tick = new QTimer(this);
    connect(tick, SIGNAL(timeout()), this, SLOT(updateInstruments()));
    tick->start(1000);

    time_link.start();

}

void GuessingMechanism::capture(const QVariant &result)
{
    js_result = result.toString();
    (*commodities_pointers.begin())->commodity->storeAttribute(js_result,(*commodities_pointers.begin())->attribute );
    if( ((*commodities_pointers.begin())->attribute) == Attribute::SELL)
    {
        (*commodities_pointers.begin())->commodity->addToHistory();

        lastData* last = new lastData();
        last->buy = (*commodities_pointers.begin())->commodity->buy;
        last->sell = (*commodities_pointers.begin())->commodity->sell;

        (*commodities_pointers.begin())->commodity->lastBuys.push_front(last);

        if( (*commodities_pointers.begin())->commodity->lastBuys.size() > 1 )
        {
            QList<lastData*>::iterator beg = (*commodities_pointers.begin())->commodity->lastBuys.begin();
            QList<lastData*>::iterator iter = beg;
            iter++;
            last->sell_diff = (*beg)->sell - (*iter)->sell;
            last->buy_diff  = (*beg)->buy  - (*iter)->buy;
        }

        //if( (*commodities_pointers.begin())->commodity->id == 11 )
            //qDebug()<<last->buy<<" "<<last->sell<<" "<<last->sell_diff<<" "<<last->buy_diff;

        if( (*commodities_pointers.begin())->commodity->lastBuys.size() > 60 )
        {
            lastData* shortage = (*commodities_pointers.begin())->commodity->lastBuys.back();
            (*commodities_pointers.begin())->commodity->lastBuys.pop_back();
            delete shortage;
        }

    }
    commodities_pointers.pop_front();
    //qDebug()<<test.elapsed();
}

void GuessingMechanism::logoutalert(const QVariant &result)
{
    //qDebug()<<result.typeName();
    if( "Tak" == result.toString() )
    {
        //qDebug()<<result.toString();
        plus500_pointer->page()->runJavaScript("$(\"div[id='alertDialogDiv']\").find('.win-contentdialog-commands').find('button')[1].click();");
    }
}

void GuessingMechanism::choosedemo(const QVariant &result)
{
    //qDebug()<<result.typeName();
    if("Tryb demo" == result.toString())
    {
        //qDebug()<<result.toString();
        plus500_pointer->page()->runJavaScript("$(\"div[class='demo-real-buttons']\").find('button')[1].click();");
    }
}

///////////////////////////////////////////////
void GuessingMechanism::isGasDisabled(const QVariant &result)
{
    if(result == "disabled")
    {
        gasEnabled = false;
        log_string += " GAS DISABLED ";
    }
    else gasEnabled = true;
}

void GuessingMechanism::isMiedzDisabled(const QVariant &result)
{
    if(result == "disabled")
    {
        miedzEnabled = false;
        log_string += " MIEDZ DISABLED ";
    }
    else miedzEnabled = true;
}

void GuessingMechanism::isKakaoDisabled(const QVariant &result)
{
    if(result == "disabled")
    {
        kakaoEnabled = false;
        log_string += " KAKAO DISABLED ";
    }
    else kakaoEnabled = true;
}

void GuessingMechanism::isBawelnaDisabled(const QVariant &result)
{
    if(result == "disabled")
    {
        bawelnaEnabled = false;
        log_string += " BAWELNA DISABLED ";
    }
    else bawelnaEnabled = true;
}

void GuessingMechanism::isGoldDisabled(const QVariant &result)
{
    if(result == "disabled")
    {
        goldEnabled = false;
        log_string += " GOLD DISABLED ";
    }
    else goldEnabled = true;
}

void GuessingMechanism::isSilverDisabled(const QVariant &result)
{
    if(result == "disabled")
    {
        silverEnabled = false;
        log_string += " SILVER DISABLED ";
    }
    else silverEnabled = true;
}

void GuessingMechanism::dialogCapture(const QVariant &result)
{
    if( (result == "OK")||(result == "Tak") )
    {
        plus500_pointer->page()->runJavaScript("$( 'div[role=\"dialog\"] div.dialog-container > ' ).eq(0).text();",dialogContent);
        actualmoment = QTime::currentTime();
        plus500_pointer->page()->runJavaScript("$( 'div[role=\"dialog\"] button' ).eq(1).click();");
        log_string += "\n";
        log_string += "DIALOG OCCURED at ";
        log_string += actualmoment.toString();
        log_string += " CONTENT: ";
    }
}

void GuessingMechanism::goldPositions(const QVariant &result)
{
    check_opens++;
    gold_opens = result.toInt();
}

void GuessingMechanism::silverPositions(const QVariant &result)
{
    check_opens++;
    silver_opens = result.toInt();
}

void GuessingMechanism::gasPositions(const QVariant &result)
{
    check_opens++;
    gas_opens = result.toInt();
}

void GuessingMechanism::bawelnaPositions(const QVariant &result)
{
    check_opens++;
    bawelna_opens = result.toInt();
}

void GuessingMechanism::kakaoPositions(const QVariant &result)
{
    check_opens++;
    kakao_opens = result.toInt();
}

void GuessingMechanism::miedzPositions(const QVariant &result)
{
    check_opens++;
    miedz_opens = result.toInt();
}

void GuessingMechanism::getValue(const QVariant &result)
{
    log_string += " ACCOUNT: ";
    log_string += result.toString();
    log_string += "\n";
}

void GuessingMechanism::dialogContent(const QVariant &result)
{
    log_string += result.toString();
    log_string += "\n";

    if( result.toString().contains("zmieniona") )
    {
        plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"11\"] div.positions.win-repeater.win-disposable >' ).length;",goldPositions);
        plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"12\"] div.positions.win-repeater.win-disposable >' ).length;",silverPositions);
        plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"123\"] div.positions.win-repeater.win-disposable >' ).length;",gasPositions);
    }
}

void GuessingMechanism::updateInstruments()
{
    QString js_code;
    //test.restart();

    ////////////////////////////////////////////////
    /*
    plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"11\"] button' ).eq(0).attr('disabled');",isGoldDisabled);
    plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"12\"] button' ).eq(0).attr('disabled');",isSilverDisabled);
    plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"123\"] button' ).eq(0).attr('disabled');",isGasDisabled);
    plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"119\"] button' ).eq(0).attr('disabled');",isBawelnaDisabled);
    plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"2265\"] button' ).eq(0).attr('disabled');",isKakaoDisabled);
    plus500_pointer->page()->runJavaScript("$( 'div[data-instrument-id=\"1462\"] button' ).eq(0).attr('disabled');",isMiedzDisabled);

*/
    plus500_pointer->page()->runJavaScript("$( 'div[role=\"dialog\"] button' ).eq(1).text();", dialogCapture);

    foreach (InstrumentLibrary* item, commodities)
    {
        js_code = item->generateCode(Attribute::BUY);
        CaptureStruct* temporary = new CaptureStruct();
        temporary->commodity = item;
        temporary->attribute = Attribute::BUY;
        commodities_pointers.push_back(temporary);
        plus500_pointer->page()->runJavaScript( js_code, capture);

        js_code = item->generateCode(Attribute::SELL);
        CaptureStruct* temporary_A = new CaptureStruct();
        temporary_A->commodity = item;
        temporary_A->attribute = Attribute::SELL;
        commodities_pointers.push_back(temporary_A);
        plus500_pointer->page()->runJavaScript( js_code, capture);

        js_code = item->generateCode(Attribute::NAME);
        CaptureStruct* temporary_B = new CaptureStruct();
        temporary_B->commodity = item;
        temporary_B->attribute = Attribute::NAME;
        commodities_pointers.push_back(temporary_B);
        plus500_pointer->page()->runJavaScript( js_code, capture);


    }
    //qDebug()<<block;
    time_me++;

    if(check_opens > 2)
    {
        check_opens = 0;
        log_string += "\n DIALOG STAWKA ZMIENIONA RESPONSE \n";
        if(object->bought || object->sell_bought)
        {
            if(gold_opens == 0)
            {
                queue.append( configs->toInfo(chosen->id) );
                if( object->bought ) queue.append( configs->prepareBuy() );
                    else queue.append( configs->prepareSell() );
                queue.append( configs->inputQuantity(chosen->qty) );
                queue.append( configs->confirmTrade() );
                log_string += "\n =GOLD OPEN SOLUTION \n";
            }else
                log_string += "\n =GOLD OPEN BUT NO ACTION \n";

        }else
        {
            if(gold_opens > 0)
            {
                queue.append( configs->prepareClose(chosen->id) );
                queue.append( configs->confirmClose() );
                log_string += "\n =GOLD CLOSE SOLUTION \n";
            }else
                log_string += "\n =GOLD CLOSE BUT NO ACTION \n";
        }

        if(object_silver->bought || object_silver->sell_bought)
        {
            if(silver_opens == 0)
            {
                queue.append( configs->toInfo(silver->id) );
                if( object_silver->bought ) queue.append( configs->prepareBuy() );
                    else queue.append( configs->prepareSell() );
                queue.append( configs->inputQuantity(silver->qty) );
                queue.append( configs->confirmTrade() );
                log_string += "\n =SILVER OPEN SOLUTION \n";
            }else
                log_string += "\n =SILVER OPEN BUT NO ACTION \n";
        }
        else
        {
            if(silver_opens > 0)
            {
                queue.append( configs->prepareClose(silver->id) );
                queue.append( configs->confirmClose() );
                log_string += "\n =SILVER CLOSE SOLUTION \n";
            }else
                log_string += "\n =SILVER CLOSE BUT NO ACTION \n";
        }

        if(object_gas->bought || object_gas->sell_bought)
        {
            if(gas_opens == 0)
            {
                queue.append( configs->toInfo(gas->id) );
                if( object_gas->bought ) queue.append( configs->prepareBuy() );
                    else queue.append( configs->prepareSell() );
                queue.append( configs->inputQuantity(gas->qty) );
                queue.append( configs->confirmTrade() );
                log_string += "\n =GAS OPEN SOLUTION \n";
            }else
                log_string += "\n =GAS OPEN BUT NO ACTION \n";
        }
        else
        {
            if(gas_opens > 0)
            {
                queue.append( configs->prepareClose(gas->id) );
                queue.append( configs->confirmClose() );
                log_string += "\n =GAS CLOSE SOLUTION \n";
            }else
                log_string += "\n =GAS CLOSE BUT NO ACTION \n";
        }
        if(object_bawelna->bought || object_bawelna->sell_bought)
        {
            if(bawelna_opens == 0)
            {
                queue.append( configs->toInfo(bawelna->id) );
                if( object_bawelna->bought ) queue.append( configs->prepareBuy() );
                    else queue.append( configs->prepareSell() );
                queue.append( configs->inputQuantity(bawelna->qty) );
                queue.append( configs->confirmTrade() );
                log_string += "\n =BAWELNA OPEN SOLUTION \n";
            }else
                log_string += "\n =BAWELNA OPEN BUT NO ACTION \n";
        }
        else
        {
            if(bawelna_opens > 0)
            {
                queue.append( configs->prepareClose(bawelna->id) );
                queue.append( configs->confirmClose() );
                log_string += "\n =BAWELNA CLOSE SOLUTION \n";
            }else
                log_string += "\n =BAWELNA CLOSE BUT NO ACTION \n";
        }
        if(object_kakao->bought || object_kakao->sell_bought)
        {
            if(kakao_opens == 0)
            {
                queue.append( configs->toInfo(kakao->id) );
                if( object_kakao->bought ) queue.append( configs->prepareBuy() );
                    else queue.append( configs->prepareSell() );
                queue.append( configs->inputQuantity(kakao->qty) );
                queue.append( configs->confirmTrade() );
                log_string += "\n =KAKAO OPEN SOLUTION \n";
            }else
                log_string += "\n =KAKAO OPEN BUT NO ACTION \n";
        }
        else
        {
            if(kakao_opens > 0)
            {
                queue.append( configs->prepareClose(kakao->id) );
                queue.append( configs->confirmClose() );
                log_string += "\n =KAKAO CLOSE SOLUTION \n";
            }else
                log_string += "\n =KAKAO CLOSE BUT NO ACTION \n";
        }
        if(object_miedz->bought || object_miedz->sell_bought)
        {
            if(miedz_opens == 0)
            {
                queue.append( configs->toInfo(miedz->id) );
                if( object_miedz->bought ) queue.append( configs->prepareBuy() );
                    else queue.append( configs->prepareSell() );
                queue.append( configs->inputQuantity(miedz->qty) );
                queue.append( configs->confirmTrade() );
                log_string += "\n =MIEDZ OPEN SOLUTION \n";
            }else
                log_string += "\n =MIEDZ OPEN BUT NO ACTION \n";
        }
        else
        {
            if(miedz_opens > 0)
            {
                queue.append( configs->prepareClose(miedz->id) );
                queue.append( configs->confirmClose() );
                log_string += "\n =MIEDZ CLOSE SOLUTION \n";
            }else
                log_string += "\n =MIEDZ CLOSE BUT NO ACTION \n";
        }
    }

    //qDebug()<<silverEnabled;
   /* if(!areZeros()) //LOOKOUT ZEROS IN MEANTIME
    {   ////////////////////////////////////////////////////////////////
        if(goldEnabled) makeDecision(chosen,object);
        if(silverEnabled) makeDecision(silver,object_silver);
        if(gasEnabled) makeDecision(gas,object_gas);      
        if(bawelnaEnabled) makeDecision(bawelna,object_bawelna);
        if(kakaoEnabled) makeDecision(kakao,object_kakao);
        if(miedzEnabled) makeDecision(miedz,object_miedz);


        silver->show();
        chosen->show();
        qDebug()<<object->lastBuys.size();
        qDebug()<<object_silver->lastBuys.size();
        qDebug()<<object->id;
        qDebug()<<object_silver->id;

    }
     else
    {
        log_string += "\n";
        log_string += "ZERO REPORTED";
        log_string += "\n";
    }*/

    if(back && (time_me>4))
    {
        QString tried = "$(\"div[id='sidebar']\").find('h3').find('button').click();";
        if(!block) plus500_pointer->page()->runJavaScript(tried);
        back = false;
    }

    if( time_link.elapsed() >= (1000*60*2) )
    {
        plus500_pointer->page()->runJavaScript("$( 'div[id=\"accountStatus\"] > ul > li ').eq(0).find('span').text();",getValue);
        foreach (InstrumentLibrary* item, commodities)
        {
            item->flush();
        }
        time_link.restart();
        QString top;
        QString a;
        top = "$(\"div[data-instrument-id='" + a.setNum(id_click) + "']\").find('.information').find('button').click();";
        if(!block) plus500_pointer->page()->runJavaScript(top);
        id_click++;
        if(id_click > 11) id_click = 10;
        back = true;
        time_me = 0;

        /*QFile log_file;
        log_file.setFileName("E:/Actual server/Dysk Google/ELEC/LOG.txt");
        log_file.open(QIODevice::Append | QIODevice::Text);
        QTextStream out(&log_file);
        QTime now = QTime::currentTime();
        out<<"\n"<<now.toString()<<log_string<<"\n";*/
        log_string.clear();
        //log_file.close();
    }
    if(!block) plus500_pointer->page()->runJavaScript("$(\"div[id='alertDialogDiv']\").find('.win-contentdialog-commands').find('button').get('1').innerHTML;",logoutalert);
    if(!block) plus500_pointer->page()->runJavaScript("$(\"div[class='demo-real-buttons']\").find('button').find('strong')[1].innerHTML;",choosedemo);
}

bool GuessingMechanism::areZeros()
{
    QList<lastData*>::iterator iter = object->lastBuys.begin();
    qint32 limit = 100;

    if( object->lastBuys.size() > 20 ) limit = 20;
        else limit = object->lastBuys.size();
    for(qint32 i = 0;i<limit;i++)
    {
        if( (*iter)->buy == 0.00 )
        {
            //qDebug()<<"TRUE";
            return true;
        }
        iter++;
    }
    //qDebug()<<"FALSE";
    return false;
}

void GuessingMechanism::connectWindow(InstrumentsWindow* window_temp)
{
    window = window_temp;
}

void GuessingMechanism::connectParent(MainWindow* window_temp)
{
    mwindow = window_temp;
}

void GuessingMechanism::queue_move()
{
    if( !queue.isEmpty() )
    {
        if( (*(queue.begin())) != "PAUSE" ) plus500_pointer->page()->runJavaScript( *(queue.begin()) );
        queue.pop_front();
        if( queue.isEmpty() ) block = false;
    }else
    {
        block = false;
    }
}

void GuessingMechanism::externalToQueue(QString temp)
{
    queue.append(temp);
    block = true;
}

void GuessingMechanism::makeDecision(const InstrumentStats* chosen_arg,InstrumentLibrary* object_arg)
{
    if( object_arg->lastBuys.size() > 40)
    {
        double answer = 0;
        QTime now;
        QString lb;
        if(!(object_arg->bought) && !(object_arg->sell_bought))
        {
            answer = sum_back(object_arg->lastBuys,chosen_arg->buy_eye,true);
            if( answer > (chosen_arg->buy_momentum)  )
            {
                account->open(chosen_arg->id,chosen_arg->qty, (*(object_arg->lastBuys.begin()))->buy, log_string ); ////
                queue.append( configs->toInfo(chosen_arg->id) );
                queue.append( configs->prepareBuy() );
                queue.append( configs->inputQuantity(chosen_arg->qty) );
                queue.append( configs->confirmTrade() );
                queue.append("PAUSE");
                queue.append("PAUSE");

                log_string += "\n";
                lb.setNum((*(object_arg->lastBuys.begin()))->buy);
                log_string += lb;
                log_string += " for ";
                lb.setNum(chosen_arg->id);
                log_string += lb;
                now = QTime::currentTime();
                log_string += " BUY OPEN ";
                log_string += " at ";
                log_string += now.toString();
                log_string += "\n";

                block = true;
                object_arg->bought = true;
                time = 0;
            }else
            {
                answer = sum_back(object_arg->lastBuys,chosen_arg->buy_eye,false);
                if( answer < -(chosen_arg->buy_momentum)  )
                {
                    account->open(chosen_arg->id,chosen_arg->qty, (*(object_arg->lastBuys.begin()))->sell, log_string ); ////
                    queue.append( configs->toInfo(chosen_arg->id) );
                    queue.append( configs->prepareSell() );
                    queue.append( configs->inputQuantity(chosen_arg->qty) );
                    queue.append( configs->confirmTrade() );
                    queue.append("PAUSE");
                    queue.append("PAUSE");

                    log_string += "\n";
                    lb.setNum((*(object_arg->lastBuys.begin()))->sell);
                    log_string += lb;
                    log_string += " for ";
                    lb.setNum(chosen_arg->id);
                    log_string += lb;
                    now = QTime::currentTime();
                    log_string += " SELL OPEN ";
                    log_string += " at ";
                    log_string += now.toString();
                    log_string += "\n";

                    object_arg->sell_bought = true;
                    block = true;
                    time = 0;
                }
            }
        }else
        {
            if( time < 8)
            {
                if((object_arg->bought)) answer = sum_back(object_arg->lastBuys, time, false);
                    else
                        if((object_arg->sell_bought)) answer = sum_back(object_arg->lastBuys, time, true);
            }
            else
                {
                    if((object_arg->sell_bought)) answer = sum_back(object_arg->lastBuys, chosen_arg->sell_eye, true);
                        else
                            if((object_arg->bought)) answer = sum_back(object_arg->lastBuys, chosen_arg->sell_eye, false);
                }

            if( (time > 8) && (object_arg->bought) && (answer < -(chosen_arg->sell_momentum))  )
            {
                account->close(chosen_arg->id, (*(object_arg->lastBuys.begin()))->sell, log_string ); ////
                queue.append( configs->prepareClose(chosen_arg->id) );

                log_string += "\n";
                lb.setNum((*(object_arg->lastBuys.begin()))->sell);
                log_string += lb;
                log_string += " for ";
                lb.setNum(chosen_arg->id);
                log_string += lb;
                now = QTime::currentTime();
                log_string += " BUY CLOSE ";
                log_string += " at ";
                log_string += now.toString();
                log_string += "\n";

                queue.append( configs->confirmClose() );
                queue.append("PAUSE");
                block = true;

                object_arg->bought = false;
                object_arg->sell_bought = false;
            }
            else
                if( (time > 8) && (object_arg->sell_bought) && (answer > (chosen_arg->sell_momentum))  )
                {
                    account->close(chosen_arg->id, (*(object_arg->lastBuys.begin()))->buy, log_string ); ////
                    queue.append( configs->prepareClose(chosen_arg->id) );

                    log_string += "\n";
                    lb.setNum((*(object_arg->lastBuys.begin()))->buy);
                    log_string += lb;
                    log_string += " for ";
                    lb.setNum(chosen_arg->id);
                    log_string += lb;
                    now = QTime::currentTime();
                    log_string += " SELL CLOSE ";
                    log_string += " at ";
                    log_string += now.toString();
                    log_string += "\n";

                    queue.append( configs->confirmClose() );
                    queue.append("PAUSE");
                    block = true;

                    object_arg->bought = false;
                    object_arg->sell_bought = false;
                }
        }

        if( (*(object_arg->lastBuys.begin()))->buy == 0.00 )
        {
            log_string += "\n";
            log_string += "ZERO IN MEANTIME";
            log_string += "\n";
        }

        time++;
        lb.clear();
    }
}

double GuessingMechanism::sum_back(QList<lastData*> &tab, qint32 count, bool buy)
{
    double sum = 0;
    QList<lastData*>::iterator iter = tab.begin();
    for(qint32 i = 0;i<count;i++)
    {
        if(buy) sum += (*iter)->buy_diff;
            else sum += (*iter)->sell_diff;
        iter++;
    }
    return sum;
}
