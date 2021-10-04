#include <iostream>
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <bits/stdc++.h>
#include <stdlib.h>
#include <vector>

struct Marker{
  int identity = -1;
  std::vector<bool> onAndOffStates;
  double x;
  double y;
  double size;
  double intensity;
  float timeSinceLastSeen;
  int timesOff = 0;
  bool newMarker = true;
  float slope;
  float r2;
};
struct identifiedMarker{
  int identity;
  int x;
  int y;
};
struct MarkerType{
  int identity;
  float onPeriod;
  float offPeriod;
};

struct less_than_key
{
    inline bool operator() (const Marker& m1, const Marker& m2)
    {
        return (m1.size > m2.size);
    }
};


std::vector<Marker> markers;

class Identifier
{
private:
public:
  MarkerType m0, m1, m2, m3, m4,m5;
  std::vector<MarkerType> realMarkers;
  Identifier();
  std::vector<Marker> getIdentifiedMarker(std::vector<Marker>& newLights, long double timestamp, cv::Mat &img);
  void updateSeenMarker(Marker& newM, Marker& oldM);
  void getMedian(std::vector<bool>& states, bool on, double& var,double& median);
  bool markerIsRealMarker(MarkerType& realM, Marker& candidate);
};
