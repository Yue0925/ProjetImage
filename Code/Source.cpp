
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
	for (int i = 11; i < 13; i++)
	{
		
		
		std::string path = "./images/escalier_" + std::to_string(i);
		std::string name = "/escalier_" + std::to_string(i) + ".jpg";
		cv::Mat img;

		int rows = 0;
		int cols = 0;


		lectureImage(img, path, name,rows,cols);

		greyscaleImage(img, path);

		equlization(img, path);

		transformFourier(img, path);

		binarization(img, path,rows,cols);
    
	

		std::string pred = "./images/escalier_"+std::to_string(i)+"/binarization.jpg";

		int nbmarche = nombreDeMarche(pred);

		std::cout << "Le nombre de marche de l'escalier est de : " << nbmarche << std::endl;

		cv::Mat prediction = cv::imread(pred);

		std::string verite = "./images/escalier_" + std::to_string(i) + "/verite.jpg";

		cv::Mat ground_t = cv::imread(verite);
	

		std::vector<float> matrice = secondMethode(prediction, ground_t);

		displayMatriceConf(matrice);

		std::vector<float> resultats = displayScore(matrice);
	
		std::ofstream fichier("./images/escalier_" + std::to_string(i) + "/Resultat.txt", std::ios::out | std::ios::trunc);
	
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
			fichier << "Précision - RAPPEL - FSCORE " << std::endl;
			fichier << " Précision	=	" << resultats[0]<<"	( pourcentage "<< resultats[0]*100<<" )"<< std::endl;
			fichier << " Rappel		=	" << resultats[1]<< "	( pourcentage " << resultats[1]*100<< " )" << std::endl;
			fichier << " F-score	=	" << resultats[2]<<"	( pourcentage " << resultats[2]*100<< " )" << std::endl;
	
			fichier.close();  
		}
		else {
			std::cerr << "Erreur à l'ouverture !" << std::endl;
		}


	}
    

    cv::waitKey();
    return 0;
}

