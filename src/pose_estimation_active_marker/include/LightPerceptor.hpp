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
#include "../../light_beacon_pose_estimation_lib/detector.cpp"
#include "pose_estimation_active_markers/light.h"
#include "pose_estimation_active_markers/lightContainer.h"
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/dictionary.hpp>


ros::Publisher pub;
ros::Publisher pubInfo;
ros::Publisher pubImage;
sensor_msgs::CameraInfo cinfo;
void createKnownBoardPos(Size boardSize, float squareEdgeLength, vector<Point3f>& corners);
void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoefficience);
void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>>& allFoundCorners);
class LightPerceptor
{
private:
public:
  void saveCameraInfo(const sensor_msgs::CameraInfo::ConstPtr& infoMsg);
  void saveImage(const sensor_msgs::Image::ConstPtr& imageMsg);
  pose_estimation_active_markers::lightContainer lightContainer;
  void testingGetLightsWithPicture();
};
