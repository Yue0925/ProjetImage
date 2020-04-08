#include "Histogramme.h"
using namespace cv;
using namespace std;



float mostFrequent(float arr[], int n) {
    // Sort the array 
    std::sort(arr, arr + n);

    // find the max frequency using linear traversal 
    int max_count = 1, curr_count = 1;
    float  res = arr[0];
    int med = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] == arr[i - 1])
            curr_count++;
        else {
            if (curr_count > max_count) {
                max_count = curr_count;
                res = arr[i - 1];
            }
            curr_count = 1;
        }
        //test: maybe mediane est mieux

        if (i == n / 2) {
            med = arr[i];
            //std::cout << "mediane est " << med << std::endl;
        }
    }

    // If last element is most frequent 
    if (curr_count > max_count) {
        max_count = curr_count;
        res = arr[n - 1];
    }
    //comparaison
    /*
    int eps = 1 / n; // à changer en fct ds resultats
    if ((med - res) < 0) {
        if ((res - med) < eps)   res = med;
    }
    */
    return res;
}

int min(vector<int> vect) {
    int min;
    if (vect.size() == 1) { min = vect[0]; }
    else {
        min = vect[0];
        for (int i = 1; i < vect.size(); i++) {
            if (min > vect[i]) min = vect[i];
        }
    }
    //cout << "min est"<<min << std::endl;
    return min;
}
int nombreDeMarche1 (std::string pathimage, std::string image){
    Mat img = imread(pathimage + image);
    //namedWindow("Prediction", cv::WINDOW_NORMAL);
    //cv::imshow("Prediction", img);
    const int size_i = img.rows;

    float histogramme[2048];
    /**********************************Creation de l'histogramme*******************************/
    for (int i = 0; i < size_i; i++) {
        float acc = 0;
        for (int j = 0; j < img.cols; j++) {
            uchar pix = img.at<uchar>(i, j);
            if (pix == 255) acc += 1;
        }
        histogramme[i] = acc;
    }

    //Normalisation de l'histogramme
    float histo_normalized[2048];
    float tmp = 0;
    for (int i = 0; i < size_i; i++) {
        tmp = histogramme[i] / size_i;
        histo_normalized[i] = tmp;
    }

    /***************************************Definir le seuil***********************************/
    int nbligne = 0;
    int prev = 0;
    float seuil = 0;
    //copy the vector 
    float input[2048];
    std::copy(histo_normalized + 1, histo_normalized + size_i, input);
    seuil = mostFrequent(input, size_i);
    //std::cout << seuil << std::endl;
    /*****************************Definir nombre de ligne***********************************/
    for (int i = size_i - 1; i >= 0; i--) {
        //std::cout << "  " << histo_normalized[i] << std::endl;
        if ((histo_normalized[i] > seuil) && (prev == 0)) {
            nbligne++;
            prev = 1;
        }
        if ((histo_normalized[i] == seuil) && (prev == 0)) {
            nbligne++;
            prev = 1;
        }
        if (histo_normalized[i] < seuil) {
            prev = 0;
        }
    }

    std::cout << "Nombre de lignes est :" << nbligne << std::endl;

    /***************************Nombre de marche**************************************/
    int nbmarche = 0;
    if (nbmarche % 2 == 0) {
        nbmarche = nbligne / 2;
    }
    else {
        nbmarche = (nbligne - 1) / 2;
    }
    return nbmarche;
}
int nombreDeMarche2(std::string pathimage, std::string image) {
    Mat img = imread(pathimage + image);
    //namedWindow("image1", WINDOW_NORMAL);
    //imshow("image1", img);
    const int size_i = img.rows;
    

    float histogramme[4096];
    /**********************************Creation de l'histogramme*******************************/
    for (int i = 0; i < size_i; i++) {
        float acc = 0;
        for (int j = 0; j < img.cols; j++) {
            uchar pix = img.at<uchar>(i, j);
            if (pix == 255) acc += 1;
        }
        histogramme[i] = acc;
    }

    //Normalisation de l'histogramme
    float histo_normalized[4096];
    float tmp = 0;
    for (int i = 0; i < size_i; i++) {
        tmp = histogramme[i] / size_i;
        histo_normalized[i] = tmp;
        
    }
    
    /***************************************Definir le seuil***********************************/

    int prev = 0;
    float seuil = 0;
    //copy the vector 
    float input[2048];
    copy(histo_normalized + 1, histo_normalized + size_i, input);
    seuil = mostFrequent(input, size_i);

    /*****************************Definir nombre de ligne***********************************/
    int ecartmin = 0;
    int epline = 0;
    //vecteur des epaisseur des lignes pour l'écart minimal 
    vector <int>epaisseline;
    Mat bin(img.rows, img.cols, CV_8UC1);
    vector <int>lines;
    for (int i = size_i - 1; i >= 0; i--) {
        
        if (histo_normalized[i] >= seuil) {

            if (epline != 0) {
                epaisseline.push_back(epline);
            }
            prev = 0;
            for (int j = 0; j < img.cols; j++) {
                bin.at<char>(i, j) = 255;
            }
            epline = 0;
        }

        if ((histo_normalized[i] < seuil) && (prev == 0)) {
            prev = 1;
            //cout << "nouvelle ligne" << endl;
            lines.push_back(i);
            epline++;
            for (int j = 0; j < img.cols; j++) {
                bin.at<char>(i, j) = 0;
            }
        }

        if ((histo_normalized[i] < seuil) && (prev == 1)) {
            epline++;
            for (int j = 0; j < img.cols; j++) {
                bin.at<char>(i, j) = 0;
            }
        }
    }
    for (int i = 0; i < lines.size(); i++) {
        //cout << "lignes est :" << lines[i] << endl;
    }

    /***************ecart max/ecart min : retirer les lignes aberantes************************************/
    // soit trop écartée ou trop proches
    int ecartmax = 0;
    int nbmarche = 0;
    //si le nombre de lignes récupéré est de deux on ne peut pas calculer d'écart max  
    if (lines.size()<=2) {
        Mat Eval(img.rows, img.cols, CV_8UC1);
        for (int i = size_i - 1; i >= 0; i--) {
            for (int j = 0; j < img.cols; j++) {
                Eval.at<char>(i, j) = 0;
            }
        }
        namedWindow("image", WINDOW_NORMAL);
        //imshow("image", Eval);
        imwrite(pathimage + "/Projection_Median.jpg", Eval);
    }
    else {
        //ecart min est le minimum des epaisseur des lignes * une constante
        ecartmin = min(epaisseline);

        //ecart max est le maximum des ecart entre les trois premiere ligne

        if ((lines[0] - lines[1]) <= (lines[1] - lines[2])) ecartmax = lines[1] - lines[2];
        else ecartmax = lines[0] - lines[1];
       

        int taille_lines = lines.size();
        vector<int> kept_lines;
        kept_lines.push_back(lines[0]);
        for (int i = 0; i < lines.size() - 1; i++) {
            //cout << lines[i] << endl;
            if (lines[i] - lines[i + 1] > ecartmax) {
                //cout << " trop grand" << endl;
            }
            if (lines[i] - lines[i + 1] < 2 * ecartmin) {
                //cout << "trop petit" << endl;
            }
            else {
                vector<int>::iterator it = find(kept_lines.begin(), kept_lines.end(), lines[i]);
                if (it != kept_lines.end()) {
                    //cout << lines[i + 1] << endl;
                    kept_lines.push_back(lines[i + 1]);
                }
            }

        }


        

        /******************************Nombre de marche*****************************************/

        int nbligne = kept_lines.size();
        if (nbmarche % 2 == 0) {
            nbmarche = nbligne / 2;
        }
        else {
            nbmarche = (nbligne - 1) / 2;
        }

        /****************************The OutPut image for the evaluation************************/

        Mat Eval(img.rows, img.cols, CV_8UC1);
        int bef = 0;

        for (int i = size_i - 1; i >= 0; i--) {

            //Si on distingué une ligne dans l'histo
            if (bin.at<char>(i, 10) == 0) {
                //Si la lignes est dans les lignes retenues(pas aberrantes)
                vector<int>::iterator it = find(kept_lines.begin(), kept_lines.end(), i);
                if (it != kept_lines.end()) {
                    //cout << "debut d'une marche" << i << endl;
                    for (int j = 0; j < img.cols; j++) {
                        Eval.at<char>(i, j) = 0;
                        bef = 1;

                    }
                }
                else {
                    if (bef == 1) {
                        //cout << "toujours la meme marche" << i << endl;
                        for (int j = 0; j < img.cols; j++) {
                            Eval.at<char>(i, j) = 0;
                        }
                    }
                    else {
                        //cout << "avant c'était une marche mais plus maintenant" << i << endl;
                        bef = 0;
                        for (int j = 0; j < img.cols; j++) {
                            Eval.at<char>(i, j) = 255;
                        }
                    }
                }
            }
            else
            {
                //cout << "Pas de de marche" << i << endl;
                bef = 0;
                for (int j = 0; j < img.cols; j++) {
                    Eval.at<char>(i, j) = 255;
                }
            }
        }
       
        namedWindow("image", WINDOW_NORMAL);
        //imshow("image", Eval);
        imwrite(pathimage + "/Projection_Median.jpg", Eval);
    }

    return nbmarche;
}
