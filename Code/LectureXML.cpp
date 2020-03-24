/*
#include "Evaluations.h"
#include "ImageProcessing.h"
#include "Histogramme.h"
#include "Markup.h"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <iostream>
#include "Markup.h"
#include <codecvt>
#include <locale>
using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

using namespace std;
using namespace cv;

std::string to_string(std::wstring wstr)
{
    return strconverter.to_bytes(wstr);
}

std::wstring to_wstring(std::string str)
{
    return strconverter.from_bytes(str);
}



int main(){
    
    vector<vector<Point>> lines;
    vector<vector<Point>> rectangles;
    
    //          LECTURE XML FILE
    CMarkup xml;
    xml.Load(MCD_T("./images/escalier_12/escalier_12.xml"));
    xml.ResetPos();
    xml.FindElem();
    xml.IntoElem();
    while (xml.FindElem(MCD_T("object"))) {
        xml.IntoElem();
        xml.FindElem(MCD_T("name"));
        wstring strName = xml.GetData();
        cout << to_string(strName) << endl;
        if (to_string(strName).compare("line") == 0) {
            vector<Point> line;

            xml.FindElem(MCD_T("polygon"));
            xml.IntoElem();
            while (xml.FindElem(MCD_T("pt"))) {

                xml.IntoElem();
                xml.FindElem(MCD_T("x"));
                wstring x_ws = xml.GetData();
                string x_s = to_string(x_ws);
                int x = atoi(x_s.c_str());
                //cout<<"x (pt line) = "<<x<<endl;
                xml.FindElem(MCD_T("y"));
                wstring y_ws = xml.GetData();
                string y_s = to_string(y_ws);
                int y = atoi(y_s.c_str());
                //cout << "y (pt  line)= " << y << endl;

                line.push_back(Point(x, y));
                xml.OutOfElem();
            }
            lines.push_back(line);
            xml.OutOfElem();
        }
        else if (to_string(strName).compare("rectangle") == 0) {
            vector<Point> rectangle;
            xml.FindElem(MCD_T("polygon"));
            xml.IntoElem();
            while (xml.FindElem(MCD_T("pt"))) {
                xml.IntoElem();
                xml.FindElem(MCD_T("x"));
                wstring x_ws = xml.GetData();
                string x_s = to_string(x_ws);
                int x = atoi(x_s.c_str());
                //cout << "x (pt rectangle) = " << x << endl;
                xml.FindElem(MCD_T("y"));
                wstring y_ws = xml.GetData();
                string y_s = to_string(y_ws);
                int y = atoi(y_s.c_str());
                //cout << "y (pt rectangle)= " << y << endl;

                rectangle.push_back(Point(x, y));
                xml.OutOfElem();
            }
            rectangles.push_back(rectangle);
            xml.OutOfElem();
        }
        else {
            cout << "erreur" << endl;
        }
        xml.OutOfElem();
    }
    for (int i = 0; i < rectangles.size(); i++) {
        cout << "rectangle " << i << " " << endl;
        for (int j = 0; j < rectangles[i].size(); j++) {
            cout << rectangles[i].at(j) << endl;
        }
    }
    for (int i = 0; i < lines.size(); i++) {
        cout << "line " << i << " " << endl;
        for (int j = 0; j < lines[i].size(); j++) {
            cout << lines[i].at(j) << endl;
        }
    }


   

    //      CONTOUR MARCHE
    
    Mat image = imread("./images/escalier_12/escalier_12.jpg", IMREAD_GRAYSCALE);
    
    cvtColor(image, image, COLOR_GRAY2BGR);
    
    
    Mat duplicate = image;
    for (int i = 0; i < rectangles.size(); i++) {
        //fillPoly(image, rectangles[i], Scalar(0, 0, 255));
        fillConvexPoly(image, rectangles[i], Scalar(0, 0, 255));

    }
    for (int i = 0; i < lines.size(); i++) {
        polylines(image, lines[i], true, Scalar(0, 0, 255));

    }
    string name_red = "./images/escalier_12/verite_red.jpg";
    imwrite(name_red, image);

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            cv::Vec3b l = image.at<Vec3b>(i, j);

            if (l[0] == l[1] && l[1] == l[2]) {
                l[0] = 0;
                l[1] = 0;
                l[2] = 0;
            }
            else {
                l[0] = 255;
                l[1] = 255;
                l[2] = 255;

            }
            image.at<Vec3b>(i, j) = l;
        }
    }

    imshow("essai", image);
    string name_bw = "./images/escalier_12/verite.jpg";
    imwrite(name_bw, image);
    



    

    waitKey();
    return 0;

}*/
