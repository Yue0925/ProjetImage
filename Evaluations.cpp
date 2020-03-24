#include "Evaluations.h"


std::vector<float> secondMethode(cv::Mat prediction, cv::Mat verite_terrain) {

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
    std::cout << " Vrai Positif  =   " << matriceConfusion[0] << std::endl;
    std::cout << " Faux Positif  =   " << matriceConfusion[1] << std::endl;
    std::cout << " Vrai Negatif  =    " << matriceConfusion[2] << std::endl;
    std::cout << " Faux Negatif  =    " << matriceConfusion[3] << std::endl;


}
std::vector<float> displayScore(std::vector<float> matriceConfusion) {

    float prec = float(matriceConfusion[0] / (matriceConfusion[0] + matriceConfusion[3]));
    float rap = float(matriceConfusion[0] / (matriceConfusion[0] + matriceConfusion[1]));
    std::cout << "Precision = " << prec << std::endl;
    std::cout << "Rappel = " << rap << std::endl;
    float f_score = float(2 * ((prec * rap) / (prec + rap)));
    std::cout << "F score = " << f_score << std::endl;
    std::vector<float> res;
    res.push_back(prec);
    res.push_back(rap);
    res.push_back(f_score);
    return res;
}