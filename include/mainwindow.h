// Copyright 2014-4 sxniu
#ifndef QIMAGEPROCESS_INCLUDE_MAINWINDOW_H_
#define QIMAGEPROCESS_INCLUDE_MAINWINDOW_H_

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class QImage;
class QSize;
class QMouseEvent;
class QPaintEvent;

class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    QImage* m_image;
    QSize* m_toolbar_size;
    int m_image_width;
    int m_image_height;

  protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif  // QIMAGEPROCESS_INCLUDE_MAINWINDOW_H_
