//
// Created by atolstenko on 2/9/2023.
//

#include "HexagonGameOfLife.h"
#include <iostream>

void HexagonGameOfLife::Step(World& world) {
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
  std::cout<<"HexagonGameOfLife::Step has run"<<std::endl;
  world.SwapBuffers();
}
int HexagonGameOfLife::CountNeighbors(World& world, Point2D point) {
  int count = 0;
  int sideSize = world.SideSize();

  Point2D offsets[6] = {
    {0, -1} , {1, 0}, // top
    {-1, 0} , {1, 0}, // middle
    {-1, 1} , {0, 1} // bottom
    };
  for (int i = 0; i < 6; i++) {
    Point2D neighbor((point.x + offsets[i].x + sideSize) % sideSize, (point.y + offsets[i].y + sideSize) % sideSize);
    if (world.Get(neighbor))
      count++;
  }
  // std::cout << "CountNeighbors: Point(" << point.x << ", " << point.y << ") has " << count << " neighbors." << std::endl;
  return count;
  }

