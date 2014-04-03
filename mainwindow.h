#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QImage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
  void paintEvent(QPaintEvent *);

private:
    Ui::MainWindow *ui;
    QImage* m_image;
};

#endif // MAINWINDOW_H
