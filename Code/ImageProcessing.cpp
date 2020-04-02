#include "ImageProcessing.h"

void recomb(cv::Mat& src, cv::Mat& dst) {
	// Recombinate quadrants
	int cx = src.cols >> 1;
	int cy = src.rows >> 1;
	cv::Mat tmp;
	tmp.create(src.size(), src.type());
	src(cv::Rect(0, 0, cx, cy)).copyTo(tmp(cv::Rect(cx, cy, cx, cy)));
	src(cv::Rect(cx, cy, cx, cy)).copyTo(tmp(cv::Rect(0, 0, cx, cy)));
	src(cv::Rect(cx, 0, cx, cy)).copyTo(tmp(cv::Rect(0, cy, cx, cy)));
	src(cv::Rect(0, cy, cx, cy)).copyTo(tmp(cv::Rect(cx, 0, cx, cy)));
	dst = tmp;
}

void forwardFFT(cv::Mat& Src, cv::Mat* FImg)
{
	int M = cv::getOptimalDFTSize(Src.rows);
	int N = cv::getOptimalDFTSize(Src.cols);
	cv::Mat padded;

	copyMakeBorder(Src, padded, 0, M - Src.rows, 0, N - Src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	// Create complex image
	// planes[0] image , planes[1] filled by zeroes
	cv::Mat planes[2] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat complexImg;

	merge(planes, 2, complexImg);

	dft(complexImg, complexImg, cv::DFT_SCALE);

	// After tranform we also get complex image 
	split(complexImg, planes);

	planes[0] = planes[0](cv::Rect(0, 0, planes[0].cols & -2, planes[0].rows & -2));
	planes[1] = planes[1](cv::Rect(0, 0, planes[1].cols & -2, planes[1].rows & -2));

	recomb(planes[0], planes[0]);
	recomb(planes[1], planes[1]);

	FImg[0] = planes[0].clone();
	FImg[1] = planes[1].clone();
}

void forwardFFT_Mag_Phase(cv::Mat& src, cv::Mat& Mag, cv::Mat& Phase) {
	// Forward FFT using Magnitude and phase
	cv::Mat planes[2];
	forwardFFT(src, planes);
	Mag.zeros(planes[0].rows, planes[0].cols, CV_32F);
	Phase.zeros(planes[0].rows, planes[0].cols, CV_32F);
	cartToPolar(planes[0], planes[1], Mag, Phase);
}

void inverseFFT(cv::Mat* FImg, cv::Mat& Dst) {
	recomb(FImg[0], FImg[0]);
	recomb(FImg[1], FImg[1]);
	cv::Mat complexImg;
	merge(FImg, 2, complexImg);
	// Inverse transform
	cv::dft(complexImg, complexImg, cv::DFT_INVERSE);
	split(complexImg, FImg);
	FImg[0].copyTo(Dst);
}

void inverseFFT_Mag_Phase(cv::Mat& Mag, cv::Mat& Phase, cv::Mat& dst) {
	// Inverse FFT using Magnitude and phase
	cv::Mat planes[2];
	planes[0].create(Mag.rows, Mag.cols, CV_32F);
	planes[1].create(Mag.rows, Mag.cols, CV_32F);
	polarToCart(Mag, Phase, planes[0], planes[1]);
	inverseFFT(planes, dst);
}

void highPassFilter(cv::Mat& Mag, cv::Mat& Phase) {
	//circls's radius
	int r = 10;
	cv::Mat mask;
	mask.create(Mag.cols, Mag.rows, CV_32F);
	int cx = Mag.cols >> 1;
	int cy = Mag.rows >> 1;
	mask = 0;

	circle(mask, cv::Point(cx, cy), r, CV_RGB(1, 1, 1), -1);
	mask = 1 - mask;

	multiply(Mag, mask, Mag);
	multiply(Phase, mask, Phase);
}

void filterDirectional(cv::Mat& Mag, cv::Mat& Phase) {
	cv::Mat mask;
	mask.create(Mag.cols, Mag.rows, CV_32F);
	mask = 1;
	/*
	for (int i = 0; i < Mag.rows; i++)
	{
		for (int j = 0; j < Mag.cols; j++)
		{
			if (i <= Mag.rows / 2) {
				if (j>=i && j<= Mag.rows-i)
					mask.at<float>(i, j) = 0;
			}
			else if (i > Mag.rows / 2) {
				if(j>= Mag.rows-i && j<= i)
					mask.at<float>(i, j) = 0;
			}

		}
	}
	*/
	cv::Point triangles[2][3];
	triangles[0][0] = cv::Point(Mag.cols / 4, 0);
	triangles[0][1] = cv::Point(Mag.cols * 3 / 4, 0);
	triangles[0][2] = cv::Point(Mag.cols / 2, Mag.cols / 2);
	triangles[1][0] = cv::Point(Mag.cols / 4, Mag.cols);
	triangles[1][1] = cv::Point(Mag.cols * 3 / 4, Mag.cols);
	triangles[1][2] = cv::Point(Mag.cols / 2, Mag.cols / 2);

	int npt[] = { 3 };
	const cv::Point* ppt[1] = { triangles[0] };
	fillPoly(mask, ppt, npt, 1, CV_RGB(0, 0, 0), 8);
	const cv::Point* ppt2[1] = { triangles[1] };
	fillPoly(mask, ppt2, npt, 1, CV_RGB(0, 0, 0), 8);

	mask = 1 - mask;

	multiply(Mag, mask, Mag);
	multiply(Phase, mask, Phase);
}

void tfOutput(cv::Mat& Mag, cv::Mat& Phase, cv::Mat& img, std::string path) {
	cv::Mat LogMag;
	LogMag.zeros(Mag.rows, Mag.cols, CV_32F);
	LogMag = (Mag + 1);

	log(LogMag, LogMag);

	//imshow("Magnitude", LogMag);
	cv::waitKey();
	cv::destroyWindow("Magnitude");
	imwrite(path + "/Magnitude.jpg", LogMag);

	//imshow("Phase", Phase);
	imwrite(path + "/Phase.jpg", Phase);
	cv::waitKey();
	cv::destroyWindow("Phase");

	// img - now in CV_32FC1 format,we need CV_8UC1 or scale it by factor 1.0/255.0  
	img.convertTo(img, CV_8UC1);
	//imshow("Filtering result", img);
	imwrite(path + "/FilteringResult.jpg", img);
	cv::waitKey();
	cv::destroyWindow("Filtering result");
}
void lectureImage(cv::Mat& img, std::string path, std::string name, int& rows, int& cols) {
	img = cv::imread(path + name, cv::IMREAD_COLOR);

	cols = img.size().width;
	rows = img.size().height;

	std::cout << "original: rows: " << rows << ", cols: " << cols << std::endl;

	// resize image of 512*512 to facilitate!
	resize(img, img, cv::Size(512, 512));

	// apply a GaussianBlur to color image to reduce the noise
	GaussianBlur(img, img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
	//imshow("color img", img);
	cv::waitKey();
	cv::destroyWindow("color img");
}

void greyscaleImage(cv::Mat& img, std::string path) {
	//convert RGB image to greyscale image
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

	//imshow("grayscale", img);
	imwrite(path + "/grayscale.jpg", img);
	cv::waitKey();
	cv::destroyWindow("grayscale");
}

void equlization(cv::Mat& img, std::string path) {
	//histogram equlization
	cv::equalizeHist(img, img);

	//imshow("equlization", img);
	imwrite(path + "/equlization.jpg", img);
	cv::waitKey();
	cv::destroyWindow("equlization");
}

void transformFourier(cv::Mat& img, std::string path) {
	cv::Mat Mag;
	cv::Mat Phase;

	//Fourier transform
	forwardFFT_Mag_Phase(img, Mag, Phase);

	highPassFilter(Mag, Phase);

	//tfOutput1(Mag, Phase, equalizeMat);

	filterDirectional(Mag, Phase);

	// Inverse transform
	inverseFFT_Mag_Phase(Mag, Phase, img);

	// Results output
	tfOutput(Mag, Phase, img, path);

}
void binarization(cv::Mat& img, std::string path, int rows, int cols) {
	//threshold(img, img, 25, 255, THRESH_BINARY_INV);

	threshold(img, img, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);

	resize(img, img, cv::Size(cols, rows));

	//imshow("binarization", img);
	imwrite(path + "/binarization.jpg", img);
	cv::waitKey();
	cv::destroyWindow("binarization");
}