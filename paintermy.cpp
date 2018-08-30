#include "paintermy.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPainter
////////////////////////////////////////////////////////////////////////////////////////////////////
QCPPainter::QCPPainter():
    QPainter(),
    mModes(pmDefault),
    mIsAntialiasing(false)
{}

QCPPainter::QCPPainter(QPaintDevice *device):
    QPainter(device),
    mModes(pmDefault),
    mIsAntialiasing(false)
{
}

QCPPainter::~QCPPainter() {}


void QCPPainter::setPen(const QPen &pen)
{
    QPainter::setPen(pen);
    if( mModes.testFlag(pmNonCosmetic))
        makeNonCosmetic();
}

void QCPPainter::setPen(const QColor &color)
{
    QPainter::setPen(color);
    if( mModes.testFlag(pmNonCosmetic))
        makeNonCosmetic();
}


void QCPPainter::setPen(Qt::PenStyle penStyle)
{
    QPainter::setPen(penStyle);
    if (mModes.testFlag(pmNonCosmetic))
      makeNonCosmetic();
}


void QCPPainter::drawLine(const QLineF &line)
{
    if( mIsAntialiasing || mModes.testFlag(pmVectorized))
        QPainter::drawLine(line);
    else
        QPainter::drawLine(line.toLine());
}


void QCPPainter::setAntialiasing(bool enabled)
{
    setRenderHint(QCPPainter::Antialiasing, enabled);
    if(mIsAntialiasing != enabled)
    {
        mIsAntialiasing = enabled;
        if(mModes.testFlag(pmVectorized))
        {
            if( mIsAntialiasing)
                translate(0.5, 0.5);
            else
                translate(-0.5, -0.5);
        }
    }
}


void QCPPainter::setModes(PainterModes modes)
{
    mModes = modes;
}


void QCPPainter::setMode(PainterMode mode, bool enabled)
{
    if( !enabled && mModes.testFlag(mode))
        mModes &= ~mode;
    else if( enabled && !mModes.testFlag(mode))
        mModes |= mode;
}


void QCPPainter::save()
{
    mAntialiasingStack.push(mIsAntialiasing);
    QPainter::save();
}


void QCPPainter::restore()
{
    if( !mAntialiasingStack.isEmpty())
        mIsAntialiasing = mAntialiasingStack.pop();
    else
        qDebug() << Q_FUNC_INFO << "Unbalanced save/restore.";
    QPainter::restore();
}


void QCPPainter::makeNonCosmetic()
{

    if( qFuzzyIsNull( pen().widthF()))
    {
        QPen p = pen();
        p.setWidth(1);
        QPainter::setPen(p);
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPScatterStyle
////////////////////////////////////////////////////////////////////////////////////////////////////

QCPScatterStyle::QCPScatterStyle():
    mSize(6)
  , mShape(ssNone)
  , mPen(Qt::NoPen)
  , mBrush(Qt::NoBrush)
  , mPenDefined(false)
{}


QCPScatterStyle::QCPScatterStyle(ScatterShape shape, double size):
    mSize(size)
  , mShape(shape)
  , mPen(Qt::Nopen)
  , mBrush(Qt::NoBrush)
  , mPenDefined(false)
{}


QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, double size) :
    mSize(size)
  , mShape(shape)
  , mPen(QPen(color))
  , mBrush(Qt::NoBrush)
  , mPenDefined(true)
{}


QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size) :
    mSize(size)
  , mShape(shape)
  , mPen(QPen(color))
  , mBrush(QBrush(fill))
  , mPenDefined(true)
{}

QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size) :
    mSize(size)
  , mShape(shape)
  , mPen(pen)
  , mBrush(brush)
  , mPenDefined(pen.style() != Qt::NoPen)
{}


QCPScatterStyle::QCPScatterStyle(const QPixmap &pixmap) :
    mSize(5)
  , mShape(ssPixmap)
  , mPen(Qt::NoPen)
  , mBrush(Qt::NoBrush)
  , mPixmap(pixmap)
  , mPenDefined(false)
{}


QCPScatterStyle::QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush, double size) :
    mSize(size)
  , mShape(ssCustom)
  , mPen(pen)
  , mBrush(brush)
  , mCustomPath(customPath)
  , mPenDefined(false)
{}


void QCPScatterStyle::setSize(double size)
{
    mSize = size;
}

void QCPScatterStyle::setShape(QCPScatterStyle::ScatterShape shape)
{
    mShape = shape;
}

void QCPScatterStyle::setPen(const QPen &pen)
{
    mPenDefined = true;
    mPen = pen;
}

void QCPScatterStyle::setBrush(const QBrush &brush)
{
    mBrush = brush;
}

void QCPScatterStyle::setPixmap(const QPixmap &pixmap)
{
    setShape(ssPixmap);
    mPixmap = pixmap;
}

void QCPScatterStyle::setCustomPath(const QPainterPath &customPath)
{
    setShape(ssCustom);
    mCustomPath = customPath;
}


void QCPScatterStyle::applyTo(QCPPainter *painter, const QPen &defaultPen) const
{
    painter->setPen(mPenDefined? mPen: defaultPen);
    painter->setBrush(mBrush);
}

void QCPScatterStyle::drawShape(QCPPainter *painter, QPointF pos) const
{
    drawShape(painter, pos.x(), pos.y());
}

void QCPScatterStyle::drawShape(QCPPainter *painter, double x, double y) const
{
    double w = mSize / 2.0;
    switch (mShape)
    {
    case ssNone: break;
    case ssDot:
        painter->drawLine(QPointF(x, y), QPointF(x+0.0001, y));
        break;
    case ssCross:
        painter->drawLine(QLineF(x-w, y-w, x+w, y+w));
        painter->drawLine(QLineF(x-w, y+w, x+w, y-w));
        break;
    case ssPlus:
        painter->drawLine(QLineF(x-w,   y, x+w,   y));
        painter->drawLine(QLineF(  x, y+w,   x, y-w));
        break;
    case ssCircle:
        painter->drawEllipse(QPointF(x , y), w, w);
        break;
    case ssDisc:
        QBrush b = painter->brush();
        painter->setBrush(painter->pen().color());
        painter->drawEllipse(QPointF(x , y), w, w);
        painter->setBrush(b);
        break;
    case ssSquare:
    {
        painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
        break;
    }
    case ssDiamond:
    {
        painter->drawLine(QLineF(x-w,   y,   x, y-w));
        painter->drawLine(QLineF(  x, y-w, x+w,   y));
        painter->drawLine(QLineF(x+w,   y,   x, y+w));
        painter->drawLine(QLineF(  x, y+w, x-w,   y));
        break;
    }
    case ssStar:
    {
        painter->drawLine(QLineF(x-w,   y, x+w,   y));
        painter->drawLine(QLineF(  x, y+w,   x, y-w));
        painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.707, y+w*0.707));
        painter->drawLine(QLineF(x-w*0.707, y+w*0.707, x+w*0.707, y-w*0.707));
        break;
    }
    case ssTriangle:
    {
        painter->drawLine(QLineF(x-w, y+0.755*w, x+w, y+0.755*w));
        painter->drawLine(QLineF(x+w, y+0.755*w,   x, y-0.977*w));
        painter->drawLine(QLineF(  x, y-0.977*w, x-w, y+0.755*w));
        break;
    }
    case ssTriangleInverted:
    {
        painter->drawLine(QLineF(x-w, y-0.755*w, x+w, y-0.755*w));
        painter->drawLine(QLineF(x+w, y-0.755*w,   x, y+0.977*w));
        painter->drawLine(QLineF(  x, y+0.977*w, x-w, y-0.755*w));
        break;
    }
    case ssCrossSquare:
    {
        painter->drawLine(QLineF(x-w, y-w, x+w*0.95, y+w*0.95));
        painter->drawLine(QLineF(x-w, y+w*0.95, x+w*0.95, y-w));
        painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
        break;
    }
    case ssPlusSquare:
    {
        painter->drawLine(QLineF(x-w,   y, x+w*0.95,   y));
        painter->drawLine(QLineF(  x, y+w,        x, y-w));
        painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
        break;
    }
    case ssCrossCircle:
    {
        painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.670, y+w*0.670));
        painter->drawLine(QLineF(x-w*0.707, y+w*0.670, x+w*0.670, y-w*0.707));
        painter->drawEllipse(QPointF(x, y), w, w);
        break;
    }
    case ssPlusCircle:
    {
        painter->drawLine(QLineF(x-w,   y, x+w,   y));
        painter->drawLine(QLineF(  x, y+w,   x, y-w));
        painter->drawEllipse(QPointF(x, y), w, w);
        break;
    }
    case ssPeace:
    {
        painter->drawLine(QLineF(x, y-w,         x,       y+w));
        painter->drawLine(QLineF(x,   y, x-w*0.707, y+w*0.707));
        painter->drawLine(QLineF(x,   y, x+w*0.707, y+w*0.707));
        painter->drawEllipse(QPointF(x, y), w, w);
        break;
    }
    case ssPixmap:
    {
        painter->drawPixmap(x-mPixmap.width()*0.5, y-mPixmap.height()*0.5, mPixmap);
        break;
    }
    case ssCustom:
        QTransform oldTransform = painter->transform();
        painter->translate(x, y);
        painter->scale(mSize/6.0, mSize/6.0);
        painter->drawPath(mCustomPath);
        painter->setTransform(oldTransform);
        break;
    }
}


