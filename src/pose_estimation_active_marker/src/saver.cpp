#include "../include/saver.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace Eigen;

void Saver::saveTf(const tf::tfMessage::ConstPtr& tfMsg){
  ros::Time t = tfMsg->transforms[0].header.stamp;
  double timeSec = t.toSec();
  timeSec = timeSec - 1618313790.0 - 2934000;
  cout << "time" << timeSec << endl;
  Eigen::Vector3d pos(tfMsg->transforms[0].transform.translation.x, tfMsg->transforms[0].transform.translation.y, tfMsg->transforms[0].transform.translation.z);
  Eigen::Quaterniond ori(tfMsg->transforms[0].transform.rotation.w, tfMsg->transforms[0].transform.rotation.x, tfMsg->transforms[0].transform.rotation.y,tfMsg->transforms[0].transform.rotation.z);
  ori.normalize();
  Eigen::Vector3d euler = Eigen::Matrix3d(ori).eulerAngles(0,1,2);
  std::pair<Vector3d,Quaterniond> poseWithoutTime = std::pair<Vector3d,Quaterniond>(pos, ori);
  Eigen::Matrix3d rot = ori.toRotationMatrix();
  poses.push_back(std::pair<std::pair<Eigen::Vector3d,Eigen::Quaterniond>,long double>(poseWithoutTime,timeSec));
}

void Saver::savePose(const pose_estimation_active_markers::pose::ConstPtr& msg){
  c++;
  cout << "test "<< c << endl;
  double t = msg->t;
  t = t - 1618313790.0 - 2900000;
  Quaterniond q(msg->qw, msg->qx, msg->qy, msg->qz);
  std::pair<Vector3d,Quaterniond> poseWithoutTime = std::pair<Vector3d,Quaterniond>(Vector3d(msg->x,msg->y,msg->z), q);
  poses.push_back(std::pair<std::pair<Eigen::Vector3d,Eigen::Quaterniond>,long double>(poseWithoutTime,t));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "saver");
  ros::NodeHandle n;
  Saver s;
  ros::Subscriber subSaveTf = n.subscribe("tf", 1000, &Saver::saveTf, &s);
  ros::Subscriber subSavePose = n.subscribe("pose", 1000, &Saver::savePose, &s);
  ros::Rate loop_rate(10);
  std::ofstream csvfiletagpose;
  csvfiletagpose.open(s.filename);
  if(!csvfiletagpose){
      std::cerr << "Error: file could not be opened" << "/n";
      exit(1);
  }
  csvfiletagpose << "apriltagerkennung position.x ; position.y; position.z; time";
  for(std::pair<std::pair<Vector3d,Quaterniond>,long double> pose : s.poses)
  {
    Eigen::Vector3d euler = getEuler(pose.first.second);
    csvfiletagpose << std::to_string(pose.first.first.x()) + ";" + std::to_string(pose.first.first.y()) + ";" + std::to_string(pose.first.first.z()) + ";" + std::to_string(euler.x()) + ";" + std::to_string(euler.y()) + ";" + std::to_string(euler.z()) + ";" + std::to_string(pose.second) << std::endl;
  }
  csvfiletagpose.close();
  return 0;
}

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
