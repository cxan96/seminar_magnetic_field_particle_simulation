#if !defined(CONDITIONS_H)
#define CONDITIONS_H

#include "vector3d.h"
#include "particle.h"
#include "propagator.h"
#include "plane3d.h"

#include<vector>

/**
 * This interface is used to specify a stopping criterion for the propagator.
 */
class Condition {
public:
  /**
   * The checking is performed here.
   *
   * @param track The current track of particles
   *
   * @return true if the stopping criterion is valid, otherwise false
   */
  virtual bool check(const std::vector<Particle> &track) const =0;
};

/**
 * A condition that tests if the current distance between the most recent particle and a reference point is greater than
 * a maximum distance.
 */
class MaximumDistanceCondition: public Condition {
public:
  MaximumDistanceCondition(double maxDistance, Vector3D referencePoint): maxDistance(maxDistance), referencePoint(referencePoint) {}
  /**
   * Returns true, if the distance is greater than the maximum distance.
   */
  bool check(const std::vector<Particle> &track) const override {
    if (track.size() == 0) {
      return false;
    }
    if ((referencePoint - track[track.size()-1].getLocation()).norm() > maxDistance) {
      return true;
    }
    return false;
  }
private:
  double maxDistance;
  Vector3D referencePoint;
};

class PlaneIntersectionCondition: public Condition {
public:
  PlaneIntersectionCondition(Plane3D plane): plane(plane) {}
  bool check(const std::vector<Particle> &track) const override {
    if (track.size() < 2) {
      return false;
    }
    Vector3D normalVector = plane.getNormalVector();
    double d = normalVector * plane.getPointInPlane();
    double t0 = d - normalVector * track[track.size()-1].getLocation();
    double t1 = d - normalVector * track[track.size()].getLocation();
    if (t0 * t1 <= 0) {
      // The sign of t0 and t1 is not the same so an intersection occured
      return true;
    }
    return false;
  }
private:
  Plane3D plane;
};

#endif
