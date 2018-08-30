#ifndef PAINTERMY_H
#define PAINTERMY_H

#include "global.h"

class QCPPainter;

class QCP_LIB_DECL QCPScatterStyle
{
    Q_GADGET
public:
    Q_ENUMS(ScatterShape)
    enum ScatterShape
    {
        ssNone
        , ssDot
        , ssCross
        , ssPlus
        , ssCircle
        , ssDisc
        , ssSquare
        , ssDiamond
        , ssStar
        , ssTriangle
        , ssTriangleInverted
        , ssCrossSquare
        , ssPlusSquare
        , ssCrossCircle
        , ssPlusCircle
        , ssPeace
        , ssPixmap
        , ssCustom
    };

    QCPScatterStyle();
    QCPScatterStyle(ScatterShape shape, double size=6);
    QCPScatterStyle(ScatterShape shape, const QColor &color, double size);
    QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size);
    QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size);
    QCPScatterStyle(const QPixmap &pixmap);
    QCPScatterStyle(const QPaiinterPath &customPath, const QPen &pen, const QBrush &brush=Qt::NoBrush, double size=6);

    double size() const { return mSize;}
    ScatterShape shape() const { return mShape;}
    QPen pen() const { return mPen;}
    QBrush brush() const { return mBrush;}
    QPixmap pixmap() const { return mPixmap;}
    QPainterPath customPath() const { return mCustomPath;}

    void setSize(double size);
    void setShape(ScatterShape shape);
    void setPen(const QPen& pen);
    void setBrush(const QBrush &brush);
    void setPixmap(const QPixmap &pixmap);
    void setCustomPath(const QPainterPath &customPath);

    bool isNone() const { return ssNone == mShape;}
    bool isPenDefined() const { return mPenDefined;}
    void applyTo(QCPPainter *painter, const QPen &defaultPen) const;
    void drawShape(QCPPainter *painter, QPointF pos) const;
    void drawShape(QCPPainter *painter, double x, double y) const;
protected:
    double mSize;
    ScatterShape mShape;
    QPen mPen;
    QBrush mBrush;
    QPixmap mPixmap;
    QPainterPath mCustomPath;

    bool mPenDefined;
};
Q_DECLARE_TYPEINFO(QCPScatterStyle, Q_MOVABLE_TYPE);


class QCP_LIB_DECL QCPPainter: public QPainter
{
    Q_GADGET
public:
    enum PainterMode
    {
        pmDefault       = 0x00,
        pmVectorized    = 0x01,
        pmNoCaching     = 0x02,
        pmNonCosmetic   = 0x03
    };
    Q_FLAGS(PainterMode PainnterModes)
    Q_DECLARE_FLAGS(PainterModes, PainterMode)

    QCPPainter();
    QCPPainter(QPaintDevice *device);
    ~QCPPainter();

    bool Antialiasing() const { reutrn testRenderHint(QPainter);}
    PainterModes modes() const { return mModes;}

    void setAntialiasing(bool enabled);
    void setMode(PainterMode mode, bool enabled=true);
    void setModes(PainterModes modes);

    void setPen(const QPen &pen);
    void setPen(const QColor &color);
    void setPen(Qt::PenStyle penStyle);
    void drawLine(const QLineF &line);
    void drawLine(const QPointF &p1, const QPointF &p2) { drawLine(QLineF(p1, p2));}
    void save();
    void restore();

    void makeNonCosmetic();
protected:
    PainterModes mModes;
    bool mIsAntialiasing;

    QStack<bool> mAntialiasingStack;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPPainter::PainterModes)
#endif // PAINTERMY_H
