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
    void mouseMoveEvent(QMouseEvent *);

private slots:
    void slidesFound(int count);
    void slide(int index, QImage image);
    void noSlide(int index);
    void intermissionTimeout();
    void hideCursorTimeout();

private:
    void scaleImage(QPainter *painter, const QImage &img) const;
    QImage instructionSlide();
    QImage outOfSlidesSlide();
    QImage intermissionSlide();
    void reloadSettings();

private:
    Settings *settings_;

    int currentSlideIndex_;
    int startSlideIndex_;
    int slideCount_;
    QImage currentSlide_;

    SlideLoader *loader_;
    QThread *loaderThread_;
    QTimer *intermissionTimer_;
    QTimer *hideCursorTimer_;

    int slidesPerTurn_;

    enum State {
        Slide,
        IntermissionTimeout,
        Intermission,
        OutOfSlides
    };
    State state_;
    int slideCounter_;

    QImage instructionsSlide_;
    QImage outOfSlidesSlide_;
    QImage intermissionSlide_;
};

#endif // PRESENTATIONWIDGET_H
