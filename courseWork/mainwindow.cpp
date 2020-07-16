#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QtWidgets>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Vlasov Roman course work");
    lightColor = QColor(Qt::gray);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    ui->openGLWidget->setFixedSize(screenGeometry.size());
    ui->openGLWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->lightButton->setAutoFillBackground(true);
    QPalette palette = ui->lightButton->palette();
    palette.setColor(ui->lightButton->backgroundRole(), lightColor);
    ui->lightButton->setPalette(palette);
    ui->lightButton->setToolTip("Pick lighting color");
    this->setToolTip(QString());
    int r, g, b;
    lightColor.getRgb(&r, &g, &b);
    ui->openGLWidget->red = static_cast<float>(r)/255.0f;
    ui->openGLWidget->green = static_cast<float>(g)/255.0f;
    ui->openGLWidget->blue = static_cast<float>(b)/255.0f;
    ui->openGLWidget->repaint();
    this->QWidget::update();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(upd()));
    timer->start(1000 / frameRate);
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete  timer;
    delete ui;
}

void MainWindow::on_lightButton_clicked()
{
    QColor c = QColorDialog::getColor(lightColor, this, QString("Select color"));
    if (c.isValid()) {
        lightColor = c;
        QPalette palette = ui->lightButton->palette();
        palette.setColor(QPalette::Button, lightColor);
        ui->lightButton->setPalette(palette);
        int r, g, b;
        c.getRgb(&r, &g, &b);
        ui->openGLWidget->red = static_cast<float>(r)/255.0f;
        ui->openGLWidget->green = static_cast<float>(g)/255.0f;
        ui->openGLWidget->blue = static_cast<float>(b)/255.0f;
    }
}

void  MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        this->close();
    }
    switch (e->key()) {
        case Qt::Key_W:
            pressedW = true;
            break;
        case Qt::Key_S:
            pressedS = true;
            break;
        case Qt::Key_A:
            pressedA = true;
            break;
        case Qt::Key_D:
            pressedD = true;
            break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
        case Qt::Key_W:
            pressedW = false;
            break;
        case Qt::Key_S:
            pressedS = false;
            break;
        case Qt::Key_A:
            pressedA = false;
            break;
        case Qt::Key_D:
            pressedD = false;
            break;
    }
}

void MainWindow::upd()
{
    if (pressedW)
        ui->openGLWidget->CamHeight += 1;
    if (pressedS)
        ui->openGLWidget->CamHeight -= 1;
    if (pressedA)
        ui->openGLWidget->CamAzimuth -= 1;
    if (pressedD)
        ui->openGLWidget->CamAzimuth += 1;
    ui->openGLWidget->update();
}
