#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void applyBilateralFilter(Mat& source, Mat filteredImage, int x, int y, int diameter, double sigmaI, double sigmaS);
Mat bilateralFilterOwn(Mat &source, int diameter, double sigmaI, double sigmaS);
float distance(int x, int y, int i, int j);
double gaussian(float x, double sigma);

float distance(int x, int y, int i, int j) {
	return float(sqrt( pow(x - i, 2) + pow(y - j, 2) ));
}

Mat bilateralFilterOwn(Mat & source, int diameter, double sigmaI, double sigmaS) {
	Mat filteredImage = Mat::zeros(source.rows, source.cols, CV_64F);
	int width = source.cols;
	int height = source.rows;

	for (int i = 2; i < height - 2; i++) {
		for (int j = 2; j < width - 2; j++) {
			// ���� �ٿ������ ó��
			applyBilateralFilter(source, filteredImage, i, j, diameter, sigmaI, sigmaS);
		}
	}
	return filteredImage;
}


double gaussian(float x, double sigma) {
	return exp(-(pow(x, 2)) / (2 * pow(sigma, 2))) / (2 * CV_PI * pow(sigma, 2));
}


void applyBilateralFilter(Mat & source, Mat filteredImage, int x, int y, int diameter, double sigmaI, double sigmaS) {
	double iFiltered = 0;
	double wP = 0;
	int neighbor_x = 0;
	int neighbor_y = 0;
	int half = diameter / 2;

	for (int i = 0; i < diameter; i++) {
		for (int j = 0; j < diameter; j++) {
			neighbor_x = x - (half - i);
			neighbor_y = y - (half - j);
																											 // �̿��� �߾��� ����þ�
			double gi = gaussian(source.at<uchar>(neighbor_x, neighbor_y) - source.at<uchar>(x, y), sigmaI); // intensity�� �ñ׸�
			double gs = gaussian(distance(x, y, neighbor_x, neighbor_y), sigmaS); // distance�� �ñ׸�
			double w = gi * gs;

			// �̿� ������ gi*gs�� �ñ׸�
			iFiltered = iFiltered + source.at<uchar>(neighbor_x, neighbor_y)*w;
			
			//gi*gs�� �ñ׸� => wP�� ���ϱ� ���� ��
			wP = wP + w; // Sigma�̴�.
		}
	}

	iFiltered = iFiltered / wP; // �ᱹ�� �븻������ ������ ���� ��
	filteredImage.at<double>(x, y) = iFiltered / 255; //normalized image

}

int main() {
	Mat src;
	src = imread("face_sample.JPG", 0);
	imshow("origin", src);
	if (!src.data) {
		return -1;
	}
	Mat filteredImage = bilateralFilterOwn(
					src,
		            5,     //diameter => ����
		            12.0,  //sigmaColor
		            16.0); //sigmaSpace
	
	// double���̱� ������ imshow �� �� 255�� �븻������ �ؾ� �Ѵ�.
	imshow("result", filteredImage);
	waitKey(0);

	return 0;

}