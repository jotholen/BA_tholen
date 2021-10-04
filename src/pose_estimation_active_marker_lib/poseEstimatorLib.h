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
#include <ceres/ceres.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <memory>
#include "../pose_estimation_active_markers/include/Manifolds.hpp"
#include "../pose_estimation_active_markers/include/OpticalFeatureConstraint.hpp"

struct identifiedM{
  Eigen::Vector2d pos;
  double score = 0;
  int identity;
};
Eigen::Vector3d lastPos = Eigen::Vector3d(0.0f,0.0f,2.5f);
Eigen::Vector4d lastOri = Eigen::Vector4d(0,0,0, 1);
class PoseEstimatorLib
{
private:
  uwv_calibration::OpticalFeatureConstraint::CameraConfiguration camera_config;
  Eigen::Vector3d feature_positions[6];
  std::vector<std::vector<identifiedM>> identifiedMarkers;
public:
  std::pair<Eigen::Vector3d, Eigen::Quaterniond> getPose(std::pair<Eigen::Vector2d, float> measurement[], double timestamp, std::vector<int> ids);
  PoseEstimatorLib();
};
