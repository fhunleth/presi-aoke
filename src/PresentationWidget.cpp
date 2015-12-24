#include "PresentationWidget.h"
#include <QKeyEvent>
#include <QPainter>
#include <QThread>
#include <QTimer>

#include "SettingsDialog.h"
#include "Settings.h"
#include "SlideLoader.h"

PresentationWidget::PresentationWidget(Settings *settings, QWidget *parent) : QWidget(parent),
    settings_(settings),
    state_(Slide)
{
    setMinimumSize(1024, 768);

    loaderThread_ = new QThread();
    loader_ = new SlideLoader();
    loader_->moveToThread(loaderThread_);
    loaderThread_->start();

    connect(loader_, SIGNAL(slide(int,QImage)), SLOT(slide(int,QImage)));
    connect(loader_, SIGNAL(slidesFound(int)), SLOT(slidesFound(int)));
    connect(loader_, SIGNAL(noSlide(int)), SLOT(noSlide(int)));

    intermissionTimer_ = new QTimer(this);
    intermissionTimer_->setSingleShot(true);
    intermissionTimer_->setInterval(1000); // Long enough to catch a double click, but not so long to be annoying.
    connect(intermissionTimer_, SIGNAL(timeout()), SLOT(intermissionTimeout()));

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

    case Qt::Key_F5:
        showFullScreen();
        break;

    case Qt::Key_Escape:
        showNormal();
        break;

    case Qt::Key_F:
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;

    case Qt::Key_S: // settings
        showSettings();
        break;

    case Qt::Key_Q: // quit
        close();
        break;

    case Qt::Key_R: // restart
        reloadSettings();
        break;

    default:
        break;
    }
}

void PresentationWidget::scaleImage(QPainter *painter, const QImage &img) const
{
    QRect ourBounds = rect();
    QRect imgBounds = img.rect();

    float sx = (float) ourBounds.width() / imgBounds.width();
    float sy = (float) ourBounds.height() / imgBounds.height();
    float s = qMin(sx, sy);
    int newWidth = qRound(s * imgBounds.width());
    int newHeight = qRound(s * imgBounds.height());
    QRect outBounds((ourBounds.width() - newWidth) / 2,
                    (ourBounds.height() - newHeight) / 2,
                    newWidth,
                    newHeight);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(outBounds, img);
}

void PresentationWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    p.fillRect(rect(), Qt::black);

    QImage img;
    switch (state_) {
    case Slide:
        if (currentSlideIndex_ < 0 || currentSlide_.isNull())
            img = instructionSlide();
        else
            img = currentSlide_;
        break;

    case IntermissionTimeout:
    case Intermission:
        img = intermissionSlide();
        break;

    default:
    case OutOfSlides:
        img = outOfSlidesSlide();
        break;
    }
    scaleImage(&p, img);
}

QImage PresentationWidget::instructionSlide()
{
    if (instructionsSlide_.isNull())
        instructionsSlide_ = QImage(":/assets/instructions.png");
    return instructionsSlide_;
}

QImage PresentationWidget::outOfSlidesSlide()
{
    if (outOfSlidesSlide_.isNull())
        outOfSlidesSlide_ = QImage(":/assets/outofslides.png");
    return outOfSlidesSlide_;
}

QImage PresentationWidget::intermissionSlide()
{
    if (intermissionSlide_.isNull())
        intermissionSlide_ = QImage(":/assets/intermission.png");
    return intermissionSlide_;
}

void PresentationWidget::slidesFound(int count)
{
    slideCount_ = count;
    update();
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
    state_ = OutOfSlides;
    update();
}

void PresentationWidget::intermissionTimeout()
{
    // Allow the user to go the next slides onces
    // they've waited long enough.
    if (state_ == IntermissionTimeout)
        state_ = Intermission;
}

void PresentationWidget::reloadSettings()
{
    slideCount_ = 0;
    QMetaObject::invokeMethod(loader_, "setSlidePath", Qt::QueuedConnection, Q_ARG(QString, settings_->slidePath()));
    slidesPerTurn_ = settings_->slidesPerTurn();

    // Reset state.
    currentSlideIndex_ = -1;
    startSlideIndex_ = 0;
    state_ = Slide;
    slideCounter_ = 0;
    intermissionTimer_->stop();

    update();
}

void PresentationWidget::nextSlide()
{
    switch (state_) {
    case Slide:
        if (currentSlideIndex_ + 1 >= slideCount_) {
            state_ = OutOfSlides;
            update();
        } else if (currentSlideIndex_ + 1 - startSlideIndex_ == slidesPerTurn_) {
            state_ = IntermissionTimeout;
            intermissionTimer_->start();
            update();
        } else {
            currentSlideIndex_++;
            QMetaObject::invokeMethod(loader_, "requestSlide", Qt::QueuedConnection, Q_ARG(int, currentSlideIndex_));
        }
        break;
    case IntermissionTimeout:
        // Ignore next events until the intermission timer expires. This
        // prevents accidents where someone hits next too aggressively.
        break;

    case Intermission:
        currentSlideIndex_++;
        QMetaObject::invokeMethod(loader_, "requestSlide", Qt::QueuedConnection, Q_ARG(int, currentSlideIndex_));
        state_ = Slide;
        startSlideIndex_ = currentSlideIndex_;
        break;

    case OutOfSlides:
        // Ignore.
        break;
    }

}

void PresentationWidget::previousSlide()
{
    if (currentSlideIndex_ - 1 < 0 || currentSlideIndex_ == startSlideIndex_)
        return;

    switch (state_) {
    case Slide:
    default:
        currentSlideIndex_--;
        break;

    case IntermissionTimeout:
        // It's ok to go back when waiting for the intermission timeout, so
        // stop the timer and go back.
        intermissionTimer_->stop();
        state_ = Slide;
        break;

    case OutOfSlides:
    case Intermission:
        // If the user goes back, we're no longer in Intermission or at the end.
        state_ = Slide;
        break;
    }

    QMetaObject::invokeMethod(loader_, "requestSlide", Qt::QueuedConnection, Q_ARG(int, currentSlideIndex_));
}

void PresentationWidget::showSettings()
{
    SettingsDialog d(settings_);
    if (d.exec()) {
        reloadSettings();
    }
}
