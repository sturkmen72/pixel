#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "px_mnist.h"
#include "cnn.h"

typedef struct Lenet
{
    int layerNum;
    ConvLayer* C1;
    PoolingLayer* S2;
    ConvLayer* C3;
    PoolingLayer* S4;
    InnerproductLayer* O5;

    float* e; // 训练误差
    float* L; // 瞬时误差能量
} Lenet;

typedef struct LenetTrainOpts
{
    int num_epochs; // 训练的迭代次数
    float alpha;   // 学习速率
} LenetTrainOpts;

#ifdef __cplusplus
extern "C" {
#endif

void setup_lenet(Lenet* net, NcSize2D inputSize, int outputSize);

void train_lenet_on_mnist(Lenet* net, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, LenetTrainOpts opts, int trainNum, FILE* fout);

float test_lenet_on_mnist(Lenet* net, px_mnist_image_array_t* inputData, px_mnist_label_array_t* outputData, int testNum);

void save_lenet(Lenet* net, const char* filename);
void load_lenet(Lenet* net, const char* filename);

void forward_lenet(Lenet* net, matrix_t* input_data); // 网络的前向传播
void backward_lenet(Lenet* net, float* output_data); // 网络的后向传播
void apply_grads_on_lenet(Lenet* net, LenetTrainOpts opts, matrix_t* input);
void clear_lenet(Lenet* net); // 将数据vyd清零

// 保存CNN网络中的相关数据
void save_lenet_data(Lenet* lenet, const char* filename, float** inputdata);

#ifdef __cplusplus
}
#endif
