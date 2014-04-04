// Copyright 2014-4 sxniu
#include "include/mainwindow.h"

#include <stdio.h>
#include <QImage>
#include <QPainter>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QSize>
#include <QMouseEvent>

#include "ui_mainwindow.h"
#include "include/utils.h"

#define IMAGE_POS_X 0
#define IMAGE_POS_Y 0
#define BLACK 0X00000000

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_image(new QImage) 
  , m_toolbar_size(new QSize)
  , m_image_width(0)
  , m_image_height(0) {
  ui->setupUi(this);

  // QToolBar *bar = this->addToolBar("Tools");
  // QAction *chooseAction = new QAction("Choose",bar);
  // bar->addAction(chooseAction);
  // *m_toolbar_size = bar->iconSize();
  // QActionGroup *group = new QActionGroup(bar);
  // group->addAction(chooseAction);

  if (!m_image->load("test_image/pepper.bmp")) {
    printf("%s", "load image fail\n");
  }
  m_image_width = m_image->width();
  m_image_height = m_image->height();
  QWidget::update();
}

void MainWindow::paintEvent(QPaintEvent* ) {
  QPainter painter(this);
  painter.drawImage(IMAGE_POS_X, IMAGE_POS_Y, *m_image);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
  if(event->button() == Qt::LeftButton) {
    int mouse_x = event->pos().x();
    int mouse_y = event->pos().y();
    if (mouse_x > IMAGE_POS_X + m_image_width ||
        mouse_y > IMAGE_POS_Y + m_image_height) {
      return;
    }

    int mouse_pixel_x = mouse_x - IMAGE_POS_X;
    int mouse_pixel_y = mouse_y - IMAGE_POS_Y;
    int arround_mouse_pixels[8];
    if (!utils::SetArroundPixelsPosition(
          arround_mouse_pixels, mouse_pixel_x, 
          mouse_pixel_y, 8,
          m_image_width, m_image_height)) {
      return;
    }
    for (int i = 0; i < 9; ++i) {
      if (i == 8) {
        m_image->setPixel(mouse_pixel_x, mouse_pixel_y, BLACK);
        break;
      }
      int x = arround_mouse_pixels[i] % m_image_width;
      int y = arround_mouse_pixels[i] / m_image_width;
      m_image->setPixel(x, y, BLACK);
    }
  }
  QWidget::update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
  int mouse_x = event->pos().x();
  int mouse_y = event->pos().y();
  if (mouse_x > IMAGE_POS_X + m_image_width ||
      mouse_y > IMAGE_POS_Y + m_image_height) {
    return;
  }

  int mouse_pixel_x = mouse_x - IMAGE_POS_X;
  int mouse_pixel_y = mouse_y - IMAGE_POS_Y;
  int arround_mouse_pixels[8];
  if (!utils::SetArroundPixelsPosition(
        arround_mouse_pixels, mouse_pixel_x, 
        mouse_pixel_y, 8,
        m_image_width, m_image_height)) {
    return;
  }
  for (int i = 0; i < 9; ++i) {
    if (i == 8) {
      m_image->setPixel(mouse_pixel_x, mouse_pixel_y, BLACK);
      break;
    }
    int x = arround_mouse_pixels[i] % m_image_width;
    int y = arround_mouse_pixels[i] / m_image_width;
    m_image->setPixel(x, y, BLACK);
  }
  QWidget::update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
}

MainWindow::~MainWindow() {
  delete ui;
  delete m_image;
  delete m_toolbar_size;
}
