#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Size of the game board
const int BOARD_SIZE = 3;

// Structure to represent a state of the game
struct GameState {
    vector<vector<int>> tiles;
    int heuristicValue;  // Manhattan distance heuristic value

    // Constructor
    GameState(const vector<vector<int>>& t) : tiles(t) {
        heuristicValue = calculateHeuristic();
    }

    // Calculate the Manhattan distance heuristic
    int calculateHeuristic() const {
        int distance = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (tiles[i][j] != 0) {
                    int targetRow = (tiles[i][j] - 1) / BOARD_SIZE;
                    int targetCol = (tiles[i][j] - 1) % BOARD_SIZE;
                    distance += abs(i - targetRow) + abs(j - targetCol);
                }
            }
        }
        return distance;
    }

    // Check if the current state is the goal state
    bool isGoal() const {
        int targetValue = 0;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (tiles[i][j] != targetValue) {
                    return false;
                }
                ++targetValue;
            }
        }
        return true;
    }

    // Check if two game states are equal
    bool operator==(const GameState& other) const {
        return tiles == other.tiles;
    }
};

// Hash function for GameState (used for unordered_set)
struct GameStateHash {
    size_t operator()(const GameState& state) const {
        size_t hashValue = 0;
        for (const auto& row : state.tiles) {
            for (int val : row) {
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
        }
        return hashValue;
    }
};

// Compare function for priority_queue
struct CompareGameState {
    bool operator()(const GameState& lhs, const GameState& rhs) const {
        return lhs.heuristicValue > rhs.heuristicValue;
    }
};

// Function to print the game state
void printGameState(const GameState& state) {
    for (const auto& row : state.tiles) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "Heuristic: " << state.heuristicValue << endl;
    cout << "-----------------" << endl;
}

// Function to perform the greedy search
void performGreedySearch(const GameState& initial) {
    priority_queue<GameState, vector<GameState>, CompareGameState> pq;
    unordered_set<GameState, GameStateHash> visited;

    pq.push(initial);

    while (!pq.empty()) {
        GameState current = pq.top();
        pq.pop();

        if (current.isGoal()) {
            cout << "Goal state reached!" << endl;
            printGameState(current);
            return;
        }

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            printGameState(current);

            // Generate possible next states
            // In this implementation, only left, right, up, and down moves are considered
            vector<int> moves = {-1, 0, 1};

            for (int dx : moves) {
                for (int dy : moves) {
                    if (abs(dx) + abs(dy) == 1) {
                        int newX = 0, newY = 0;

                        // Find the position of the zero (empty space)
                        for (int i = 0; i < BOARD_SIZE; ++i) {
                            for (int j = 0; j < BOARD_SIZE; ++j) {
                                if (current.tiles[i][j] == 0) {
                                    newX = i + dx;
                                    newY = j + dy;
                                    break;
                                }
                            }
                        }

                        // Check if the new position is within the bounds
                        if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE) {
                            // Create a new state by swapping the zero and the adjacent tile
                            vector<vector<int>> newTiles = current.tiles;
                            swap(newTiles[newX][newY], newTiles[newX - dx][newY - dy]);

                            GameState nextState(newTiles);

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
    // Initial game state
    vector<vector<int>> initialTiles = {
        {8, 0, 6},
        {5, 4, 7},
        {2, 3, 1}
    };

    GameState initialGameState(initialTiles);

    cout << "Initial state:" << endl;
    printGameState(initialGameState);

    cout << "Starting greedy search with Manhattan distance heuristic..." << endl;
    performGreedySearch(initialGameState);

    return 0;
}

