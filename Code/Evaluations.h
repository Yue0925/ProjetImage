#pragma once
#ifndef EVALUATIONS_H_
#define EVALUATIONS_H_

#include<opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include<iostream>
#include <vector>

std::vector<float> secondMethode(const cv::Mat& prediction, const cv::Mat& verite_terrain);

void displayMatriceConf(std::vector<float> matriceConfusion);

std::vector<float>  displayScore(std::vector<float> matriceConfusion);

#endif // !EVALUATIONS_H_