#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
  std::vector<Point2D> path = generatePath(world);
  auto pos = world->getCat();

  if (!path.empty()) {
    Point2D nextPos = path.back();

    // Check if the next position is blocked
    if (!world->getContent(nextPos)) {
      // If not blocked, move to the next position
      return nextPos;
    }
  }

  // Not making this better
  for (int attempt = 0; attempt < 6; ++attempt) {
    auto rand = Random::Range(0, 5);
    Point2D newPos;

    switch (rand) {
      case 0: newPos = World::NE(pos); break;
      case 1: newPos = World::NW(pos); break;
      case 2: newPos = World::E(pos); break;
      case 3: newPos = World::W(pos); break;
      case 4: newPos = World::SW(pos); break;
      case 5: newPos = World::SE(pos); break;
      default: throw std::out_of_range("Random number out of range");
    }

    // Check if the new position is valid
    if (world->isValidPosition(newPos) && !world->getContent(newPos)) {
      // If the new position is valid and not blocked, move there
      return newPos;
    }
  }

  // If all attempts fail, return the current position to indicate no movement
  return pos; // or you could throw an error if you prefer
}
