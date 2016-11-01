#include "gifanimationdemowidget.h"
#include "ui_gifanimationdemowidget.h"
#include <QMediaPlayer>
#include <QDir>
#include <QUrl>

GIFAnimationDemoWidget::GIFAnimationDemoWidget( QWidget* parent ) :
    QWidget( parent ), ui( new Ui::GIFAnimationDemoWidget ), m_movie( ":/pusheen.gif" )
{
    ui->setupUi( this );
    ui->lbMovie->setMovie( &m_movie );
    m_movie.start();
    QWidget ::setWindowFlags(Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/creedence.mp3"));
    player->play();
}

GIFAnimationDemoWidget::~GIFAnimationDemoWidget() {
    delete ui;
}
