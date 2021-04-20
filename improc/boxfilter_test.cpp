#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>

void blur_1d(float* src, float* dst, int src_size, int kernel_len)
{
    assert(kernel_len>0);
    assert(src!=dst);
    
    int offset = kernel_len / 2;
    int norm = kernel_len * kernel_len;
    for (int i=0; i<src_size; i++) {
        float sum = 0;
        for (int j=0; j<kernel_len; j++) {
            int ti = (i-offset) + j;
            if (ti<0) {
                ti = -ti;
            }
            else if (ti> src_size -1) {
                ti = 2* src_size - 2 - ti; // size-1 - (ti-(size-1))  =>  size - 1 - (ti - size + 1) => size - 1 - ti + size - 1 => 2*size - 2 - ti
            }
            sum += src[ti];
        }
        dst[i] = sum / norm;
    }
}

int reflect101_clip(int ti, int size) {
    if (ti<0) {
        return -ti;
    }
    else if (ti>size-1) {
        return 2*size - 2 - ti; // size-1 - (ti-(size-1))  =>  size - 1 - (ti - size + 1) => size - 1 - ti + size - 1 => 2*size - 2 - ti
    }
    else {
        return ti;
    }
}

void my_boxfilter()
{
    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    int height = size.height;
    int width = size.width;

    unsigned char* src = image.data;
    cv::Mat result = image.clone();
    unsigned char* dst = result.data;

    std::vector<int> kernel = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    };

    int denominator = 9;

    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            int blue = 0;
            int green = 0;
            int red = 0;
            for(int ki=0; ki<3; ki++) {
                for (int kj=0; kj<3; kj++) {
                    int ti = (i+ki-1);
                    int tj = (j+kj-1);
                    ti = reflect101_clip(ti, height);
                    tj = reflect101_clip(tj, width);
                    blue += src[ti*width*3+tj*3];
                    green += src[ti*width*3+tj*3+1];
                    red += src[ti*width*3+tj*3+2];
                }
            }
            blue = blue / denominator;
            green = green / denominator;
            red = red / denominator;
            if (blue>255) blue = 255;
            if (green>255) green = 255;
            if (red>255) red = 255;

            dst[i*width*3+j*3] = blue;
            dst[i*width*3+j*3+1] = green;
            dst[i*width*3+j*3+2] = red;
        }
    }

    cv::imwrite("sky_myboxfilter.png", result);
}

int main2() {

    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    printf("image: h=%d, w=%d\n", size.height, size.width);

    cv::Mat result;
    cv::Size kernel_size(3, 3);
    cv::boxFilter(image, result, 8, kernel_size);
    size = result.size();
    printf("result: h=%d, w=%d\n", size.height, size.width);

    cv::imwrite("sky_boxfilter.png", result);

    my_boxfilter();

    return 0;
}

int main3() {

    cv::Mat image = cv::imread("sky.png");
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::Mat result;
    cv::Size kernel_size(3, 3);
    cv::boxFilter(image, result, 8, kernel_size);

    cv::imwrite("sky_boxfilter_opencv4.png", result);

    return 0;
}










#include "opencv2/opencv.hpp"
#include "iostream"
#include "algorithm"
#include "vector"
#include "stdio.h"
using namespace std;
using namespace cv;

void BoxFilter(const Mat& src, Mat& dst, int ksize) {
    CV_Assert(src.channels() == 1 || src.channels() == 3); //只处理单通道或者三通道图像
    //生成一维的
    double* matrix = new double[ksize];
    double norm = 0;
    int origin = ksize / 2;
    for (int i = 0; i < ksize; i++) {
        double g = 1.0;
        norm += g;
        matrix[i] = g;
    }
    for (int i = 0; i < ksize; i++) matrix[i] /= norm;
    int border = ksize / 2;
    copyMakeBorder(src, dst, border, border, border, border, BORDER_CONSTANT);
    int channels = dst.channels();
    int rows = dst.rows - border;
    int cols = dst.cols - border;
    //水平方向
    for (int i = border; i < rows; i++) {
        for (int j = border; j < cols; j++) {
            double sum[3] = { 0 };
            for (int k = -border; k <= border; k++) {
                if (channels == 1) {
                    sum[0] += matrix[border + k] * dst.at<uchar>(i, j + k);
                }
                else if (channels == 3) {
                    Vec3b rgb = dst.at<Vec3b>(i, j + k);
                    sum[0] += matrix[border + k] * rgb[0];
                    sum[1] += matrix[border + k] * rgb[1];
                    sum[2] += matrix[border + k] * rgb[2];
                }
            }
            for (int k = 0; k < channels; k++) {
                if (sum[k] < 0) sum[k] = 0;
                else if (sum[k] > 255) sum[k] = 255;
            }
            if (channels == 1)
                dst.at<Vec3b>(i, j) = static_cast<uchar>(sum[0]);
            else if (channels == 3) {
                Vec3b rgb = { static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2]) };
                dst.at<Vec3b>(i, j) = rgb;
            }
        }
    }
    //竖直方向
    for (int i = border; i < rows; i++) {
        for (int j = border; j < cols; j++) {
            double sum[3] = { 0 };
            for (int k = -border; k <= border; k++) {
                if (channels == 1) {
                    sum[0] += matrix[border + k] * dst.at<uchar>(i + k, j);
                }
                else if (channels == 3) {
                    Vec3b rgb = dst.at<Vec3b>(i + k, j);
                    sum[0] += matrix[border + k] * rgb[0];
                    sum[1] += matrix[border + k] * rgb[1];
                    sum[2] += matrix[border + k] * rgb[2];
                }
            }
            for (int k = 0; k < channels; k++) {
                if (sum[k] < 0) sum[k] = 0;
                else if (sum[k] > 255) sum[k] = 255;
            }
            if (channels == 1)
                dst.at<Vec3b>(i, j) = static_cast<uchar>(sum[0]);
            else if (channels == 3) {
                Vec3b rgb = { static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2]) };
                dst.at<Vec3b>(i, j) = rgb;
            }
        }
    }
    delete[] matrix;
}

int main4() {
    Mat src = imread("sky.png");
    Mat dst(src.rows, src.cols, CV_8UC3);
    BoxFilter(src, dst, 3);
    imwrite("sky_boxfilter_bbuf.png", dst);



    cv::Mat image = cv::imread("sky.png");

    cv::Mat result;
    cv::Size kernel_size(3, 3);
    cv::boxFilter(image, result, 8, kernel_size);

    cv::imwrite("sky_boxfilter_opencv4.png", result);


    return 0;
}

int main()
{
    bool print_mat = false;

    bool norm = true;

#if 0
    print_mat = true;

    int height = 20;
    int width = 10;

    srand(7767517);

    unsigned char* src_shadow = (unsigned char*)malloc(sizeof(unsigned char) * height * width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int t = rand() % 30;
            //int t = 1;
            src_shadow[i*width+j] = t;
        }
    }
    cv::Size size;
    size.height = height;
    size.width = width;
    cv::Mat input(size, CV_8UC1, src_shadow);
#else
    cv::Mat input = cv::imread("sky.png");
    //cv::cvtColor(input, input, cv::COLOR_BGR2GRAY);
    cv::Size size = input.size();
    int height = size.height;
    int width = size.width;
#endif

    cv::Mat dst_opencv = input.clone();
    cv::Size kernel_size(3, 3);
    cv::Point anchor(2, 0);
    cv::boxFilter(input, dst_opencv, 8, kernel_size, anchor, norm);

    if (print_mat) {
        std::cout << "--- input is " << std::endl;
        std::cout << input << std::endl;

        std::cout << "--- dst_opencv is " << std::endl;
        std::cout << dst_opencv << std::endl;
    }


    //---------------------------------------

    //cv::Size size = input.size();
    //int height = size.height;
    //int width = size.width;

    unsigned char* src = input.data;
    cv::Mat dst_zz = input.clone();
    unsigned char* dst = dst_zz.data;

    std::vector<int> kernel = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    };

    int denominator = 1;
    if (norm) {
        denominator = 9;
    }
    int radius = kernel.size() / 2;
    int channels = input.channels();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel[3] = { 0 };
            for (int ki = 0; ki < 3; ki++) {
                for (int kj = 0; kj < 3; kj++) {
                    //int ti = (i + ki - 1);
                    //int tj = (j + kj - 1);
                    int ti = (i + ki - anchor.y);
                    int tj = (j + kj - anchor.x);
                    ti = reflect101_clip(ti, height);
                    tj = reflect101_clip(tj, width);
                    if (channels==3) {
                        pixel[0] += src[ti * width * 3 + tj * 3];
                        pixel[1] += src[ti * width * 3 + tj * 3 + 1];
                        pixel[2] += src[ti * width * 3 + tj * 3 + 2];
                    }
                    else if (channels == 1) {
                        pixel[0] += src[ti * width + tj];
                    }
                }
            }
            if (channels == 3) {
                pixel[0] = (pixel[0]+radius) / denominator;
                pixel[1] = (pixel[1]+radius) / denominator;
                pixel[2] = (pixel[2]+radius) / denominator;
                if (pixel[0] > 255) pixel[0] = 255;
                if (pixel[1] > 255) pixel[1] = 255;
                if (pixel[2] > 255) pixel[2] = 255;
                dst[i * width * 3 + j * 3] = pixel[0];
                dst[i * width * 3 + j * 3 + 1] = pixel[1];
                dst[i * width * 3 + j * 3 + 2] = pixel[2];
            }
            else if (channels == 1) {
                pixel[0] = (pixel[0] + radius) / denominator;
                if (pixel[0] > 255) pixel[0] = 255;
                dst[i * width + j] = pixel[0];
            }
        }
    }

    if (print_mat) {
        std::cout << "--- dst_zz is " << std::endl;
        std::cout << dst_zz << std::endl;
    }

    cv::Mat diff;
    cv::absdiff(dst_opencv, dst_zz, diff);
    
    cv::Scalar diff_scalar = cv::sum(diff);
    int total_diff = diff_scalar[0] + diff_scalar[1] + diff_scalar[2] + diff_scalar[3];
    std::cout << "-----------------------" << std::endl;
    std::cout << "total_diff is " << total_diff << std::endl;
    if (total_diff != 0 && print_mat) {
        std::cout << "--- dst_opencv - dst_zz " << std::endl;
        std::cout << diff << std::endl;
    }

    cv::imwrite("sky_gray_boxfilter_opencv4.png", dst_opencv);
    cv::imwrite("sky_gray_boxfilter_zz.png", dst_zz);

    return 0;
}
