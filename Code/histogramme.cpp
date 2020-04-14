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
int max(vector<int> vect) {
    int max;
    if (vect.size() == 1) { max = vect[0]; }
    else {
        max = vect[0];
        for (int i = 1; i < vect.size(); i++) {
            if (max < vect[i]) max = vect[i];
        }
    }
    //cout << "min est"<<min << std::endl;
    return max;
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

    //On s'assure d'avoir une image binaire. 
    threshold(img, img, 10, 255,  THRESH_BINARY);
    namedWindow("image1", WINDOW_NORMAL);
    imshow("image1", img);
    imwrite("FM_1_binaire.png", img);

    cout << "Rows : " << img.rows << endl;
    cout << "Colomns : " << img.cols << endl;
    const int size_i = img.rows;
    const int size_cols = img.cols;
   
    cout << "Nouveau ...." << endl;
    vector<float> histogramme;
    vector<float> hist_noirs;
    vector<float> hist_autres;

    /**********************************Creation de l'histogramme par rapport aux pixels blancs (fond) *******************************/
    for (int i = 0; i < size_i; i++) {
        float acc = 0;
        float black = 0;
        float autre = 0;
        for (int j = 0; j < size_cols; j++) {
            uchar pix = img.at<uchar>(i, j);
            if (i == 214) {
                cout << " colonne = " << j << " valeur = " << (int)pix << endl;
            }
            if ((int)pix == (int)255) {
                acc++;
                
            }
            else if((int)pix == (int)0) {
                black++;
            }
            else {
                autre++;
            }
         
        }
        histogramme.push_back(acc);
        hist_noirs.push_back(black);
        hist_autres.push_back(autre);
    }
    namedWindow("apres hist", WINDOW_NORMAL);
    imshow("apres hist", img);
    imwrite("FM_1_avant_Seuil.png", img);
    
    /*
    //Normalisation de l'histogramme
    float histo_normalized[4096];
    float tmp = 0;
    for (int i = 0; i < size_i; i++) {
        tmp = histogramme[i] / size_i;
        histo_normalized[i] = tmp;
        
    }
    */
    //Affichage de l'histogramme
    cout << "Histogramme (si autres = 0 dans tous les lignes c est que c'est bien une binaire" << endl;
    for (int i = 0 ; i < histogramme.size(); i++) {
        float pix_noirs = size_cols - histogramme.at(i);
        cout << "Ligne : " << i << "      Pixel Blanc = " << histogramme.at(i) << "      Pixel Noirs = " <<pix_noirs<<"        Hist Noirs = " << hist_noirs.at(i) <<"           Hist autre = "<< hist_autres.at(i)<<endl;
    }
    
    Mat avantSeuil1(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < size_i; i++) {
        float pix = histogramme.at(i);
        if (pix == size_cols) {
            for (int j = 0; j < size_cols; j++) {
                avantSeuil1.at<char>(i, j) = 255;
            }
        }
        else {
            for (int j = 0; j < size_cols; j++) {
                avantSeuil1.at<char>(i, j) = 0;
            }
        }
    }
    namedWindow("avant_Seuil", WINDOW_NORMAL);
    imshow("avant_Seuil", avantSeuil1);

    //imwrite("FM_1_avant_Seuil.png", avantSeuil1);
    
    
    /***************************************Definir le seuil***********************************/
    /*
    int prev = 0;
    float seuil = 0;
    //copy the vector 
    float input[2048];
    copy(histo_normalized + 1, histo_normalized + size_i, input);
    seuil = mostFrequent(input, size_i);
    //cout << "seuil est" << seuil << endl;
    */
    /*
    int bound = size_i - 20;
    vector<float> valeurKeep;
    for (int i = 0; i < size_i; i++) {
        float nb_pix = histogramme.at(i);
        if (nb_pix < bound) {
            valeurKeep.push_back(nb_pix);
        }
    }

    Mat apresSeuil(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < size_i; i++) {
        float pix = histogramme.at(i);
        if (pix >= bound) {
            for (int j = 0; j < size_cols; j++) {
                apresSeuil.at<char>(i, j) = 255;
            }
        }
        else {
            for (int j = 0; j < size_cols; j++) {
                apresSeuil.at<char>(i, j) = 0;
            }
        }
    }
    namedWindow("apres_Seuil", WINDOW_NORMAL);
    imshow("apres_Seuil", apresSeuil);
    */

    /*
    Sort les valeurs concervées de la plus petite (là ou il y a le plus de pixel noires) 
    à la plus grande valeur (là ou il y a le moins de pixel noirs)
    */
    /*
    sort(valeurKeep.begin(), valeurKeep.end());
    float etendu = 0;
    float mediane = 0;
    float moyenne = 0;
    int size_valeurKeep = valeurKeep.size();
    int last = size_valeurKeep - 1;

    etendu = valeurKeep.at(last) - valeurKeep.at(0);
    etendu = etendu / 2;

    int centre = size_valeurKeep / 2;
    mediane = valeurKeep.at(centre);

    for (int i = 0; i < size_valeurKeep; i++) {
        moyenne += valeurKeep.at(i);
    }
    moyenne = moyenne / size_valeurKeep;

    cout << endl;
    cout << "SEUIL : " << endl;
    cout << "Mediane = " << mediane << endl;
    cout << "Etendu  = " << etendu << endl;
    cout << "Moyenne  = " << moyenne << endl;
    */
    /*
    cout << "Histogramme par rapport à la ETENDU " << endl;
    cout << endl;
    for (int i = 0; i < size_i; i++) {
        float nb_pix = histogramme.at(i);
        if (nb_pix < etendu) {
            cout << "B" << endl;
        }
        else {
            cout << "." << endl;
        }
    }
    */
    /*
    Mat apresEtendu(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < size_i; i++) {
        float pix = histogramme.at(i);
        if (pix < etendu) {
            for (int j = 0; j < size_cols; j++) {
                apresEtendu.at<char>(i, j) = 0;
            }
        }
        else {
            for (int j = 0; j < size_cols; j++) {
                apresEtendu.at<char>(i, j) = 255;
            }
        }
    }
    namedWindow("apresEtendu", WINDOW_NORMAL);
    imshow("apresEtendu", apresEtendu);

    */
    
    /*****************************Definir nombre de ligne***********************************/

    /*
    int ecartmin = 0;
    int epline = 0;
    //vecteur des epaisseur des lignes pour l'écart minimal 
    vector <int>epaisseline;
    Mat bin(img.rows, img.cols, CV_8UC1);
    vector <float>lines;
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

    */
    /*
    for (int i = 0; i < lines.size(); i++) {
        cout << "lignes est :" << lines[i] << endl;
    }
    */
    /***************ecart max/ecart min : retirer les lignes aberantes************************************/
    // soit trop écartée ou trop proches

    /*
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
        //ecart max est le maximum des ecart entre les 4 premiere ligne

        vector<int> nv_ecartmax;
       
        for (int i = 0; i < 3; i++) {
            nv_ecartmax.push_back(lines[i] - lines[i+1]);
        }
        ecartmax = max(nv_ecartmax);
        
        //cout << "ecartmax est" << ecartmax << endl;
        //cout << "ecartmin est" << ecartmin << endl;


        int taille_lines = lines.size();
        vector<int> kept_lines;
        kept_lines.push_back(lines[0]);
        for (int i = 0; i < lines.size() - 1; i++) {
            //cout << lines[i] << endl;
            if (lines[i] - lines[i + 1] <= ecartmax) {
                if (lines[i] - lines[i + 1] > (3 * ecartmin)/2) {
                //if (lines[i] - lines[i + 1] >= (2 * ecartmin))  {
                    vector<int>::iterator it = find(kept_lines.begin(), kept_lines.end(), lines[i]);
                    if (it != kept_lines.end()) {
                        //cout << lines[i + 1] << endl;
                        kept_lines.push_back(lines[i + 1]);
                    }
                }
            }

        }
        */
        /*
        for (int i = 0; i < kept_lines.size(); i++) {
            cout << "lignes est :" << kept_lines[i] << endl;
        }
        */
        
        
        /******************************Nombre de marche*****************************************/
        /*
        int nbligne = kept_lines.size();
        if (nbmarche % 2 == 0) {
            nbmarche = nbligne / 2;
        }
        else {
            nbmarche = (nbligne - 1) / 2;
        }
        */
        /****************************The OutPut image for the evaluation************************/
        /*
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
    */
    int nbmarche = 10000;
    return nbmarche;
    
}
