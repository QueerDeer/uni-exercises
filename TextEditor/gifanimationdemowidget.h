#ifndef GIFANIMATIONDEMOWIDGET_H
#define GIFANIMATIONDEMOWIDGET_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class GIFAnimationDemoWidget;
}

class GIFAnimationDemoWidget : public QWidget {
    Q_OBJECT

public:
    explicit GIFAnimationDemoWidget( QWidget* parent = 0 );
    ~GIFAnimationDemoWidget();

private:
    Ui::GIFAnimationDemoWidget* ui;

    QMovie m_movie;
};

#endif // GIFANIMATIONDEMOWIDGET_H
