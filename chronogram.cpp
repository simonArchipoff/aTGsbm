#include "chronogram.h"
#include <QPainter>


Chronogram::Chronogram() {
    setFrequency(3);
    setSampleRate(48000);
    setHistorySize(2);
}

void Chronogram::setSampleRate(int sr){
    sampleRate = sr;
    resetImage();
}

void Chronogram::setFrequency(int f){
    frequency = f;
    resetImage();
}

void Chronogram::setFold(bool b){
    fold = b;
    resetImage();
}

void Chronogram::setHistorySize(int s){
    history = s;
    resetImage();
}

void Chronogram::addNewData( QVector<float> b){
    //qDebug() << image.size();
#if 0
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            float fx = 1.0 * x / image.width();
            float fy = 1.0 * y / image.height();
            // Crée un motif caractéristique : un dégradé basé sur la position
            int red = int((fx + fy)*256) % 256;  // Composante rouge variant en fonction des coordonnées
            int green = int((fx * fy)*256) % 256;  // Composante verte
            int blue = int((x*256)) % 256;  // Composante bleue

            // Définit la couleur du pixel
            QColor color(red, green, blue);
            image.setPixelColor(x, y, color);  // Remplit le pixel avec la couleur définie
        }
    }
    update();
    return;
#endif

    for(int i = 0; i < b.size(); i++){
        float val = abs(b[i]);

        auto v = int(val * 255*10);
        if(v>255)
            v = 255;
        auto c = QColor(v,v,v);
        image.setPixelColor(QPoint(currentRow,currentLine++),c);

        auto s = image.size();
        if(currentLine == s.height()){
            currentLine = 0;
            currentRow++;
            if(currentRow == s.width()){
                currentRow = 0;
            }
        }
    }
    update();
}

void Chronogram::resetImage(){
    int width = 1000 ;
    assert(sampleRate % frequency == 0);
    int height = sampleRate / frequency ;// * (fold ? 2 :1));
    qDebug() << "reset" <<  sampleRate << width << height ;
    if(width > 0 && height > 0){
        image = QImage(width, height, QImage::Format_RGB32);
        currentLine = 0;
        currentRow = 0;
    }
    qDebug() << "reset" << image.size();
}

void Chronogram::paint(QPainter *painter) {
    addNewData({});

    QRectF targetRect = boundingRect();

    // Calcul du facteur d'échelle pour conserver les proportions
    // Appliquer une transformation pour redimensionner l'image
//painter->save();  // Sauvegarde l'état actuel du QPainter
//    painter->scale(1.0*targetRect.width() / image.width(),1.0 * targetRect.height() / image.height());

    painter->drawImage(targetRect, image);
//«    painter->restore();
}
