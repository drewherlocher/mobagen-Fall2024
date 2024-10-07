#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>

bool RecursiveBacktrackerExample::Step(World* w) {
  // If the stack is empty, start a new maze generation
  if (stack.empty()) {
    // Get a random start point
      Point2D startPoint = randomStartPoint(w);
    if (startPoint.x == INT_MAX && startPoint.y == INT_MAX)
      return false; // Maze is done

    visited[startPoint.y][startPoint.x] = true;
    stack.push_back(startPoint);
    w->SetNodeColor(startPoint, Color::Red);
  }

  // Get the current position from the stack
  Point2D current = stack.back();

  // Get unvisited neighbors
  auto visitables = getVisitables(w, current);

  w->SetNodeColor(current, Color::Red);

  // If there are visitable neighbors, choose one to move to
  if (!visitables.empty()) {
    Point2D next = visitables[Random::Range(0, visitables.size() - 1)];

    // Remove the wall between current and next point
    setWall(w, current, next);
    w->SetNodeColor(next, Color::Green);
    // Mark the next point as visited and push it onto the stack
    visited[next.y][next.x] = true;
    stack.push_back(next);
  } else {
    w->SetNodeColor(current, Color::Black);
    // If no neighbors, backtrack
    stack.pop_back();

  }

  return true; // Continue the process on the next call
}



void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  std::vector<Point2D> visitables;

  for (const auto& delta : deltas) {
    Point2D neighbor = {p.x + delta.x, p.y + delta.y};
    if (neighbor.x >= -sideOver2 && neighbor.x <= sideOver2 &&
        neighbor.y >= -sideOver2 && neighbor.y <= sideOver2 &&
        !visited[neighbor.y][neighbor.x]) {
      visitables.push_back(neighbor);
        }
  }
  std::cout << "Current visitables: " << visitables.size() << std::endl;
  return visitables;
}

void RecursiveBacktrackerExample::setWall(World* world, const Point2D& point, const Point2D& nextPoint) {
  // Set walls based on the direction of the movement
  if (nextPoint.x < point.x) {
    // Move left
    world->SetWest(point, false);
  } else if (nextPoint.x > point.x) {
    // Move right
    world->SetEast(point, false);
  } else if (nextPoint.y < point.y) {
    // Move up
    world->SetNorth(point, false);
  } else if (nextPoint.y > point.y) {
    // Move down
    world->SetSouth(point, false);
  }
}
