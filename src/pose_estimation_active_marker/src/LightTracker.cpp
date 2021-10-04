#include "../include/LightTracker.h"

using namespace cv;
using namespace std;

/**
 * callback function für lightContainer-Msg, ruft identifier.cpp auf um gefundene Lichter zu identifizieren.
 * identifizierte Lichter werden dann gepublished
 *
 * @param msg lightContainer
 */
void LightTracker::saveLights(const pose_estimation_active_markers::lightContainer::ConstPtr& msg)
{
  Identifier i;
  cv_bridge::CvImagePtr cv_ptr;
  cv_ptr = cv_bridge::toCvCopy(msg->img, sensor_msgs::image_encodings::MONO8);
  Mat img = cv_ptr->image;
  cvtColor(img, img, CV_GRAY2BGR);
  //fügt Lichter aus msg in eine Liste aus Marker
  vector<Marker> newLights;
  for(int i = 0; i < msg->lights.size(); i++){
    Marker mark;
    mark.x = msg->lights[i].x;
    mark.y = msg->lights[i].y;
    mark.intensity = msg->lights[i].intensity;
    mark.size = msg->lights[i].radius;
    newLights.push_back(mark);
  }
  vector<Marker> markers = i.getIdentifiedMarker(newLights, msg->timestamp, img);
  cout << "6" << endl;
  //fügt identifizierte Lichter in einen Container um sie zu publishen
  if(markers.size() >= 3){
    pose_estimation_active_markers::identifiedLightContainer lightC;
    for(Marker& m : markers){
      circle(img, cv::Point(m.x,m.y), m.size, Scalar(255,0,0), 2);
      pose_estimation_active_markers::identifiedLight l;
      l.x = m.x;
      l.y = m.y;
      l.intensity = m.intensity;
      l.radius = m.size;
      l.identity = m.identity;
      l.certainty = m.timesOff;
      lightC.lights.push_back(l);
    }
    lightC.timestamp = msg->timestamp;
    pubIdentifiedLights.publish(lightC);
  }

  cout << "identifiedMarker:" << markers.size() << endl;
}

int main (int argc, char **argv)
{
  ros::init(argc, argv, "LightTracker");
  ros::NodeHandle n;
  LightTracker lt;
  ros::Subscriber sub = n.subscribe("detected_lights", 1000, &LightTracker::saveLights, &lt);
  pubIdentifiedLights = n.advertise<pose_estimation_active_markers::identifiedLightContainer>("identified_lights", 1000);
  ros::Rate loop_rate(10);

  while(ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 1;
}
