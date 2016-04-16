#ifndef VISUALISATIONFRAME_H
#define VISUALISATIONFRAME_H

#include <QObject>
#include <QFrame>
#include <QPainter>

class VisualisationFrame : public QFrame
{

    Q_OBJECT

public:
    VisualisationFrame(QWidget *parent);


    enum visualisationType {
        Scope,
        Bars
    };

public slots:
    void setVisualisation(QVector<qint16> visualisation);
    void setVisualisationType(visualisationType type);

signals:
    void visualisationRateChanged(int rate);

public:
    void paintEvent(QPaintEvent *paintEvent);
    void resizeEvent(QResizeEvent*);

private:
    QVector<qint16> visualisations;
    visualisationType VisType = Scope;
};

#endif // VISUALISATIONFRAME_H
