#include "PresentationWidget.h"
#include <QKeyEvent>
#include <QPainter>
#include <QThread>
#include "SettingsDialog.h"
#include "Settings.h"
#include "SlideLoader.h"

PresentationWidget::PresentationWidget(Settings *settings, QWidget *parent) : QWidget(parent),
    settings_(settings)
{
    setMinimumSize(1024, 768);

    loaderThread_ = new QThread();
    loader_ = new SlideLoader();
    loader_->moveToThread(loaderThread_);
    loaderThread_->start();

    connect(loader_, SIGNAL(slide(int,QImage)), SLOT(slide(int,QImage)));
    connect(loader_, SIGNAL(slidesFound(int)), SLOT(slidesFound(int)));
    connect(loader_, SIGNAL(noSlide(int)), SLOT(noSlide(int)));

    reloadSettings();
}

PresentationWidget::~PresentationWidget()
{
    loaderThread_->quit();
    loaderThread_->wait();
    delete loader_;
    delete loaderThread_;
}

void PresentationWidget::keyPressEvent(QKeyEvent *k)
{
    switch (k->key()) {
    case Qt::Key_PageDown:
    case Qt::Key_Right:
    case Qt::Key_Space:
    case Qt::Key_Down:
    case Qt::Key_N:
    case Qt::Key_K: // vi
        nextSlide();
        break;

    case Qt::Key_PageUp:
    case Qt::Key_Left:
    case Qt::Key_Up:
    case Qt::Key_P:
    case Qt::Key_J: // vi
        previousSlide();
        break;

    case Qt::Key_F:
    case Qt::Key_F5:
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;

    case Qt::Key_S:
        showSettings();
        break;

    case Qt::Key_Escape:
    case Qt::Key_Q:
        close();
        break;

    default:
        break;
    }
}


void PresentationWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.fillRect(rect(), Qt::black);

    if (!currentSlide_.isNull()) {
        QRect ourBounds = rect();
        QRect imgBounds = currentSlide_.rect();

        float sx = (float) ourBounds.width() / imgBounds.width();
        float sy = (float) ourBounds.height() / imgBounds.height();
        float s = qMin(sx, sy);
        int newWidth = qRound(s * imgBounds.width());
        int newHeight = qRound(s * imgBounds.height());
        QRect outBounds((ourBounds.width() - newWidth) / 2,
                        (ourBounds.height() - newHeight) / 2,
                        newWidth,
                        newHeight);
        p.drawImage(outBounds, currentSlide_);
    }
}

void PresentationWidget::slidesFound(int count)
{
    slideCount_ = count;
}

void PresentationWidget::slide(int index, QImage image)
{
    if (index == currentSlideIndex_) {
        currentSlide_ = image;
        update();
    }
}

void PresentationWidget::noSlide(int index)
{
    slideCount_ = index;
}

void PresentationWidget::reloadSettings()
{
    slideCount_ = 0;
    currentSlideIndex_ = 0;
    QMetaObject::invokeMethod(loader_, "setSlidePath", Qt::QueuedConnection, Q_ARG(QString, settings_->slidePath()));
    slidesPerTurn_ = settings_->slidesPerTurn();
}


void PresentationWidget::nextSlide()
{
    if (currentSlideIndex_ + 1 >= slideCount_)
        return;

    currentSlideIndex_++;
    QMetaObject::invokeMethod(loader_, "requestSlide", Qt::QueuedConnection, Q_ARG(int, currentSlideIndex_));
}

void PresentationWidget::previousSlide()
{
    if (currentSlideIndex_ - 1 < 0)
        return;
    currentSlideIndex_--;
    QMetaObject::invokeMethod(loader_, "requestSlide", Qt::QueuedConnection, Q_ARG(int, currentSlideIndex_));
}

void PresentationWidget::showSettings()
{
    SettingsDialog d(settings_);
    if (d.exec()) {
        reloadSettings();
    }
}
