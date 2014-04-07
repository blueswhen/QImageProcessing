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

#include "../ui_mainwindow.h"
#include "include/utils.h"

#define IMAGE_POS_X 0
#define IMAGE_POS_Y 33
#define SCRIBBLE_SIZE 5
#define SCRIBBLE_SIZE_INC 5
#define FILENAME "test.bmp"

#define RED 0x00ff0000
#define GREEN 0x0000ff00
#define BLUE 0x000000ff
#define BLACK 0x00000000
#define NO_ALPHA 0x00ffffff

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , m_image(new QImage)
  , m_toolbar_size(new QSize)
  , m_image_width(0)
  , m_image_height(0)
  , m_scribble_size(SCRIBBLE_SIZE)
  , m_image_data(NULL)
  , m_paint_scribble_method(DRAW) {
  ui->setupUi(this);

  QToolBar *bar = this->addToolBar("tools");
  QAction *save_action = new QAction("save", bar);
  bar->addAction(save_action);
  connect(save_action, SIGNAL(triggered()), this, SLOT(SaveImage()));

  QAction *draw_action = new QAction("draw", bar);
  bar->addAction(draw_action);
  connect(draw_action, SIGNAL(triggered()), this, SLOT(DrawImage()));

  QAction *erase_action = new QAction("erase", bar);
  bar->addAction(erase_action);
  connect(erase_action, SIGNAL(triggered()), this, SLOT(EraseImage()));

  QAction *reset_action = new QAction("reset", bar);
  bar->addAction(reset_action);
  connect(reset_action, SIGNAL(triggered()), this, SLOT(ResetImage()));

  QAction *execute_action = new QAction("execute", bar);
  bar->addAction(execute_action);
  connect(execute_action, SIGNAL(triggered()), this, SLOT(ExecuteAlgorithm()));

  LoadImage(FILENAME);
}

void MainWindow::LoadImage(const char* filename) {
  if (filename == NULL) {
    printf("error: no file name\n");
  }
  if (!m_image->load(filename)) {
    printf("load image %s fail\n", filename);
  }
  m_image_width = m_image->width();
  m_image_height = m_image->height();

  m_image_data = new int[m_image_width * m_image_height];
  for (int y = 0; y < m_image_height; ++y) {
    for (int x = 0; x < m_image_width; ++x) {
      m_image_data[y * m_image_width + x] = m_image->pixel(x, y);
    }
  }
  QWidget::update();
}

void MainWindow::SaveImage() {
  m_image->save("result.bmp");
}

void MainWindow::ResetImage() {
  LoadImage(FILENAME);
}

void MainWindow::ExecuteAlgorithm() {
  for (int y = 1; y < m_image_height - 1; ++y) {
    for (int x = 1; x < m_image_width - 1; ++x) {
      if ((m_image->pixel(x, y) & NO_ALPHA) == BLACK) {
        m_image->setPixel(x, y, RED);
      }
    }
  }
  QWidget::update();
}

void MainWindow::DrawImage() {
  m_paint_scribble_method = DRAW;
}

void MainWindow::EraseImage() {
  m_paint_scribble_method = ERASE;
}

void MainWindow::DrawScribbles(int mouse_pos_x, int mouse_pos_y,
                               int colour, QMouseEvent* event,
                               PAINT_METHOD paint_method) {
  if (mouse_pos_x > IMAGE_POS_X + m_image_width ||
      mouse_pos_x < IMAGE_POS_X ||
      mouse_pos_y > IMAGE_POS_Y + m_image_height ||
      mouse_pos_y < IMAGE_POS_Y) {
    return;
  }

  if (event->button() == Qt::MiddleButton) {
    m_scribble_size += SCRIBBLE_SIZE_INC;
  } else if (event->button() == Qt::RightButton) {
    m_scribble_size -= SCRIBBLE_SIZE_INC;
  }

  int center_x = mouse_pos_x - IMAGE_POS_X;
  int center_y = mouse_pos_y - IMAGE_POS_Y;

  for (int y = center_y - m_scribble_size; y < center_y + m_scribble_size; ++y) {  // NOLINT
    for (int x = center_x - m_scribble_size; x < center_x + m_scribble_size; ++x) {  // NOLINT
      if (x < 0 || x > m_image_width - 1 || y < 0 || y > m_image_height - 1) {
        continue;
      }
      if (paint_method == DRAW) {
        m_image->setPixel(x, y, colour);
      } else if (paint_method == ERASE) {
        m_image->setPixel(x, y, m_image_data[y * m_image_width + x]);
      }
    }
  }
  QWidget::update();
}

void MainWindow::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.drawImage(IMAGE_POS_X, IMAGE_POS_Y, *m_image);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
  DrawScribbles(event->pos().x(), event->pos().y(), BLACK,
                event, m_paint_scribble_method);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
  DrawScribbles(event->pos().x(), event->pos().y(), BLACK,
                event, m_paint_scribble_method);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
}

MainWindow::~MainWindow() {
  delete ui;
  delete m_image;
  delete m_toolbar_size;
  delete [] m_image_data;
}
