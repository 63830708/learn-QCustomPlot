#ifndef LAYERMY_H
#define LAYERMY_H


#include "global.h"

class QCPPainter;
class QCustomPlot;
class QCPLayerable;
class QCPLayoutElement;
class QCPLayout;

class QCP_LIB_DECL QCPLayer: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QCustomPlot* parentPlot READ parentPlot)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(int index READ index)
    Q_PROPERTY(QList<QCPLayerable*> children READ children)
public:
    QCPLayer(QCustomPlot* parentPlot, const QString &layerName);
    ~QCPLayer();

    //getter
    QCustomPlot *parentPlot() const { return mParentPlot;}
    QString name() const {return mName;}
    int index() const { return mIndex;}
    QList<QCPLayerable*> children() const { return mChildren;}
protected:
    // property
    QCustomPlot* mParentPlot;
    QString     mName;
    int         mIndex;
    QList<QCPLayerable*>    mChildren;

    void addChild(QCPLayerable *layerable, bool prepend);
    void removeChild(QCPLayerable* layerable);

private:
    Q_DISABLE_COPY(QCPLayer)

    friend class QCustomPlot;
    friend class QCPLayerable;
};



#endif // LAYERMY_H
