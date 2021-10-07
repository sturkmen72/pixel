#include "vcalt.hpp"

namespace pxl {

// vcalt means vector compare absolute value, less than

/*
如果| a[i] | < | b[i] |，那么D[i]的二进制是全1的
如果| a[i] | >= | b[i] |，那么D[i]的二进制是全0的
*/

//----------------------------------------------------------------------
// vcalt_type
//----------------------------------------------------------------------
uint32x2_t vcalt_f32(float32x2_t a, float32x2_t b)
{
    uint32x2_t D;
    for (int i=0; i<2; i++)
    {
        if ( fabs(a[i]) < fabs(b[i]) ) {
            D[i] = 0xFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint64x1_t vcalt_f64(float64x1_t a, float64x1_t b)
{
    uint64x1_t D;
    for (int i=0; i<1; i++)
    {
        if ( fabs(a[i]) < fabs(b[i]) ) {
            D[i] = 0xFFFFFFFFFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

//uint16x4_t vcalt_f16(float16x4_t a, float16x4_t b);

//----------------------------------------------------------------------
// vcaltq_type
//----------------------------------------------------------------------
uint32x4_t vcaltq_f32(float32x4_t a, float32x4_t b)
{
    uint32x4_t D;
    for (int i=0; i<4; i++)
    {
        if ( fabs(a[i]) < fabs(b[i]) ) {
            D[i] = 0xFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

uint64x2_t vcaltq_f64(float64x2_t a, float64x2_t b)
{
    uint64x2_t D;
    for (int i=0; i<2; i++)
    {
        if ( fabs(a[i]) < fabs(b[i]) ) {
            D[i] = 0xFFFFFFFFFFFFFFFF;
        } else {
            D[i] = 0;
        }
    }
    return D;
}

//uint16x8_t vcaltq_f16(float16x8_t a, float16x8_t b);

//----------------------------------------------------------------------
// vcaltX_type
//----------------------------------------------------------------------
uint32_t vcalts_f32(float32_t a, float32_t b)
{
    uint32_t D;
    if ( fabs(a) >= fabs(b) ) {
        D = 0xFFFFFFFF;
    } else {
        D = 0;
    }
    return D;
}

uint64_t vcaltd_f64(float64_t a, float64_t b)
{
    uint64_t D;
    if ( fabs(a) >= fabs(b) ) {
        D = 0xFFFFFFFFFFFFFFFF;
    } else {
        D = 0;
    }
    return D;
}

//uint16_t vcalth_f16(float16_t a, float16_t b);

} // namespace pxl