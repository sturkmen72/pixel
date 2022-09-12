#include "lenet.h"

static void apply_grads_on_lenet_C1_layer(Lenet* net, LenetTrainOpts opts, matrix_t* input)
{
    // C1层的权重更新
    px_size_t dSize = px_create_size(net->S2->in_width, net->S2->in_height);
    px_size_t ySize = px_create_size(net->C1->in_width, net->C1->in_height);
    px_size_t mapSize = px_create_size(net->C1->map_size, net->C1->map_size);

    for (int i = 0; i < net->C1->out_channels; i++)
    {
        for (int j = 0; j < net->C1->in_channels; j++)
        {
            matrix_t* flipinput = get_rotate180_matrix(input);
            matrix_t map;
            map.height = dSize.height;
            map.width = dSize.width;
            map.data = net->C1->d[i];
            matrix_t* C1dk = conv(&map, flipinput, NC_VALID);
            multifactor(C1dk, C1dk, -1 * opts.lr);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = net->C1->mapData[j][i];

            matrix_t mat1;
            mat1.height = mapSize.height;
            mat1.width = mapSize.width;
            mat1.data = net->C1->mapData[j][i];

            addmat(&mat1, C1dk, &res);
            destroy_matrix_ptr(C1dk);
            destroy_matrix_ptr(flipinput);
        }
        matrix_t mat2;
        mat2.height = dSize.height;
        mat2.width = dSize.width;
        mat2.data = net->C1->d[i];
        net->C1->biasData->data[i] = net->C1->biasData->data[i] - opts.lr * summat(&mat2);
    }
}

static void apply_grads_on_lenet_C3_layer(Lenet* net, LenetTrainOpts opts)
{
    px_size_t dSize;
    px_size_t ySize;
    px_size_t mapSize;

    // C3层的权重更新
    dSize.width = net->S4->in_width;
    dSize.height = net->S4->in_height;
    ySize.width = net->C3->in_width;
    ySize.height = net->C3->in_height;
    mapSize.width = net->C3->map_size;
    mapSize.height = net->C3->map_size;
    for (int i = 0; i < net->C3->out_channels; i++)
    {
        for (int j = 0; j < net->C3->in_channels; j++)
        {
            matrix_t tmp_input;
            tmp_input.height = ySize.height;
            tmp_input.width = ySize.width;
            tmp_input.data = net->S2->y[j];
            matrix_t* flipinput = get_rotate180_matrix(&tmp_input);

            matrix_t map;
            map.height = dSize.height;
            map.width = dSize.width;
            map.data = net->C3->d[i];
            matrix_t* C3dk = conv(&map, flipinput, NC_VALID);
            multifactor(C3dk, C3dk, -1.0 * opts.lr);

            matrix_t res;
            res.height = mapSize.height;
            res.width = mapSize.width;
            res.data = net->C3->mapData[j][i];

            matrix_t mat1;
            mat1.height = mapSize.height;
            mat1.width = mapSize.width;
            mat1.data = net->C3->mapData[j][i];

            addmat(&mat1, C3dk, &res);
            destroy_matrix_ptr(C3dk);
            destroy_matrix_ptr(flipinput);
        }
        matrix_t mat3;
        mat3.height = dSize.height;
        mat3.width = dSize.width;
        mat3.data = net->C3->d[i];
        net->C3->biasData->data[i] = net->C3->biasData->data[i] - opts.lr * summat(&mat3);
    }
}

static void apply_grads_on_lenet_O5_layer(Lenet* net, LenetTrainOpts opts)
{
    // 输出层
    // 首先需要将前面的多维输出展开成一维向量
    float* O5inData = (float*)malloc((net->O5->inputNum) * sizeof(float));
    px_size_t outSize = px_create_size(net->S4->in_height / net->S4->map_size, net->S4->in_width / net->S4->map_size);
    for (int i = 0; i < (net->S4->out_channels); i++)
    {
        for (int r = 0; r < outSize.height; r++)
        {
            for (int c = 0; c < outSize.width; c++)
            {
                O5inData[i * outSize.height * outSize.width + r * outSize.width + c] = net->S4->y[i][r][c];
            }
        }
    }

    for (int j = 0; j < net->O5->outputNum; j++)
    {
        for (int i = 0; i < net->O5->inputNum; i++)
        {
            net->O5->wData[j][i] = net->O5->wData[j][i] - opts.lr * net->O5->d->data[j] * O5inData[i];
        }
        net->O5->biasData->data[j] = net->O5->biasData->data[j] - opts.lr * net->O5->d->data[j];
    }
    free(O5inData);
}

void apply_grads_on_lenet(Lenet* net, LenetTrainOpts opts, matrix_t* input) // 更新权重
{
    // 这里存在权重的主要是卷积层和输出层
    // 更新这两个地方的权重就可以了
    apply_grads_on_lenet_C1_layer(net, opts, input);
    apply_grads_on_lenet_C3_layer(net, opts);
    apply_grads_on_lenet_O5_layer(net, opts);
}
