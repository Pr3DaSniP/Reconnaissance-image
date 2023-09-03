#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector> 

#include "function.h"

using std::pair;
using std::vector;

using cv::Mat;
using cv::String;
using cv::Point;
using cv::Scalar;

const String filename = "../Images/lena.png";

const vector<pair<int, int>> Directions = {
	{-1 ,-1},	// Haut-gauche
	{-1 , 0},	// Haut
	{-1 , 1},	// Haut-droite
	{ 0 , 1},	// Droite
	{ 1 , 1},	// Bas-droite
	{ 1 , 0},	// Bas
	{ 1 ,-1},	// Bas-gauche
	{ 0 ,-1}	// Gauche
};

#define SHOW_IMAGE 1

///////////////////////////////////////////////////////////////////////////////

int s(int value) {
	return value <= 0 ? 1 : 0;
}

void LBP(Mat& image) {
	Mat lbp = image.clone();
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			int centre = image.at<uchar>(y, x);
			int value = 0;
			for (int i = 0; i < 8; i++) {
				int x2 = x + Directions[i].first;
				int y2 = y + Directions[i].second;
				if (x2 >= 0 && x2 < image.cols && y2 >= 0 && y2 < image.rows) {
					int niveauGrisVoisin = image.at<uchar>(y2, x2);
					value += s(niveauGrisVoisin - centre) * pow(2, i);
				}
			}
			lbp.at<uchar>(y, x) = value;
		}
	}
	
	// Si SHOW_IMAGE est defini, afficher l'image
	int hist_w = 512;
	int hist_h = 400;
	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
#if SHOW_IMAGE
	imshow("LBP", lbp);
#endif
	afficherHistog(lbp, histImage, "LBP");
	cv::imwrite("output/LBP/LBP_Histo.jpg", histImage);
	cv::imwrite("output/LBP/LBP.jpg", lbp);
}

///////////////////////////////////////////////////////////////////////////////

void MeanLBP(Mat& image) {
	Mat mlbp = image.clone();
	for (int y = 1; y < image.rows; y++) {
		for (int x = 1; x < image.cols; x++) {
			double m = (
				image.at<uchar>(y - 1, x - 1) +
				image.at<uchar>(y - 1, x) +
				image.at<uchar>(y, x) +
				image.at<uchar>(y, x - 1)
			) / 4;
				
			mlbp.at<uchar>(y, x) =
				s(image.at<uchar>(y - 1, x - 1) - m) * pow(2, 3) +
				s(image.at<uchar>(y - 1, x) - m) * pow(2, 2) +
				s(image.at<uchar>(y, x) - m) * pow(2, 1) +
				s(image.at<uchar>(y, x - 1) - m);
		}
	}
	int hist_w = 512;
	int hist_h = 400;
	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
#if SHOW_IMAGE
	imshow("MLBP", mlbp);
#endif
	afficherHistog(mlbp, histImage, "MLBP");
	cv::imwrite("output/MLBP/MLBP_Histo.jpg", histImage);
	cv::imwrite("output/MLBP/MLBP.jpg", mlbp);
	
	// Egalisation
	Mat EgImage = egalisation(mlbp);
	Mat histImageE(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
#if SHOW_IMAGE
	imshow("MLBP Egalisation", EgImage);
#endif
	afficherHistog(EgImage, histImageE, "egalisation");
	cv::imwrite("output/MLBP/MLBP_Egalisation_Histo.jpg", histImageE);
	cv::imwrite("output/MLBP/MLBP_Egalisation.jpg", EgImage);

	// Specification
	Mat SpeImage = specification(mlbp, image);
	Mat histImageS(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
#if SHOW_IMAGE
	imshow("MLBP Specification", SpeImage);
#endif
	afficherHistog(SpeImage, histImageS, "specification");
	cv::imwrite("output/MLBP/MLBP_Specification_Histo.jpg", histImageS);
	cv::imwrite("output/MLBP/MLBP_Specification.jpg", SpeImage);
}

///////////////////////////////////////////////////////////////////////////////

void XCS_LBP(Mat& image) {
	Mat xcs_lbp = image.clone();
	for (int y = 1; y < image.rows - 1; y++) {
		for (int x = 1; x < image.cols - 1; x++) {

			/*
			
				g5	|	g6	|	g7
				---------------------
				g4	|	gc	|	g0
				---------------------
				g3	|	g2	|	g1
			
			*/

			int gc = image.at<uchar>(y, x);
			int g0 = image.at<uchar>(y, x + 1);
			int g1 = image.at<uchar>(y + 1, x + 1);
			int g2 = image.at<uchar>(y + 1, x);
			int g3 = image.at<uchar>(y + 1, x - 1);
			int g4 = image.at<uchar>(y, x - 1);
			int g5 = image.at<uchar>(y - 1, x - 1);
			int g6 = image.at<uchar>(y - 1, x);
			int g7 = image.at<uchar>(y - 1, x + 1);

			xcs_lbp.at<uchar>(y, x) =
				s((g0 - g4) + gc + (g0 - gc) * (g4 - gc)) * pow(2, 0) +
				s((g1 - g5) + gc + (g1 - gc) * (g5 - gc)) * pow(2, 1) +
				s((g2 - g6) + gc + (g2 - gc) * (g6 - gc)) * pow(2, 2) +
				s((g3 - g7) + gc + (g3 - gc) * (g7 - gc)) * pow(2, 3);
			
		}
	}
	int hist_w = 512;
	int hist_h = 400;
	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
#if SHOW_IMAGE
	imshow("XCS_LBP", xcs_lbp);
#endif
	afficherHistog(xcs_lbp, histImage, "XCS_LBP");
	cv::imwrite("output/XCS_LBP/XCS_LBP_Histo.jpg", histImage);
	cv::imwrite("output/XCS_LBP/XCS_LBP.jpg", xcs_lbp);

	// Egalisation
	Mat EgImage = egalisation(xcs_lbp);
	Mat histImageE(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
#if SHOW_IMAGE
	imshow("XCS_LBP Egalisation", EgImage);
#endif
	afficherHistog(EgImage, histImageE, "egalisation");
	cv::imwrite("output/XCS_LBP/XCS_LBP_Egalisation_Histo.jpg", histImageE);
	cv::imwrite("output/XCS_LBP/XCS_LBP_Egalisation.jpg", EgImage);

	// Specification
	Mat SpeImage = specification(xcs_lbp, image);
	Mat histImageS(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
#if SHOW_IMAGE
	imshow("XCS_LBP Specification", SpeImage);
#endif
	afficherHistog(SpeImage, histImageS, "specification");
	cv::imwrite("output/XCS_LBP/XCS_LBP_Specification_Histo.jpg", histImageS);
	cv::imwrite("output/XCS_LBP/XCS_LBP_Specification.jpg", SpeImage);
}

int main() {
	Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	
	LBP(image);
	MeanLBP(image);
	XCS_LBP(image);
	
	cv::waitKey(0);
	return 0;
}