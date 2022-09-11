#pragma once

#include "naive_cnn.h"

typedef struct mnist_image_array_t
{
    int size;
    NcImage* images;
    int n_cols;
    int n_rows;
} mnist_image_array_t;

typedef px_array_t mnist_label_t;

typedef struct mnist_label_array_t
{
    int size;
    int* label;
    mnist_label_t* one_hot_label;
} mnist_label_array_t;

#ifdef __cplusplus
extern "C" {
#endif

mnist_label_array_t* read_mnist_label(const char* filename);
mnist_image_array_t* read_mnist_image(const char* filename);
void extract_mnist_image_and_save(const char* mnist_data_dir);
void destroy_mnist_image_array(mnist_image_array_t* image_array);
void destroy_mnist_label_array(mnist_label_array_t* label_array);

#ifdef __cplusplus
}
#endif