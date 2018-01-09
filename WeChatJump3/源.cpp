#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "cv.h"            
#include "cvaux.h"
#include "cxcore.h"

#include <windows.h>
#include <math.h>
#include <time.h>


using namespace std;
using namespace cv;


CvPoint destination(0,0);


void myMouseCallback(int event, int x, int y, int flags, void* param)
{
	IplImage* img = (IplImage*)param;
	IplImage* img1 = cvCloneImage(img);
	CvFont font;
	char text[20];
	char pointaxis[20];
	uchar* ptr;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 3, 8);

	if (event == CV_EVENT_MOUSEMOVE)
	{
		ptr = cvPtr2D(img1, y, x, NULL);
		sprintf(text, "(%d,%d,%d)", ptr[2], ptr[1], ptr[0]);
		sprintf(pointaxis, "(%d,%d)", x, y);

		if (img->width - x <= 180 || img->height - y <= 20) {
			cvPutText(img1, pointaxis, cvPoint(x - 180, y - 50), &font, CV_RGB(255, 255, 255));
			cvPutText(img1, text, cvPoint(x - 180, y - 30), &font, CV_RGB(255, 255, 255));
		}
		else {
			cvPutText(img1, pointaxis, cvPoint(x, y), &font, CV_RGB(0, 0, 0));
			cvPutText(img1, text, cvPoint(x, y + 40), &font, CV_RGB(0, 0, 0));
			cvShowImage("auto", img1);
		}
	}
}

int main()
{
	std::system("adb shell input swipe 200 100 200 100 718");
	Sleep(3000);
	while (1) {
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[24];
		time(&rawtime);
		//printf("%ld\n", rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", timeinfo);
		buffer[19] = '.'; buffer[20] = 'p'; buffer[21] = 'n'; buffer[22] = 'g'; buffer[23] = '\0';
		//printf("%s\n", buffer);
		char screencap[60] = "adb shell screencap -p /sdcard/jump/";
		for (int i = 36; i < 60; i++) {
			screencap[i] = buffer[i - 36];
		}
		//cout << screencap;
		Sleep(2000);
		std::system(screencap);
		//Sleep(2000);
		char pullcmd[100] = "adb pull /sdcard/jump/";
		char location[14] = " E:\\wzh\\Jump\\";
		strcat_s(pullcmd, buffer);
		strcat_s(pullcmd, location);
		//cout << pullcmd;
		std::system(pullcmd);

		char filename[50] = "E:\\wzh\\Jump\\";
		strcat_s(filename, buffer);
		IplImage* img = cvLoadImage(filename);
		//cvNamedWindow("auto", 0);
		//cvResizeWindow("auto", 450, 800);
		cvSetMouseCallback("auto", myMouseCallback, (void*)img);
		//cvShowImage("auto", img);

		IplImage* gray = cvCloneImage(img);

		for (int i = 0; i < img->height; i++)
		{
			for (int j = 0; j < img->width; j++)
			{
				CvScalar s = cvGet2D(gray, i, j);
				if (s.val[0] > 65 && s.val[0] < 110 && s.val[1]>40 && s.val[1] < 60 && s.val[2]>40 && s.val[2] < 100) {
					s.val[0] = s.val[1] = s.val[2] = 255;
					cvSet2D(gray, i, j, s);
				}
				else {
					s.val[0] = s.val[1] = s.val[2] = 0;
					cvSet2D(gray, i, j, s);
				}
			}
		}

		CvPoint start(0, 0), end(0, 0), finalstart(0, 0);
		CvPoint desstart(0, 0), desend(0, 0), finaldes(0, 0);
		int max = 0, desmax = 0;
		for (int i = 0; i < img->height; i++)
		{
			for (int j = 0; j < img->width; j++)
			{
				CvScalar s = cvGet2D(gray, i, j);
				if (s.val[0] == 255 && s.val[1] == 255 && s.val[2] == 255) {
					end.y = i;
					end.x = j;
				}
			}
			for (int k = img->width - 1; k >= 0; k--) {
				CvScalar s = cvGet2D(gray, i, k);
				if (s.val[0] == 255 && s.val[1] == 255 && s.val[2] == 255) {
					start.y = i;
					start.x = k;
				}
			}

			if (end.x - start.x > max) {
				max = end.x - start.x;
				finalstart.x = start.x;
				finalstart.y = start.y;
			}


			start.x = start.y = 0;
			end.x = end.y = 0;
		}

		IplImage *binaryImg = cvCloneImage(img);
		for (int i = 0; i < img->height; i++)
		{
			for (int j = 0; j < img->width; j++)
			{
				CvScalar s = cvGet2D(img, i, j);
				if (s.val[0] == 245 && s.val[1] == 245 && s.val[2] == 245) {
					cvSet2D(binaryImg, i, j, CvScalar(255, 255, 255));
				}
				else cvSet2D(binaryImg, i, j, CvScalar(0, 0, 0));
			}
		}

		IplImage* erodeimg = cvCreateImage(CvSize(img->width, img->height), 8, 1);
		cvCvtColor(binaryImg, erodeimg, CV_BGR2GRAY);
		//IplConvKernel* kernel = cvCreateStructuringElementEx(3,3,1,1, CV_SHAPE_RECT);
		cvErode(erodeimg, erodeimg);

		for (int i = 0; i < erodeimg->height; i++)
		{
			for (int j = 0; j < erodeimg->width; j++)
			{
				CvScalar s = cvGet2D(erodeimg, i, j);
				if (s.val[0]==255) {
					desend.x = j;
					desend.y = i;
				}
			}

			for (int k = img->width - 1; k >= 0; k--) {
				CvScalar s = cvGet2D(erodeimg, i, k);
				if (s.val[0] == 255) {
					desstart.x = k;
					desstart.y = i;
				}
			}

			if (desend.x - desstart.x > desmax) {
				desmax = desend.x - desstart.x;
				finaldes.x = desstart.x;
				finaldes.y = desstart.y;
			}

			desstart.x = desstart.y = 0;
			desend.x = desend.y = 0;
		}

		CvPoint center(finalstart.x + max / 2, finalstart.y);
		CvPoint destination(finaldes.x + desmax / 2, finaldes.y);
		std::cout << "棋子中心点：" << center.x << "\t" << center.y << endl;
		std::cout << "目的中心点:" << destination.x << "\t" << destination.y << endl;

		if (destination.x != 0 && destination.y != 0) {
			int distance = sqrt(pow(destination.x - center.x, 2) + pow(destination.y - center.y, 2));
			std::cout << "距离:" << distance << endl;
			int time = distance*1.375;
			std::cout << "time:" << time << endl;
			char t[4];
			sprintf(t, "%d%d%d%d", time / 1000, (time % 1000) / 100, (time % 100) / 10, (time % 10));
			//cout << t << endl;
			//cvNamedWindow("gray", 0);
			//cvResizeWindow("gray", 450, 800);
			//cvShowImage("gray", gray);

			char pre[39] = "adb shell input swipe 200 100 200 100 ";
			char cmd[43];
			for (int i = 0; i < 39; i++) {
				cmd[i] = pre[i];
			}
			cmd[38] = t[0];
			cmd[39] = t[1];
			cmd[40] = t[2];
			cmd[41] = t[3];
			cmd[42] = '\0';
			std::system(cmd);
		}
		//cvReleaseStructuringElement(&kernel);
		waitKey(1000);
	}
	cvDestroyAllWindows();
	return 0;
}


