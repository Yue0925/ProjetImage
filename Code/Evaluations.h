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

std::vector<int> secondMethode(cv::Mat prediction, cv::Mat verite_terrain);

void displayMatriceConf(std::vector<int> matriceConfusion);

std::vector<float>  displayScore(std::vector<int> matriceConfusion);

#endif // !EVALUATIONS_H_
