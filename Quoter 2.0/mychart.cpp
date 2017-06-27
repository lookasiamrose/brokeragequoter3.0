#include "mychart.h"

MyChart::MyChart(QWidget *parent) : QChartView(parent)
{
    setRenderHint(QPainter::Antialiasing);
}
