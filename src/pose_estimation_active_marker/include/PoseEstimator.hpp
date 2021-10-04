#include "ros/ros.h"
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
#include "pose_estimation_active_markers/light.h"
#include "pose_estimation_active_markers/lightContainer.h"
#include "pose_estimation_active_markers/pose.h"
#include "../include/LightTracker.h"
#include <ceres/ceres.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <memory>
#include "Manifolds.hpp"


class PoseEstimator
{
private:
public:
  void estimatePose(const pose_estimation_active_markers::identifiedLightContainer::ConstPtr& msg);
  ros::Publisher pubPose;
};
