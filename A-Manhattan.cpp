#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Size of the grid
const int GRID_SIZE = 3;

// Structure to represent a grid state
struct GridState {
    vector<vector<int>> layout;
    int heuristic;  // Manhattan distance heuristic value
    int steps;       // Steps taken to reach this state

    // Constructor
    GridState(const vector<vector<int>>& l, int s) : layout(l), steps(s) {
        heuristic = computeHeuristic();
    }

    // Calculate the Manhattan distance heuristic
    int computeHeuristic() const {
        int totalDistance = 0;

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                int value = layout[i][j];
                if (value != 0) {
                    int targetRow = (value - 1) / GRID_SIZE;
                    int targetCol = (value - 1) % GRID_SIZE;
                    totalDistance += abs(i - targetRow) + abs(j - targetCol);
                }
            }
        }

        return totalDistance;
    }

    // Check if the current state is the goal state
    bool isGoal() const {
        int targetValue = 0;
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (layout[i][j] != targetValue) {
                    return false;
                }
                ++targetValue;
            }
        }
        return true;
    }

    // Check if two grid states are equal
    bool operator==(const GridState& other) const {
        return layout == other.layout;
    }
};

// Hash function for GridState (used for unordered_set)
struct GridStateHash {
    size_t operator()(const GridState& state) const {
        size_t hashValue = 0;
        for (const auto& row : state.layout) {
            for (int val : row) {
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
        }
        return hashValue;
    }
};

// Compare function for priority_queue
struct CompareGridState {
    bool operator()(const GridState& lhs, const GridState& rhs) const {
        return (lhs.steps + lhs.heuristic) > (rhs.steps + rhs.heuristic);
    }
};

// Function to print the grid state
void printGridState(const GridState& state) {
    for (const auto& row : state.layout) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "Heuristic: " << state.heuristic << " Steps: " << state.steps << endl;
    cout << "-----------------" << endl;
}

// Function to perform the A* search with Manhattan distance heuristic
void aStarSearch(const GridState& initial) {
    priority_queue<GridState, vector<GridState>, CompareGridState> pq;
    unordered_set<GridState, GridStateHash> visited;

    pq.push(initial);

    while (!pq.empty()) {
        GridState current = pq.top();
        pq.pop();

        if (current.isGoal()) {
            cout << "Goal state reached!" << endl;
            printGridState(current);
            return;
        }

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            printGridState(current);

            // Generate possible next states
            vector<int> moves = {-1, 0, 1};

            for (int dx : moves) {
                for (int dy : moves) {
                    if (abs(dx) + abs(dy) == 1) {
                        int newX = 0, newY = 0;

                        // Find the position of the zero (empty space)
                        for (int i = 0; i < GRID_SIZE; ++i) {
                            for (int j = 0; j < GRID_SIZE; ++j) {
                                if (current.layout[i][j] == 0) {
                                    newX = i + dx;
                                    newY = j + dy;
                                    break;
                                }
                            }
                        }

                        // Check if the new position is within the bounds
                        if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
                            // Create a new state by swapping the zero and the adjacent tile
                            vector<vector<int>> newLayout = current.layout;
                            swap(newLayout[newX][newY], newLayout[newX - dx][newY - dy]);

                            GridState nextState(newLayout, current.steps + 1);

                            // Add the new state to the priority queue
                            pq.push(nextState);
                        }
                    }
                }
            }
        }
    }

    cout << "Goal state not reachable!" << endl;
}

int main() {
    // Initial grid state
    vector<vector<int>> initialLayout = {
        {8, 0, 6},
        {5, 4, 7},
        {2, 3, 1}
    };

    GridState initialGridState(initialLayout, 0);

    cout << "Initial state:" << endl;
    printGridState(initialGridState);

    cout << "Starting A* search with Manhattan distance heuristic..." << endl;
    aStarSearch(initialGridState);

    return 0;
}
 

