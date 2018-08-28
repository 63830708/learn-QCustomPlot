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


class QCP_LIB_DECL QCPLayerable: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible)
    Q_PROPERTY(QCustomPlot* parentPlot READ parentPlot)
    Q_PROPERTY(QCPLayerable* parentLayerable READ parentLayerable)
    Q_PROPERTY(QCPLayer* layer READ layer WRITE setLayer)
    Q_PROPERTY(bool antialiased READ antialiased WRITE setAntialiased)

public:
    QCPLayerable(QCustomPlot *plot, QString targetLayer="", QCPLayerable *parentLayerable);
    ~QCPLayerable();

    bool visible() const { return mVisible;}
    QCustomPlot *parentPlot() const { return mParentPlot;}
    QCPLayerable *parentLayerable() const { return mParentLayerable.data();}
    QCPLayer *layer() const { return mLayer;}
    bool antialiased() const { return mAntialiased;}

    void setVisible(bool on);
    bool setLayer(QCPLayer *layer);
    bool setLayer(const QString &layerName);
    void setAntialiased(bool enabled);

    virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;

    bool realVisibility() const;

protected:
    // property
    bool mVisible;
    QCustomPlot *mParentPlot;
    QWeakPointer<QCPLayerable> mParentLayerable;
    QCPLayer *mLayer;
    bool mAntialiased;

    virtual void parentPlotInitialized(QCustomPlot *parentPlot);
    virtual QCP::Interaction selectionCategory() const;
    virtual QRect clipRect() const;
    virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
    virtual void draw(QCPPainter *painter) = 0;

    // events
    virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
    virtual void deselectEvent(bool *selectionStateChanged);

    //
    void initializeParentPlot(QCustomPlot *parentPlot);
    void setParentLayerable(QCPLayerable *parentLayerable);
    bool moveToLayer(QCPLayer *layer, bool prepend);
    void applyAntialiasingHint(QCPPainter* painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const;

private:
    Q_DISABLE_COPY(QCPLayerable)

    friend class QCustomPlot;
    friend class QCPAxisRect;
};


#endif // LAYERMY_H
