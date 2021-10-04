#include "ros/ros.h"
//sonstiges includen
#include "std_msgs/String.h"
#include "sensor_msgs/Image.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image_transport/image_transport.h"
#include "cv_bridge/cv_bridge.h"
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "../../light_beacon_pose_estimation_lib/identifier.cpp"
#include "pose_estimation_active_markers/light.h"
#include "pose_estimation_active_markers/lightContainer.h"
#include "pose_estimation_active_markers/identifiedLight.h"
#include "pose_estimation_active_markers/identifiedLightContainer.h"

ros::Publisher pubIdentifiedLights;

class LightTracker
{
private:
public:
  void saveLights(const pose_estimation_active_markers::lightContainer::ConstPtr& msg);
};

cv::Mat getUndistImg(cv::Mat img);
