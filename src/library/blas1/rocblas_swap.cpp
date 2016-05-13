/* ************************************************************************
 * swapright 2016 Advanced Micro Devices, Inc.
 *
 * ************************************************************************ */

#include <hip_runtime.h>
#include "rocblas.h"


#define NB_X 256

template<class T>
__global__ void
swap_kernel(hipLaunchParm lp,
    rocblas_int n,
    T *x, rocblas_int incx,
    T* y,  rocblas_int incy)
{
    int tx  = hipThreadIdx_x;
    int tid = hipBlockIdx_x * hipBlockDim_x + hipThreadIdx_x;

    T tmp;
    //bound
    if ( tid < n ) {
        tmp = y[tid*incy];
        y[tid*incy] =  x[tid * incx];
        x[tid*incx] =  tmp;
    }
}


/*! \brief BLAS Level 1 API

    \details
    swap  interchange vector x[i] and y[i], for  i = 1 , … , n

        y := x; x := y

    @param[in]
    handle    rocblas_handle.
              handle to the rocblas library context queue.
    @param[in]
    n         rocblas_int.
    @param[inout]
    x         pointer storing vector x on the GPU.
    @param[in]
    incx      specifies the increment for the elements of x.
    @param[inout]
    y         pointer storing vector y on the GPU.
    @param[in]
    incy      rocblas_int
              specifies the increment for the elements of y.

    ********************************************************************/

template<class T>
rocblas_status
rocblas_swap_template(rocblas_handle handle,
    rocblas_int n,
    T *x, rocblas_int incx,
    T* y, rocblas_int incy)
{

    if ( n < 0 )
        return rocblas_invalid_dim;
    else if ( x == NULL )
        return rocblas_invalid_vecX;
    else if ( incx < 0 )
        return rocblas_invalid_incx;
    else if ( y == NULL )
        return rocblas_invalid_vecY;
    else if ( incy < 0 )
        return rocblas_invalid_incy;

    /*
     * Quick return if possible.
     */

    if ( n == 0)
        return rocblas_success;

    int blocks = (n-1)/ NB_X + 1;

    dim3 grid( blocks, 1, 1 );
    dim3 threads( NB_X, 1, 1 );

    //hipLaunchKernel(HIP_KERNEL_NAME(swap_kernel), dim3(grid), dim3(threads), 0, 0 , n, x, incx, y, incy);

    return rocblas_success;
}

/* ============================================================================================ */

    /*
     * ===========================================================================
     *    template interface
     *    template specialization
     * ===========================================================================
     */


template<>
rocblas_status
rocblas_swap<float>(rocblas_handle handle,
    rocblas_int n,
    float *x, rocblas_int incx,
    float* y, rocblas_int incy){

    return rocblas_swap_template<float>(handle, n, x, incx, y, incy);
}

template<>
rocblas_status
rocblas_swap<double>(rocblas_handle handle,
    rocblas_int n,
    double *x, rocblas_int incx,
    double* y, rocblas_int incy){

    return rocblas_swap_template<double>(handle, n, x, incx, y, incy);
}

template<>
rocblas_status
rocblas_swap<rocblas_float_complex>(rocblas_handle handle,
    rocblas_int n,
    rocblas_float_complex *x, rocblas_int incx,
    rocblas_float_complex* y, rocblas_int incy){

    return rocblas_swap_template<rocblas_float_complex>(handle, n, x, incx, y, incy);
}

template<>
rocblas_status
rocblas_swap<rocblas_double_complex>(rocblas_handle handle,
    rocblas_int n,
    rocblas_double_complex *x, rocblas_int incx,
    rocblas_double_complex* y, rocblas_int incy){

    return rocblas_swap_template<rocblas_double_complex>(handle, n, x, incx, y, incy);
}

/* ============================================================================================ */

    /*
     * ===========================================================================
     *    C wrapper
     * ===========================================================================
     */


extern "C"
rocblas_status
rocblas_sswap_kernel(rocblas_handle handle,
    rocblas_int n,
    float *x, rocblas_int incx,
    float* y, rocblas_int incy){

    return rocblas_swap<float>(handle, n, x, incx, y, incy);
}


extern "C"
rocblas_status
rocblas_dswap_kernel(rocblas_handle handle,
    rocblas_int n,
    double *x, rocblas_int incx,
    double* y, rocblas_int incy){

    return rocblas_swap<double>(handle, n, x, incx, y, incy);
}


extern "C"
rocblas_status
rocblas_cswap(rocblas_handle handle,
    rocblas_int n,
    rocblas_float_complex *x, rocblas_int incx,
    rocblas_float_complex* y, rocblas_int incy){

    return rocblas_swap<rocblas_float_complex>(handle, n, x, incx, y, incy);
}

extern "C"
rocblas_status
rocblas_zswap(rocblas_handle handle,
    rocblas_int n,
    rocblas_double_complex *x, rocblas_int incx,
    rocblas_double_complex* y, rocblas_int incy){

    return rocblas_swap<rocblas_double_complex>(handle, n, x, incx, y, incy);
}

/* ============================================================================================ */