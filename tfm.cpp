#include <QtWidgets>
#include "tfm.h"
Tfm::Tfm(QWidget *p):QWidget(p)
{
    createContextMenu();

    initmap();


    model=new QFileSystemModel;
    model->setRootPath(QDir::rootPath());
    model->setReadOnly(false);

    proxy=new  TreeProxyFilter;
    proxy->setDynamicSortFilter(true);
    proxy->setSourceModel(model);
    proxy->setFilterRegExp(".*");

    line=new QLineEdit;
    line->setPlaceholderText("Search");

    line2=new QLineEdit;
    line2->setPlaceholderText("Path like Disc:/subdir/../");

    pic=new QLabel("Empty");
    pic->setToolTip("File");

    name=new QLabel("Empty");
    name->setWordWrap(true);

    path=new QLabel("Empty");
    path->setWordWrap(true);


    information=new QLabel("Information");
    information->setFont(QFont("Times", 18));

    navigation=new QLabel("Navigation");
    navigation->setFont(QFont("Times", 12));

    tv=new QTreeView;
    tv->setModel(model);
    tv->setDragDropMode(QAbstractItemView::DragDrop);
    tv->setSortingEnabled(true);
    tv->setSelectionMode(QAbstractItemView::SingleSelection);

    tv2=new QTreeView;
    tv2->setModel(proxy);
    tv2->setSortingEnabled(true);
    tv2->setSelectionMode(QAbstractItemView::SingleSelection);

    splitter=new QSplitter;
    splitter->setOrientation(Qt::Horizontal);

    infowidget=new QWidget;

    initInfo();

    connect(tv,&QTreeView::activated,this,&Tfm::setInfo);
    connect(tv2,&QTreeView::activated,this,&Tfm::setInfoP);

    connect(tv, SIGNAL(clicked(const QModelIndex&)), this, SLOT(MapppedsetRootIndex(const QModelIndex&)) ) ;
    connect(tv, SIGNAL(activated(const QModelIndex&)),this, SLOT(MapppedsetCurrentIndex(const QModelIndex&)));
    connect(tv, SIGNAL(activated(const QModelIndex&)),this, SLOT(MapppedsetRootIndex(const QModelIndex&)) ) ;

    connect(tv2->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(SelectInMainTree(QModelIndex,QModelIndex)));

    connect(line,SIGNAL(textChanged(QString)),this,SLOT(filterT(QString)));
    connect(line2,SIGNAL(returnPressed()),this,SLOT(dirPath()));

    back=new QPushButton("Back");
    back->setToolTip("Return to the above directory");

    connect(back,SIGNAL(clicked()),this,SLOT(getBack()));


    QWidget *proxy2=new QWidget;

    infowidget->setStyleSheet("QWidget{background-color:#ead3cd;}");

    splitter->addWidget(tv);

    QVBoxLayout *v=new QVBoxLayout;

    v->addWidget(navigation);
    v->setAlignment(navigation,Qt::AlignCenter);
    v->addWidget(line);
    v->addWidget(line2);
    v->addWidget(tv2);
    v->addWidget(back);

    proxy2->setLayout(v);

    splitter->addWidget(proxy2);
    splitter->addWidget(infowidget);


    QHBoxLayout *hob=new QHBoxLayout;
    hob->addWidget(splitter);

    setStyleSheet(loadqss());

    setFontA("Book Antiqua");

    setLayout(hob);

    minS();

    dirPath();

}

void Tfm::initInfo()
{
    QVBoxLayout *hb=new QVBoxLayout;

    hb->addWidget(information);
    hb->setAlignment(information,Qt::AlignCenter);


    hb->setSpacing(10);

    hb->addWidget(path);
    hb->setAlignment(path,Qt::AlignCenter);

    hb->addWidget(pic);
    hb->setAlignment(pic,Qt::AlignCenter);

    hb->addWidget(name);
    hb->setAlignment(name,Qt::AlignCenter);

    hb->addStretch(1);

    infowidget->setLayout(hb);
}

void Tfm::initmap()
{
    map.insert("Folder","fold7.png");

    map.insert("exe","ex.png");

    map.insert("audio","cmp3.jpg");

    map.insert("video","video.png");

    map.insert("text","txt.png");

    map.insert("other","file.jpg");

    map.insert("picture","images.png");
}

void Tfm::setInfo(QModelIndex index)
{

    QFileInfo fileinfo;

    fileinfo=model->fileInfo(index);

    path->setText(fileinfo.absoluteFilePath());

    path->setAlignment(Qt::AlignCenter);

    name->setText(fileinfo.baseName()+"\n Size: "+QString::number(listFolder(fileinfo.absoluteFilePath())/ double(1024 * 1024))+" Mbytes");

    name->setAlignment(Qt::AlignCenter);

    if(fileinfo.suffix()=="txt" || fileinfo.suffix()=="doc" || fileinfo.suffix()=="docx" ||fileinfo.suffix()=="pdf" )
    {
        pic->setPixmap(getPix(map["text"]));

    }else if(fileinfo.suffix()=="mp3" || fileinfo.suffix()=="wav" || fileinfo.suffix()=="ogg" || fileinfo.suffix()=="mp4")
    {
        pic->setPixmap(getPix(map["audio"]));

    } else if(fileinfo.suffix()=="jpg" || fileinfo.suffix()=="png" || fileinfo.suffix()=="bmp" || fileinfo.suffix()=="gif" || fileinfo.suffix()=="JPG")
    {

        pic->setPixmap(getPix(map["picture"]));

    }else if(fileinfo.suffix()=="avi")
    {
        pic->setPixmap(getPix(map["video"]));

    } else if (fileinfo.isDir())
    {
        pic->setPixmap(getPix(map["Folder"]));

    } else if(fileinfo.suffix()=="exe")
    {
        pic->setPixmap(getPix(map["exe"]));

    } else {

        pic->setPixmap(getPix(map["other"]));
    }

}

QPixmap Tfm::getPix(const QString &s)
{

    QPixmap p(s);

    p=p.scaled(QSize(120,120),Qt::KeepAspectRatio);

    return p;
}

void Tfm::delF()
{
    model->remove(tv->selectionModel()->currentIndex());
}

void Tfm::openF()
{

    QString pathOfOpenFile=model->fileInfo(tv->selectionModel()->currentIndex()).absoluteFilePath();

    pathOfOpenFile.replace('//','\\');

    openProcess(pathOfOpenFile,model->fileInfo(tv->selectionModel()->currentIndex()).isExecutable());

}

void Tfm::createContextMenu()
{
    menu=new QMenu;

    menu->addAction(QIcon("close.png"),"Delete",this,SLOT(delF()));

    menu->addSeparator();

    menu->addAction(QIcon("open.png"),"Open",this,SLOT(openF()));

    menu->addAction(QIcon("copy.png"),"Copy",this,SLOT(copyF()));

    menu->addAction(QIcon("insert.png"),"Insert",this,SLOT(iNF()));

    menu->addAction(QIcon("rename.png"),"Rename",this,SLOT(renameF()));

    menu->addSeparator();

    menu->addAction(QIcon("fold2.png"),"Create Folder",this,SLOT(createDir()));
}

void Tfm::openProcess(const QString &s, bool ex)
{
    if(ex)
    {
        QScopedPointer<QProcess> p(new QProcess);

        p->startDetached(s);

    } else QDesktopServices::openUrl(QUrl(s));
}

void Tfm::copyF()
{
    QFileInfo inf=model->fileInfo(tv->selectionModel()->currentIndex());

    QClipboard *cl=QApplication::clipboard();

    QFileInfoMimeData* pmd = new QFileInfoMimeData;

    pmd->setFileinfo(inf);

    cl->setMimeData(pmd);

    //qDebug()<<inf.absoluteFilePath();
}

void Tfm::iNF()
{
    QClipboard *cl=QApplication::clipboard();

    QFileInfo f;

    f=dynamic_cast<const QFileInfoMimeData*>(cl->mimeData())->info();

    insertedfiles(f);

}

void Tfm::copyFile(const QString &source, const QString &destination)
{

    if ( QFileInfo( source ).isDir() )
    {
        QDir().mkdir( destination );

        foreach ( const QString & entry, QDir( source ).entryList( QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot ) )
            copyFile( source + QDir::separator() + entry, destination + QDir::separator() + entry );
    } else {
        if ( QFile::exists(destination) )
        {
            QFileInfo infoS = QFileInfo(source);
            QFileInfo infoD = QFileInfo (destination);

            if ( infoS.lastModified() > infoD.lastModified() )
            {
                QFile::remove(destination);

                bool ok = QFile::copy( source, destination );

                if ( ok )
                {
                    //  countFile++;
                }
            }
        } else
        {
            bool ok = QFile::copy( source, destination );

            if ( ok )
            {
                //countFile++;

            }
        }
    }
}

void Tfm::insertedfiles(QFileInfo f)
{
    if(f.isFile())
    {
        QString savepath=model->fileInfo(tv->selectionModel()->currentIndex()).absoluteFilePath()+"\\"+f.baseName()+"."+f.suffix();

        if(!QFile(savepath).exists())
        {
            QFile::copy(f.absoluteFilePath(),savepath);

        } else {
            static QMap<QString,int>cou;

            ++cou[savepath];

            savepath=model->fileInfo(tv->selectionModel()->currentIndex()).absoluteFilePath()+"\\"+f.baseName()+"("+QString::number(cou[savepath])+")"+"."+f.suffix();

            QtConcurrent::run(&QFile::copy,f.absoluteFilePath(),savepath);

        }
    } else if(f.isDir())
    {
        QtConcurrent::run(this,&copyFile,f.absoluteFilePath(),model->fileInfo(tv->selectionModel()->currentIndex()).absoluteFilePath());
    }
}

void Tfm::renameF()
{
    tv->edit(tv->selectionModel()->currentIndex());
}


quint32 Tfm::listFolder (const QString &path )
{
    QFileInfo f(path);

    if(f.isFile()) return f.size();

    QDir currentFolder( path );

    quint32 totalsize = 0;

    currentFolder.setFilter( QDir::Dirs | QDir::Files | QDir::NoSymLinks );
    currentFolder.setSorting( QDir::Name );

    QFileInfoList folderitems( currentFolder.entryInfoList() );

    foreach ( QFileInfo i, folderitems )
    {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;

        if ( i.isDir() )
        {
            QFuture<quint32> fut=QtConcurrent::run(this,&listFolder, path+"/"+iname);
            totalsize +=fut.result();
        }
        else
            totalsize += i.size();
    }
    return totalsize;
}

void Tfm::MapppedsetRootIndex(const QModelIndex& index)
{
    tv2->setRootIndex(proxy->mapFromSource(index));
}

void Tfm::MapppedsetCurrentIndex(const QModelIndex& index)
{
    tv2->setCurrentIndex(proxy->mapFromSource(index));
}

void Tfm::SelectInMainTree(QModelIndex o,QModelIndex n)
{
    tv->selectionModel()->setCurrentIndex(proxy->mapToSource(o),QItemSelectionModel::SelectCurrent);

}

void Tfm::getBack()
{
    MapppedsetRootIndex(model->parent(tv2->currentIndex()));
}

void Tfm::createDir()
{
    model->mkdir(tv->selectionModel()->currentIndex(),"New Folder");
}

void Tfm::filterT(const QString &s)
{
    if(!s.isEmpty())
    {
        proxy->setFilterRegExp(s);

    } else proxy->setFilterRegExp(".*");

}

void Tfm::minS()
{
    tv->setMinimumWidth(400);

    tv2->setMinimumWidth(400);

    infowidget->setMinimumWidth(300);
}

void Tfm::setFontA(const QString &font)
{
    size_t start=font.indexOf(',');

    size_t end=font.indexOf(',',start+1);

    QString size=font.mid(start+1,end-start-1);

    tv->setStyleSheet("QTreeView{font-family:"+font+"; font-size:"+size+"pt;}");

    tv2->setStyleSheet("QTreeView{font-family:"+font+"; font-size:"+size +"pt;}");
}

void Tfm::setFiltr()
{
      QAction *fi=((QAction*)sender());

      //qDebug()<<fi->text();

      if(fi->text()=="No filters")
      {
          model->setFilter(QDir::AllEntries);

      } else if(fi->text()=="Dirs")
      {
          model->setFilter(QDir::AllDirs);

      }else if(fi->text()=="Files")
      {
          model->setFilter(QDir::Files);

      }else if(fi->text()=="Drives")
      {
          model->setFilter(QDir::Drives);

      }else if(fi->text()=="Executable")
      {
          model->setFilter(QDir::Executable);

      }else if(fi->text()=="Hidden")
      {
          model->setFilter(QDir::Hidden);

      }else if(fi->text()=="Sustem")
      {
          model->setFilter(QDir::System);
      }

}

void Tfm::dirPath ()
{
    if(line2->text().isEmpty()) model->setRootPath(QDir::rootPath());

    tv2->setRootIndex(proxy->mapFromSource(model->setRootPath(line2->text())));

}

QString Tfm::loadqss()
{
    QFile style("simple.qss");
    style.open(QIODevice::ReadOnly);
    QString qss=style.readAll();
    return qss;
}

void Tfm::setInfoP(QModelIndex index)
{
    setInfo(proxy->mapToSource(index));
}
