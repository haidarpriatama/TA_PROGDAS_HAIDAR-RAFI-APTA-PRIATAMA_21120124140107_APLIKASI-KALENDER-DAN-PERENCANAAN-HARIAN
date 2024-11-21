#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QMap>
#include <QPair>
#include <QString>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addEvent();
    void showEvents(const QDate &date);
    void editEvent();
    void deleteEvent();

private:
    void highlightToday();
    void highlightEventDates();

    Ui::MainWindow *ui;
    QMap<QDate, QVector<QPair<QString, QString>>> events;
};

#endif // MAINWINDOW_H
