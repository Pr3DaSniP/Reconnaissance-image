#include <opencv2/opencv.hpp>
#include <iostream>

using cv::Mat;
using cv::Mat1b;
using cv::String;
using cv::Point;
using cv::Scalar;

#define DEBUG 0;

#if DEBUG
#include <fstream>
#endif

const String filename = "../Images/lena.png";

std::vector<std::vector<int>> integraleImage(Mat& image) {
	std::vector<std::vector<int>> ii(image.rows, std::vector<int>(image.cols, 0));
	ii[0][0] = image.at<uchar>(0, 0);
	for (int x = 1; x < image.cols; ++x) {
		ii[0][x] = ii[0][x - 1] + image.at<uchar>(0, x);
	}
	for (int y = 1; y < image.rows; ++y) {
		ii[y][0] = ii[y - 1][0] + image.at<uchar>(y, 0);
		for (int x = 1; x < image.cols; ++x) {
			ii[y][x] = ii[y - 1][x] + ii[y][x - 1] - ii[y - 1][x - 1] + image.at<uchar>(y, x);
		}
	}
	return ii;
}

int getValue(
	std::vector<std::vector<int>>& ii,
	std::pair<int, int> A, std::pair<int, int> B,std::pair<int, int> C, std::pair<int, int> D
) {
	int valueA = ii[A.first - 1][A.second - 1];
	int valueB = ii[B.first - 1][B.second];
	int valueC = ii[C.first][C.second];
	int valueD = ii[D.first][D.second - 1];

	return valueC - valueB - valueD + valueA;
}

Mat pseudoHaar(Mat& image, std::vector<std::vector<int>>& ii) {
	Mat result = Mat::zeros(image.rows - 6, image.cols - 7, CV_8UC1);

#if DEBUG 
	std::ofstream file("debug/im.csv");
	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			file << (int)image.at<uchar>(y, x) << ";";
		}
		file << std::endl;
	}
	file.close();

	std::ofstream file2("debug/ii.csv");
	for (auto& i : ii) {
		for (auto& j : i) {
			file2 << j << ";";
		}
		file2 << std::endl;
	}
	file2.close();

	std::ofstream res("debug/res.csv");
#endif
	
	for (int y = 1; y < image.rows - 6; ++y) {
		for (int x = 1; x < image.cols - 7; ++x) {

			/*
			*		A -------- B
			*		|   NOIR   |
			*		D -------- C
			*		|   BLANC  |
			*		E -------- F
			*/
			
			std::pair<int, int> A = { y, x };
			std::pair<int, int> B = { A.first, A.second + 6 };
			std::pair<int, int> C = { A.first + 2, A.second + 6 };
			std::pair<int, int> D = { A.first + 2, A.second };
			std::pair<int, int> F = { C.first + 2, A.second + 6 };
			std::pair<int, int> E = { D.first + 2, A.second };
			
			int noir = getValue(ii, A, B, C, D);
			int blanc = getValue(ii, D, C, F, E);

			int value = blanc - noir;
			if (value > 255) {
				value = 255;
			}
			else if (value < 0) {
				value = 0;
			}
			result.at<uchar>(y, x) = value;
#if DEBUG 			
			res << value << ";";
#endif
		}
#if DEBUG
		res << std::endl;
#endif
	}
#if DEBUG
	res.close();
#endif
	return result;
}

int main() {
	Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	auto ii = integraleImage(image);
	imshow("image", image);
	imshow("haar", pseudoHaar(image, ii));
	cv::imwrite("output/haar.png", pseudoHaar(image, ii));
	cv::waitKey(0);
	return 0;
}