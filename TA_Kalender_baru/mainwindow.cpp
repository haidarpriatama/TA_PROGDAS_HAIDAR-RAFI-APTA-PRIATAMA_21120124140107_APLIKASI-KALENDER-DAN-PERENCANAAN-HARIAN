#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDate>
#include <QColor>
#include <QTextCharFormat>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Kalender");
    ui->textEdit->setReadOnly(true);

    // Hubungkan tombol dan widget dengan fungsi terkait
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &MainWindow::showEvents);
    connect(ui->addEventButton, &QPushButton::clicked, this, &MainWindow::addEvent);
    connect(ui->editEventButton, &QPushButton::clicked, this, &MainWindow::editEvent);
    connect(ui->deleteEventButton, &QPushButton::clicked, this, &MainWindow::deleteEvent);

    highlightToday(); // Sorot hari ini
    highlightEventDates(); // Sorot tanggal dengan acara

    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::highlightToday()
{
    // Sorot tanggal hari ini
    QDate today = QDate::currentDate();
    ui->calendarWidget->setSelectedDate(today);

    QTextCharFormat format;
    format.setFontUnderline(true);
    format.setForeground(Qt::blue);

    ui->calendarWidget->setDateTextFormat(today, format);
    showEvents(today);
}

void MainWindow::highlightEventDates()
{
    // Hapus format lama
    for (const auto &date : events.keys()) {
        ui->calendarWidget->setDateTextFormat(date, QTextCharFormat());
    }

    // Tampilkan tanda hijau pada tanggal dengan acara
    QTextCharFormat dotFormat;
    dotFormat.setForeground(Qt::green);

    QDate today = QDate::currentDate();
    bool hasTodayEvents = events.contains(today) && !events[today].isEmpty();

    for (const auto &date : events.keys()) {
        if (!events[date].isEmpty()) {
            ui->calendarWidget->setDateTextFormat(date, dotFormat);
        }
    }

    QTextCharFormat todayFormat;
    if (hasTodayEvents) {
        todayFormat.setFontUnderline(true);
        todayFormat.setForeground(Qt::green);
    } else {
        todayFormat.setFontUnderline(true);
        todayFormat.setForeground(QColor("#2af2ff"));
    }
    ui->calendarWidget->setDateTextFormat(today, todayFormat);
}

void MainWindow::addEvent()
{
    bool ok;
    // Tambah acara baru
    QString title = QInputDialog::getText(this, tr("Tambah Acara"),
                                          tr("Judul Acara:"), QLineEdit::Normal,
                                          "", &ok);
    if (ok && !title.isEmpty()) {
        QString description = QInputDialog::getText(this, tr("Tambah Acara"),
                                                    tr("Deskripsi Acara:"), QLineEdit::Normal,
                                                    "", &ok);
        if (ok) {
            QDate selectedDate = ui->calendarWidget->selectedDate();
            events[selectedDate].append(qMakePair(title, description));
            showEvents(selectedDate);
            highlightEventDates();
        }
    }
}

void MainWindow::showEvents(const QDate &date)
{
    // Tampilkan acara untuk tanggal tertentu
    ui->textEdit->clear();
    if (events.contains(date)) {
        for (const auto &event : events[date]) {
            ui->textEdit->append("Judul: " + event.first + "\nDeskripsi: " + event.second + "\n");
        }
    }
}

void MainWindow::editEvent()
{
    // Edit acara pada tanggal tertentu
    QDate selectedDate = ui->calendarWidget->selectedDate();
    if (!events.contains(selectedDate) || events[selectedDate].isEmpty()) {
        QMessageBox::warning(this, tr("Edit Acara"), tr("Tidak ada acara pada tanggal ini."));
        return;
    }

    QStringList eventList;
    for (const auto &event : events[selectedDate]) {
        eventList << event.first;
    }

    bool ok;
    QString selectedEvent = QInputDialog::getItem(this, tr("Edit Acara"), tr("Pilih acara untuk diedit:"), eventList, 0, false, &ok);

    if (ok && !selectedEvent.isEmpty()) {
        QString currentDescription;
        for (const auto &event : events[selectedDate]) {
            if (event.first == selectedEvent) {
                currentDescription = event.second;
                break;
            }
        }

        QString newTitle = QInputDialog::getText(this, tr("Edit Acara"),
                                                 tr("Judul Baru Acara:"), QLineEdit::Normal,
                                                 selectedEvent, &ok);
        if (ok && !newTitle.isEmpty()) {
            QString newDescription = QInputDialog::getText(this, tr("Edit Acara"),
                                                           tr("Deskripsi Baru Acara:"), QLineEdit::Normal,
                                                           currentDescription, &ok);
            if (ok) {
                auto &eventList = events[selectedDate];
                for (auto &event : eventList) {
                    if (event.first == selectedEvent) {
                        event.first = newTitle;
                        event.second = newDescription;
                        break;
                    }
                }
                showEvents(selectedDate);
                highlightEventDates();
            }
        }
    }
}

void MainWindow::deleteEvent()
{
    // Hapus acara pada tanggal tertentu
    QDate selectedDate = ui->calendarWidget->selectedDate();
    if (!events.contains(selectedDate) || events[selectedDate].isEmpty()) {
        QMessageBox::warning(this, tr("Hapus Acara"), tr("Tidak ada acara pada tanggal ini."));
        return;
    }

    QStringList eventList;
    for (const auto &event : events[selectedDate]) {
        eventList << event.first;
    }

    bool ok;
    QString selectedEvent = QInputDialog::getItem(this, tr("Hapus Acara"), tr("Pilih acara untuk dihapus:"), eventList, 0, false, &ok);

    if (ok && !selectedEvent.isEmpty()) {
        auto &eventList = events[selectedDate];
        eventList.removeIf([&selectedEvent](const QPair<QString, QString> &event) {
            return event.first == selectedEvent;
        });
        showEvents(selectedDate);
        highlightEventDates();
    }
}
