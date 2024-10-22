    #include "Agent.h"
    #include <unordered_set>
    #include <unordered_map>
    #include <queue>
    #include "World.h"

    #include <algorithm>
    #include <iostream> // Include for debugging output

    using namespace std;

    struct AStarNode {
      Point2D point;
      int accumulatedDistance; // gCost
      int heuristicDistance;   // hCost

      bool operator<(const AStarNode &other) const {
        return accumulatedDistance + heuristicDistance > other.accumulatedDistance + other.heuristicDistance;
      }

      // Constructor
      AStarNode(Point2D p, int g, int h) : point(p), accumulatedDistance(g), heuristicDistance(h) {}

      // New heuristic method to calculate the distance to the border
      static int heuristic(const Point2D &p, int sideOver2) {
        // Correct distance calculation
        int distanceToRight = sideOver2 - p.x;
        int distanceToLeft = p.x + sideOver2;
        int distanceToTop = sideOver2 - p.y;
        int distanceToBottom = p.y + sideOver2;

        // Find the minimum distance to any of the borders
        int minDistance = std::min({distanceToRight, distanceToLeft, distanceToTop, distanceToBottom});

        std::cout << "Current Distance to border: " << minDistance << std::endl;
        return minDistance;
      }
    };

    std::vector<Point2D> Agent::generatePath(World* w) {
    priority_queue<AStarNode> frontier;
    unordered_map<Point2D, Point2D> cameFrom;
    unordered_set<Point2D> frontierSet;
    unordered_map<Point2D, bool> visited;

    auto sideOver2 = w->getWorldSideSize() / 2;
    auto catPos = w->getCat();
    int initialHeuristic = AStarNode::heuristic(catPos, sideOver2);
    AStarNode startNode(catPos, 0, initialHeuristic);

    frontier.push(startNode);
    frontierSet.insert(catPos);
    Point2D borderExit = Point2D::INFINITE;

    while (!frontier.empty()) {
        AStarNode currentNode = frontier.top();
        frontier.pop();
        Point2D currentPoint = currentNode.point;
        frontierSet.erase(currentPoint);

        visited[currentPoint] = true;

        // Check if the current point is on the border
        if (currentPoint.x == sideOver2 || currentPoint.x == -sideOver2 ||
            currentPoint.y == sideOver2 || currentPoint.y == -sideOver2) {
            borderExit = currentPoint;
            break;
        }

        // Get valid neighbors
        vector<Point2D> allNeighbors = getNeighbors(w, currentPoint);

        for (auto neighbor : allNeighbors) {
            if (!visited[neighbor] && !frontierSet.count(neighbor)) {
                cameFrom[neighbor] = currentPoint;
                int newAccumulatedDistance = currentNode.accumulatedDistance + 1;
                int heuristicCost = AStarNode::heuristic(neighbor, sideOver2);
                AStarNode neighborNode(neighbor, newAccumulatedDistance, heuristicCost);

                frontier.push(neighborNode);
                frontierSet.insert(neighbor);
            }
        }
    }

    // Construct the path
    vector<Point2D> path;
    if (borderExit != Point2D::INFINITE) {
        Point2D currentPoint = borderExit;
        while (currentPoint != catPos) {
            path.push_back(currentPoint);
            currentPoint = cameFrom[currentPoint];
        }
    }
    // Ensure we return the path
    return path; // Add this to avoid the control reaches end of non-void function warning
}


    std::vector<Point2D> Agent::getNeighbors(World* w, const Point2D &p) {
        std::vector<Point2D> neighbors;

        std::vector<Point2D> offsets = {
            w->NW(p), w->NE(p), w->E(p),
            w->SE(p), w->SW(p), w->W(p)
        };

        for (const auto& neighbor : offsets) {
            if (neighbor != w->getCat() && w->isValidPosition(neighbor)) {
                neighbors.push_back(neighbor);
            }
        }

        return neighbors;
    }
