#include "Histogramme.h"



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


int nombreDeMarche(std::string pathimage) {
    cv::Mat img = cv::imread(pathimage);
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
