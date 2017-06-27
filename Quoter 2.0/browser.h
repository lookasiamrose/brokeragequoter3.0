#ifndef BROWSER_H
#define BROWSER_H
#include <QtWebEngineWidgets/QWebEngineView>

class Browser : public QWebEngineView
{
public:
    Browser();
    Browser(QString address_temp);
    Browser& operator=(const Browser &argument);
    ~Browser();
};

#endif // BROWSER_H
