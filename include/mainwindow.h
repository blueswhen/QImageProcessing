// Copyright 2014-4 sxniu
#ifndef INCLUDE_MAINWINDOW_H_
#define INCLUDE_MAINWINDOW_H_

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class QImage;
class QSize;
class QMouseEvent;
class QPaintEvent;

enum PAINT_METHOD {
  DRAW,
  ERASE,
};

class MainWindow : public QMainWindow {
Q_OBJECT  // NOLINT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private:
  void DrawScribbles(int mouse_pos_x, int mouse_pos_y,
                     int colour, QMouseEvent* event,
                     PAINT_METHOD paint_method);
  void LoadImage(const char* filename);
  Ui::MainWindow *ui;
  QImage* m_image;
  QSize* m_toolbar_size;
  int m_image_width;
  int m_image_height;
  int m_scribble_size;
  int* m_image_data;
  PAINT_METHOD m_paint_scribble_method;

 private slots:  // NOLINT
  void SaveImage();
  void ResetImage();
  void ExecuteAlgorithm();
  void DrawImage();
  void EraseImage();

 protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
};

#endif  // INCLUDE_MAINWINDOW_H_
