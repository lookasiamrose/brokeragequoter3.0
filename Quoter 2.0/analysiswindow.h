#ifndef ANALYSISWINDOW_H
#define ANALYSISWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class AnalysisWindow;
}

class OpenPosition;
class AccountManager;
class Goal;

class AnalysisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalysisWindow(QWidget *parent = 0);
    ~AnalysisWindow();
    double buy_instrument(const quint32& count,const double& actual_value);
    double sell_instrument(const quint32& count,const double& actual_value);

    double sum_back(double* tab, qint32 index, qint32 count);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void slider_changed();
    void slider_changed_2();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    AccountManager* account;
    Ui::AnalysisWindow *ui;
    double* sell;
    double* buy;
    QTime* x_axis;
    quint64 tab_size;
    double buy_value;

    QLineSeries *series;
    QLineSeries *series2;
    QLineSeries *series3;
    QScatterSeries *series4;

    double value;
    double* actual_value;
    double* value_infront;

    qint32 value_place;
    qint32 value_value;
    qint32 value_flush;
    bool bought_flag;
};

class AccountManager : public QObject
{
    Q_OBJECT
public:
    AccountManager();
    AccountManager(double value);
    AccountManager(double value, quint32 proc);
    bool open( quint32 id_temp, quint32 quantity_temp, double actual_value_temp, QString &log);
    bool find(quint32 id_temp);
    bool find(quint32 id_temp, OpenPosition* &item_ref);
    bool close( quint32 id_temp, double actual_value_temp, QString &log);
    double actual_value();

    double account_value;
private:
    double account_proc;
    QList<OpenPosition*> account_positions;

};

static bool goalsComparison(const Goal &s1, const Goal &s2);

class OpenPosition : public QObject
{
    Q_OBJECT
public:
    OpenPosition();
    OpenPosition( quint32 id_temp, quint32 quantity_temp, double actual_value_temp, qint32 proc_temp);

public:
    quint32 id;
    quint32 quantity;
    double actual_value;
    double total_value;
    quint32 proc;
};

class Goal
{
public:
    double goal0;
    double goal1;
    double goal2;
    double goal3;
    double goal4;
    double goal5;
};

#endif // ANALYSISWINDOW_H
