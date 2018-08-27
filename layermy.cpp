#include "layermy.h"

#include "painter.h"
#include "core.h"

QCPLayer::QCPLayer(QCustomPlot *parentPlot, const QString &layerName):
    QObject(parentPlot),
    mParentPlot(parentPlot),
    mName(layerName),
    mIndex(-1)
{

}

QCPLayer::~QCPLayer()
{
    while(!mChildren.isEmpty())
    {
        mChildren.last()->setLayer(0);
        if(mParentPlot->currentLayer() == this)
        {
            qDebug() << Q_FUNC_INFO << "The parent plots mCurrentLayer will be dangling pointer. Should have been set to a valid layer or 0 beforehand.";
        }
    }
}


void QCPLayer::addChild(QCPLayerable *layerable, bool prepend)
{
    if( !mChildren.contains(layerable))
    {
        if(prepend)
            mChildren.prepend(layerable);
        else
            mChildren.append(layerable);
    }
    else
    {
        qDebug() << Q_FUNC_INFO  << "layerable is already child of this layer"
                 << reinterpret_cast<quintptr>(layerable);
    }
}


void QCPLayer::removeChild(QCPLayerable *layerable)
{
    if( !mChildren.removeOne(layerable))
    {
        qDebug() << Q_FUNC_INFO << "layerable is not child of this layer"
                 << reinterpret_cast<quintptr>(layerable);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayerable
////////////////////////////////////////////////////////////////////////////////////////////////////
