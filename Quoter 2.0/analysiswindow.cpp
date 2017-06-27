#include "analysiswindow.h"
#include "ui_analysiswindow.h"
#include <QDebug>
#include <QTime>
#include <QtAlgorithms>
#include <QElapsedTimer>


AnalysisWindow::AnalysisWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalysisWindow)
{
    ui->setupUi(this);

    //ui->verticalSlider->sliderMoved();
    connect(ui->verticalSlider, SIGNAL( sliderReleased() ), this, SLOT( slider_changed() ));
    connect(ui->verticalSlider_2, SIGNAL( sliderReleased() ), this, SLOT( slider_changed_2() ));

    bought_flag = false;

    account = new AccountManager(1000,100);

}

void AnalysisWindow::slider_changed()
{
    value_place = tab_size * ui->verticalSlider->value() / (ui->verticalSlider->maximum() + 1 );
    ui->lcdNumber->display(value_place);
    ui->lcdNumber_4->display(buy[value_place]);
}

void AnalysisWindow::slider_changed_2()
{
    value_flush = tab_size * ui->verticalSlider_2->value() / (ui->verticalSlider_2->maximum() + 1 );
    ui->lcdNumber_3->display(value_flush);
    ui->lcdNumber_5->display(sell[value_flush]);
}

AnalysisWindow::~AnalysisWindow()
{
    delete ui;
    delete [] sell;
    delete [] buy;
    delete [] x_axis;
}

double AnalysisWindow::buy_instrument( const quint32& count, const double& actual_value)
{
    buy_value = count * actual_value;

    return buy_value;
}

double AnalysisWindow::sell_instrument(const quint32& count,const double& actual_value)
{
    return count * actual_value;
}

void AnalysisWindow::on_pushButton_clicked()
{

    QFile instrument;
    instrument.setFileName("E:/NoRegrets/NoRegrets/Data/1509srebro.hs");
    instrument.open(QIODevice::ReadOnly | QIODevice::Text);

    quint64 i = 0;
    while(!instrument.atEnd())
    {
        instrument.readLine();
        i++;
    }

    sell = new double[i];
    buy = new double[i];
    x_axis = new QTime[i];
    value_infront = new double[i];
    tab_size = i;

    series = new QLineSeries();
    series->setName("line");

    value = 0;
    actual_value = 0;
    double line_arg = 0;

    instrument.seek(0);
    i = 0;
    while(!instrument.atEnd())
    {
        QString line = instrument.readLine();
        line.remove("\n");

        line_arg = line.left(line.indexOf(' ')).toDouble();
        if(line_arg != 0) buy[i] = line_arg;
            else buy[i] = buy[i-1];

        if(i == 0) value = buy[i]*10;

       // if( (i>56000)&&(i<66000) )
            //if( 0 == (i%300) )
            series->append(i,buy[i]);

        line = line.remove(0,line.indexOf(' ')+1);

        line_arg = line.left(line.indexOf(' ')).toDouble();
        if(line_arg != 0) sell[i] = line_arg;
            else sell[i] = sell[i-1];

        line = line.remove(0,line.indexOf(' ')+1);
        x_axis[i] = QTime::fromString(line,"h:m:s");

        i++;

    }

    qDebug()<<"LOAD SAMPLE - "<<buy[20000]<<" "<<sell[20000];
    instrument.close();
    ui->pushButton->setEnabled(false);
}

void AnalysisWindow::on_pushButton_2_clicked()
{
  //  QFile analyze;
  //  analyze.setFileName("E:\\NoRegrets\\NoRegrets\\Data\\analyze.hs");
  //  analyze.open(QIODevice::WriteOnly | QIODevice::Text);

  //  QTextStream out(&analyze);

 /*   double* buy_diff = new double[tab_size];
    double* sell_diff = new double[tab_size];
    //double* pendulum_buy = new double[tab_size];
    actual_value = new double[tab_size];
    bool pend_flag = false;

    series2 = new QLineSeries();
    series2->setName("line2");


    double buy_stright_sum = 0;

    for(int i = 1;i<tab_size;i++)
    {
        buy_diff[i] = buy[i] - buy[i-1];
        sell_diff[i] = sell[i] - sell[i-1];
        actual_value[i] = buy[i]*10 - value;

        buy_stright_sum += buy_diff[i];

        //if( (i>56000)&&(i<66000) )
            //if( 0 == (i%300) )
                series2->append(i,buy_stright_sum);
       //if( (i>56000)&&(i<66000) )
            //if( 0 == (i%300) )
            {
                if( (buy[i] - buy[i-1])>0 ) pend_flag = true;
                else
                {
                    if( (buy[i] - buy[i-1]) != 0 ) pend_flag = false;
                }

                if(pend_flag == true)
                {
                    series3->append(i,buy[i]);
                }
            }
       // if( (i>56000)&&(i<66000) )
            //if( 0 == (i%300) )
               // series2->append(i,actual_value[i]);

        /* QString out_line;
        out_line.setNum(buy_diff[i]);
        out<<out_line;
        out<<" ";
        out_line.setNum(sell_diff[i]);
        out<<out_line;
        out<<"\n"; */

   // }

    //analyze.close();

   /* ui->pushButton_2->setEnabled(false);*/
}

void AnalysisWindow::on_pushButton_3_clicked()
{
    /*
    series->append(0, 6);
    series->append(2, 4);
    series->append(10, 5);*/

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Buy");
    chart->createDefaultAxes();
    chart->axisX()->setRange(0,tab_size);
    //chart->axisY()->setRange(45, 47);
    ui->widget->setChart(chart);

   /* QChart *chart2 = new QChart();
    chart2->legend()->hide();
    chart2->addSeries(series4);
    chart2->setTitle("Test");
    chart2->createDefaultAxes();
    //chart2->
    chart2->axisX()->setRange(0,tab_size);
    //chart2->axisY()->setRange(-0.10, 0.10);
    ui->widget_2->setChart(chart2);*/

}

void AnalysisWindow::on_pushButton_4_clicked()
{
    for(int i = 1;i<tab_size;i++)
    {
        if( (!bought_flag) && (value_place == i) )
        {
            value_value = buy_instrument(50,buy[i]);
            bought_flag = true;
        }
        else if( (value_flush == i) && (bought_flag) )
        {
            value_value = sell_instrument(50,sell[i]);
            bought_flag = false;
        }
    }
    ui->lcdNumber_2->display(value_value);
}

AccountManager::AccountManager()
{

}

AccountManager::AccountManager(double value)
{
    account_value = value;
}

AccountManager::AccountManager(double value, quint32 proc)
{
    account_value = value;
    account_proc = proc;
}

double AccountManager::actual_value()
{
    return account_value;
}

bool AccountManager::open( quint32 id_temp, quint32 quantity_temp, double actual_value_temp, QString &log)
{
    if( !find(id_temp) )
    {
        OpenPosition* temp = new OpenPosition(id_temp,quantity_temp,actual_value_temp,account_proc);
        account_positions.append(temp);

        account_value -= quantity_temp * actual_value_temp / account_proc;

        log += "\n";
        log += "ACCOUNT:BOUGHT - ";
        QString val;
        val.setNum(account_value);
        log += val;
        log += "\n";
        //qDebug()<<"ACCOUNT:BOUGHT - "<<account_value;
        return true;
    }else
    {
        qDebug()<<"ERROR: POSITION HAS BEEN OPENNED BEFORE";
        return false;
    }

}

bool AccountManager::close( quint32 id_temp,double actual_value_temp, QString &log)
{
    OpenPosition* temp;

    if( find(id_temp,temp) )
    {
        account_value += (actual_value_temp * temp->quantity) - temp->total_value + temp->actual_value;
        account_positions.removeOne(temp);
        delete temp;

        log += "\n";
        log += "ACCOUNT:SOLD - ";
        QString val;
        val.setNum(account_value);
        log += val;
        log += "\n";
        //qDebug()<<"ACCOUNT:SOLD - "<<account_value;
        return true;
    }
    else
    {
        return false;
    }
}

bool AccountManager::find( quint32 id_temp)
{
    if( !account_positions.isEmpty() )
    {
        foreach( OpenPosition* item, account_positions)
        {
            if( item->id == id_temp )
            {
                return true;
            }
        }
        return false;
    }else
        return false;
}

bool AccountManager::find( quint32 id_temp, OpenPosition* &item_ref)
{
    if( !account_positions.isEmpty() )
    {
        foreach( OpenPosition* item, account_positions)
        {
            if( item->id == id_temp )
            {
                item_ref = item;
                return true;
            }
        }
        return false;
    }else
        return false;
}

OpenPosition::OpenPosition()
{
    id = 0;
    quantity = 0;
    actual_value = 0;
    total_value = 0;
}

OpenPosition::OpenPosition( quint32 id_temp, quint32 quantity_temp, double actual_value_temp, qint32 proc_temp)
{
    id = id_temp;
    quantity = quantity_temp;
    actual_value = actual_value_temp * quantity_temp / proc_temp;
    total_value = actual_value_temp * quantity;
    proc = proc_temp;
}

void AnalysisWindow::on_pushButton_5_clicked()
{
    //tab_size
    series4 = new QScatterSeries();
    series4->setName("scatter");
    series4->setMarkerSize(3.0);
    series4->setColor(Qt::GlobalColor::black);
    series4->setBorderColor(Qt::GlobalColor::black);

    bool bought = false;
    bool sell_bought = false;
    //double last_four;
    //double last_ten;
    //double last_thirty;
    qint32 time;

    double* sell_diff = new double[tab_size];
    double* buy_diff = new double[tab_size];
    sell_diff[0] = 0;
    buy_diff[0] = 0;
    for(int i = 1;i<tab_size;i++)
    {
        sell_diff[i] = sell[i] - sell[i-1];
        buy_diff[i] = buy[i] - buy[i-1];
    }

    QString rary;
    double answer = 0;
    for(int i = 0;i<tab_size;i++)
    {
        if( i > 30)
        {
            if(!bought && !sell_bought)
            {
                answer = sum_back(buy_diff,i,3);
                if( (answer) > 0.39  )
                {
                    account->open(10,10,buy[i],rary);
                    series4->append(i,answer);
                    bought = true;
                    time = i;
                }else
                    if( (answer) < -0.39  )
                    {
                        account->open(10,10,sell[i],rary);
                        series4->append(i,answer);
                        sell_bought = true;
                        time = i;
                    }
            }

            if(bought || sell_bought)
            {
                if((i-time) < 10) answer = sum_back(buy_diff,i,(i-time));
                    else answer = sum_back(buy_diff,i,10);
                if( ((i-time) > 10) && bought && answer < -0.44  )
                {
                    series4->append(i,answer);
                    account->close(10,sell[i],rary);

                    qDebug()<<" "<<(i-time)<<" "<<account->actual_value()<<" "<<answer;
                    bought = false;
                    sell_bought = false;
                }
                else if( ((i-time) > 10) && sell_bought && answer > 0.44  )
                {
                    series4->append(i,answer);
                    account->close(10,buy[i],rary);

                    qDebug()<<" "<<(i-time)<<" "<<account->actual_value()<<" "<<answer;
                    bought = false;
                    sell_bought = false;
                }
            }
        rary.clear();
        }else
        {

        }
    }
    account->close(10,sell[tab_size-1],rary);
    qDebug()<<account->actual_value();
}

double AnalysisWindow::sum_back(double* tab, qint32 index, qint32 count)
{
    double sum = 0;
    for(qint32 i = 0;i<count;i++)
    {
        sum += tab[index-i];
    }
    return sum;
}

bool goalsComparison(const Goal &s1, const Goal &s2)
{
    return s1.goal0 > s2.goal0;
}

void AnalysisWindow::on_pushButton_6_clicked()
{
    //tab_size
    series4 = new QScatterSeries();
    series4->setName("scatter");
    series4->setMarkerSize(3.0);
    series4->setColor(Qt::GlobalColor::black);
    series4->setBorderColor(Qt::GlobalColor::black);

    bool bought = false;
    bool sell_bought = false;
    double answer = 0;
    qint32 count = 0;
    qint32 time;
    QElapsedTimer check;
    check.start();

    double sell_t;
    double buy_t;
    double* goal = new double[10];
    QList<Goal> lastGoals;
    for(qint32 i=0;i<10;i++) goal[i] = 0;

    Goal temps;
    temps.goal0 = 0;
    temps.goal1 = 0;
    temps.goal2 = 0;
    temps.goal3 = 0;
    temps.goal4 = 0;
    temps.goal5 = 0;
    lastGoals.push_front(temps);

    double* sell_diff = new double[tab_size];
    double* buy_diff = new double[tab_size];
    sell_diff[0] = 0;
    buy_diff[0] = 0;
    QString rary;
    for(qint32 i = 1;i<tab_size;i++)
    {
        sell_diff[i] = sell[i] - sell[i-1];
        buy_diff[i] = buy[i] - buy[i-1];
    }
    for(qint32 sell_eye = 2;sell_eye<19;sell_eye++)
    {
    for(qint32 buy_eye = 2;buy_eye<17;buy_eye++)
    {
    for(qint32 buy_i = 2;buy_i<41;buy_i++)
    {
        for(qint32 sell_i = 2;sell_i<41;sell_i++)
        {
            account->account_value = 1000;
            answer = 0;
            bought = false;
            sell_bought = false;
            count = 0;
            buy_t = double(buy_i) * 0.001;
            sell_t = double(sell_i) * 0.001;
            for(int i = 0;i<(tab_size-3);i++)
            {
                if( i > 30)
                {
                    if(!bought && !sell_bought)
                    {
                        answer = sum_back(buy_diff,i,buy_eye);
                        if( (answer) > buy_t  )
                        {
                            account->open(10,1000,buy[i+3],rary);
                            //qDebug()<<"OPENED BUY VALUE- "<<buy[i+3]<<" "<<i<<" >>"<<x_axis[i+3].toString();
                            bought = true;
                            time = i;
                            count++;
                        }else
                        {
                            answer = sum_back(sell_diff,i,buy_eye);
                            if( (answer) < -buy_t  )
                            {
                                account->open(10,1000,sell[i+3],rary);
                                //qDebug()<<"OPENED SELL VALUE- "<<sell[i+3]<<" "<<i<<" >>"<<x_axis[i+3].toString();
                                sell_bought = true;
                                time = i;
                                count++;
                            }
                        }
                    }else
                    {
                        if((i-time) < 8)
                        {
                            if(bought) answer = sum_back(sell_diff,i,(i-time));
                                else
                                     answer = sum_back(buy_diff,i,(i-time));
                        }
                        else
                            {
                                if(sell_bought) answer = sum_back(buy_diff,i,sell_eye);
                                    else
                                         answer = sum_back(sell_diff,i,sell_eye);
                            }
                        if( ((i-time) > 8) && bought && answer < -sell_t  )
                        {
                            account->close(10,sell[i+3],rary);
                            //qDebug()<<"CLOSED BUY VALUE- "<<sell[i+3]<<" TIME- "<<((i-time))<<" "<<i<<" >>"<<x_axis[i+3].toString();
                            bought = false;
                            sell_bought = false;
                        }
                        else if( ((i-time) > 8) && sell_bought && answer > sell_t  )
                        {
                            account->close(10,buy[i+3],rary);
                            //qDebug()<<"CLOSED SELL VALUE- "<<buy[i+3]<<" TIME- "<<((i-time))<<" "<<i<<" >>"<<x_axis[i+3].toString();
                            bought = false;
                            sell_bought = false;
                        }
                    }
                rary.clear();
                }else
                {

                }
            }
            account->close(10,sell[tab_size-1],rary);
                if( (count<40) && (account->account_value > lastGoals.back().goal0) )
                {
                    //goal[0] = account->account_value;
                    //goal[1] = count;
                    //goal[2] = sell_t;
                    //goal[3] = buy_t;
                    //goal[4] = buy_eye;
                    //goal[5] = sell_eye;

                    Goal temp;
                    temp.goal0 = account->account_value;
                    temp.goal1 = count;
                    temp.goal2 = sell_t;
                    temp.goal3 = buy_t;
                    temp.goal4 = buy_eye;
                    temp.goal5 = sell_eye;
                    lastGoals.push_front(temp);
                    if(lastGoals.size() > 100) lastGoals.pop_back();

                    qSort(lastGoals.begin(),lastGoals.end(),goalsComparison);
                }
        }
    }
    //qDebug()<<"MICROPART";
    }
    //qDebug()<<"SECTION "<<(sell_eye-2)<<"/12";
    qDebug()<<"PART";
    }
    //qDebug()<<goal[0]<<" - COUNT- "<<goal[1]<<" SELL_MOMENTUM- "<<goal[2]<<" BUY_MOMENTUM- "<<goal[3]<<" BUY_EYE- "<<goal[4]<<" SELL_EYE-"<<goal[5];
    qDebug()<<"LIST:";
    foreach(Goal item,lastGoals)
    {
        qDebug()<<(item.goal0-1000)<<" - COUNT- "<<item.goal1<<" SELL_MOMENTUM- "<<item.goal2<<" BUY_MOMENTUM- "<<item.goal3<<" BUY_EYE- "<<item.goal4<<" SELL_EYE-"<<item.goal5;
    }
    qDebug()<<"FINISHED IN "<<check.elapsed()<<"ms.";
}
