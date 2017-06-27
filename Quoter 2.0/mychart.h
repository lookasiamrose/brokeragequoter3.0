#ifndef MYCHART_H
#define MYCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>

QT_CHARTS_USE_NAMESPACE

class MyChart : public QChartView
{
    Q_OBJECT
public:
    explicit MyChart(QWidget *parent = 0);

signals:

public slots:
};

#endif // MYCHART_H
