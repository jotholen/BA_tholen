#include <iostream>
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv/cv.h>
#include <stdlib.h>
#include <vector>

struct Light{
  double x;
  double y;
  double intensity;
  double size;
  double slope;
  double r2;
};

class Detector
{
private:
  double distance;
  double n;
  double maxSize;
  double minIntensity;
public:
  Detector();
  void getLinearFit(cv::Mat& src, cv::Point p, int neighbours, double& slope, double& r2);
  std::vector<Light> getLights(const cv::Mat& src);
  float getMean(const cv::Mat &src, cv::Point p, int neighbours);
  void getLinearFitHelp(std::vector<std::pair<double, double>>& radialContrasts, double& slope, double& intercept, double& middle);
  cv::Point getMiddle(cv::Mat& src);
  cv::Point getBrightest(cv::Mat& src);
  float getSize(const cv::Mat &src, cv::Point p);
  void getMiddle(const cv::Mat& src, const cv::Point& startPoint, float distance, cv::Point& p, bool& isLight);
  cv::Point getCandidate(const cv::Mat& src);
  cv::Mat getCroppedImage(const cv::Mat& src, cv::Point p, const float size);
  float getBrightestVal(cv::Mat& src);
  cv::Mat getUndistImg(cv::Mat& img);
  bool isLight(Light& l, double& slope, double& r2);
};
