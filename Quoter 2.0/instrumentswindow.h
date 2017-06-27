#ifndef INSTRUMENTSWINDOW_H
#define INSTRUMENTSWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtWebEngineWidgets/QWebEnginePage>
#include "browser.h"
#include "instrument.h"
#include "instrumentlibrary.h"

namespace Ui{
    class InstrumentsWindow;
}

class InstrumentsWindow : public QMainWindow
{
    Q_OBJECT

public:
    InstrumentsWindow(QWidget *parent = 0);
    void connectBrowser(Browser* browser_temp);
    void connectInstrument(QList<InstrumentLibrary*> *buy_temp);
    ~InstrumentsWindow();

private:
    Browser* plus500_pointer;
    static QList<InstrumentLibrary*> *temporary;
    static QList<InstrumentLibrary*> commodities_pointers;
    Ui::InstrumentsWindow *ui;
    QTimer* tick;

public slots:
    void updateInstruments();

};

#endif // INSTRUMENTSWINDOW_H
