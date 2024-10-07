#include "HuntAndKillExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>

bool HuntAndKillExample::Step(World* w) {
    // Start the hunt phase if the stack is empty
    if (stack.empty()) {
        // Get a random start point
        Point2D startPoint = randomStartPoint(w);
        std::cout << "Starting at ( " << startPoint.x << " , " << startPoint.y << " )" << std::endl;

        // Check if there are more cells to be visited
        if (startPoint.x == INT_MAX && startPoint.y == INT_MAX)
            return false;

        // Push unvisited cell onto the stack
        stack.push_back(startPoint);
        // Mark the cell as visited
        visited[startPoint.y][startPoint.x] = true;

        // Get visited neighbors
        auto neighbors = getVisitedNeighbors(w, startPoint);
        for (const auto& neighbor : neighbors) {
            setWall(w, startPoint, neighbor);
        }
    }

    // Get the current position from the stack
    Point2D current = stack.back();
    // Check neighboring cells to see if unvisited
    auto visitables = getVisitables(w, current);

      if (!visitables.empty()) {
          Point2D next = visitables[Random::Range(0, visitables.size() - 1)];
          setWall(w, current, next);
          visited[next.y][next.x] = true;
          stack.push_back(next);
      } else {
          stack.pop_back();
      }

    // Move on to the next step
    std::cout << "Taking a Step to: " << current.x << ", " << current.y << std::endl;
    return true;
}

void HuntAndKillExample::Clear(World* world) {
    // Clear visited cells and stack
    visited.clear();
    stack.clear();
    auto sideOver2 = world->GetSize() / 2;

    // Mark all cells as unvisited
    for (int i = -sideOver2; i <= sideOver2; i++) {
        for (int j = -sideOver2; j <= sideOver2; j++) {
            visited[i][j] = false;
        }
    }
}

Point2D HuntAndKillExample::randomStartPoint(World* world) {
    auto sideOver2 = world->GetSize() / 2;

    for (int y = -sideOver2; y <= sideOver2; y++) {
        for (int x = -sideOver2; x <= sideOver2; x++) {
            if (!visited[y][x]) return {x, y};
        }
    }
    return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKillExample::getVisitables(World* w, const Point2D& p) {
    std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    std::vector<Point2D> visitables;
    auto sideOver2 = w->GetSize() / 2;

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

std::vector<Point2D> HuntAndKillExample::getVisitedNeighbors(World* w, const Point2D& p) {
    std::vector<Point2D> deltas = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
    std::vector<Point2D> neighbors;
    auto sideOver2 = w->GetSize() / 2;

    for (const auto& delta : deltas) {
        Point2D neighbor = {p.x + delta.x, p.y + delta.y};

        if (neighbor.x >= -sideOver2 && neighbor.x <= sideOver2 &&
            neighbor.y >= -sideOver2 && neighbor.y <= sideOver2 &&
            visited[neighbor.y][neighbor.x]) {
            neighbors.push_back(neighbor);
        }
    }
    std::cout << "Current neighbors: " << neighbors.size() << std::endl;
    return neighbors;
}

void HuntAndKillExample::setWall(World* world, const Point2D& point, const Point2D& nextPoint) {
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
