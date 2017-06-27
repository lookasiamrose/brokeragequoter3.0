#include "browser.h"

Browser::Browser()
{

}

Browser::Browser(QString address_temp)
{
    this->load( (QUrl)address_temp );
}

Browser& Browser::operator=(const Browser &argument)
{
    return *this;
}

Browser::~Browser()
{

}
