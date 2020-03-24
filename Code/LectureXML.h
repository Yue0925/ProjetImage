#pragma once
#ifndef LECTUREXML_H_
#define LECTUREXML_H_

#include<windows.h>

#include<opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include<iostream>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include "Markup.h"

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

void lectureXML(std::string path);

#endif // !LECTUREXML_H_