#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    qtLanguageTranslator.load(QString("GAUSS_MAPREDUCE_") + QString("ru"));
    qApp->installTranslator(&qtLanguageTranslator);

    ui->setupUi(this);

    ui->comboBox->addItem("RU");
    ui->comboBox->addItem("EN");
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->spinBox_2->setValue(QThread::idealThreadCount());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_3_clicked()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg)"));

    if (file_name.isNull())
        return;

    int w = ui->label->maximumWidth();
    int h = ui->label->maximumHeight();

    ui->label->setPixmap(QPixmap(file_name).scaled(w,h,Qt::KeepAspectRatio));
    ui->label->show();

    ui->label_2->setPixmap(QPixmap(file_name).scaled(w,h,Qt::KeepAspectRatio));
    ui->label_2->show();

    ui->progressBar->setValue(0);

    ui->pushButton_2->setEnabled(true);
}

void Widget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1=="RU")
    {
        qtLanguageTranslator.load(QString("GAUSS_MAPREDUCE_") + QString("ru"));
        qApp->installTranslator(&qtLanguageTranslator);

        ui->retranslateUi(this);
    }
    else
    {
        qtLanguageTranslator.load(QString("GAUSS_MAPREDUCE_") + QString("en"));
        qApp->installTranslator(&qtLanguageTranslator);

        ui->retranslateUi(this);
    }
}

struct Task
{
    int i_coord;
    int j_coord;
    int kernel_size;
    int kernel_half_size;
    QColor point;
    QColor **extended_matrix;
    double **gKernel;
};

using Tasks = std::vector<Task>;

Task map(const Task& task)
{
    double R = 0.0, G = 0.0, B = 0.0;
    for (int k = 0; k < task.kernel_size; k++)
    {
        for (int l = 0; l < task.kernel_size; l++)
        {
            R += task.extended_matrix[task.i_coord-task.kernel_half_size+k][task.j_coord-task.kernel_half_size+l].red() * task.gKernel[k][l];
            G += task.extended_matrix[task.i_coord-task.kernel_half_size+k][task.j_coord-task.kernel_half_size+l].green() * task.gKernel[k][l];
            B += task.extended_matrix[task.i_coord-task.kernel_half_size+k][task.j_coord-task.kernel_half_size+l].blue() * task.gKernel[k][l];
        }
    }
    if (R > 255) R = 255;
    if (G > 255) G = 255;
    if (B > 255) B = 255;
    if (R < 0) R = 0;
    if (G < 0) G = 0;
    if (B < 0) B = 0;
    Task task2;
    task2.i_coord = task.i_coord;
    task2.j_coord = task.j_coord;
    task2.point = task.point;
    task2.point.setRgb(R,G,B);
    R = 0;
    G = 0;
    B = 0;
    return task2;
}

void reduce(Tasks &result, const Task &task)
{
    usleep(20);
    result.push_back(task);
}


void Widget::on_pushButton_2_clicked()
{
    flag = 1;
    ui->pushButton_5->setEnabled(true);

    kernel_size = ui->spinBox->text().toInt();
    int kernel_half_size = (kernel_size-1)/2;

    QImage image (file_name);

    w = image.width();
    h = image.height();

    QImage completed_image = image;

    QColor point;

    QColor **matrix = new QColor *[h];

    for (int i = 0; i < h; i++) {
      matrix[i] = new QColor [w];
    }

    QColor **extended_matrix = new QColor *[h+2*kernel_half_size];

    for (int i = 0; i < h+2*kernel_half_size; i++) {
      extended_matrix[i] = new QColor [w+2*kernel_half_size];
    }

    for (int i=0; i<h; i++)
    {
        for (int j=0; j<w; j++)
        {
            matrix[i][j] = image.pixel(j, i);
        }
    }

    for (int i=0; i<h; i++)
    {
        for (int j=0; j<w; j++)
        {
            extended_matrix[i+kernel_half_size][j+kernel_half_size] = matrix[i][j];
        }
    }

    for (int i=0; i<kernel_half_size; i++)
    {
        for (int j=kernel_half_size; j<w+kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[0][j-kernel_half_size];
        }
    }

    for (int i=h+kernel_half_size; i<h+2*kernel_half_size; i++)
    {
        for (int j=kernel_half_size; j<w+kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[h-1][j-kernel_half_size];
        }
    }

    for (int i=kernel_half_size; i<h+kernel_half_size; i++)
    {
        for (int j=0; j<kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[i-kernel_half_size][0];
        }
    }

    for (int i=kernel_half_size; i<h+kernel_half_size; i++)
    {
        for (int j=w+kernel_half_size; j<w+2*kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[i-kernel_half_size][w-1];
        }
    }

    for (int i=0; i<kernel_half_size; i++)
    {
        for (int j=0; j<kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[0][0];
        }
    }

    for (int i=0; i<kernel_half_size; i++)
    {
        for (int j=w+kernel_half_size; j<w+2*kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[0][w-1];
        }
    }

    for (int i=h+kernel_half_size; i<h+2*kernel_half_size; i++)
    {
        for (int j=w+kernel_half_size; j<w+2*kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[h-1][w-1];
        }
    }

    for (int i=h+kernel_half_size; i<h+2*kernel_half_size; i++)
    {
        for (int j=0; j<kernel_half_size; j++)
        {
            extended_matrix[i][j] = matrix[h-1][0];
        }
    }

    Tasks tasks;

    double **Kernel = new double *[kernel_size];

    for (int i = 0; i < kernel_size; i++) {
      Kernel[i] = new double [kernel_size];
    }

    for (int i = 0; i < kernel_size; i++)
    {
        for (int j = 0; j < kernel_size; j++)
        {
            Kernel[i][j] = gKernel[i][j];
        }
    }

    for (int i=kernel_half_size; i<h+kernel_half_size; i++)
    {
        for (int j=kernel_half_size; j<w+kernel_half_size; j++)
        {
            tasks.push_back(Task{i, j, kernel_size, kernel_half_size, point, extended_matrix, Kernel});

        }
    }

    QFutureWatcher<Tasks> watcher;
    connect(ui->pushButton_5, SIGNAL(clicked(bool)),
    &watcher, SLOT(cancel()));

    QFuture<Tasks> res = QtConcurrent::mappedReduced(tasks, map, reduce);
    watcher.setFuture(res);

    while (!watcher.isFinished())
    {
        ui->progressBar->setValue(100*watcher.progressValue()/watcher.progressMaximum());
        QApplication::processEvents();
    }

    if (flag)
    {
        Tasks vec = res.result();
        for(int i = 0; i<vec.size(); i++)
        {
            completed_image.setPixel(vec[i].j_coord-kernel_half_size, vec[i].i_coord-kernel_half_size, vec[i].point.rgb());
        }
    }

    ui->label_2->setPixmap(QPixmap::fromImage(completed_image));
    for_save = completed_image;
    ui->pushButton_5->setEnabled(false);

    flag = 1;
}

void Widget::on_pushButton_clicked()
{
    int size = ui->spinBox->text().toInt();
    int half_size = (size-1)/2;

    ui->tableWidget->setRowCount(size);
    ui->tableWidget->setColumnCount(size);


    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;

    double sum = 0.0;

    for (int x = -half_size; x <= half_size; x++)
    {
        for(int y = -half_size; y <= half_size; y++)
        {
            r = sqrt(x*x + y*y);
            gKernel[x + half_size][y + half_size] = (exp(-(r*r)/s))/(M_PI * s);
            sum += gKernel[x + half_size][y + half_size];
        }
    }

    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            gKernel[i][j] /= sum;

    for(int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(gKernel[i][j])));
        }
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void Widget::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    gKernel[item->row()][item->column()] = item->text().toDouble();
}

void Widget::on_pushButton_4_clicked()
{
    QFileInfo source_name(file_name);

    if (for_save.width()*for_save.height() != 0)
    {
        for_save.save("./" + source_name.baseName() + QString::number(++iter) + '.' + source_name.suffix());
    }
}

void Widget::on_pushButton_5_clicked()
{
    ui->pushButton_5->setEnabled(false);
    flag = 0;
}
