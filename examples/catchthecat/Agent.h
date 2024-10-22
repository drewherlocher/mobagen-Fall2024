#ifndef AGENT_H
#define AGENT_H
#include "math/Point2D.h"
#include <vector>

class World;

class Agent {
public:
  explicit Agent() = default;

  virtual Point2D Move(World*) = 0;

  std::vector<Point2D> generatePath(World* w);
  std::vector<Point2D> getNeighbors(World *w, const Point2D &p);

  int heuristic(const Point2D &a, const Point2D &b);
private:

};

#endif  // AGENT_H
