#pragma once

#include "px_typebase.h"

typedef struct px_image_t
{
    uint8_t* data;
    int width;
    int height;
    int channel;
    int stride;
} px_image_t;

typedef struct px_size_t
{
    int width;
    int height;
} px_size_t;

typedef enum px_fourcc_fmt_t{
    PX_FOURCC_FMT_NV21,
    PX_FOURCC_FMT_NV12,
    PX_FOURCC_FMT_BGR,
    PX_FOURCC_FMT_RGB,
    PX_FOURCC_FMT_RGBA,
    PX_FOURCC_FMT_BGRA,
} px_fourcc_fmt_t;

typedef struct px_fourcc_t
{
    int width;
    int height;
    int pitch[4];
    uint8_t planes[4];
    px_fourcc_fmt_t fmt;
} px_fourcc_t;

#ifdef __cplusplus
extern "C" {
#endif

px_image_t* px_create_image_header(int height, int width, int channel);
px_image_t* px_create_image(int height, int width, int channel);
void px_destroy_image(px_image_t* image);
void px_destroy_image_header(px_image_t* image);

void px_rgb2bgr(px_image_t* src, px_image_t* dst);
void px_rgb2gray(px_image_t* src, px_image_t* dst);

// flipmode
// input     vertical    horizontal    both
// A  B      C  D        B  A          D  C
// C  D      A  B        D  C          B  A
typedef enum PX_FLIP_MODE
{
    PX_FLIP_VERTICAL = 0,
    PX_FLIP_HORIZONTAL = 1,
    PX_FLIP_BOTH = 2
} PX_FLIP_MODE;
void px_flip(px_image_t* src, px_image_t* dst, PX_FLIP_MODE flipmode);

void px_resize_nearest(px_image_t* src, px_image_t* dst, px_size_t dsize);
void px_resize_linear(px_image_t* src, px_image_t* dst, px_size_t dsize);
void px_resize_cubic(px_image_t* src, px_image_t* dst, px_size_t dsize);

void px_rgb_to_nv21(px_image_t* rgb, px_image_t* nv21);

#ifdef __cplusplus
}
#endif