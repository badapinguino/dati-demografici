#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_mainwindow.h>
#include <QtCharts>

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
    void on_comboBox_activated(const QString &arg1);
    void mostraPercentualeSlice(QPieSlice *slice);

private:
    Ui::MainWindow *ui;

    QStringList list;

    void open();
    void aggiornaTabellaGrafici(const QString &regioneSelezionata);

    QString label_iniziale;

};

#endif // MAINWINDOW_H
