#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTime>

QWebEnginePage* MainWindow::page = nullptr;
QWebEngineView* MainWindow::view = nullptr;
qint32 MainWindow::save60 = 0;
QMap<QString,Instrument*> MainWindow::instruments;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    page = new QWebEnginePage();
    view = new QWebEngineView();

    page->load( QUrl("https://app.plus500.com/trade/all-commodities") );

    sek = new QTimer(this);
    connect( sek, SIGNAL(timeout()), this, SLOT(sekTick()) );

    view->setPage(page);

    view->show();

    Instrument* temp = new Instrument("10");
    Instrument* temp1 = new Instrument("11");
    Instrument* temp2 = new Instrument("123");
    Instrument* temp3 = new Instrument("12");
    Instrument* temp4 = new Instrument("431");
    Instrument* temp5 = new Instrument("1460");
    Instrument* temp6 = new Instrument("101");
    Instrument* temp7 = new Instrument("1462");
    Instrument* temp8 = new Instrument("115");
    Instrument* temp9 = new Instrument("117");
    Instrument* temp10 = new Instrument("116");
    Instrument* temp11 = new Instrument("430");
    Instrument* temp12 = new Instrument("2265");
    Instrument* temp13 = new Instrument("118");
    Instrument* temp14 = new Instrument("119");
    Instrument* temp15 = new Instrument("1461");

    instruments.insert("10",temp);
    instruments.insert("11",temp1);
    instruments.insert("123",temp2);
    instruments.insert("12",temp3);
    instruments.insert("431",temp4);
    instruments.insert("1460",temp5);
    instruments.insert("101",temp6);
    instruments.insert("1462",temp7);
    instruments.insert("115",temp8);
    instruments.insert("117",temp9);
    instruments.insert("116",temp10);
    instruments.insert("430",temp11);
    instruments.insert("2265",temp12);
    instruments.insert("118",temp13);
    instruments.insert("119",temp14);
    instruments.insert("1461",temp15);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_execRun_clicked()
{
    sek->start(995);
}
void MainWindow::sekTick()
{
    page->runJavaScript(" var a = [];\n"
                        "$('div[id=\"instrumentsTable\"]> :eq(1) >').each(function(){\n"
                        "var arr = [];\n"
                        "arr.push( $(this).attr('data-instrument-id') );\n"
                        "arr.push( $(this).children().eq(2).text() );\n"
                        "arr.push( $(this).children().eq(4).text() );\n"
                        "a.push(arr);\n"
                        "});\n"
                        "a;",dataCapture);
    if( save60 > 59 )
    {
        QFile file;
        foreach(Instrument* item, instruments )
        {
            file.setFileName( QApplication::applicationDirPath() + QString("\\Data\\") + item->id + QString(".txt") );
            if( file.open(QIODevice::Append | QIODevice::Text) )
            {
                QTextStream in(&file);
                foreach(History history, item->history)
                {
                    in<<history.buy<<" "<<history.sell<<" "<<history.time<<endl;
                }
                item->history.clear();
            }
            file.close();
        }
        save60 = 0;
    }
    save60++;

    page->runJavaScript("if( typeof $('button[id=\"demoMode\"]').val() != 'undefined' ) $('button[id=\"demoMode\"]').click(); else 0;");
    page->runJavaScript("$( 'div[id=\"alertDialogDiv\"]' ).attr('class');",dialogclass);
    page->runJavaScript("$( 'div[class=\"login-grid\"]' ).size();",isloginscreen);
}
void MainWindow::dialogclass(const QVariant & result)
{
    if( result.toString().contains("visible") )
    {
        page->runJavaScript("$( 'div[role=\"dialog\"] button' ).eq(1).click();");
    }
}
void MainWindow::isloginscreen(const QVariant & result)
{
    if( result.toInt() > 0 )
    {
        page->runJavaScript("$( 'button[id=\"submitLogin\"]' ).click();");
    }
}
void MainWindow::dataCapture(const QVariant & result)
{
    foreach( QVariant item, result.toList() )
    {
        History temp;
        temp.sell = item.toStringList()[1];
        temp.buy = item.toStringList()[2];

        temp.buy.remove( QChar(' ') );
        temp.buy.remove( 0xA0 );
        temp.buy.replace( QChar(','), QChar('.') );

        temp.sell.remove( QChar(' ') );
        temp.sell.remove( 0xA0 );
        temp.sell.replace( QChar(','), QChar('.') );

        QTime now;
        now = QTime::currentTime();
        temp.time = now.toString();

        if( instruments.contains(item.toStringList()[0]) ) instruments[item.toStringList()[0]]->history.append(temp);
    }
}
Instrument::Instrument(QString id_arg)
{
    id = id_arg;
}
