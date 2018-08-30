#ifndef LINEENDINGMY_H
#define LINEENDINGMY_H


#include "global.h"

class QCPPainter;

class QCP_LIB_DECL QCPLineEnding
{
    Q_GADGET
public:
    Q_ENUMS(EndingStyle)
    enum EndingStyle
    {
        esNone,
        esFlatArrow,
        esSpikeArrow,
        esLineArrow,
        esDisc,
        esSquare,
        esDiamond,
        esBar,
        esHalfBar,
        esSkewedBar
    };

    QCPLineEnding();
    QCPLineEnding(EndingStyle style, double width=8, double length=10, bool inverted=false);

    EndingStyle style() const { return mStyle;}
    double width() const { return mWidth;}
    double length() const { return mLength;}
    double inverted() const { return mInverted;}

    void setStyle(EndingStyle style);
    void setWidth(double w);
    void setLength(double l);
    void setInverted(bool inverted);

    double boundingDistance() const;
    double realLength() const;
    void draw(QCPPainter* painter, const QVector2D &pos, const QVector2D &dir) const;
    void draw(QCPPainter *painter, const QVector2D &pos, double angle) const;
protected:
    EndingStyle mStyle;
    double mWidth, mLength;
    bool mInverted;
};
Q_DECLARE_TYPEINFO(QCPLineEnding, Q_MOVABLE_TYPE);

#endif // LINEENDINGMY_H
