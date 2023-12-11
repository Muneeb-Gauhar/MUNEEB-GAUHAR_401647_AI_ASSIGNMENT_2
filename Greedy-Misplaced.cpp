#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Dimension of the puzzle
const int PuzzleSize = 3;

// Structure representing the puzzle's state
struct GameState {
    vector<vector<int>> gameBoard;
    int heuristicValue;  // Evaluation based on misplaced tiles

    // Constructor
    GameState(const vector<vector<int>>& board) : gameBoard(board) {
        heuristicValue = calculateHeuristic();
    }

    // Calculating heuristic based on misplaced tiles
    int calculateHeuristic() const {
        int misplacedTiles = 0;
        int target = 0;

        for (int i = 0; i < PuzzleSize; ++i) {
            for (int j = 0; j < PuzzleSize; ++j) {
                if (gameBoard[i][j] != target) {
                    ++misplacedTiles;
                }
                ++target;
            }
        }

        return misplacedTiles;
    }

    // Checking if the current state is the goal state
    bool isGoalState() const {
        int target = 0;
        for (int i = 0; i < PuzzleSize; ++i) {
            for (int j = 0; j < PuzzleSize; ++j) {
                if (gameBoard[i][j] != target) {
                    return false;
                }
                ++target;
            }
        }
        return true;
    }

    // Comparison for equality between two game states
    bool operator==(const GameState& other) const {
        return gameBoard == other.gameBoard;
    }
};

// Hash function for GameState (used in unordered_set)
struct GameStateHash {
    size_t operator()(const GameState& state) const {
        size_t hashValue = 0;
        for (const auto& row : state.gameBoard) {
            for (int val : row) {
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
        }
        return hashValue;
    }
};

// Comparison function for priority_queue
struct CompareGameState {
    bool operator()(const GameState& lhs, const GameState& rhs) const {
        return lhs.heuristicValue > rhs.heuristicValue;
    }
};

// Function to display the puzzle's state
void displayGameState(const GameState& state) {
    for (const auto& row : state.gameBoard) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "Heuristic: " << state.heuristicValue << endl;
    cout << "-----------------" << endl;
}

// Function to perform a greedy search
void performGreedySearch(const GameState& initial) {
    priority_queue<GameState, vector<GameState>, CompareGameState> priorityQueue;
    unordered_set<GameState, GameStateHash> visitedStates;

    priorityQueue.push(initial);

    while (!priorityQueue.empty()) {
        GameState currentState = priorityQueue.top();
        priorityQueue.pop();

        if (currentState.isGoalState()) {
            cout << "Reached the goal state!" << endl;
            displayGameState(currentState);
            return;
        }

        if (visitedStates.find(currentState) == visitedStates.end()) {
            visitedStates.insert(currentState);
            displayGameState(currentState);

            // Generating possible next states
            vector<int> moves = {-1, 0, 1};

            for (int dx : moves) {
                for (int dy : moves) {
                    if (abs(dx) + abs(dy) == 1) {
                        int newX = 0, newY = 0;

                        // Finding the position of the zero (empty space)
                        for (int i = 0; i < PuzzleSize; ++i) {
                            for (int j = 0; j < PuzzleSize; ++j) {
                                if (currentState.gameBoard[i][j] == 0) {
                                    newX = i + dx;
                                    newY = j + dy;
                                    break;
                                }
                            }
                        }

                        // Checking if the new position is within the bounds
                        if (newX >= 0 && newX < PuzzleSize && newY >= 0 && newY < PuzzleSize) {
                            // Creating a new state by swapping the zero and the adjacent tile
                            vector<vector<int>> newBoard = currentState.gameBoard;
                            swap(newBoard[newX][newY], newBoard[newX - dx][newY - dy]);

                            GameState nextState(newBoard);

                            // Adding the new state to the priority queue
                            priorityQueue.push(nextState);
                        }
                    }
                }
            }
        }
    }

    cout << "The goal state is not reachable!" << endl;
}

int main() {
    // Initial state of the puzzle
    vector<vector<int>> initialPuzzle = {
        {8, 0, 6},
        {5, 4, 7},
        {2, 3, 1}
    };

    GameState initialGameState(initialPuzzle);

    cout << "Initial state:" << endl;
    displayGameState(initialGameState);

    cout << "Starting the greedy search..." << endl;
    performGreedySearch(initialGameState);

    return 0;
}

