#include "./CudaBAL.h"
#define D2H cudaMemcpyKind::cudaMemcpyDeviceToHost
#define H2D cudaMemcpyKind::cudaMemcpyHostToDevice
#define ERR_CHECK(_err) if (_err != cudaError::cudaSuccess)
#define ERR_CHECK_A(_err) (_err != cudaError::cudaSuccess)

CudaColorArray CudaColorMapCreate(Color* referance)
{
    
}