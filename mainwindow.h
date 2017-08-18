#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tfm.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:

    void setFontFam();

    void unchecked();

private:

    Ui::MainWindow *ui;

    Tfm *t;

    QMenu *mainmenu,*secondmenu;
};

#endif // MAINWINDOW_H
