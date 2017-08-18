#pragma once
#include <QtCore>
#include <QtWidgets>
#include "thr.h"
#include <memory>
#include "mime.h"
#include <QtConcurrent>
#include <QFuture>
#include "treeproxy.h"
class Tfm final:public QWidget
{
    Q_OBJECT
private:

    TreeProxyFilter *proxy;

    QFileSystemModel *model;

    QTreeView *tv,*tv2;

    QSplitter *splitter;

    QWidget *infowidget;

    QMap<QString,QString> map;

    QLabel *pic,*name,*path,*information,*navigation;

    QMenu *menu;

    QLineEdit *line,*line2;

    QPushButton *back;

protected:

    void contextMenuEvent(QContextMenuEvent *pe) override
    {
        menu->exec(pe->globalPos());
    }

public:

    Tfm(QWidget *p=0);

public slots:

    void initInfo();

    void initmap();

    void setInfo(QModelIndex index);

    QPixmap getPix(const QString &s);

    void delF();

    void openF();

    void createContextMenu();

    void openProcess(const QString &s, bool ex);

    void copyF();

    void iNF();

    void copyFile(const QString &source, const QString &destination);

    void insertedfiles(QFileInfo f);

    void renameF();

    quint32 listFolder (const QString &path );

    void MapppedsetRootIndex(const QModelIndex& index);

    void MapppedsetCurrentIndex(const QModelIndex& index);

    void SelectInMainTree(QModelIndex o,QModelIndex n);

    void getBack();

    void createDir();

    void filterT(const QString &s);

    void minS();

    void setFontA(const QString &font);

    void setFiltr();

    void dirPath ();

    QString loadqss();

    void setInfoP(QModelIndex index);
};
