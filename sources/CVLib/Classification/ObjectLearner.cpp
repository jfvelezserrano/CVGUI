#include "ObjectLearner.h"

#include <opencv2/flann/flann.hpp>

void ObjectLearner::learnObjects(cv::Mat_<cv::Vec3b> image, const std::vector<std::vector<cv::Point> > &polygon)
{
    //Recorrer la imagen con un paso de S

    //Comprobar si el punto cae dentro de alguno de los poligonos

        //Si cae dentro calcular los descriptores para ese punto

        //Almacenar el descriptor

    //Tomar un descriptor, calcular la distancia al segundo y guardarla

    //Calcular la densidad optima de clasificación

    //cv::flann::Index flann_index(samples,cv::flann::KDTreeIndexParams(4),cvflann::FLANN_DIST_EUCLIDEAN);

}

//Recorrer la imagen con un paso de S

//Comprobar si el punto cae dentro de alguno de los poligonos

    //Si cae dentro calcular los descriptores para ese punto

    //Gardar el descriptor en el objeto correspondiente a ese poligono

//Tomar un descriptor, calcular la distancia al segundo y guardarla





std::vector<std::vector<cv::Point> > ObjectLearner::process(cv::Mat_<cv::Vec3b> image)
{
    std::vector <std::vector <cv::Point> > output;

    std::vector <cv::Point> polygon;
    polygon.push_back(cv::Point(100,100));
    polygon.push_back(cv::Point(150,100));
    polygon.push_back(cv::Point(200,200));

    output.push_back(polygon);

    output.push_back(polygon);

    //Calcular el quadtree de la imagen

    //Recorrer la imagen con un paso de S

        //Por cada punto calcular su descriptor

        //Si está dentro del limite de distancia respecto a un descripor guardado

            //Aumentar en un punto la confianza en la zona donde está el punto



    //Devolver las zonas con una densidad superior al umbral




    return output;
}

//Recorrer la imagen con un paso de S

    //Por cada punto calcular su descriptor

    //Si está dentro del limite de distancia respecto a un descripor guardado

        //Guardar a que objeto apunta dicho descriptor

//Split

//Tomar la imagen y comprobar si el 80% de los descriptores pueden pertenecer al mismo objeto

//En caso contrario romper la imagen y repetir con cada trozo

//Merge

//Por cada trozo mirar si al unirlo con un vecino adyacente se obtiene un objeto más fuerte
