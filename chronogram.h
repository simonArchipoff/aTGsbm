#ifndef CHRONOGRAM_H
#define CHRONOGRAM_H

#include <QObject>
#include <QRgb>
#include <QList>
#include <QPair>
#include <QQuickPaintedItem>
#include <QImage>


class Chronogram : public QQuickPaintedItem {
    Q_OBJECT
public:
    Chronogram();


public slots:
    void addNewData( QVector<float> b);
    void setSampleRate(int);
    void setFrequency(int f);
    void setFold(bool);
    void setHistorySize(int);
    void resetImage();

    void paint(QPainter *painter) override;

signals:
    void newSamples(int lastSample);

private:
    int sampleRate = 0;
    int frequency = 0;
    int history = 0;
    bool fold = false;


    QImage image;
    int currentLine;
    int currentRow;
    float f = 0.0;
};


#endif // CHRONOGRAM_H
