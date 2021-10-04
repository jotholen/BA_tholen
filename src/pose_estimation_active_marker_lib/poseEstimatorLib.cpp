#include "./poseEstimatorLib.h"

using namespace cv;
using namespace std;
using namespace Eigen;
using namespace uwv_calibration;

struct State
{
    uint64_t timestamp;
    Eigen::Vector3d position;
    Eigen::Vector4d orientation;

    State() : timestamp(0), position(0,0,0), orientation(0,0,0,1) {}
};

PoseEstimatorLib::PoseEstimatorLib(){
  this->camera_config.cx = 583.8024453851884f;
  this->camera_config.cy = 647.309442f;
  this->camera_config.fx = 1343.4903556784793f;
  this->camera_config.fy = 1326.186200f;

  this->feature_positions[0] = Eigen::Vector3d(0.3375, -0.3825, 0.054);
  this->feature_positions[1] = Eigen::Vector3d(-0.4725, -0.3825, 0.054);
  this->feature_positions[2] = Eigen::Vector3d(-0.2475, -0.010, 0.054);
  this->feature_positions[3] = Eigen::Vector3d(-0.6975, 0.005, 0.054);
  this->feature_positions[4] = Eigen::Vector3d(0.3375,0.3825, 0.054);
  this->feature_positions[5] = Eigen::Vector3d(-0.4725,0.2475, 0.054);
}

std::pair<Vector3d, Quaterniond> PoseEstimatorLib::getPose( pair<Vector2d, float> measurement[], double timestamp, vector<int> ids){
  cout << " siotgnsdghnsjltrhnwrjksgnsgsrzhezdtghfrt  " << ids.size() << endl;
  for(int i : ids){
    cout << "idaasdasd  " << i << endl;
  }
  vector<identifiedM> newMarkers = vector<identifiedM>();
  //identifiedMarkers ist Liste aus den in den letzten Frames erkannten Markern. Die die länger als 4 Frames her sind werden gelöscht.
  if(identifiedMarkers.size() > 4){
    identifiedMarkers.pop_back();
  }
  //für alle erkannten ids wird das gemessene in newMarkers gefügt
  for(int i : ids){
    identifiedM m;
    m.identity = i;
    m.pos = measurement[i].first;
    m.score = measurement[i].second;
    newMarkers.push_back(m);
  }
  identifiedMarkers.insert(identifiedMarkers.begin(), newMarkers);

  Eigen::Matrix2d feature_cov = Eigen::Vector2d(1.0, 1.0).cwiseAbs2().asDiagonal();

  ceres::Problem::Options problem_options;
  ceres::Problem problem(problem_options);
  ceres::LocalParameterization* quat_parameterization = new ceres::EigenQuaternionParameterization;
  ceres::LossFunction* loss_function = new ceres::SoftLOneLoss(0.3);
  std::vector<std::shared_ptr<State>> states;
  std::shared_ptr<State> new_state(new State);
  new_state->timestamp = timestamp;

  new_state->position = lastPos;
  new_state->orientation = lastOri;
  vector<int> identitys;
  //für alle neuen erkannten Marker und die aus den letzten Frames wird ein ResidualBLock angelegt
  int counter = 0;
  for(vector<identifiedM> ms : identifiedMarkers ){
    for(identifiedM m : ms){
     if(std::find(identitys.begin(), identitys.end(), m.identity) != identitys.end()){
        continue;
      }
      identitys.push_back(m.identity);
        problem.AddResidualBlock(OpticalFeatureConstraint::Create(m.pos,
          feature_cov, this->camera_config,
          this->feature_positions[m.identity]),
          loss_function,
          new_state->orientation.data(), new_state->position.data());
        problem.SetParameterization(new_state->orientation.data(), quat_parameterization);
        counter++;
        if(counter > 6){
          break;
        }
    }
  }

  ceres::Solver::Options solver_options;
  solver_options.max_num_iterations = 3000;
  solver_options.num_threads = 7;
  ceres::Solver::Summary summary;
  ceres::Solve(solver_options, &problem, &summary);

  //transformiert Pose der Kamera in Pose des Markers in Relation zur Kamera
  Eigen::Quaterniond cam_pose(new_state->orientation.data());
  if(sqrt(pow(0 - abs(new_state->position.x()),2) + pow(0 - abs(new_state->position.y()),2) + sqrt(pow(0 - abs(new_state->position.z()),2))) < 20){
    lastPos = new_state->position;
    lastOri = new_state->orientation;
}
  Eigen::Quaterniond tag_pose;
  tag_pose = cam_pose.inverse();
  Quaterniond resultingOri = tag_pose;
  cout << resultingOri.toRotationMatrix()<< endl;
  Eigen::Vector3d resultingPos = tag_pose._transformVector(new_state->position);
  resultingPos = resultingPos * -1;
  cout << "lastPos  " << "x:  " << lastPos.x() << "   y:  " << lastPos.y() << "   z:   " << lastPos.z() << endl;
  cout << "x:  " << resultingPos.x() << "   y:  " << resultingPos.y() << "   z:   " << resultingPos.z() << endl;
  resultingOri.normalize();
  return std::pair<Vector3d, Quaterniond>(Vector3d(resultingPos.x(),resultingPos.y(),resultingPos.z()), resultingOri);
}
