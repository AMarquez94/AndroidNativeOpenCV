/*
 * alien.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al efecto alien. Consiste en detectar la piel y
 * 				cambiar su color a rojo, verde o azul.
 *
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
void alien(cv::Mat img, cv::Mat dst, int color);

void alien(cv::Mat img, cv::Mat dst, int color) {
	double alienCb;
	double alienCr;

	switch (color) {

	case 1: //ROJO
		alienCb = 90;
		alienCr = 240;
		break;
	case 2:	//AZUL
		alienCb = 240;
		alienCr = 110;
		break;
	case 3: //VERDE
		alienCb = 54;
		alienCr = 34;
	}

	/* Pasa a un espacio de color en el que se tiene en cuenta la luminosidad */
	Mat srcYCrCb(img.size(),CV_8UC3);
	cvtColor(img,srcYCrCb,CV_BGR2YCrCb);

	vector<Mat> canales;
	split(srcYCrCb,canales);

	/* Recorre los canales cromaticos para modificar el color detectado como piel */
	for (int i = 0; i < canales[1].rows; i++) {
		uchar* data1 = canales[1].ptr<uchar>(i);
		uchar* data2 = canales[2].ptr<uchar>(i);
		for (int j=0; j < canales[1].cols; j++) {
			if( (data1[j] >= 133 && data1[j] <= 173) &&
				(data2[j] >= 77 && data2[j] <= 127) ){
				data1[j] = alienCr;
				data2[j] = alienCb;
			}
		}
	}
	merge(canales,srcYCrCb);

	/* Vuelve al espacio de color original */
	cvtColor(srcYCrCb,dst,CV_YCrCb2BGR);
}
