#include "../../BALE/colormap/colormap.h"

typedef ColorMap CudaColorMap;

CudaColorMap CudaColorMapCreate(ColorMap* referance);

int CudaColorMapUpdate(CudaColorMap* destination, ColorMap* source);

int CudaColorMapMerge(CudaColorMap* plate, int xPos, int yPos, CudaColorMap* image);

int CudaColorMapDestroy(CudaColorMap map);