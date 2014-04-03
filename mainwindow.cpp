#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <qimage.h>
#include <qpainter.h>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_image(new QImage) {
    if (!m_image->load("test_image/pepper.bmp")) {
      printf("%s", "load image fail");
    }
    for (int i = 0; i < 100; ++i) {
      for (int k = 0; k < 100; ++k) {
        m_image->setPixel(k, i, 0x000000ff);
      }
    }
    QWidget::update();
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent* ) {
    QPainter painter(this);
    painter.drawImage(0, 0, *m_image);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_image;
}
