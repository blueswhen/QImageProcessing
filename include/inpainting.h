#ifndef inpaint_inpainting_H_
#define inpaint_inpainting_H_

#include <QImage>
#include <fstream>
#include <iostream>
#define PAINT_COLOR  RGB(0,255,0)
#define SOURCE 0
#define TARGET -1
#define BOUNDARY -2
#define myWinSize 2
#define save_path "result"
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;
typedef unsigned int COLORREF;
typedef struct {
  double grad_x;
  double grad_y;
}gradient;

typedef struct {
  long map_x;
  long map_y;
}mapping_point;

typedef struct {
  double norm_x;
  double norm_y;
}norm;

class inpainting {
 public:
    QImage *Image;
    bool Process();
    inpainting(QImage *Image);
    inpainting(char *name);
    bool Copy(QImage *resizeImage);
 private :
  //ofstream myfile;
  double pix_to_inpaint;
  double pix_been_inpainted;
  int boundary_top;
  int boundary_bottom;
  int boundary_left;
  int boundary_right;
  int max_height;
  int max_width;
  int m_width;
  int m_height;
  int *number_to_fill_x;
  int *number_to_fill_y;
  bool bOpen;
  int height_diff;
  int width_diff;
  U8 m_pDIBBits;
int count_pic;
  U32 m_dwBytesPerLine;
  double * m_r;
  double * m_g;
  double * m_b;
  int * m_mark;
  double * m_confid;
  double * m_pri;
  double * m_gray;
  bool * m_source;
  int m_top, m_bottom, m_left, m_right;
  int *m_color;
  void DrawBoundary();
  double ComputeConfidence(int i, int j);
  double ComputePriority(int x, int y);
  double ComputeData(int i, int j);
  void Convert2Gray(void);
  gradient GetGradient(int i, int j);
  norm GetNorm(int i, int j);
  bool DrawSource(void);
  bool PatchTexture(int x, int y,int &patch_x,int &patch_y,int** &most_similar_patch,int** &most_similar_patch_2);
  bool FillTarget(int target_x, int target_y, int source_x, int source_y, double confid);
  bool TargetExist(void);
  void UpdateBoundary(int i, int j);
  void UpdatePriority(int i, int j);
  int *CountCR();

  
};
#endif

