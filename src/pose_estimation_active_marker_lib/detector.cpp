#include "./detector.h"

using namespace cv;
using namespace std;


Detector::Detector(){
  this->distance = 30;
  this->n = 20;
  this->maxSize = 60;
  this->minIntensity = 252;
}

/**
 * Funktion, die Lichter im Bild findet
 *
 * @param src source image
 * @return detected lights
 */
vector<Light> Detector::getLights(const Mat& src){
  //Kopie damit man Sachen einzeichnen kann ohne die src zu verändern
  Mat srcCopy;
  Mat srcTest;
  src.copyTo(srcCopy);
  cvtColor(src, srcTest, CV_GRAY2BGR);
  vector<Light> result;
  //Gradientenbild
  Mat grad;
  //minimale Distanz zwischen den Lichtern
  //Anwenden des Sobelfilters
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  Sobel( src, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
  Sobel( src, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );
  convertScaleAbs( grad_y, abs_grad_y );
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
  //Durchschnittswert des Bildes
  Scalar mean = cv::mean(src);
  int i = 0;
  //es wird n-mal nach einem Kandidaten gesucht, bei dem der Mittelpunkt, Intensität
  //und Größe gesucht wird und geprüft wird ob er als Licht in Frage kommt
  while(i < n){
    //p wird entweder der hellste Punkt des Gradientenbilds oder des normalen Bilds
    Point p;
    if(i > n/2){
      p = this->getCandidate(srcCopy);
    }
    else{
      p = this->getCandidate(grad);
    }
    //Es wird erstmal davon ausgegangen, dass es sich um ein Licht handelt. Beie einem Widerspruch wird die Variable false gesetzt.
    bool light = true;
    //sucht die Mitte des Lichts
    Point middle;
    bool isLightOfGetMiddle;
    this->getMiddle(srcCopy, p, distance, middle, isLightOfGetMiddle);
    light = light && isLightOfGetMiddle;
     //Größe vom Licht
     double size = this->getSize(src, middle);
     if(size >= maxSize - 4|| size <= 4){
       light = false;
     }
     vector<Point> candidates;
     Mat crop = this->getCroppedImage(srcCopy, middle, size*1.5);
     double slope, r2;
     this->getLinearFit(crop, Point(size*1.5,size*1.5), size*1.1, slope, r2);
     Light l;
     l.x = middle.x;
     l.y = middle.y;
     if(size > 2){
       size = size-2;
     }
     l.size = size;
     l.slope = slope;
     l.r2 = r2;
     l.intensity = this->getMean(src,middle,l.size/4);
     double roundeds = (int)(slope * 1000.0);
     double roundedr= (int)(r2 * 1000.0);
     if(isLight(l, slope, r2) && light){
     for(Light li : result){
       float distOfMiddles = sqrt(pow(abs(li.x) - abs(l.x),2) + pow(abs(li.y) - abs(l.y),2));
       if(distOfMiddles < (li.size + l.size) || distOfMiddles < distance){
         light = false;
       }
     }
     if(light && l.intensity > 150){
       result.push_back(l);
        String s = "s: " + to_string(size) + " r: " + to_string(roundedr);
     }}
     int circlewidth = size;
     if(size <= 10){
       circlewidth = 10;
     }
     circle(grad, middle, (l.size+2) * 2, 0, CV_FILLED);
     circle(srcCopy, middle, (l.size+2)* 2, 0, CV_FILLED);
     if(l.intensity < 150){
         String s = "s: " + to_string(l.intensity);
     }
     i++;
   }
  return result;
}

/**
 * function to get a part of the src img
 *
 * @param src source image
 * @param p Point that will be the middle of the cropped img
 * @param size half of the pixels at each side
 * @return croppedImg of src
 */
Mat Detector::getCroppedImage(const Mat& src, Point p, const float size){
  if(size == 0){
    return Mat();
  }
  if(p.x < size){
    p.x = size;
  }
  if(p.y > src.cols){
    p.y = src.cols - size *2;
  }
  Rect rect = Rect( p.x - size, p.y - size,size*2, size*2);
  Rect image_rect = Rect({}, src.size());
  Rect intersection = image_rect & rect;
  Rect interrect = intersection - rect.tl();
  Mat crop = cv::Mat::zeros(rect.size(), src.type());
  src(intersection).copyTo(crop(interrect));
  return crop;
}

/**
 * function to get brightest point of the image
 *
 * @param src source image
 * @return brightest Point
 */
Point Detector::getCandidate(const cv::Mat& src){
  Point p(0,0), min(0,0), max(0,0);
  double minVal, maxVal;
  minMaxLoc(src, &minVal, &maxVal, &min, &p);
  return p;
}

/**
 * function to get middle Point of the image
 *
 * @param src source image
 * @param p point in light
 * @param distance min distance between lights
 * @return middle point of light
 */
void Detector::getMiddle(const Mat& src, const Point& startPoint, float distance, Point& p, bool& isLight){
  bool finished = false;
  int counter = 0;
  Point origP = Point(startPoint.x, startPoint.y);
  p = Point(origP.x, origP.y);
  Mat cropSrc = this->getCroppedImage(src, p, distance);
  //es wird ein Bildausschnitt um den kandidaten-Punkt angesehen, dann wird auf das Bild ein threshold vom Durchschnitt des Bildausschnitt
  //gelegt und es wird geguckt was der Durchschnittspixel von allen Pixeln die höher als der Durchschnittshelligkeitswert sind, ist.
  //wenn dieser nicht im nahen Umfeld vom Kandidaten ist, wird der Bildausschnitt etwas vergrößert und neu zentriert.
  //es wird solange iteriert bis entweder der counter erreicht ist oder ein Punkt gefunden ist.
  while(!finished && counter < 35){
    counter ++;
    float maxVal = this->getBrightestVal(cropSrc);
    int mean = static_cast<int>(0, cv::mean(cropSrc)[0]);
    Mat thresh;
    threshold(cropSrc, thresh, mean + (maxVal - mean)/2, 255, CV_THRESH_TOZERO);
    double x = 0;
    double y = 0;
    double c1 = 0;
    for(int i = 0; i < thresh.rows; i++){
      for(int j = 0; j < thresh.cols; j++){
        if(static_cast<int>(thresh.at<uchar>(Point(i,j))) != 0){
          x += i;
          y += j;
          c1++;
        }
      }
    }
    if(c1 != 0){
      x = x/ c1;
      y = y/ c1;
    }
    Mat color;
    cvtColor(cropSrc, color, CV_GRAY2BGR);
    float dist = sqrt(pow(abs(x) - abs(distance),2) + pow(abs(y) - abs(distance),2));
    if(dist/distance < 0.05){
      finished = true;
    }
    else{
      distance += 1;
      p = Point(x + p.x - distance + 1,y + p.y -distance +1);
      cropSrc = this->getCroppedImage(src, p, distance);
    }
  }
  if(counter >= 15){
    isLight = false;
    p = origP;
    return;
  }
  isLight = true;
  return;
}

/**
 * function to get size of the light
 *
 * @param src source image
 * @param p middle point of light
 * @return brightest Point
 */
float Detector::getSize(const Mat& src, Point p){
  Mat cropSrc = this->getCroppedImage(src, p , maxSize);
  double firstMean = static_cast<int>(src.at<uchar>(p));
  double mean = firstMean;
  double nextMean = static_cast<int>(src.at<uchar>(p));
  int counter = 0;
  int counter2 = 1;
  bool nearlySameAsMiddle = true;
  //er geht vom Mittelpunkt aus durch größer werdende Radien durch und Berechnet immer den Durchschnittsmittelwert,
  //solange er sehr ähnlich bleibt geht er davon aus, dass die Größe des Lichts noch größer ist.
  //wenn der Durchschnittswert das erste Mal ein Stück weniger als der Durchschnitt ist, wird erwartet, dass er immer
  //kleiner wird, wenn das nicht mehr erfüllt ist, ist die Größe gefunden.
  while((nearlySameAsMiddle || counter < 2) && counter2 < this->maxSize){
    if(firstMean > nextMean + 10){
      nearlySameAsMiddle = false;
    }
    mean = nextMean;
    nextMean = this->getMean(cropSrc, Point(maxSize,maxSize), counter2);
    if(!nearlySameAsMiddle){
      if(mean > nextMean +5){
        counter = 0;
      }
      else{
        counter++;
      }
    }
    counter2 = counter2 +1;
  }
    return (counter2 - 3);
}

/**
 * Funktion die den liear fit ausgibt
 *
 * @param src source image
 * @param p Mittelpunkt
 * @param Punkte um den Mitelpunkt die berücksichtigt werden sollen
 * @return pair aus slope und R2
 */
void Detector::getLinearFit(Mat& src, Point p, int neighbours, double& slope, double& r2){
  int radius = 1;
  int cycles = neighbours;
  int c = 1;
  double mean = static_cast<int>(src.at<uchar>(p.x,p.y));
  double firstMean = static_cast<int>(src.at<uchar>(p.x,p.y));
  vector<pair<double, double>> radialContrasts;
  bool nearlySameAsMiddle = true;
  while(c < cycles){
    double radialMean = this->getMean(src, p, c * radius);
    if(firstMean > radialMean + 10){
      nearlySameAsMiddle = false;
    }
    if(nearlySameAsMiddle == false){
      double radialContrast = (mean - radialMean) / mean;
      radialContrasts.push_back(pair<double,double>(radius * c, radialContrast));
    }
    c++;
  }
  double intercept, middle;
  this->getLinearFitHelp(radialContrasts, slope, intercept, middle);
  double sum1, sum2;
  for(auto rc : radialContrasts){
    sum1 += pow(rc.second - (slope*rc.first + intercept), 2);
    sum2 += pow(rc.second - middle, 2);
  }
  r2 = 1 - (sum1 / sum2);

  return;
}

/**
 * Help function for getLinearFit
 *
 * @param radialContrasts
 * @return pair aus pair aus slope und intercept und middle
 */
void Detector::getLinearFitHelp(vector<pair<double, double>>& radialContrasts, double& slope, double& intercept, double& middle){
  double xSum = 0, ySum = 0, xxSum = 0, xySum = 0;
  for(auto rc : radialContrasts){
    xSum += rc.first;
    ySum += rc.second;
    xxSum += rc.first * rc.first;
    xySum += rc.first * rc.second;
  }
  slope = (radialContrasts.size() * xySum - xSum * ySum) / (radialContrasts.size() * xxSum - xSum * xSum);
  intercept = (ySum - slope * xSum) / radialContrasts.size();
  pair<double,double> slopeAndIntercept = pair<double, double>(slope, intercept);
  middle = ySum / radialContrasts.size();
  return;
}

/**
 * Funktion um den Durchschnitt von Nachbarn um einen Punkt rum zu bekommen
 *
 * @param src source image
 * @param p Ausgangspunkt
 * @param neighbours Nachbarn
 * @return Durchschnittswert der Nachbarn
 */
float Detector::getMean(const Mat &src, Point p, int neighbours){
  int count = 0;
  int pixelCount = 0;
  Mat color;
  cvtColor(src, color, CV_GRAY2BGR);
  circle(color, p, neighbours, Scalar(255,0,0), 1);
  for(int i = 0; i < color.rows; i++){
    for(int k = 0; k < color.cols; k++){
      if(color.at<Vec3b>(i,k)[0] == 255){
        count += static_cast<int>(src.at<uchar>(i,k));
        pixelCount++;
      }
    }
  }
  if(pixelCount != 0){
    return count / pixelCount;
  }
  return -1;
}

/**
 * function to get brightest point of the image
 *
 * @param src source image
 * @return brightest Point
 */
Point Detector::getBrightest(Mat& src){
  Point min(0,0), max(0,0);
  double minVal, maxVal;
  minMaxLoc(src, &minVal, &maxVal, &min, &max);
  return max;
}

/**
 * Funktion, die den hellsten Wert im Bild ausgibt
 *
 * @param src source image
 * @return brightest Value
 */
float Detector::getBrightestVal(Mat& src){
  Point min(0,0), max(0,0);
  double minVal, maxVal;
  minMaxLoc(src, &minVal, &maxVal, &min, &max);
  return maxVal;
}

/**
 * Funktion die das undistorted img ausgibt
 *
 * @param img
 * @return undistImg
 */
Mat Detector::getUndistImg(Mat& img){
  Mat undistImg;
  cv::Mat cam(3, 3, cv::DataType<float>::type);
  cam.at<float>(0, 0) = 1343.4903556784793f;
  cam.at<float>(0, 1) = 0.0f;
  cam.at<float>(0, 2) = 583.8024453851884f;

  cam.at<float>(1, 0) = 0.0f;
  cam.at<float>(1, 1) = 1326.186200f;
  cam.at<float>(1, 2) = 647.309442f;

  cam.at<float>(2, 0) = 0.0f;
  cam.at<float>(2, 1) = 0.0f;
  cam.at<float>(2, 2) = 1.0f;

  cv::Mat dist(5, 1, cv::DataType<float>::type);
  dist.at<float>(0, 0) = -0.29304035493544256f;
  dist.at<float>(1, 0) = 0.15974793650666747f;
  dist.at<float>(2, 0) = -0.00036672174984122755f;
  dist.at<float>(3, 0) = -0.003624512126425571f;
  dist.at<float>(4, 0) = 0.0f;
  cv::Mat map1, map2;
 cv::initUndistortRectifyMap(cam, dist, cv::Mat(), cam, cv::Size(img.cols, img.rows), CV_32FC1, map1, map2);

 cv::remap(img, undistImg, map1, map2, cv::INTER_CUBIC);
  return undistImg;
}

bool Detector::isLight(Light& l, double& slope, double& r2){
  if(slope > 0){
    return true;
  }
  return true;
}
