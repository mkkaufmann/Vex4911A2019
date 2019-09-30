#include "util.hpp"

//see util.hpp

double Util::toRadians(double degrees){
  return M_PI * degrees / 180;
}

double Util::toDegrees(double radians){
  return 180 * radians / M_PI;
}

double Util::unit(double num){
  if(num >= 0){
    return 1;
  }else{
    return -1;
  }
}

double Util::largest(double arr[], int len){
  double val = -1000000000;
  for(int i = 0; i < len; i++){\
    if(arr[i] > val){
      val = arr[i];
    }
  }
  return val;
};

double Util::largestMagnitude(double arr[], int len){
  double val = 0;
  for(int i = 0; i < len; i++){
    double positive = std::abs(arr[i]);
    if(positive > val){
      val = positive;
    }
  }
  return val;
};

double Util::safeDivide(double a, double b){
  if (b == 0){
    return 0;
  }else{
    return a/b;
  }
};

double Util::map(double x, double pmin, double pmax, double min, double max){
  return (x - pmin)*(max - min)/(pmax - pmin)+min;
};

int Util::convertToVoltage(double t){
  return std::floor(map(t, -1, 1, -127, 127));
};

double Util::convertToPercent(int t){
  return map(t, -127, 127, -1, 1);
};

bool Util::epsilonEquals(double a, double b, double epsilon){
  double absA = std::abs(a);
  double absB = std::abs(b);
  double diff = std::abs(a - b);

  if (a == b) { // shortcut, handles infinities
    return true;
  } else if (a == 0 || b == 0 || (absA + absB < DBL_MIN)) {
    // a or b is zero or both are extremely close to it
    // relative error is less meaningful here
    return diff < (epsilon * DBL_MIN);
  } else { // use relative error
    return diff / std::min((absA + absB), DBL_MAX) < epsilon;
  }
};
