#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
  auto side = world->getWorldSideSize() / 2;
  std::vector<Point2D> path = generatePath(world);

  // Check if there is a valid path
  if (!path.empty()) {
    Point2D nextPos = path.back();

    // If the next position is not blocked, move there
    if (!world->getContent(nextPos)) {
      return nextPos;
    }
  }

  // If the path is empty or the next position is blocked, try random moves
  for (;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    auto cat = world->getCat();

    // Ensure the catcher does not move to the cat's position and the position is not blocked
    if ((cat.x != p.x || cat.y != p.y) && !world->getContent(p)) {
      return p;
    }
  }
}
