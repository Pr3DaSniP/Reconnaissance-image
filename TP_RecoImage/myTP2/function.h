#ifndef FUNCTION_H
#define FUNCTION_H

#include <opencv2/opencv.hpp>

using cv::Mat;
using cv::String;
using cv::Point;
using cv::Scalar;

void afficherHistog(const Mat& image, Mat& histImage, String nom)
{
	int histogram[256];

	// initialize all intensity values to 0
	for (int i = 0; i < 255; i++)
	{
		histogram[i] = 0;
	}

	// calculate the no of pixels for each intensity values
	for (int y = 0; y < image.rows; y++)
		for (int x = 0; x < image.cols; x++)
			histogram[(int)image.at<uchar>(y, x)]++;

	/*for (int i = 0; i < 256; i++)
		cout << histogram[i] << " ";*/

		// draw the histograms
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	//Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = histogram[0];
	for (int i = 1; i < 256; i++) {
		if (max < histogram[i]) {
			max = histogram[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows

	for (int i = 0; i < 255; i++) {
		histogram[i] = (int)(((1.0 * histogram[i]) / max) * histImage.rows + 0.5);
	}


	// draw the intensity line for histogram
	for (int i = 0; i < 255; i++)
	{
		line(histImage, Point(bin_w * (i), hist_h),
			Point(bin_w * (i), hist_h - histogram[i]),
			Scalar(0, 0, 0), 1, 8, 0);
	}
#if SHOW_IMAGE
	imshow("Histogramme " + nom, histImage);
#endif
}

void calculHistogramme(const Mat& img, int*& histo) {
	histo = new int[256];
	for (int i = 0; i < 255; i++)
		histo[i] = 0;

	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++)
			histo[(int)img.at<uchar>(y, x)]++;
}
void calculHistogramme(const Mat& img, double*& histo) {
	histo = new double[256];
	for (int i = 0; i < 255; i++)
		histo[i] = 0;

	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++)
			histo[(int)img.at<uchar>(y, x)]++;
}

int minimum(const Mat& img) {
	int* histo;
	calculHistogramme(img, histo);
	int min = 0;
	while (histo[min] == 0)
		min++;
	return min;
}
int maximum(const Mat& img) {
	int* histo;
	calculHistogramme(img, histo);
	int max = 254;
	while (histo[max] == 0)
		max--;
	return max;
}

Mat etirement(const Mat& image) {
	int min = minimum(image);
	int max = maximum(image);

	Mat EtImage(image);

	for (int y = 0; y < image.rows; y++)
		for (int x = 0; x < image.cols; x++)
			EtImage.at<uchar>(Point(x, y)) = (int)((255 / (double)(max - min)) * (image.at<uchar>(Point(x, y)) - min));

	return EtImage;
}

Mat egalisation(const Mat& img) {
	// Calcul Histo
	double* histo;
	calculHistogramme(img, histo);

	// Normalisation
	for (int i = 0; i < 255; i++)
		histo[i] /= (img.rows * img.cols);

	double C[256];
	C[0] = histo[0];
	for (int i = 1; i < 255; i++)
		C[i] = C[i - 1] + histo[i];

	Mat EgImage(img);

	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++)
			EgImage.at<uchar>(Point(x, y)) = C[(int)img.at<uchar>(y, x)] * 255;

	return EgImage;
}

Mat specification(const Mat& img1, const Mat& img2) {
	double* histo;
	/////////////////////////////////////////////////////
	// Calcul Histo
	calculHistogramme(img1, histo);

	// Normalisation
	for (int i = 0; i < 255; i++)
		histo[i] /= (img1.rows * img1.cols);

	double C[256];
	C[0] = histo[0];
	for (int i = 1; i < 255; i++)
		C[i] = C[i - 1] + histo[i];
	/////////////////////////////////////////////////////
	// 
	/////////////////////////////////////////////////////
	// Calcul Histo
	calculHistogramme(img2, histo);

	// Normalisation
	for (int i = 0; i < 255; i++)
		histo[i] /= (img2.rows * img2.cols);

	double D[256];
	D[0] = histo[0];
	for (int i = 1; i < 255; i++)
		D[i] = D[i - 1] + histo[i];
	/////////////////////////////////////////////////////
	// Calcul specification
	for (int i = 0; i < 255; i++) {
		int tmp = 0;
		while (C[i] > D[tmp]) {
			tmp++;
		}
		histo[i] = tmp;
	}

	Mat SpeImage(img1);
	for (int y = 0; y < img1.rows; y++)
		for (int x = 0; x < img1.cols; x++)
			SpeImage.at<uchar>(y, x) = histo[(int)img1.at<uchar>(y, x)];

	return SpeImage;
}

#endif // FUNCTION_H