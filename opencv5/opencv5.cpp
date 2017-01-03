// opencv5.cpp : Defines the entry point for the console application.
//
//C:\Users\Acer1\Pictures\silver2.jpg
#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath> 
#include <math.h>
#define PI 3.14159265 
void Find_Lenght_And_Width(IplImage* image, int r, int g, int b, int &len, int &wid, int &leftX, int &topY)
{
	int p = 0;
	for( int y=0; y<image->height; y++ ) 
		{
                uchar* ptr = (uchar*) (image->imageData + y * image->widthStep);
				p = 0;
                for( int x=0; x<image->width; x++ ) 
				{
                        // 3 канала 
						if ((ptr[3*x] == b) && (ptr[3*x+1] == g) && (ptr[3*x+2] == r))//проверяем соотв. цвета кв. из 2-ой картинки к цвету на 1-ой картинке
						{
							if ((x==0) || (ptr[3*(x-1)] != b) || (ptr[3*(x-1)+1] != g) || (ptr[3*(x-1)+2] != r))
							{
								leftX = x;//левая координата X
								topY = y;//верхяя координата Y
							}
							p++;//увеличиваем промежуточную длину квадрата
						}
                }
				if (p != 0)
					wid++;//увеличиваем ширину квадрата
				if (p) 
					len = p; 
				//printf("pixelsX_square %d\n", pixelsX_square);
        }
};
void Calculation(IplImage* image, int h, int pixels, int x, int y, int leftX, int topY, double &tg, double &degree, double &s)
{
		float centerX = float(leftX) + (float)x/2;//это координата X центра квадрата
		float centerY = float(topY - y) + (float)y/2;//это координата Y центра квадрата
		printf("centerX:	%f \ncenterY:	%f\n", centerX, centerY);
        // пробегаемся по пикселям изображения
		float hh, dolya;
		dolya = float(x)/float(pixels);//отношение длины квадрата к размеру фото
		hh = 2 * float(h) / 2 / dolya;//длина перпендикуляра к центру фото
		//printf("h	%f", hh);
		double tg_x = abs(image->width/2 - centerX) * 2 * h /x; 
		double tg_y = abs(image->height/2 - centerY) * 2 * h /y; 
		tg = sqrt(tg_x*tg_x + tg_y * tg_y)/hh;
		degree = atan(tg) * 180.0 / PI;
		s = hh / cos(degree * PI / 180.0);
};
void ShowResult(IplImage* image1, IplImage* image2)
{
		// окно для отображения картинки
		cvNamedWindow("first",CV_WINDOW_AUTOSIZE);
        cvNamedWindow("second",CV_WINDOW_AUTOSIZE);
        // показываем картинку
		cvShowImage("first",image1);
        cvShowImage("second",image2);
        // ждём нажатия клавиши
        cvWaitKey(0);
        // освобождаем ресурсы
		cvReleaseImage(&image1);
        cvReleaseImage(&image2);
        // удаляем окно
        cvDestroyWindow("first");
        cvDestroyWindow("second");
}
int main(int argc, char* argv[])
{	
		IplImage* image2 = 0;
		IplImage* image1 = 0;
		int h = 0, blue = 0, green = 0, red = 0;
		printf("Input distance to square\n");
		scanf("%d", &h);
		printf("Length of square %d\n", 2*h);
		char* img1 = "2.jpg";
        char* img2 = "1.jpg";
        // получаем картинку
		image1 = cvLoadImage(img1, 1);
        image2 = cvLoadImage(img2,1);

        uchar* ptr2 = (uchar*) (image2->imageData + 0 * image2->widthStep);//работаем с второй картинкой, на которой только квадрат
        // получаем цвет квадрата, 3 цвета
		blue = ptr2[0];
		green = ptr2[1];
		red = ptr2[2];
		/*printf("blue %d\n", blue);
		printf("green %d\n", green);
		printf("red %d\n", red);*/

		int pixels = image2->width;//получили размер 2-ой картинки, у 1-ой такое же

		int pixelsX_square = 0;
		int pixelsY_square = 0;
		int top_leftX = 0, top_leftY = 0;

        Find_Lenght_And_Width(image1, red, green, blue, pixelsX_square, pixelsY_square, top_leftX, top_leftY);
		//printf("pixelsY_square %d\n", pixelsY_square);
		printf("left_top_X: %d\nleft_top_Y: %d\n", top_leftX, top_leftY-pixelsY_square);

		double tg_x_y, azimut, distance;
		Calculation(image1, h, pixels, pixelsX_square, pixelsY_square, top_leftX, top_leftY, tg_x_y, azimut, distance);
		printf("tg: %f\nazimut: %f\ndistance to square: %f",tg_x_y, azimut , distance);
         //printf("%f",dolya);
		ShowResult(image1, image2);

        return 0;
}