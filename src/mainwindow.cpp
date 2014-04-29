// Copyright 2014-4 sxniu
#include "include/mainwindow.h"
#include "include/inpainting.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <QImage>
#include <QPainter>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QSize>
#include <QMouseEvent>
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#include "../ui_mainwindow.h"
#include "include/utils.h"

#define IMAGE_POS_X 0
#define IMAGE_POS_Y 33
#define SCRIBBLE_SIZE 5
#define SCRIBBLE_SIZE_INC 50
#define FILENAME "louti.bmp"
#define GetRValue(rgb) ((U8)(rgb))^0x00000000
#define GetGValue(rgb) ((U8)(((U16)(rgb)) >> 8))^0x00000000
#define GetBValue(rgb) ((U8)((rgb)>>16))^0x00000000
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
  m_image->save("pix.bmp");

}

void MainWindow::ResetImage() {
  LoadImage(FILENAME);
}

void MainWindow::ExecuteAlgorithm() {
    m_image->save("tmp.bmp");

    IplImage *src = 0;   //源图像指针

    IplImage *dst = 0;   //目标图像指针
    IplImage *dst_opencv = 0;   //目标图像指针
    float scale = 0.618;  //缩放倍数为0.618倍

    CvSize dst_cvsize;   //目标图像尺寸
    src = cvLoadImage("tmp.bmp");
    dst_cvsize.width = src->width * scale;  //目标图像的宽为源图象宽的scale倍
    dst_cvsize.height = src->height * scale; //目标图像的高为源图象高的scale倍
    float xx,yy;
    int a,b;
    int rr,gg,bb;
    dst = cvCreateImage( dst_cvsize, src->depth, src->nChannels); //构造目标图象
    dst_opencv = cvCreateImage( dst_cvsize, src->depth, src->nChannels);
    double k = 0.618;
    double widthScale = (float)(1.0 / k);
    double heightScale = (float)(1.0 / k);
    for(int x=(int)k;x<dst_cvsize.width -k ; ++x) {
      for(int y = (int)k;y<dst_cvsize.height - k ; ++y) {
         xx = x * widthScale ;
         yy = y * heightScale ;
         if(xx <= 1e-8) {
           xx = 0;
         }
         if(xx > src->width - 2)
           xx=(float)(src->width - 2);
         if(yy <= 1e-8)
           yy = 0;
         if(yy > src->height - 2)
           yy = (float)(src->height - 2);
         a = (int)xx;
         b = (int)yy;
         //分别得到对应像素的R、G、B值并用双线性插值得到新像素的R、G、B值
         int g11,g12,g21,g22;
         g11 = GetGValue(m_image->pixel(a,b));
         g12 = GetGValue(m_image->pixel(a + 1,b));
         g21 = GetGValue(m_image->pixel(a,b + 1));
         g22 = GetGValue(m_image->pixel(a + 1,b + 1));
         gg = (int)(g11 * (a + 1 - xx) * (b + 1 - yy) + g12 * (a + 1 - xx) *(yy - b) +g21 * (xx - a) * (b + 1 - yy) + g22 *(xx - a) * (yy - b));
         if(g11 == 255 || g12 == 255 || g21 == 255 || g22 == 255) {
                      ((uchar*)(dst->imageData + dst->widthStep*y))[x * 3 + 2] = 0;
                      ((uchar*)(dst->imageData + dst->widthStep*y))[x * 3 + 1] = 255;
                      ((uchar*)(dst->imageData + dst->widthStep*y))[x * 3] = 0;
                      continue;
         }
         int r11,r12,r21,r22;
         m_image->pixel(a,b);
         r11 = GetRValue(m_image->pixel(a,b));
         r12 = GetRValue(m_image->pixel(a + 1,b));
         r21 = GetRValue(m_image->pixel(a,b + 1));
         r22 = GetRValue(m_image->pixel(a + 1,b + 1));
         rr = (int)(r11 * (a + 1 - xx) * (b + 1 - yy) + r12 * (a + 1 - xx) * (yy - b) + r21 * (xx - a) * (b + 1 - yy) + r22 *(xx - a) * (yy - b));
         int b11,b12,b21,b22;
         b11 = GetBValue(m_image->pixel(a,b));
         b12 = GetBValue(m_image->pixel(a + 1,b));
         b21 = GetBValue(m_image->pixel(a,b + 1));
         b22 = GetBValue(m_image->pixel(a + 1,b + 1));
         bb = (int)(b11 * (a + 1 - xx) * (b + 1 - yy) + b12 * (a + 1 - xx) * (yy - b) + b21 * (xx - a) * (b + 1 - yy) + b22 * (xx - a) * (yy - b));
         //将得到的新R、G、B值写到新图像中
         ((uchar*)(dst->imageData + dst->widthStep * y))[x * 3 + 2] = MIN(255,bb);
         ((uchar*)(dst->imageData + dst->widthStep * y))[x * 3 + 1] = MIN(255,gg);
         ((uchar*)(dst->imageData + dst->widthStep * y))[x * 3] = MIN(255,rr);
              }
          }
      cvResize(src, dst_opencv, CV_INTER_LINEAR); //缩放源图像到目标图像
      //覆盖掉openc得到的修补区域
      for(int x = 0;x < dst_cvsize.width;++x) {
         for(int y = 0;y < dst_cvsize.height;++y) {
            if(((uchar*)(dst->imageData + dst->widthStep * y))[x * 3 + 1] == 255){
              ((uchar*)(dst_opencv->imageData + dst_opencv->widthStep * y))[x * 3 + 2] = 0;
              ((uchar*)(dst_opencv->imageData + dst_opencv->widthStep * y))[x * 3 + 1] = 255;
              ((uchar*)(dst_opencv->imageData + dst_opencv->widthStep * y))[x * 3] = 0;
            }
        }
      }
   cvSaveImage("opencvsave.bmp",dst_opencv);
   inpainting test("opencvsave.bmp");
   test.Process();
   QImage* resizeImage = new QImage();
   resizeImage->load("result.bmp");
   //将修复完的结果放大回原来的大小
   IplImage *src2 = 0;
   IplImage *dst2 = 0;
   float scale2 = 1.0/0.618;
   src2 = cvLoadImage("result.bmp");
   dst_cvsize.width = src2->width * scale2;  
   dst_cvsize.height = src2->height * scale2;
   dst2 = cvCreateImage( dst_cvsize, src2->depth, src2->nChannels); 
   cvResize(src2, dst2, CV_INTER_LINEAR);
   cvSaveImage("save2.bmp",dst2);
   QImage* lastImage= new QImage();
   lastImage->load("save2.bmp");
   inpainting test2(m_image);
   //将修补部分替换掉
   test2.Copy(lastImage);
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
  DrawScribbles(event->pos().x(), event->pos().y(), GREEN,
                event, m_paint_scribble_method);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
  DrawScribbles(event->pos().x(), event->pos().y(), GREEN,
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
