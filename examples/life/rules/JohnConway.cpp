#include "JohnConway.h"
#include <iostream>

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  int sideSize = world.SideSize();
  for (int y = 0;  y < sideSize; y++) {
    for (int x = 0; x < sideSize; x++) {
      Point2D point(x, y);
      int aliveNeighbors = CountNeighbors(world, point);
      bool isAlive = world.Get(point);
      if (isAlive) {
        if ((aliveNeighbors == 2) || (aliveNeighbors == 3))
          world.SetNext(point, true);
        else
          world.SetNext(point, false);
      } else if (!isAlive) {
        if (aliveNeighbors == 3)
          world.SetNext(point, true);
        else
          world.SetNext(point, false);

      }
    }
  }
  std::cout<<"JohnConway::Step has run"<<std::endl;
  world.SwapBuffers();
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  int count = 0;
  int sideSize = world.SideSize();
  for (int dy = -1; dy < 2; dy++) {
    for (int dx = -1; dx < 2; dx++) {
      if (dx == 0 && dy == 0)
        continue;
      Point2D neighbor((point.x + dx + sideSize) % sideSize, (point.y + dy + sideSize) % sideSize);
      if (world.Get(neighbor))
        count++;
    }
  }
 // std::cout << "CountNeighbors: Point(" << point.x << ", " << point.y << ") has " << count << " neighbors." << std::endl;
  return count;
}
