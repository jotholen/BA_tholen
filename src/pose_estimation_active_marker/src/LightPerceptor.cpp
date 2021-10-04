#include "../include/LightPerceptor.hpp"

using namespace cv;
using namespace std;

int main (int argc, char **argv)
{
  ros::init(argc, argv, "LightPerceptor");
  ros::NodeHandle n;
  LightPerceptor lp;
  ros::Subscriber image = n.subscribe("/prosilica/image_raw",1000, &LightPerceptor::saveImage, &lp);
  pub = n.advertise<pose_estimation_active_markers::lightContainer>("detected_lights", 1000);
  ros::Rate loop_rate(10);
  while(ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 1;
}

/**
 * callback function für imageMsg, ruft den detector.cpp an und published im Bild gefundene Lichter
 *
 * @param imageMsg
 */
void LightPerceptor::saveImage(const sensor_msgs::Image::ConstPtr& imageMsg){
  LightPerceptor lp;
  Detector d;
  cv_bridge::CvImagePtr cv_ptr;
  cv_ptr = cv_bridge::toCvCopy(imageMsg, sensor_msgs::image_encodings::MONO8);
  Mat img = cv_ptr->image;
  img = d.getUndistImg(img);
  ros::Time t = imageMsg->header.stamp;
  long double timeSec = t.toSec();
  long double timeNSec = t.toNSec();
  long double time = timeSec;
  Mat color;
  cvtColor(img, color, CV_GRAY2BGR);
  vector<Light> lights = d.getLights(img);
  pose_estimation_active_markers::lightContainer lightC;
  lightC.timestamp = time;
  for(auto l : lights){
    circle(color, cv::Point(l.x,l.y), l.size, Scalar(0,0,255), 3.5);
    pose_estimation_active_markers::light light;
    light.x = l.x;
    light.y = l.y;
    light.intensity = l.intensity;
    light.radius = l.size;
    lightC.lights.push_back(light);
  }
  cout << "lights:  " << lights.size() << endl;
  cv_bridge::CvImage cv_image;
  cv_image.image = img;
  cv_image.encoding = "mono8";
  sensor_msgs::Image ros_image;
  cv_image.toImageMsg(ros_image);
  lightC.img = ros_image;
  lightContainer = lightC;
  pub.publish(lightContainer);
  cv::String windowName = "deepleng_image"; //Name of the window
   namedWindow(windowName); // Create a window
   imshow(windowName,color);
   waitKey(10);
}
