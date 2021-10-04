#include "../include/PoseEstimator.hpp"
#include "../../light_beacon_pose_estimation_lib/poseEstimatorLib.cpp"

using namespace cv;
using namespace std;
using namespace Eigen;
using namespace uwv_calibration;



int main (int argc, char **argv)
{
  ros::init(argc, argv, "PoseEstimator");
  ros::NodeHandle n;
  PoseEstimator pe;
  ros::Subscriber sub = n.subscribe("identified_lights", 1000, &PoseEstimator::estimatePose, &pe);
  pe.pubPose = n.advertise<pose_estimation_active_markers::pose>("pose", 1000);
  ros::Rate loop_rate(10);
  while(ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 1;
}

/**
 * callback function für identifiedLightContainer msg.
 * veröffentlicht die aus den identifizierten Markern abgeleitete Pose
 *
 * @param msg
 */
void PoseEstimator::estimatePose(const pose_estimation_active_markers::identifiedLightContainer::ConstPtr& msg)
{
  PoseEstimatorLib p;
  pair<Eigen::Vector2d, float> measurement[6];
  vector<int> ids = vector<int>();
  measurement[0] = std::pair<Eigen::Vector2d, float>(Vector2d(2,2), 2.1f);
  //Wenn mehrere Lichter auf einer Linie sind, soll importantLight aussagen, dass min eins nicht auf der Linie ist
  bool importantlight = true;
  int counter = 0;
  //identifizierte Lichter werden in Liste gefügt um getPose aufzurufen
  for(int i = 0; i < msg->lights.size(); i++){
    //Wenn Unsicherheit zu groß soll Licht nicht hinzugefügt werden
    /*if(msg->lights[i].certainty > 1){
      continue;
    }*/
    //Wenn ein Licht 2 mal identifiziert, wird abgebrochen
    if(std::find(ids.begin(), ids.end(), msg->lights[i].identity) != ids.end()) {
      return;
    }
    ids.push_back(msg->lights[i].identity);
    counter++;
    if(msg->lights[i].identity == 1 || msg->lights[i].identity == 4){
      importantlight = true;
    }
    //pair aus Pixel und uncertainty
    pair<Vector2d, float> t = pair<Vector2d, float>(Vector2d(msg->lights[i].x, msg->lights[i].y), msg->lights[i].certainty);
    measurement[msg->lights[i].identity] = t;
  }
  if(counter < 3 || !importantlight){
    return;
  }
  std::pair<Vector3d, Quaterniond> pose = p.getPose(measurement, msg->timestamp, ids);
  pose_estimation_active_markers::pose poseMsg;
  poseMsg.x = pose.first.x();
  poseMsg.y = pose.first.y();
  poseMsg.z = pose.first.z();
  poseMsg.qx = pose.second.x();
  poseMsg.qy = pose.second.y();
  poseMsg.qz = pose.second.z();
  poseMsg.qw = pose.second.w();
  poseMsg.t = msg->timestamp;
  if(sqrt(pow(0 - abs(pose.first.x()),2) + pow(0 - abs(pose.first.y()),2) + sqrt(pow(0 - abs(pose.first.z()),2))) < 20)
    this->pubPose.publish(poseMsg);
}
