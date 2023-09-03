#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using std::vector;
using std::pair;

const vector<pair<int, int>> directions = {
	{0,1},
	{-1,1},
	{-1,0},
	{-1,-1},
	{0,-1},
	{1,-1},
	{1,0},
	{1,1}
};

// Merci a Jean-Charles
const vector<int> prio = { 0,2,1,4,3,6,5,7 };

/*
	__________
	__XX______
	_X__X__XX_
	_X__XXXX_X
	__X______X
	___X____X_
	____X__X__
	_____XX___
*/
vector<vector<bool>> exemple()
{
	vector<vector<bool>> image(10, vector<bool>(10, false));
	image[1][2] = true;
	image[1][3] = true;
	image[2][4] = true;
	image[3][4] = true;
	image[3][5] = true;
	image[3][6] = true;
	image[3][7] = true;
	image[2][7] = true;
	image[2][8] = true;
	image[3][9] = true;
	image[4][9] = true;
	image[5][8] = true;
	image[6][7] = true;
	image[7][6] = true;
	image[7][5] = true;
	image[6][4] = true;
	image[5][3] = true;
	image[4][2] = true;
	image[3][1] = true;
	image[2][1] = true;
	return image;
}

cv::Mat imageToMat(const vector<vector<bool>>& image)
{
	cv::Mat mat(image.size(), image[0].size(), CV_8UC1);
	for (int i = 0; i < image.size(); ++i)
	{
		for (int j = 0; j < image[i].size(); ++j)
		{
			mat.at<uchar>(i, j) = image[i][j] ? 255 : 0;
		}
	}
	mat.at<uchar>(2, 1) = 127;
	return mat;
}

bool contient(const vector<pair<int, int>>& dejaVu, pair<int, int> pixel) {
	return std::find(dejaVu.begin(), dejaVu.end(), pixel) == dejaVu.end();
}

vector<int> freemanCode(const vector<vector<bool>>& image, pair<int, int> premierPixel) {
	vector<pair<int, int>> dejaVu;
	dejaVu.push_back(premierPixel);

	int x = premierPixel.first;
	int y = premierPixel.second;
	int direction = 0;
	vector<int> freemanCode;

	while (true) {
		int xSuivant = x + directions[prio[direction]].first;
		int ySuivant = y + directions[prio[direction]].second;

		if (xSuivant == premierPixel.first && ySuivant == premierPixel.second) {
			freemanCode.push_back(prio[direction]);
			break;
		}

		if (
			xSuivant >= 0 && xSuivant < image.size() &&				// xSuivant dans l'image
			ySuivant >= 0 && ySuivant < image[xSuivant].size() &&	// ySuivant dans l'image
			image[xSuivant][ySuivant]								// pixel suivant est noir
			) {
			if (contient(dejaVu, { xSuivant, ySuivant })) {
				dejaVu.push_back({ xSuivant, ySuivant });
				freemanCode.push_back(prio[direction]);
				x = xSuivant;
				y = ySuivant;
				direction = 0;
			}
			else {
				direction = (direction + 1) % 8;
			}
		}
		else {
			direction = (direction + 1) % 8;
		}
		
		
		std::cout << "===================================" << std::endl;
		std::cout << "x = " << x << ", y = " << y << ", direction = " << direction << std::endl;
		std::cout << "===================================" << std::endl;

	}

	return freemanCode;
}

void affiche(const vector<vector<bool>>& image)
{
	for (auto ligne : image)
	{
		for (auto pixel : ligne)
		{
			std::cout << (pixel ? '*' : ' ');
		}
		std::cout << std::endl;
	}
}

void f_exemple1() {
	auto image = exemple();
	auto mat = imageToMat(image);
	auto code = freemanCode(image, std::make_pair(2, 1));

	std::cout << std::endl;
	for (auto direction : code) {
		std::cout << direction << "";
	}
	std::cout << "\n10760002076555433332 ATTENDU\n";
	
	resize(mat, mat, cv::Size(), 20, 20, cv::INTER_NEAREST);
	cv::imshow("image", mat);
	cv::waitKey();
}

int main()
{
	f_exemple1();
	
	return 0;
}