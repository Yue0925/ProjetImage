#include "Evaluations.h"


std::vector<float> secondMethode(const cv::Mat& prediction, const cv::Mat& verite_terrain) {
   
    //cv::imshow("Prediction", prediction);
    std::cout << "Prediction: rows: " << prediction.size().width << ", cols: " << prediction.size().height << std::endl;
    //cv::imshow("Verite Terrain", verite_terrain);
    std::cout << "verite_terrain: rows: " << verite_terrain.size().width << ", cols: " << verite_terrain.size().height << std::endl;
    std::vector<float> matriceConfusion;
    int vp = 0;
    int fp = 0;
    int vn = 0;
    int fn = 0;
    for (int i = 0; i < prediction.rows; i++) {
        for (int j = 0; j < prediction.cols; j++) {
            /*  TEST
            if (i == 332 && j == 380) {
                std::cout << "a mediter : " << std::endl;
                std::cout << "eval : "<<int(prediction.at<uchar>(i, j)) << std::endl; //doit etre = 0
                std::cout << "verite :   "<<int(verite_terrain.at<uchar>(i, j)) << std::endl; //doit etre = 255
            }
            */
            if (prediction.at<uchar>(i, j) == 0 && verite_terrain.at<uchar>(i, j) == 255) {
                //Vrai positif
                vp++;
            }
            if (prediction.at<uchar>(i, j) == 0 && verite_terrain.at<uchar>(i, j) == 0) {
                //Faux positif
                fp++;
            }
            if (prediction.at<uchar>(i, j) == 255 && verite_terrain.at<uchar>(i, j) == 0) {
                //Vrai Negatif
                vn++;
            }
            if (prediction.at<uchar>(i, j) == 255 && verite_terrain.at<uchar>(i, j) == 255) {
                //Faux Negatif
                fn++;
            }
        }
    }
    matriceConfusion.push_back(vp);
    matriceConfusion.push_back(fp);
    matriceConfusion.push_back(vn);
    matriceConfusion.push_back(fn);


    return matriceConfusion;
}
void displayMatriceConf(std::vector<float> matriceConfusion) {
    std::cout << "MATRICE DE CONFUSION " << std::endl;
    std::cout << " Vrai Positif  =   "<<matriceConfusion[0] << std::endl;
    std::cout <<" Faux Positif  =   "<< matriceConfusion[1] << std::endl;
    std::cout <<" Vrai Negatif  =    "<< matriceConfusion[2] << std::endl;
    std::cout <<" Faux Negatif  =    "<< matriceConfusion[3] << std::endl;

    
}
std::vector<float> displayScore(std::vector<float> matriceConfusion) {
    


    int total = matriceConfusion[0] + matriceConfusion[1] + matriceConfusion[2] + matriceConfusion[3];

    float sensibilite;
    if (matriceConfusion[0] + matriceConfusion[3] == 0)
        sensibilite = 0;
    else
        sensibilite = matriceConfusion[0] / float(matriceConfusion[0] + matriceConfusion[3]);

    float specificite;
    if (matriceConfusion[0] + matriceConfusion[1] == 0)
        specificite = 0;
    else
        specificite = (matriceConfusion[0] / float(matriceConfusion[0] + matriceConfusion[1]));

    float accuracy = (float(matriceConfusion[0] + matriceConfusion[2]) / total);

    std::cout << "Sensibilite (Recall) = " << sensibilite << std::endl;
    std::cout << "Specificite (Precision) = " << specificite << std::endl;
    std::cout << "Accuracy = " << accuracy << std::endl;

    float f_score;
    if (sensibilite + specificite == 0)
        f_score = 0;
    else
        f_score = 2 * ((sensibilite * specificite) / (sensibilite + specificite));

    std::cout << "F score = " << f_score << std::endl;
    std::vector<float> res;
    res.push_back(sensibilite);
    res.push_back(specificite);
    res.push_back(f_score);
    res.push_back(accuracy);
    return res;
}
