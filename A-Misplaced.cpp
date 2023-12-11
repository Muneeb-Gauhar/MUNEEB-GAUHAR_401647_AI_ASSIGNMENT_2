#include <bits/stdc++.h> 
#include <chrono> 
 
using namespace std; 
using namespace chrono; 
 
#define MATRIX_SIZE 3 
int moves = 0; 
int rows[] = {1, 0, -1, 0}; 
int columns[] = {0, -1, 0, 1}; 
 
int targetMatrix[MATRIX_SIZE][MATRIX_SIZE] = { 
        {0, 1, 2}, 
        {3, 4, 5}, 
        {6, 7, 8} 
}; 
 
struct Tile { 
    Tile *ancestor; 
    int layout[MATRIX_SIZE][MATRIX_SIZE]; 
    int x, y; 
    int cost; 
    int level; 
}; 
 
int displayMatrix(int layout[MATRIX_SIZE][MATRIX_SIZE]) { 
    for (int i = 0; i < MATRIX_SIZE; i++) { 
        for (int j = 0; j < MATRIX_SIZE; j++) 
            printf("%d ", layout[i][j]); 
        printf("\n"); 
    } 
    return 0; 
} 
 
Tile *createTile(int layout[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int newX, int newY, int level, Tile *ancestor) { 
    Tile *node = new Tile; 
    node->ancestor = ancestor; 
 
    for (int i = 0; i < MATRIX_SIZE; i++) { 
        for (int j = 0; j < MATRIX_SIZE; j++) { 
            node->layout[i][j] = layout[i][j]; 
        } 
    } 
 
    int temp = node->layout[x][y]; 
    node->layout[x][y] = node->layout[newX][newY]; 
    node->layout[newX][newY] = temp; 
    node->cost = INT_MAX; 
    node->level = level; 
    node->x = newX; 
    node->y = newY; 
    return node; 
} 
 
int computeCost(int layout[MATRIX_SIZE][MATRIX_SIZE], int target[MATRIX_SIZE][MATRIX_SIZE]) { 
    int count = 0; 
    for (int i = 0; i < MATRIX_SIZE; i++) 
        for (int j = 0; j < MATRIX_SIZE; j++) 
            if (layout[i][j] != target[i][j]) { 
                count++; 
            } 
    return count; 
} 
 
int checkBounds(int x, int y) { 
    return (x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE); 
} 
 
void showMatrix(Tile *root) { 
    if (root == NULL) 
        return; 
    showMatrix(root->ancestor); 
    displayMatrix(root->layout); 
    printf("\n"); 
} 
 
struct comparison { 
    bool operator()(const Tile *lhs, const Tile *rhs) const { 
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level); 
    } 
}; 
 
// Function to convert the matrix into a string 
std::string stringifyMatrix(int layout[MATRIX_SIZE][MATRIX_SIZE]) { 
    std::ostringstream oss; 
    for (int i = 0; i < MATRIX_SIZE; i++) { 
        for (int j = 0; j < MATRIX_SIZE; j++) { 
            oss << layout[i][j] << " "; 
        } 
    } 
    return oss.str(); 
} 
 
void solvePuzzle(int initial[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int target[MATRIX_SIZE][MATRIX_SIZE]) { 
    auto start_time = high_resolution_clock::now(); // Record the start time 
 
    priority_queue<Tile *, vector<Tile *>, comparison> pq; 
 
    unordered_set<string> visitedStates; // Keep track of visited states 
 
    Tile *root = createTile(initial, x, y, x, y, 0, NULL); 
    root->cost = computeCost(initial, target); 
    pq.push(root); 
 
    while (!pq.empty()) { 
        Tile *min = pq.top(); 
        pq.pop(); 
 
        if (min->cost == 0) { 
            auto end_time = high_resolution_clock::now(); // Record the end time 
            auto duration = duration_cast<milliseconds>(end_time - start_time); 
 
            cout << "Solved state reached in " << moves << " moves.\n"; 
            cout << "Time taken: " << duration.count() << " milliseconds\n"; 
            showMatrix(min); 
            return; 
        } 
 
        for (int i = 0; i < 4; i++) { 
            if (checkBounds(min->x + rows[i], min->y + columns[i])) { 
                Tile *child = createTile(min->layout, min->x, min->y, min->x + rows[i], min->y + columns[i], min->level + 1, min); 
                child->cost = computeCost(child->layout, target); 
 
                if (visitedStates.find(stringifyMatrix(child->layout)) == visitedStates.end()) { 
                    pq.push(child); 
                    visitedStates.insert(stringifyMatrix(child->layout)); 
                    moves++; 
                } 
            } 
        } 
    } 
} 
 
int main() { 
 
    int initial[MATRIX_SIZE][MATRIX_SIZE]; 
 
    cout << "\n\t\t----------------------------------------------------------------------------\n"; 
    cout << " Enter the starting state of puzzle in undermentioned form \n"; 
    cout << "*** 2 3 1 5 6 0 8 4 7 ***\n>> "; 
 
    for(int i=0;i<3;i++) 
        for(int j=0;j<3;j++) 
            cin>>initial[i][j]; 
 
    cout << "Starting form of the puzzle is:  \n>> "; 
    displayMatrix(initial); 
    cout << "\n\t\t----------------------------------------------------------------------------\n"; 
    cout << "Solving the puzzle, please wait  \n>> "; 
 
    int x = 1, y = 2; 
 
    solvePuzzle(initial, x, y, targetMatrix); 
 
    return 0; 
} 

