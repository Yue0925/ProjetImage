
#include "Evaluations.h"
#include "ImageProcessing.h"
#include "Histogramme.h"
#include<opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <iostream>
#include <fstream>

int main()
{

	// for escalier_1 to escalier_12
	float accSensibilite = 0;
	float accSpecificite = 0;
	float accAccuracy = 0;
	float accFscore = 0;

	for (int i = 1; i < 13; i++)
	{

		std::string path = "./images/escalier_" + std::to_string(i);
		std::string name = "/escalier_" + std::to_string(i) + ".jpg";
		cv::Mat img;

		int rows = 0;
		int cols = 0;

		lectureImage(img, path, name, rows, cols);
		std::cout << "HERE i: " << i << std::endl;
		greyscaleImage(img, path);

		equlization(img, path);

		transformFourier(img, path);

		binarization(img, path, 0);

		resize(img, img, cv::Size(cols, rows));

		//REDUCTION DU BRUIT

		
		filterMedian(img, path);
		//erosion(img, path);
		//skeleton(img, path);


		

		

		int nbmarche = nombreDeMarche2(path, img);

		std::cout << "Le nombre de marche de l'escalier est de : " << nbmarche << std::endl;

//			Phase d'evaluation du programme 

		cv::Mat evaluation;
		cv::Mat groundtruth;

		std::string eval = "/evaluation_FM.jpg";

		std::string verite =  "/verite.jpg";


		lectureImage(evaluation, path, eval);
		lectureImage(groundtruth, path, verite);


		std::vector<float> matrice = secondMethode(evaluation, groundtruth);

		displayMatriceConf(matrice);

		std::vector<float> resultats = displayScore(matrice);

		std::ofstream fichier("./images/escalier_" + std::to_string(i) + "/Resultat_Eval_FM.txt", std::ios::out | std::ios::trunc);

		if (fichier)
		{
			fichier << "Resultat :" << std::endl;
			fichier << "Le nombre de marche de l'escalier est de : " << nbmarche << std::endl;
			std::cout << std::endl;
			fichier << "MATRICE DE CONFUSION " << std::endl;
			fichier << " Vrai Positif	=	" << matrice[0] << std::endl;
			fichier << " Faux Positif	=	" << matrice[1] << std::endl;
			fichier << " Vrai Negatif	=	" << matrice[2] << std::endl;
			fichier << " Faux Negatif	=	" << matrice[3] << std::endl;
			std::cout << std::endl;
			fichier << "Sensibilite - Specificite - FSCORE " << std::endl;
			fichier << " Sensibilite	=	" << resultats[0] << "	( pourcentage " << resultats[0] * 100 << " )" << std::endl;
			fichier << " Specificite		=	" << resultats[1] << "	( pourcentage " << resultats[1] * 100 << " )" << std::endl;
			fichier << " Accuracy	=	" << resultats[3] << "	( pourcentage " << resultats[3] * 100 << " )" << std::endl;
			fichier << " F-score	=	" << resultats[2] << "	( pourcentage " << resultats[2] * 100 << " )" << std::endl;

			accSensibilite += resultats[0] * 100;
			accSpecificite += resultats[1] * 100;
			accAccuracy += resultats[3] * 100;
			accFscore += resultats[2] * 100;

			fichier.close();
		}
		else {
			std::cerr << "Erreur a l'ouverture !" << std::endl;
		}




		std::ofstream fichier_final("./images/Resultat_Final_Fct_2_FM_2.txt", std::ios::out | std::ios::trunc);

		if (fichier_final)
		{
			fichier_final << "Resultat :" << std::endl;

			fichier_final << "Sensibilite mean = " << (accSensibilite / 12) << std::endl;
			fichier_final << "Specificite mean = " << (accSpecificite / 12) << std::endl;
			fichier_final << "Accuracy mean = " << (accAccuracy / 12) << std::endl;
			fichier_final << "F-score mean = " << (accFscore / 12) << std::endl;

			fichier_final.close();
		}
		else {
			std::cerr << "Erreur a l'ouverture !" << std::endl;
		}
		

	}


	cv::waitKey();
	return 0;
}
