#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv/cv.h>
#include <vector>
#include <Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <Eigen/Geometry>
#include <string>
using namespace std;
using namespace Eigen;

Eigen::Vector3d getEuler(const Eigen::Quaterniond& orientation)
{
    const Eigen::Matrix3d m = orientation.toRotationMatrix();
    double x = Eigen::Vector2d(m.coeff(2,2) , m.coeff(2,1)).norm();
    Eigen::Vector3d res(0,std::atan2(-m.coeff(2,0), x),0);
    if (x > Eigen::NumTraits<double>::dummy_precision()){
        res[0] = std::atan2(m.coeff(1,0), m.coeff(0,0));
        res[2] = std::atan2(m.coeff(2,1), m.coeff(2,2));
    }else{
        res[0] = 0;
        res[2] = (m.coeff(2,0)>0?1:-1)* std::atan2(-m.coeff(0,1), m.coeff(1,1));
    }
    return res;
}

void calc(int pose){
  fstream file("/home/johanna/lichter01234pose" + to_string(pose) + "sehrkurze2.csv", ios::in);
  vector<Vector3d> vec = vector<Vector3d>();
  vector<double> result = vector<double>();
  if (!file)
  {
    cerr << "Can't open file..." << endl;
    return;
  }
  double rollApril(3.121783), yawApril(-0.043985), pitchApril(3.075432);
  //Pose1
  if(pose == 1){
    rollApril = 3.121783;
    yawApril = -0.043985;
    pitchApril = 3.075432;
  }
  //Pose 2
  if(pose == 2){
    rollApril = 3.125801;
    yawApril =-0.090564;
    pitchApril = 3.063856;
  }
  //Pose3
  if(pose == 3){
    rollApril= (3.115590);
    yawApril = (-0.075653);
    pitchApril = (-3.139372);
  }
  //Pose 4
  if(pose == 4){
    rollApril = (3.072806);
    yawApril = (0.541483);
    pitchApril = (3.045368);
  }
  //Pose 5
  if(pose == 5){
    rollApril = (3.066592);
    yawApril = (0.522411);
    pitchApril = (3.027483);
  }
  //Pose6
  if(pose == 6){
    rollApril = (3.070586);
    yawApril = (0.515360);
    pitchApril = (3.050635);
  }
  if(pose==7){
    rollApril= (-3.129801);
    yawApril = (-0.497433);
    pitchApril = (3.080927);
  }
  //Pose 8
  if(pose == 8){
    rollApril = (-3.12);
    yawApril = (-0.506);
    pitchApril = (3.0539);
  }
  //Pose 9
  if(pose == 9){
    rollApril = (-3.128373);
    yawApril = (-0.519923);
    pitchApril = (3.065634);
  }
  Eigen::AngleAxisd rollAnglea(rollApril, Eigen::Vector3d::UnitZ());
  Eigen::AngleAxisd yawAnglea(yawApril, Eigen::Vector3d::UnitY());
  Eigen::AngleAxisd pitchAnglea(pitchApril, Eigen::Vector3d::UnitX());

  Eigen::Quaternion<double> qa = rollAnglea * yawAnglea * pitchAnglea;
  Eigen::AngleAxisd aa(qa);

  Eigen::Matrix3d rotationMatrixa = qa.matrix();
  int counter = 0;
  while ( !file.eof() ) 	// Einlesen bis Dateiende
  {
    string row;
    getline(file, row);	// Einlesen einer Zeile

    if ( !row.empty() ) // Falls Zeile nicht leer
    {
      istringstream ss(row);		// StringStream zum zerlegen der Zeile
      string temp;

      getline(ss, temp, ';');		// Ersten Wert bis zum Trennzeichen
                    // einlesen
      int c = 0;
      double vecalpha(0), vecgamma(0), vecbeta(0);
      do
      {
        stringstream convert;	// StringStream für die Konvertierung
        convert << temp;		// Konvertierung
        double dtemp;			// des String
        convert >> dtemp;
        if(c == 3)		// nach double
            vecalpha= dtemp;
        else if(c==4)
           vecbeta = dtemp;
        else if(c==5){// und einfügen in den vector
            vecgamma = dtemp;
            if(counter != 0){
              vec.push_back(Vector3d(vecalpha, vecbeta, vecgamma));
            }
            counter++;
          }

        temp.clear();			// String leeren
        getline(ss, temp, ';');	// Nächsten Wert einlesen
        c++;
      } while ( !temp.empty() );	// Falls Temp Leer ist, gibt es keine
    }							// neuen Werte in der aktuellen Zeile
  }
  file.close();
  std::ofstream csvfiletagpose;
  for(Vector3d v : vec){
    Eigen::AngleAxisd rollAngle(v.x(), Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd yawAngle(v.y(), Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd pitchAngle(v.z(), Eigen::Vector3d::UnitX());

    Eigen::Quaternion<double> q = rollAngle * yawAngle * pitchAngle;
  /*  Matrix3d rott, rott2;
    rott << 1,0,0
    ,0,1,0,
    0,0,1;
    rott2 << 0,-1,0,
    1,0,0,
    0,0,1;
    q = Quaterniond(rott);
    qa = Quaterniond(rott2);*/
    q = q * qa.inverse();
    /*Matrix3d rm = q.matrix();
    cv::Mat om;
    cv::eigen2cv(rm, om);
    float error = cv::norm( cv::Rodrigues (om);*/
    Eigen::AngleAxisd aa(q);
    cout << "angel  " << aa.angle() << endl;
    result.push_back(aa.angle());
    //cout << "axis  " << aa.axis() << endl;
    Eigen::Matrix3d rotationMatrix = q.matrix();
  }
  csvfiletagpose.open("/home/johanna/orienterrorpose" + to_string(pose) + "01234.csv");
  if(!csvfiletagpose){ // file couldn't be opened
      std::cerr << "Error: file could not be opened" << "/n";
      exit(1);
  }
  for(double e : result)
  {
    csvfiletagpose << std::to_string(e) << std::endl;
  }
  csvfiletagpose.close();
}
int main(){
  int pose = 1;
  while(pose < 10){
    calc(pose);
    pose++;
  }
	return 0;
}
