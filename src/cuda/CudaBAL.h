#include "colormap/color.h"

typedef Color* CudaColorArray;

CudaColorArray CudaColorArrayCreate(int width, int height);
void CudaColorArrayMerge(CudaColorArray bottom, CudaColorArray top);
void CudaColorArrayMergeA(CudaColorArray plate, CudaColorArray bottom, CudaColorArray top);
void CudaColorArrayDestroy(CudaColorArray arr);