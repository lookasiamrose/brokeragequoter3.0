#include "instrumentswindow.h"
#include "ui_instrumentswindow.h"

QList<InstrumentLibrary*> InstrumentsWindow::commodities_pointers;
QList<InstrumentLibrary*> *InstrumentsWindow::temporary;

InstrumentsWindow::InstrumentsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstrumentsWindow)
{
    ui->setupUi(this);

    tick = new QTimer(this);
    connect(tick, SIGNAL(timeout()), this, SLOT(updateInstruments()));
    tick->start(1000);

}

InstrumentsWindow::~InstrumentsWindow()
{
    delete ui;
}

void InstrumentsWindow::connectBrowser(Browser* browser_temp)
{
    plus500_pointer = browser_temp;
}

void InstrumentsWindow::connectInstrument(QList<InstrumentLibrary*> *commodity_temp)
{
    //commodities_pointers.push_back(commodity_temp);
    temporary = commodity_temp;
}

void InstrumentsWindow::updateInstruments()
{
        if( !(*temporary).isEmpty() )
        {
            QList<InstrumentLibrary*>::iterator iter = (*temporary).begin();
            ui->lcdNumber_0_B->display( (*iter)->buy );
            ui->lcdNumber_0_A->display( (*iter)->sell );
            ui->label->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_1_B->display( (*iter)->buy );
            ui->lcdNumber_1_A->display( (*iter)->sell );
            ui->label_2->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_2_B->display( (*iter)->buy );
            ui->lcdNumber_2_A->display( (*iter)->sell );
            ui->label_3->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_3_B->display( (*iter)->buy );
            ui->lcdNumber_3_A->display( (*iter)->sell );
            ui->label_4->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_4_B->display( (*iter)->buy );
            ui->lcdNumber_4_A->display( (*iter)->sell );
            ui->label_5->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_5_B->display( (*iter)->buy );
            ui->lcdNumber_5_A->display( (*iter)->sell );
            ui->label_6->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_6_B->display( (*iter)->buy );
            ui->lcdNumber_6_A->display( (*iter)->sell );
            ui->label_7->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_7_B->display( (*iter)->buy );
            ui->lcdNumber_7_A->display( (*iter)->sell );
            ui->label_8->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_8_B->display( (*iter)->buy );
            ui->lcdNumber_8_A->display( (*iter)->sell );
            ui->label_9->setText( (*iter)->name );
            iter++;

            ui->lcdNumber_9_B->display( (*iter)->buy );
            ui->lcdNumber_9_A->display( (*iter)->sell );
            ui->label_10->setText( (*iter)->name );

        }


}
