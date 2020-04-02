#pragma once
#ifndef IMAGEPROCESSING_H_
#define IMAGEPROCESSING_H_

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

void recomb(cv::Mat& src, cv::Mat& dst);

void forwardFFT(cv::Mat& Src, cv::Mat* FImg);

void forwardFFT_Mag_Phase(cv::Mat& src, cv::Mat& Mag, cv::Mat& Phase);

void inverseFFT(cv::Mat* FImg, cv::Mat& Dst);

void inverseFFT_Mag_Phase(cv::Mat& Mag, cv::Mat& Phase, cv::Mat& dst);

void highPassFilter(cv::Mat& Mag, cv::Mat& Phase);

void filterDirectional(cv::Mat& Mag, cv::Mat& Phase);

void tfOutput(cv::Mat& Mag, cv::Mat& Phase, cv::Mat& img, std::string path);

void lectureImage(cv::Mat& img, std::string path, std::string name, int& rows, int& cols);

void greyscaleImage(cv::Mat& img, std::string path);

void equlization(cv::Mat& img, std::string path);

void transformFourier(cv::Mat& img, std::string path);

void binarization(cv::Mat& img, std::string path, int first);

void filterMedian(cv::Mat& img, std::string path);

void erosion(cv::Mat& img, std::string path);

void skeleton(cv::Mat& img, std::string path);


//void filterGaussian(cv::Mat& img, std::string path);

#endif // !IMAGEPROCESSING_H_