#ifndef PRESENTATIONWIDGET_H
#define PRESENTATIONWIDGET_H

#include <QWidget>

class Settings;
class SlideLoader;

class PresentationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PresentationWidget(Settings *settings, QWidget *parent = 0);
    ~PresentationWidget();

public slots:
    void nextSlide();
    void previousSlide();
    void showSettings();

protected:
    void keyPressEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);

private slots:
    void slidesFound(int count);
    void slide(int index, QImage image);
    void noSlide(int index);

private:
    void scaleImage(QPainter *painter, const QImage &img) const;
    void drawInstructionSlide(QPainter *painter);
    void reloadSettings();

private:
    Settings *settings_;

    int currentSlideIndex_;
    int slideCount_;
    QImage currentSlide_;

    SlideLoader *loader_;
    QThread *loaderThread_;

    int slidesPerTurn_;

    enum State {
        Slide,
        Intermission,
        OutOfSlides
    };
    State state_;
    int slideCounter_;

    QImage instructions_;
};

#endif // PRESENTATIONWIDGET_H
