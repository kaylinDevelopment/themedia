#include "visualisationframe.h"

VisualisationFrame::VisualisationFrame(QWidget* parent) : QFrame(parent)
{

}

void VisualisationFrame::setVisualisation(QVector<qint16> visualisation) {
    visualisations = visualisation;
}

void VisualisationFrame::paintEvent(QPaintEvent *paintEvent) {
    if (visualisations.count() != 0) {
        QPainter* painter = new QPainter(this);
        painter->setBrush(this->palette().brush(QPalette::WindowText));
        qint16 oldypoint = visualisations.at(0);
        int iteration = 0;
        for (qint16 ypoint : visualisations) {
            if (VisType == Scope) {
                painter->drawLine(iteration, oldypoint * this->height() / 2 / 32767 + this->height() / 2, iteration + 1, ypoint * this->height() / 2 / 32767 + this->height() / 2);
                oldypoint = ypoint;
            } else if (VisType == Bars) {
                painter->drawLine(iteration, this->height() / 2, iteration, this->height() / 2 + ypoint / 50);
            }
            iteration++;
        }
    }
}

void VisualisationFrame::resizeEvent(QResizeEvent *) {
    emit visualisationRateChanged(this->width());
}

void VisualisationFrame::setVisualisationType(visualisationType type) {
    this->VisType = type;


}
