#include "ros/ros.h"
#include "std_msgs/String.h"
#include "tf/tfMessage.h"

#include "sensor_msgs/Image.h"
#include "pose_estimation_active_markers/pose.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <Eigen/Core>
#include <Eigen/Geometry>


class Saver
{
public:
  std::vector<std::pair<std::pair<Eigen::Vector3d,Eigen::Quaterniond>,long double>> poses;
  void saveTf(const tf::tfMessage::ConstPtr& stateMsg);
  void savePose(const pose_estimation_active_markers::pose::ConstPtr& msg);
  bool firstPositionFound = false;
  std::string filename = "/home/johanna/lichter01234pose9sehrkurze2.csv";
  int c = 0;
  Eigen::Vector3d startPosition;
  bool dataflowActual = false;
  bool dataflowActualLastFrame = false;
  int dataflowActualCounter = 0;
  bool dataflowDesired = false;
  bool dataflowDesiredLastFrame = false;
  int dataflowDesiredCounter = 0;
  bool datastartedActual = false;
  bool datastartedDesired = false;
};

Eigen::Vector3d getEuler(const Eigen::Quaterniond& orientation);
