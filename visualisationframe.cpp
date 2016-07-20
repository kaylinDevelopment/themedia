#include "visualisationframe.h"

VisualisationFrame::VisualisationFrame(QWidget* parent) : QFrame(parent)
{
    for (int i = 0; i < 1024; i++) {
        previousVolumes.append(0);
    }
}

void VisualisationFrame::setVisualisation(QVector<qint16> visualisation) {
    if (VisType == Lines) {
        qreal loudness = 0;
        qreal peak_value(USHRT_MAX);

        for (int i = 0; i < visualisation.count(); i++) {
            qreal value = qAbs(qreal(visualisation.at(i)));
            if (value > loudness) {
                loudness = value;
            }
        }

        loudness /= peak_value;

        previousVolumes.append(loudness);

        while (previousVolumes.count() > this->width() / 2) {
            previousVolumes.removeFirst();
        }

        while (previousVolumes.count() < this->width() / 2) {
            previousVolumes.prepend(0);
        }
    } else {
        visualisations = visualisation;
    }
}

void VisualisationFrame::paintEvent(QPaintEvent *paintEvent) {
    QPainter painter(this);
    if (VisType == Lines) {
        int i = 0;
        for (qreal level : previousVolumes) {
            //painter.drawLine(i, this->height(), i, this->height() - (level * this->height()));
            int distanceFromMiddle = (level * this->height()) / 2;
            painter.drawLine(i, this->height() / 2 + distanceFromMiddle, i, this->height() / 2 - distanceFromMiddle);
            i = i + 2;
        }

    } else {
        if (visualisations.count() != 0) {
            painter.setBrush(this->palette().brush(QPalette::WindowText));
            qint16 oldypoint = visualisations.at(0);
            int iteration = 0;
            for (qint16 ypoint : visualisations) {
                if (VisType == Scope) {
                    painter.drawLine(iteration, oldypoint * this->height() / 2 / 32767 + this->height() / 2, iteration + 1, ypoint * this->height() / 2 / 32767 + this->height() / 2);
                    oldypoint = ypoint;
                } else if (VisType == Bars) {
                    painter.drawLine(iteration, this->height() / 2, iteration, this->height() / 2 + ypoint / 50);
                }
                iteration++;
            }
        }
    }

}

void VisualisationFrame::resizeEvent(QResizeEvent *) {
    if (this->VisType == Lines) {
        emit visualisationRateChanged(500);
    } else {
        emit visualisationRateChanged(this->width());
    }
}

void VisualisationFrame::setVisualisationType(visualisationType type) {
    this->VisType = type;
    if (this->VisType == Lines) {
        emit visualisationRateChanged(500);
    } else {
        emit visualisationRateChanged(this->width());
    }


}
