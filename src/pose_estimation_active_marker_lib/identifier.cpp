#include "./identifier.h"


using namespace cv;
using namespace std;

Identifier::Identifier(){
  m0.identity = 3;
  m0.onPeriod = 0.125;
  m0.offPeriod = 0.075;
  m1.identity = 2;
  m1.onPeriod = 0.225;
  m1.offPeriod = 0.175;
  m2.identity = 4;
  m2.onPeriod = 0.125;
  m2.offPeriod = 0.175;
  m3.identity = 1;
  m3.onPeriod = 0.225;
  m3.offPeriod = 0.075;
  m4.identity = 0;
  m4.onPeriod = 0.325;
  m4.offPeriod = 0.075;
  m5.identity = 5;
  m5.onPeriod = 0.125;
  m5.offPeriod = 0.275;
  realMarkers.push_back(m0);
  realMarkers.push_back(m1);
  realMarkers.push_back(m2);
  realMarkers.push_back(m3);
  realMarkers.push_back(m4);
  realMarkers.push_back(m5);
}

/**
* Funktion, die identifizierte Marker ausgibt
*
* @param newLights Liste von erkannten Lichtern im Bilds
* @param timestamp Zeitpunkt der Aufnahme
* @param img Aufnahme
* @return Vector von identifizierten Markern
*/
vector<Marker> Identifier::getIdentifiedMarker(vector<Marker> &newLights, long double timestamp, Mat &img){
  float time = 0.05;
  vector<Marker> result;
  for(Marker& m : markers){
    m.timeSinceLastSeen += time;
  }
  for(Marker& newM : newLights){
    for(Marker& oldM : markers){
      int i = sqrt(pow(abs(newM.x) - abs(oldM.x),2) + pow(abs(newM.y) - abs(oldM.y),2));
      if(i <= 30 && newM.newMarker == true) {
        this->updateSeenMarker(newM, oldM);
        break;
      }
    }
    if(newM.newMarker){
      newM.timeSinceLastSeen = time;
      newM.onAndOffStates.insert(newM.onAndOffStates.begin(), true);
      if(newM.onAndOffStates.size() > 30){
        newM.onAndOffStates.erase(newM.onAndOffStates.begin() + 40);
      }
      markers.push_back(newM);
    }
  }
  for(Marker& m : markers){
    if(m.timeSinceLastSeen != time){
    circle(img, cv::Point(m.x,m.y), 10, Scalar(0,100,0), 5);
  }
  else{
    circle(img, cv::Point(m.x,m.y), 10, Scalar(100,0,0), 5);
  }
    if(m.onAndOffStates.size() > 40){
      m.onAndOffStates.erase(m.onAndOffStates.begin() + 40);
    }
    if(m.timeSinceLastSeen != time){
      m.onAndOffStates.insert(m.onAndOffStates.begin(), false);
    }
  }
  for(int i = 0; i < markers.size(); i++){
    if(markers[i].timeSinceLastSeen >= 1){
      markers.erase(markers.begin() + i);
      i--;
    }
  }
  std::sort(markers.begin(), markers.end(), less_than_key());
  for(MarkerType& realM : realMarkers){
    for(Marker& m : markers){
        if(this->markerIsRealMarker(realM, m)){
          m.identity = realM.identity;
          if(m.identity == 0)
            circle(img, cv::Point(m.x,m.y), 10, Scalar(255,0,0), 5);
          else if(m.identity == 1)
            circle(img, cv::Point(m.x,m.y), 15, Scalar(0,255,0), 5);
          else if(m.identity == 2)
            circle(img, cv::Point(m.x,m.y), 20, Scalar(0,0,255), 5);
          else if(m.identity == 3)
            circle(img, cv::Point(m.x,m.y), 22, Scalar(0,255,255), 5);
            else if(m.identity == 4)
              circle(img, cv::Point(m.x,m.y), 22, Scalar(255,0,255), 5);
            else
              circle(img, cv::Point(m.x,m.y), 30, Scalar(0,100,0), 5);

            result.push_back(m);
          break;
        }
    }
  }
  cv::String windowName = "test";
   namedWindow(windowName);
   imshow(windowName,img);
   waitKey(10);
  return result;
}

void Identifier::updateSeenMarker(Marker& newM, Marker& oldM){
  oldM.x = newM.x;
  oldM.y = newM.y;
  oldM.size = (newM.size +oldM.size*2)/3;
  oldM.intensity = newM.intensity;
  oldM.timeSinceLastSeen = 0.05;
  oldM.onAndOffStates.insert(oldM.onAndOffStates.begin(), true);
  newM.newMarker = false;
}

void Identifier::getMedian(vector<bool>& states, bool on, double& var, double& median){
  vector<double> periods = vector<double>();
  double period = 0;
  bool first = true;
  bool oneFalse = true;
  for(bool b : states){
    if(period != 0 && b != on){
      if(oneFalse == true){
        oneFalse = true;
        if(!first){
          periods.push_back(period);
        }
        else{
          first = false;
        }
        period = 0;
      }
      else{
        oneFalse = true;
      }
    }
    else if(b == on){
      period += 0.05;
    }
  }
  if(states[0] != on && period != 0){
    periods.push_back(period);
  }
  sort(periods.begin(), periods.end());
  if(periods.size() >= 1){
      if( periods.size() % 2 == 0 ) {
        median = 0.5 * (periods[periods.size() / 2 - 1] + periods[periods.size() / 2]);
      }
      else {
        median = periods[periods.size() / 2];
      }
  }
  else{
    median = 0;
  }
  double middle = 0;
  for(double o : periods){
      middle += o;
  }
  if(middle != 0)
    middle = middle / periods.size();
  var = 0;
  int counter = 0;
  for(double o : periods){
      var += pow((o - middle),2);
      counter++;
  }
  var = var / counter;
}

bool Identifier::markerIsRealMarker(MarkerType& realM, Marker& candidate){
  double onPeriodMean, offPeriodMean, varOn, varOff = 0;
  this->getMedian(candidate.onAndOffStates, true, varOn, onPeriodMean);
  this->getMedian(candidate.onAndOffStates, false, varOff, offPeriodMean);
  float expected = realM.onPeriod / (realM.offPeriod + realM.onPeriod);
  float observed = onPeriodMean / (onPeriodMean + offPeriodMean);
  float score = 1 - (abs(expected- observed) / expected);
  if(abs(onPeriodMean - realM.onPeriod) < 0.026 &&
  abs(offPeriodMean - realM.offPeriod) < 0.026 && abs(onPeriodMean - realM.onPeriod) < 0.04 && onPeriodMean != 0 && offPeriodMean != 0 && candidate.timeSinceLastSeen < 0.8 && varOn < 0.044 && varOff < 0.044){
    return true;
  }
  return false;
}
