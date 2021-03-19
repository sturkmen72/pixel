#ifndef PIXEL_MATCALC_MEAN_STDDEV_H
#define PIXEL_MATCALC_MEAN_STDDEV_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    void mean_stddev_u8_naive(unsigned char* src, size_t len, float* mean, float* stddev);

#ifdef __cplusplus
}
#endif


#endif // PIXEL_MATCALC_MEAN_STDDEV_H