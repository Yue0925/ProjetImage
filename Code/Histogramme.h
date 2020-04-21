#pragma once
#ifndef HISTOGRAMME_H_
#define HISTOGRAMME_H_
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include <algorithm>



int nombreDeMarche1(std::string path, const cv::Mat& img);
int nombreDeMarche2(std::string path, const cv::Mat& img);
int min(std::vector<int> vect);
int max(std::vector<int> vect);


#endif // !HISTOGRAMME_H_