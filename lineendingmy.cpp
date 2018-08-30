
#include "lineendingmy.h"
#include "paintermy.h"


QCPLineEnding::QCPLineEnding():
    mStyle(esNone),
    mWidth(8),
    mLength(10),
    mInverted(false)
{
}


QCPLineEnding::QCPLineEnding(EndingStyle style, double width, double length, bool inverted):
    mStyle(style),
    mWidth(width),
    mLength(length),
    mInverted(inverted)
{
}

void QCPLineEnding::setStyle(EndingStyle style)
{
    mStyle = style;
}

void QCPLineEnding::setWidth(double w)
{
    mWidth = width;
}

void QCPLineEnding::setLength(double l)
{
    mLength = length;
}

void QCPLineEnding::setInverted(bool inverted)
{
    mInverted = inverted;
}


double QCPLineEnding::boundingDistance() const
{
    switch(mStyle)
    {
    case esNone:
        return 0;
    case esFlatArrow:
    case esSpikeArrow:
    case esLineArrow:
    case esSkewedBar:
        return qSqrt(mWidth*mWidth + mLength*mLength);
    case esDisc:
    case esSquare:
    case esDiamond:
    case esBar:
    case esHalfBar:
        return mWidth*1.42;  // width * sqrt(2)
    }
    return 0;
}

double QCPLineEnding::realLength() const
{
    switch (mStyle)
    {
    case esNone:
    case esLineArrow:
    case esSkewedBar:
    case esBar:
    case esHalfBar:
        return 0;
    case esFlatArrow:
        return mLength;
    case esDisc:
    case esSquare:
    case esDiamond:
        return mWidth*0.5;
    case esSpikeArrow:
        return mLength*0.8;
    }
    return 0;
}


void QCPLineEnding::draw(QCPPainter *painter, const QVector2D &pos, const QVector2D &dir) const
{
    if(esNone == mStyle)
        return;

    QVector2D lengthVec(dir.normalized());
    if(lengthVec.isNull())
        lengthVec = QVector2D(1, 0);

    QVector2D widthVec(-lengthVec.y(), lengthVec.x());
    lengthVec *= mLength *(mInverted? -1: 1);
    widthVec  *= mWidth  *(mInverted? -1: 1) * 0.5;

    QPen   penBackup    = painter->pen();
    QBrush brushBackup  = painter->brush();
    QPen   miterPen     = penBackup;
    miterPen.setJoinStyle(Qt::MiterJoin);
    QBrush brush(painter->pen.color(), Qt::SolidPattern);

    switch(mStyle)
    {
    case esFlatArrow:
    {
        QPointF points[3] = { pos.toPointF(), (pos-lengthVec+widthVec).toPointF(), (pos-lengthVec-widthVec).toPointF()};
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 3);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esSpikeArrow:
    {
        QPointF points[4] = {pos.toPointF(),
                             (pos-lengthVec+widthVec).toPointF(),
                             (pos-lengthVec*0.8).toPointF(),
                             (pos-lengthVec-widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 4);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esLineArrow:
    {
        QPointF points[3] = {(pos-lengthVec+widthVec).toPointF(),
                             pos.toPointF(),
                             (pos-lengthVec-widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->drawPolyline(points, 3);
        painter->setPen(penBackup);
        break;
    }
    case esDisc:
    {
        painter->setBrush(brush);
        painter->drawEllipse(pos.toPointF(),  mWidth*0.5, mWidth*0.5);
        painter->setBrush(brushBackup);
        break;
    }
    case esSquare:
    {
        QVector2D widthVecPerp(-widthVec.y(), widthVec.x());
        QPointF points[4] = {(pos-widthVecPerp+widthVec).toPointF(),
                             (pos-widthVecPerp-widthVec).toPointF(),
                             (pos+widthVecPerp-widthVec).toPointF(),
                             (pos+widthVecPerp+widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 4);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esDiamond:
    {
        QVector2D widthVecPerp(-widthVec.y(), widthVec.x());
        QPointF points[4] = {(pos-widthVecPerp).toPointF(),
                             (pos-widthVec).toPointF(),
                             (pos+widthVecPerp).toPointF(),
                             (pos+widthVec).toPointF()
                            };
        painter->setPen(miterPen);
        painter->setBrush(brush);
        painter->drawConvexPolygon(points, 4);
        painter->setBrush(brushBackup);
        painter->setPen(penBackup);
        break;
    }
    case esBar:
    {
        painter->drawLine((pos+widthVec).toPointF(), (pos-widthVec).toPointF());
        break;
    }
    case esHalfBar:
    {
        painter->drawLine((pos+widthVec).toPointF(), pos.toPointF());
        break;
    }
    case esSkewedBar:
    {
        if (qFuzzyIsNull(painter->pen().widthF()) && !painter->modes().testFlag(QCPPainter::pmNonCosmetic))
        {
            // if drawing with cosmetic pen (perfectly thin stroke, happens only in vector exports), draw bar exactly on tip of line
            painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)).toPointF(),
                              (pos-widthVec-lengthVec*0.2*(mInverted?-1:1)).toPointF());
        } else
        {
            // if drawing with thick (non-cosmetic) pen, shift bar a little in line direction to prevent line from sticking through bar slightly
            painter->drawLine((pos+widthVec+lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*qMax(1.0, (double)painter->pen().widthF())*0.5).toPointF(),
                              (pos-widthVec-lengthVec*0.2*(mInverted?-1:1)+dir.normalized()*qMax(1.0, (double)painter->pen().widthF())*0.5).toPointF());
        }
        break;
    }
    }

}

void QCPLineEnding::draw(QCPPainter *painter, const QVector2D &pos, double angle) const
{
    draw(painter, pos, QVector2D(qCos(angle), qSin(angle)));
}
