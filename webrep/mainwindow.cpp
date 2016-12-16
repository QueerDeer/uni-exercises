#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->webviewer->load(QUrl::fromEncoded("qrc:/typicall_report_2.html"));
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
    ui->webviewer->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    ui->webviewer->page()->setPluginFactory(new pluginFactory(ui->webviewer->page()));

    frame = ui->webviewer->page()->mainFrame();

    connect(ui->webviewer, SIGNAL(loadFinished(bool)), this, SLOT(loadFinish()));
    connect(ui->calendarWidget, SIGNAL(selectionChanged()), this, SLOT(selectionChange()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_uploading_report_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", QDir::currentPath(), "html file (*.html)");
    if(fileName.isNull())
        return;
    else
        ui->webviewer->load(QUrl::fromLocalFile(fileName));
}

void MainWindow::loadFinish()
{
    frame->addToJavaScriptWindowObject("calendar", ui->calendarWidget, QScriptEngine::QtOwnership);
    data.clear();
    QWebElementCollection reports = frame->findAllElements(QString("div.report"));

    foreach(QWebElement report, reports)
    {
        QString id = report.attribute("id");
        QRegExp rx("report_(\\d+)_(\\d+)_(\\d+)");
        if(!rx.exactMatch(id))
            continue;
        QDate date(rx.cap(3).toInt(), rx.cap(2).toInt(),  rx.cap(1).toInt());
        data.insert(date, report);
    }

    QTextCharFormat highlight;

    highlight.clearBackground();
    QMap<QDate, QTextCharFormat> has_changed = ui->calendarWidget->dateTextFormat();
    QMapIterator<QDate, QTextCharFormat> i(has_changed);

    while (i.hasNext())
    {
        i.next();
        ui->calendarWidget->setDateTextFormat(i.key(), highlight);
    }

    highlight.setBackground(Qt::darkGreen);
    QMapIterator<QDate, QWebElement> j(data);

    while (j.hasNext())
    {
        j.next();
        j.value().findFirst("div.reportcontent").setStyleProperty("display", "none");
        ui->calendarWidget->setDateTextFormat(j.key(), highlight);
    }

    selectionChange();
}

void MainWindow::selectionChange()
{
    temporary_seleceted.findFirst("div.reportcontent").setStyleProperty("display", "none");
    QMapIterator<QDate, QWebElement> i(data);

    while (i.hasNext())
    {
        i.next();
        if (i.key() == ui->calendarWidget->selectedDate())
        {
            i.value().findFirst("div.reportcontent").setStyleProperty("display", "block");
            temporary_seleceted = i.value();
        }
    }
}
