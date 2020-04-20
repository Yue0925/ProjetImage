#include "Histogramme.h"
using namespace cv;
using namespace std;


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
int nombreDeMarche1(std::string path, const cv::Mat& img) {
    cout << "Rows : " << img.rows << endl;
    cout << "Colomns : " << img.cols << endl;
    const int size_rows = img.rows;
    const int size_cols = img.cols;
    vector<int> hist_noirs;

    /**********************************Creation de l'histogramme par rapport aux pixels noirs (fond) *******************************/
    for (int i = 0; i < size_rows; i++) {
        int black = 0;
        for (int j = 0; j < size_cols; j++) {
            uchar pix = img.at<uchar>(i, j);

            if ((int)pix == 0) {
                black++;
            }

        }
        hist_noirs.push_back(black);
    }


    //Affichage de l'histogramme
    /*for (int i = 0; i < hist_noirs.size(); i++) {
        cout << "Ligne : " << i << " " << hist_noirs.at(i) << endl;
    }
    */
    //Construction de limage avant l'histogramme
    Mat avantSeuil1(img.rows, img.cols, CV_8UC1);

    for (int i = 0; i < size_rows; i++) {
        int pix = hist_noirs.at(i);
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
    /*
    imshow("FM_1_avant_Seuil", avantSeuil1);
    cv::waitKey();
    cv::destroyWindow("Projection_avant_Seuil");

    imwrite(path + "/Projection_avant_Seuil.jpg", avantSeuil1);
    */

    /***************************************Definir le seuil***********************************/


    float seuil = 4;
    int size_i = img.rows;
    vector<float> valeurKeep;

    for (int i = 0; i < size_i; i++) {
        float nb_pix = hist_noirs.at(i);
        if (nb_pix > 10) {
            valeurKeep.push_back(nb_pix);
        }
    }

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

    int bound = mediane;

    
    /**********************************************Definir nombre de ligne**************************************************/

    Mat apresSeuil(img.rows, img.cols, CV_8UC1);
    int prev = 0;
    vector<int> med;
    vector <int> detetcted_lines(size_i, 0);
    for (int i = size_i - 1; i >= 0; i--) {
        if (hist_noirs[i] < seuil) {
            prev = 0;
            detetcted_lines[i] = 0;
            if (med.size() > 0) {
                int maxi = max(med);
                if (maxi < mediane) {
                    for (int l = 1; l <= med.size(); l++) {
                        detetcted_lines[l + i] = 0;
                    }
                }
                med.clear();
            }
        }
        if ((hist_noirs[i] >= seuil) && (prev == 1)) {
            detetcted_lines[i] = 1;
            med.push_back(hist_noirs[i]);
        }
        if ((hist_noirs[i] >= seuil) && (prev == 0)) {
            prev = 1;
            med.push_back(hist_noirs[i]);
            detetcted_lines[i] = 2;
        }
    }
   
    //Enregistrement de l'image apres le seuil
    for (int i = 0; i < size_i; i++) {

        if (detetcted_lines[i] > 0) {
            for (int j = 0; j < size_cols; j++) {
                apresSeuil.at<char>(i, j) = 0;
            }
        }
        else {
            for (int j = 0; j < size_cols; j++) {
                apresSeuil.at<char>(i, j) = 255;
            }
        }
    }
    /*
    imshow("apres_Seuil", apresSeuil);
    cv::waitKey();
    cv::destroyWindow("apres_Seuil");

    cv::imwrite(path + "/Projection_apres_Seuil.jpg", apresSeuil);
    */


    /*
    for (int i = 0; i < detetcted_lines.size(); i++) {
        if(detetcted_lines[i] == 2)   cout << "lignes est :" << deb_lines[i] << endl;
    }
    */

    int nbligne = 0;
    for (int i = 0; i < detetcted_lines.size(); i++) {
        if (detetcted_lines[i] == 2) { nbligne++; }
    }
    std::cout << "Nombre de lignes est :" << nbligne << std::endl;
    
    /***************************************************Nombre de marche****************************************************/
    int nbmarche = 0;
    if (nbmarche % 2 == 0) {
        nbmarche = nbligne / 2;
    }
    else {
        nbmarche = (nbligne - 1) / 2;
    }
    return nbmarche;
}

int nombreDeMarche2(std::string path, const cv::Mat& img) {

    /*
    imshow("FM_1_binaire", img);
    cv::waitKey();
    cv::destroyWindow("FM_1_binaire");

    //imwrite(path + "/FM_1_binaire.jpg", img);
    */
    cout << "Rows : " << img.rows << endl;
    cout << "Colomns : " << img.cols << endl;
    const int size_rows = img.rows;
    const int size_cols = img.cols;
    vector<int> hist_noirs;

    /**********************************Creation de l'histogramme par rapport aux pixels noirs (fond) *******************************/
    for (int i = 0; i < size_rows; i++) {
        int black = 0;
        for (int j = 0; j < size_cols; j++) {
            uchar pix = img.at<uchar>(i, j);
          
            if ((int)pix == 0) {
                black++;
            }
           
        }
        hist_noirs.push_back(black);
    }


    //Affichage de l'histogramme
    /*
    for (int i = 0; i < hist_noirs.size(); i++) {
        cout << "Ligne : " << i <<" "<< hist_noirs.at(i) << endl;
    }
    */

    Mat avantSeuil1(img.rows, img.cols, CV_8UC1);

    for (int i = 0; i < size_rows; i++) {
        int pix = hist_noirs.at(i);
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
    /*
    imshow("FM_1_avant_Seuil", avantSeuil1);
    cv::waitKey();
    cv::destroyWindow("Projection_avant_Seuil");
    */
    cv::imwrite(path + "/Projection_avant_Seuil.jpg", avantSeuil1);
    

    /**************************************************************Definir le seuil********************************************************/
    vector<float> valeurKeep;
    

    for (int i = 0; i < size_rows; i++) {
        float nb_pix = hist_noirs.at(i);
        if (nb_pix >10) {
            valeurKeep.push_back(nb_pix);
        }
    }
    
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

    //Nous prenons la mediane comme seuil pour detecter les lignes

    /*****************************Definir nombre de ligne***********************************/

    Mat apresSeuil(img.rows, img.cols, CV_8UC1);
    float seuil = 4; //detetcter au moins un pixel noir
    int prev = 0;
    int epline = 0;
    //vecteur des epaisseur des lignes pour l'ecart minimal
    vector <int>epaisseline;
    vector<int> med;
    vector <int> detetcted_lines(size_rows,0);
    for (int i = size_rows - 1; i >= 0; i--) {
        if (hist_noirs[i] < seuil) {
            prev = 0;
            detetcted_lines[i]=0;
            if (epline != 0) {
                epaisseline.push_back(epline);
            }
            if (med.size() > 0) {
                int maxi = max(med);
                if (maxi < mediane) {
                    for (int l = 1;l<=med.size();l++) {
                        detetcted_lines[l+i] = 0;
                    }
                    epaisseline.pop_back();
                }
                epline = 0;
                med.clear();
            }
        }
        if ((hist_noirs[i] >= seuil) && (prev == 1)) {
            epline++;
            detetcted_lines[i]=1;
            med.push_back(hist_noirs[i]);
        }
        if ((hist_noirs[i] >= seuil) && (prev == 0)) {
            prev = 1;
            epline++;
            med.push_back(hist_noirs[i]);
            detetcted_lines[i]=2;

        }
    }


    // rajouter l'epaisseur de de la derniere ligne
    if (epline != 0) {
        epaisseline.push_back(epline);
    }

    //Enregistrer l'image apres l'application du seuil
    for (int i = 0; i < size_rows; i++) {
        
        if (detetcted_lines[i] > 0) {
            for (int j = 0; j < size_cols; j++) {
                apresSeuil.at<char>(i, j) = 0;
            }
        }
        else {
            for (int j = 0; j < size_cols; j++) {
                apresSeuil.at<char>(i, j) = 255;
            }
        }
    }
    /*
    imshow("apres_Seuil", apresSeuil);
    cv::waitKey();
    cv::destroyWindow("apres_Seuil");
    */
    cv::imwrite(path + "/Projection_apres_Seuil.jpg", apresSeuil);
    


    /*
    for (int i = 0; i < detetcted_lines.size(); i++) {
        if(detetcted_lines[i] == 2)   cout << "lignes est :" << deb_lines[i] << endl;
    }
    */

    /***************ecart max/ecart min : retirer les lignes aberantes************************************/

    int ecartmin = 0;  //ecart min est le minimum des epaisseur des lignes * une constante
    int ecartmax = 0;  //ecart max est le maximum des ecart entre les 4 premiere ligne
   
    ecartmin = min(epaisseline);
    //ecartmin = 5;
    vector<int> nv_ecartmax;
    vector<int> lines;
    
    for (int i = detetcted_lines.size()-1 ; i >=0 ; i--) {
        if(detetcted_lines[i] == 2)   lines.push_back(i);
    }

    //S'assurer que le nombre de lignes est egales au nombre de d'epaisseur de lignes
    if (lines.size() == epaisseline.size()) cout << "nickel !" << endl;
    else {
        cout << "Mon dieu !" << endl;
       // cout <<"nb line "<< lines.size() << "  nb epaisse" << epaisseline.size() << endl;
    }



    /*
    for (int i = 0; i < lines.size(); i++) {
        cout << "lignes est :" << lines[i] << endl;
    }
    */


    for (int i = 0; i < 3; i++) {
        nv_ecartmax.push_back((lines[i]-epaisseline[i] )- (lines[i+1]));
    }
    ecartmax = max(nv_ecartmax);

    cout << "ecartmax est" << ecartmax << endl;
    cout << "ecartmin est" << ecartmin << endl;
    
    vector<int> kept_lines;
    kept_lines.push_back(lines[0]);
    
    for (int i = 0; i < lines.size() - 1; i++) {
        int ecart = (lines[i] - epaisseline[i]) - (lines[i + 1]);
        cout << " " << ecart << endl;
        if (ecart <= ecartmax) {
            if (ecartmax < (3 * ecartmin) / 2) {
                if ((ecart >= ecartmin) || (ecart < 5)) {
                    if (count(kept_lines.begin(), kept_lines.end(), lines[i])) {
                        kept_lines.push_back(lines[i + 1]);
                        cout << " " << ecart << endl;
                    }

                }

            }
            else {
                if ((ecart >= (3 * ecartmin) / 2) || (ecart < 5)) {
                    if (count(kept_lines.begin(), kept_lines.end(), lines[i])) {
                        kept_lines.push_back(lines[i + 1]);
                        cout << " " << ecart << endl;
                    }

                }

            }
            
        }
    }
    

    /*
    for (int i = 0; i < kept_lines.size(); i++) {
        cout << "lignes est :" << kept_lines[i] << endl;
    }
    */


    //vecteur pour l'evaluation 1 pour la ligne marqué et 0 sinon 
    prev = 0;
    for (int i = size_rows - 1; i >= 0; i--) {
        if (detetcted_lines[i] == 2) {
            if (count(kept_lines.begin(), kept_lines.end(), i)) {
                prev = 1;
                //cout << "hey" << endl;
                detetcted_lines[i] == 1;
            }
            else {
                prev = 0;
                detetcted_lines[i] = 0;
            }

        }
        if (detetcted_lines[i] == 1) {
            if(prev==0) detetcted_lines[i] = 0;
        }
    }
    /*
    for (int i = 0; i < detetcted_lines.size(); i++) {
        cout << "lignes est :" << detetcted_lines[i] << endl;
    }
    */
    //enlevr la premiere ligne qui est du au  bruit
    int nbligne = kept_lines.size();
    if (kept_lines[0] == size_rows -1) {
        for (int i = 0; i < epaisseline[0]; i++) {
            detetcted_lines[(size_rows-1) - i] = 0;
            //cout << "lignes est :" << detetcted_lines[(size_rows-1) - i] <<" "<<i << endl;
        }
        nbligne--;
    }

    Mat apres_Nettoyage(img.rows, img.cols, CV_8UC1);
    //visualiser les résultats
    for (int i = 0; i < size_rows; i++) {

        if (detetcted_lines[i] > 0) {
            for (int j = 0; j < size_cols; j++) {
                apres_Nettoyage.at<char>(i, j) = 0;
            }
        }
        else {
            for (int j = 0; j < size_cols; j++) {
                apres_Nettoyage.at<char>(i, j) = 255;
            }
        }
    }

    imshow("apres_Nettoyage", apres_Nettoyage);
    cv::waitKey();
    cv::destroyWindow("apres_Nettoyage");

    cv::imwrite(path + "/Projection_apres_Nettoyage.jpg", apres_Nettoyage);

    /******************************Nombre de marche*****************************************/
    int nbmarche = 0;
    if (nbmarche % 2 == 0) {
        nbmarche = nbligne / 2;
    }
    else {
        nbmarche = (nbligne - 1) / 2;
    }
    
    /****************************The OutPut image for the evaluation************************/
    /*
    Mat Eval(img.rows, img.cols, CV_8UC1);
    int bef = 0;
    for (int i = size_i - 1; i >= 0; i--) {
        //Si on distingu¨¦ une ligne dans l'histo
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
                    //cout << "avant c'¨¦tait une marche mais plus maintenant" << i << endl;
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
    /*
    imshow("FM_1_avant_Seuil", avantSeuil1);
    cv::waitKey();
    cv::destroyWindow("Projection_avant_Seuil");

imwrite(path + "/Projection_avant_Seuil.jpg", avantSeuil1);
*/

return nbmarche;
}