#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* arg[])
{
	Mat img0 = imread("hougha.jpg");
	Mat img;
	cvtColor(img0, img, CV_RGB2GRAY);

	int w = img.cols;
	int h = img.rows;
	unsigned char* data = img.data;
	int pmax = sqrt(w*w + h*h)/2;
	const int maxAngle = 180;
	Mat hough = Mat::zeros(Size(maxAngle, pmax), CV_32SC1);
	int* hdata = (int*) hough.data;
	memset(hdata, 0, sizeof(int) * pmax * maxAngle);
	const double pi = 3.14159;

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; j++)
		{
			if (data[i * w + j] > 100)
			{
				data[i * w + j] = 255;
				for (int m = 0; m < maxAngle; m ++)
				{
					double theta = m * 2 * pi / 180;
					double a = sin(theta);
					double b = (cos(theta));
					int p = (int)((j-w/2) * b + (i-h/2) * a) / 2;
					if(p>0)
					hdata[p * maxAngle + m] = hdata[p * maxAngle + m] + 1;
				}
			}
		}
	}
	double minA, maxA;
	Point locx, locy;
	minMaxLoc(hough, &minA, &maxA, &locx, &locy);
	
	Mat B;
	hough.convertTo(B, CV_8U, 255 / (maxA -minA));
	
	namedWindow("hough", WINDOW_AUTOSIZE);
	imshow("hough", B);
	waitKey(0);

	Point lines[2];
	int maxa[2] = { 0,0 };
	Point p[2];

	int count = 0;
	for (int m = 0; m < pmax; m++)
	{
		for (int n = 0; n < maxAngle; n++)
		{
			if (hdata[m * maxAngle + n] > maxa[0])
			{
				maxa[1] = maxa[0];
				p[1].x = p[0].x;
				p[1].y = p[0].y;
				maxa[0] = hdata[m * maxAngle + n];
				p[0].x = m;
				p[0].y = n;
			}
			else if (hdata[m * maxAngle + n] > maxa[1])
			{
				maxa[1] = hdata[m * maxAngle + n];
				p[1].x = m;
				p[1].y = n;
			}
		}
	}

	for (int m = 0; m < 2; m++)
	{
		double theta = p[m].y * 2 * pi / 180;
		double pp = p[m].x;
		if (theta == 0)
		{
			int x = pp;
			line(img, Point(x, 0), Point(x, h), Scalar(255, 255,255), 2);
			continue;
		}
		for (int x = 0; x < w; x++)
		{
			int y = (pp - (x-w/2) * cos(theta)) / sin(theta) + h/2;
			circle(img, Point(x, y), 2, Scalar(255, 255,255), 2);
		}
	}
	imshow("image0", img0);
	imshow("image", img);
	waitKey(0);
}
