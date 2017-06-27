#include "mainwindow.h"
#include "guessingmechanism.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QApplication>
//#include <QWebEngineSettings>

QElapsedTimer MainWindow::test;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    plus500 = new Browser("https://app.plus500.com");
   // plus500_A = new Browser("https://app.plus500.com");

    instruments = new InstrumentsWindow();
    analysis = new AnalysisWindow();
    machine = new GuessingMechanism();

    connect( plus500, SIGNAL(loadFinished(bool)), this, SLOT(webpageLoaded(bool)));

   // QWebEngineSettings *mysets = plus500->page()->settings();

   // mysets->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls,true);
   // mysets->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls,true);
  //  mysets->setAttribute(QWebEngineSettings::PluginsEnabled,true);

    plus500->load( QUrl("https://app.plus500.com") );

    machine->connectBrowser(plus500);
    machine->connectWindow(instruments);

    instruments->connectBrowser(plus500);
    instruments->show();

    analysis->show();

    ui->setupUi(this);
    //ui->startMachineButton->setEnabled(false);


    {
        QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,"C:\\Users\\lukasz.jamroz\\Documents\\Qt Projects\\NoRegrets\\build-NoRegrets-Desktop_Qt_5_7_0_MSVC2013_64bit-Debug\\release\\Data");

        configs = new Config(plus500);

        machine->connectConfigs(configs);
    }
    //test = new QElapsedTimer();

}

void MainWindow::showLog(QString text)
{

}

MainWindow::~MainWindow()
{
    delete instruments;
    delete plus500;
    delete ui;
}

void MainWindow::webpageLoaded(bool)
{
    ui->startMachineButton->setEnabled(true);
}

void MainWindow::on_startMachineButton_clicked()
{
    machine->start();
    ui->startMachineButton->setEnabled(false);
}

void MainWindow::on_viewButton_clicked()
{
    plus500->show();
}

void MainWindow::on_pushButton_clicked()
{
    QString temp = ui->lineEdit->text();
    if( temp == "" )
        machine->externalToQueue( configs->toInfo( 10 ) );
    else
        machine->externalToQueue( configs->toInfo( temp.toInt() ) );
}

void MainWindow::on_pushButton_2_clicked()
{
    machine->externalToQueue( configs->prepareBuy() );
}

void MainWindow::on_pushButton_3_clicked()
{
    machine->externalToQueue( configs->prepareSell() );
}

void MainWindow::on_pushButton_4_clicked()
{
    QString temp = ui->lineEdit_2->text();
    if( temp == "" )
        machine->externalToQueue( configs->inputQuantity( 100 ) );
    else
        machine->externalToQueue( configs->inputQuantity( temp.toInt() ) );
}

void MainWindow::on_pushButton_5_clicked()
{
    machine->externalToQueue( configs->confirmTrade() );
}

void MainWindow::on_pushButton_6_clicked()
{
    QString temp = ui->lineEdit_3->text();
    if( temp == "" )
        machine->externalToQueue( configs->prepareClose( 10 ) );
    else
        machine->externalToQueue( configs->prepareClose( temp.toInt() ) );
}

void MainWindow::on_pushButton_7_clicked()
{
    machine->externalToQueue( configs->confirmClose() );
}

void MainWindow::on_pushButton_8_clicked()
{
    /*machine->externalToQueue( configs->toInfo(11) );
    machine->externalToQueue( configs->prepareBuy() );
    machine->externalToQueue( configs->inputQuantity(10) );
    machine->externalToQueue( configs->confirmTrade() );*/

    //test.restart();

    //plus500->page()->runJavaScript("$( 'div[data-instrument-id=\"11\"] div.positions.win-repeater.win-disposable >' ).length;",captureTest);
    //plus500->page()->runJavaScript("$( 'div[data-instrument-id=\"1001001\"] button' ).eq(0).attr('disabled');",captureTest);
}

void MainWindow::captureTest(const QVariant &result)
{
    //qDebug()<<(result == "disabled");
    //qDebug()<<test.elapsed();
    qDebug()<<result.toString();
}

void MainWindow::on_pushButton_9_clicked()
{
    QFile log_file;
    log_file.setFileName("E:/Actual server/Dysk Google/ELEC/LOG.txt");
    log_file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&log_file);
    log_file.close();
}

void MainWindow::on_pushButton_10_clicked()
{
    QString script = ui->lineEdit_4->text();
    plus500->page()->runJavaScript(script,captureTest);
}
