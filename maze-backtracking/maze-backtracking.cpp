#include <array>
#include <iostream>
#include <thread>

using namespace std;

#define DEBUG

// maze size
const auto rowCount = 6;
const auto colCount = 5;
using maze_t = array<array<int8_t, colCount>, rowCount>;

void printMaze(maze_t &maze) {
    for (auto x : maze) {
        for (auto y : x) {
            cout << (y ? '+' : 'o') << "  ";
        }
        cout << endl;
    }
}

bool solve(maze_t &maze, maze_t &solution, int x, int y) {
    if (x >= 0 && x <= colCount - 1 && y >= 0 && y <= rowCount - 1 &&
        maze[y][x] == 1 && solution[y][x] != 1) {
        // Mark (x,y) as part of the solution path.
        solution[y][x] = 1;

#ifdef DEBUG
        printMaze(solution);
        this_thread::sleep_for(chrono::milliseconds(500));
        system("cls");
#endif // DEBUG

        if (x == 0 || x == colCount - 1 || y == 0 ||
            y == rowCount - 1) // found an exit
            return true;
        // Move Up
        if (solve(maze, solution, x, y - 1))
            return true;
        // Move Right
        if (solve(maze, solution, x + 1, y))
            return true;
        // Move Down
        if (solve(maze, solution, x, y + 1))
            return true;
        // Move Left
        if (solve(maze, solution, x - 1, y))
            return true;

        // If none of the above worked, backtrack and unmark (x,y).
        solution[y][x] = 0;
    }

    return false;
}

void Solve(maze_t &maze, int startX, int startY) {
    maze_t solution = array<array<int8_t, colCount>, rowCount>();

    system("cls"); // <stdlib.h>
    bool result = solve(maze, solution, startX, startY);

    cout << "Maze:" << endl;
    printMaze(maze);
    cout << endl;

    if (result) {
        cout << "Solution:" << endl;
        printMaze(solution);
    } else {
        cout << "No solution found." << endl;
    }

#ifdef DEBUG
    this_thread::sleep_for(chrono::milliseconds(3000));
#endif // DEBUG
}

int main() {
    maze_t maze1{{{0, 0, 0, 0, 0},
                  {0, 1, 0, 1, 0},
                  {0, 1, 1, 1, 0},
                  {0, 0, 1, 1, 0},
                  {0, 0, 0, 1, 0},
                  {0, 0, 0, 1, 0}}};
    maze_t maze2{{{0, 0, 0, 0, 0},
                  {0, 1, 0, 1, 0},
                  {0, 1, 1, 1, 0},
                  {0, 0, 0, 1, 0},
                  {0, 1, 1, 1, 0},
                  {0, 0, 0, 0, 0}}};
    Solve(maze1, 2, 2);
    Solve(maze2, 2, 2);
    return 0;
}
