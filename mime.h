#pragma once
#include <QtWidgets>

class QFileInfoMimeData : public QMimeData
{
private:

    QFileInfo f;

public:

    QFileInfoMimeData() : QMimeData()
    {      
    }

    virtual ~QFileInfoMimeData()
    {
    }

    static QString mimeType()
    {
        return "application/fileinfo";
    }

    void setFileinfo(QFileInfo &ff)
    {
        f = ff;

        setData(mimeType(), QByteArray());
    }

   QFileInfo  info() const
    {
        return f;
    }
};
