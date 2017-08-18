#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("InFolder");
    setWindowIcon(QIcon("manager2.png"));

    QCursor c(QPixmap("cursor_arrow.ico"));
    setCursor(c);

    t=new Tfm;

    mainmenu=new QMenu("View");

    mainmenu->addAction("Choice font",this,SLOT(setFontFam()));

    secondmenu=new QMenu("Filters");

    QAction *nofilter=secondmenu->addAction("No filters",t,SLOT(setFiltr()));
    nofilter->setCheckable(true);
    nofilter->setChecked(true);

    QAction *dirs=secondmenu->addAction("Dirs");
    dirs->setCheckable(true);

    QAction *files=secondmenu->addAction("Files");
    files->setCheckable(true);

    QAction *drives=secondmenu->addAction("Drives");
    drives->setCheckable(true);

    QAction *executable=secondmenu->addAction("Executable");
    executable->setCheckable(true);

    QAction *hidden=secondmenu->addAction("Hidden");
    hidden->setCheckable(true);

    QAction *system=secondmenu->addAction("System");
    system->setCheckable(true);

    connect(nofilter,SIGNAL(triggered(bool)),t,SLOT(setFiltr()));
    connect(nofilter,SIGNAL(triggered(bool)),this,SLOT(unchecked()));

    connect(dirs,SIGNAL(triggered(bool)),t,SLOT(setFiltr()));
    connect(dirs,SIGNAL(triggered(bool)),this,SLOT(unchecked()));

    connect(files,SIGNAL(triggered(bool)),t,SLOT(setFiltr()));
    connect(files,SIGNAL(triggered(bool)),this,SLOT(unchecked()));

    connect(drives,SIGNAL(triggered(bool)),t,SLOT(setFiltr()));
    connect(drives,SIGNAL(triggered(bool)),this,SLOT(unchecked()));

    connect(executable,SIGNAL(triggered(bool)),t,SLOT(setFiltr()));
    connect(executable,SIGNAL(triggered(bool)),this,SLOT(unchecked()));

    connect(hidden,SIGNAL(triggered(bool)),t,SLOT(setFiltr()));
    connect(hidden,SIGNAL(triggered(bool)),this,SLOT(unchecked()));

    connect(system,SIGNAL(triggered(bool)),t,SLOT(setFiltr()));
    connect(system,SIGNAL(triggered(bool)),this,SLOT(unchecked()));

    menuBar()->addMenu(mainmenu);

    menuBar()->addMenu(secondmenu);

    setStyleSheet("MainWindow{background-color:#5a5a5a;}");

    setCentralWidget(t);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFontFam()
{
    bool bok;

    QFont fnt = QFontDialog::getFont(&bok);

    if (bok)
    {
        t->setFontA(fnt.toString());
    }
}

void MainWindow::unchecked()
{
      QAction *fi=((QAction*)sender());

      for(QAction *&a:secondmenu->actions())
      {
          if(a->text()!=fi->text())
          {
              a->setChecked(false);
          }
      }
}
