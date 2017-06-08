#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    qtLanguageTranslator.load(QString("GAUSS_MAPREDUCE_") + QString("ru"));
    qApp->installTranslator(&qtLanguageTranslator);

    ui->setupUi(this);

    ui->comboBox->addItem("RU");
    ui->comboBox->addItem("EN");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_3_clicked()
{
    file_name = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg)"));

    if (file_name.isNull())
        return;

    int w = ui->label->maximumWidth();
    int h = ui->label->maximumHeight();

    ui->label->setPixmap(QPixmap(file_name).scaled(w,h,Qt::KeepAspectRatio));
    ui->label->show();
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

void Widget::on_pushButton_2_clicked()
{
    int kernel_size = ui->spinBox->text().toInt();
    int kernel_half_size = (kernel_size-1)/2;
    double R = 0.0, G = 0.0, B = 0.0;

    QImage image (file_name);

    int w = image.width();
    int h = image.height();

    QImage completed_image(w, h, QImage::Format_RGB32);

    QColor matrix[h][w];
    QColor extended_matrix[h+2*kernel_half_size][w+2*kernel_half_size];
    QColor point;

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

    for (int i=kernel_half_size; i<h+kernel_half_size; i++)
    {
        for (int j=kernel_half_size; j<w+kernel_half_size; j++)
        {
            for (int k = 0; k < kernel_size; k++)
            {
                for (int l = 0; l < kernel_size; l++)
                {
                    R += extended_matrix[i-kernel_half_size+k][j-kernel_half_size+l].red() * gKernel[k][l];
                    G += extended_matrix[i-kernel_half_size+k][j-kernel_half_size+l].green() * gKernel[k][l];
                    B += extended_matrix[i-kernel_half_size+k][j-kernel_half_size+l].blue() * gKernel[k][l];
                }
            }
            if (R > 255) R = 255;
            if (G > 255) G = 255;
            if (B > 255) B = 255;
            if (R < 0) R = 0;
            if (G < 0) G = 0;
            if (B < 0) B = 0;
            point.setRgb(R,G,B);
            completed_image.setPixel(j-kernel_half_size, i-kernel_half_size, point.rgb());
            R = 0;
            G = 0;
            B = 0;
        }
    }

    ui->label_2->setPixmap(QPixmap::fromImage(completed_image));
    for_save = completed_image;
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
