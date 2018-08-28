#include "layermy.h"

#include "painter.h"
#include "core.h"

// 5层
// legend
// axes
// main
// grid   QCPGrid实例属于QCPAxis     QCPAxis::grid
// background   QCPAxisRect实例

// 不应该直接构造，应该由QCustomPlot::addLayer
QCPLayer::QCPLayer(QCustomPlot *parentPlot, const QString &layerName):
    QObject(parentPlot),
    mParentPlot(parentPlot),
    mName(layerName),
    mIndex(-1)
{

}

// 应该由QCustomPlot::removeLayer 删除 这样层中的每个layerable先从层中移除，再删除层
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

QCPLayerable::QCPLayerable(QCustomPlot *plot, QString targetLayer, QCPLayerable *parentLayerable):
    QObject(plot),
    mVisible(true),
    mParentPlot(plot),
    mParentLayeralbe(parentLayerable),
    mLayer(0),
    mAntialiased(true)
{
    if(mParentPlot)
    {
        if(targetLayer.isEmpty())
            setLayer(mParentPlot->currentLayer());
        else if(!setLayer(targetLayer))
            qDebug() << Q_FUNC_INFO << "setting QCPLayerable initial layer to " << targetLayer << "Failed.";
    }
}

QCPLayerable::~QCPLayerable()
{
    if(mLayer)
    {
        mLayer->removeChild(this);
        mLayer = 0;
    }
}

void QCPLayerable::setVisible(bool on)
{
    mVisible = on;
}

bool QCPLayerable::setLayer(QCPLayer *layer)
{
    return moveToLayer(layer, false);
}


bool QCPLayerable::setLayer(const QString &layerName)
{
    if( !mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set.";
        return false;
    }
    if(QCPLayer* layer = mParentPlot->layer(layerName))
    {
        return setLayer(layer);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "there's no layer with name " << layerName;
        return false;
    }
}

void QCPLayerable::setAntialiased(bool enabled)
{
    mAntialiased = enabled;
}

bool QCPLayerable::realVisibility() const
{
    return mVisible && (!mParentLayerable || mParentLayerable.data()->realVisibility());
}


double QCPLayerable::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    Q_UNUSED(pos);
    Q_UNUSED(onlySelectable);
    Q_UNUSED(details);
    return -1.0;
}


void QCPLayerable::initializeParentPlot(QCustomPlot *parentPlot)
{
    if(mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "called with mParentPlot already initialized.";
        return;
    }

    if(!parentPlot)
        qDebug() << Q_FUNC_INFO << "called with parentPlot zero.";

    mParentPlot = parentPlot;
    parentPlotInitialized(mParentPlot);
}


void QCPLayerable::setParentLayerable(QCPLayerable *parentLayerable)
{
    mParentLayerable = parentLayerable;
}


bool QCPLayerable::moveToLayer(QCPLayer *layer, bool prepend)
{
    if( layer && !mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set";
        return false;
    }
    if( layer && layer->parentPlot() !=mParentPlot)
    {
        qDebug() << Q_FUNC_INFO << "layer " << layer->name() << "is not in same QCustomPlot as this layerable.";
        return false;
    }

    if( mLayer)
        mLayer->removeChild(this);

    mLayer = layer;
    if(mLayer)
        mLayer->addChild(this, prepend);

    return true;
}


void QCPLayerable::applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const
{
    if(mParentPlot && mParentPlot->notAntialiasedElements().testFlag(overrideElement))
        painter->setAntialiasing(false);
    else if( mParentPlot && mParentPlot->antialiasedElements().testFlag(overrideElement))
        painter->setAntialiasing(true);
    else
        painter->setAntialiasing(localAntialiased);
}


// 被initializeParentPlot调用
void QCPLayerable::parentPlotInitialized(QCustomPlot *parentPlot)
{
    Q_UNUSED(parentPlot);
}


QCP::Interaction QCPLayerable::selectionCategory() const
{
    return QCP::iSelectOther;
}


QRect QCPLayerable::clipRect() const
{
    if(mParentPlot)
        return mParentPlot->viewport();
    else
        return QRect();
}


void QCPLayerable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event);
    Q_UNUSED(additive);
    Q_UNUSED(details);
    Q_UNUSED(selectionStateChanged);
}


void QCPLayerable::deselectEvent(bool *selectionStateChanged)
{
    Q_UNUSED(selectionStateChanged);
}







