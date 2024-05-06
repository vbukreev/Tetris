#include "SDL.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <unordered_map>

// Screen dimension constants
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 800;
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;
const int CELL_SIZE = 40; // Size of the cell in pixels
int grid[GRID_HEIGHT][GRID_WIDTH] = { 0 };

const SDL_Color colors[] = {
    {255, 0, 0, 255},   // Red
    {0, 255, 0, 255},   // Green
    {0, 0, 255, 255},   // Blue
    {255, 255, 0, 255}, // Yellow
    {255, 0, 255, 255}, // Magenta
    {0, 255, 255, 255}, // Cyan
    {255, 165, 0, 255}, // Orange
};

const int COLOR_COUNT = sizeof(colors) / sizeof(colors[0]);

// Function to generate a random color
SDL_Color getRandomColor() {
    return colors[rand() % COLOR_COUNT];
}
const int TETROMINO_SIZE = 4;
int currentTetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
};

int I_Tetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

int O_Tetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {1, 1},
    {1, 1},
};

int T_Tetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

int S_Tetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

int Z_Tetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

int J_Tetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {1, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

int L_Tetromino[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 0, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

int tetrominoX = GRID_WIDTH / 2 - TETROMINO_SIZE / 2;
int tetrominoY = 0;
int score = 0; // Global variable to store the score

// Speed settings
int initialSpeedDelay = 500; // Initial delay between frame updates in milliseconds
int speedIncrement = 20; // Amount of milliseconds to decrease delay by
int currentSpeedDelay = initialSpeedDelay; // Current delay between frame updates
int maxSpeedDelay = 70; //  Variable to limit the speed

// Starts up SDL and creates window
bool init(SDL_Window** gWindow, SDL_Renderer** gRenderer);

// Main loop flag
bool quit = false;

// Event handler
SDL_Event e;

//Queue
std::queue<int(*)[TETROMINO_SIZE][TETROMINO_SIZE]> tetrominoQueue;

//Hash
std::unordered_map<std::string, int> highScores;

// Function prototypes
void drawGrid(SDL_Renderer* gRenderer);
void drawTetromino(SDL_Renderer* gRenderer, int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int posX, int posY);
void placeTetrominoInGrid();
void displayScore();
void rotateTetromino(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE]);
void renderGrid(SDL_Renderer* gRenderer); // Updated function

void clearLines();
void updateSpeed();
bool gameOver();
int highestScore = -1;



void drawGrid(SDL_Renderer* gRenderer) {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // Black color for grid lines
    for (int i = 0; i <= GRID_HEIGHT; ++i) {
        SDL_RenderDrawLine(gRenderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
    }
    for (int j = 0; j <= GRID_WIDTH; ++j) {
        SDL_RenderDrawLine(gRenderer, j * CELL_SIZE, 0, j * CELL_SIZE, SCREEN_HEIGHT);
    }
}

void drawTetromino(SDL_Renderer* gRenderer, int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int posX, int posY, SDL_Color color) {
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
    for (int i = 0; i < TETROMINO_SIZE; ++i) {
        for (int j = 0; j < TETROMINO_SIZE; ++j) {
            if (tetromino[i][j] == 1) {
                SDL_Rect fillRect = { (posX + j) * CELL_SIZE, (posY + i) * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(gRenderer, &fillRect);
            }
        }
    }
}

bool checkBottomCollision() {
    for (int i = 0; i < TETROMINO_SIZE; ++i) {
        for (int j = 0; j < TETROMINO_SIZE; ++j) {
            if (currentTetromino[i][j] != 0) { // Check only the filled parts of the tetromino
                int newX = tetrominoX + j;
                int newY = tetrominoY + i + 1; // Calculate the new Y position
                if (newY >= GRID_HEIGHT || (newX >= 0 && newX < GRID_WIDTH && grid[newY][newX] != 0)) {
                    return true; // Collision detected with bottom or a placed block
                }
            }
        }
    }
    return false;
}

void placeTetrominoInGrid() {
    for (int i = 0; i < TETROMINO_SIZE; ++i) {
        for (int j = 0; j < TETROMINO_SIZE; ++j) {
            if (currentTetromino[i][j] != 0) {
                int newX = tetrominoX + j;
                int newY = tetrominoY + i;
                if (newX >= 0 && newX < GRID_WIDTH && newY < GRID_HEIGHT) {
                    grid[newY][newX] = 1; // Mark the grid cell as filled
                    score += 10; // Increase score for each block placed
                }
            }
        }
    }
    if (score > highestScore) {
        highestScore = score;
    }
    // Reset tetromino position for the next tetromino
    tetrominoY = 0;
    tetrominoX = GRID_WIDTH / 2 - TETROMINO_SIZE / 2;
   
}

void selectNewTetromino() {
    
    int randomIndex = rand() % 7; 
    int(*nextTetromino)[TETROMINO_SIZE][TETROMINO_SIZE];
    
    switch (randomIndex) {

        case 0: nextTetromino = &I_Tetromino; 
            break;

        case 1: nextTetromino = &O_Tetromino;
            break;
        case 2: nextTetromino = &T_Tetromino; 
            break;

        case 3: nextTetromino = &S_Tetromino; 
            break;

        case 4: nextTetromino = &Z_Tetromino; 
            break;

        case 5: nextTetromino = &J_Tetromino; 
            break;

        case 6: nextTetromino = &L_Tetromino; 
            break;
    }

    // Enqueue the next Tetromino
    tetrominoQueue.push(nextTetromino);
}

void initializeTetrominoQueue() {
    for (int i = 0; i < 3; ++i) { 
        selectNewTetromino();
    }
}

void updateCurrentTetrominoFromQueue() {
    if (!tetrominoQueue.empty()) {
        auto nextTetromino = tetrominoQueue.front();
        tetrominoQueue.pop();
        
        memcpy(currentTetromino, *nextTetromino, sizeof(currentTetromino));

        // Enqueue a new random Tetromino to maintain the queue size
        selectNewTetromino();
    }
}

void displayScore() {
    std::cout << "Score: " << score << std::endl;
}

void updateHighScore(const std::string& playerName, int currentScore) {
    auto search = highScores.find(playerName);
    if (search != highScores.end()) {
        if (currentScore > search->second) {
            highScores[playerName] = currentScore;
            std::cout << "New high score for " << playerName << ": " << currentScore << std::endl;
        }
    }
    else {
        highScores[playerName] = currentScore;
        std::cout << "First score for " << playerName << ": " << currentScore << std::endl;
    }
}

void rotateTetromino(int tetromino[TETROMINO_SIZE][TETROMINO_SIZE]) {
    int temp[TETROMINO_SIZE][TETROMINO_SIZE];

    // Copy the original tetromino to a temporary matrix
    for (int i = 0; i < TETROMINO_SIZE; ++i) {
        for (int j = 0; j < TETROMINO_SIZE; ++j) {
            temp[i][j] = tetromino[i][j];
        }
    }

    // Rotate the tetromino 90 degrees clockwise
    for (int i = 0; i < TETROMINO_SIZE; ++i) {
        for (int j = 0; j < TETROMINO_SIZE; ++j) {
            tetromino[j][TETROMINO_SIZE - 1 - i] = temp[i][j];
        }
    }
}

void renderGrid(SDL_Renderer* gRenderer) {
    // Draw the grid lines
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // Black color for grid lines
    for (int i = 0; i <= GRID_HEIGHT; ++i) {
        SDL_RenderDrawLine(gRenderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
    }
    for (int j = 0; j <= GRID_WIDTH; ++j) {
        SDL_RenderDrawLine(gRenderer, j * CELL_SIZE, 0, j * CELL_SIZE, SCREEN_HEIGHT);
    }

    // Draw the filled cells (placed tetromino blocks)
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            if (grid[row][col] != 0) { // If the cell is filled
                SDL_Rect fillRect = { col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF); // Set color for filled cells
                SDL_RenderFillRect(gRenderer, &fillRect);
            }
        }
    }
}

void clearLines()
{
    int linesCleared = 0;

    for (int i = GRID_HEIGHT - 1; i >= 0; --i) {
        bool lineFilled = true;
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (grid[i][j] == 0) {
                lineFilled = false;
                break;
            }
        }
        if (lineFilled) {
            // Clear the top line
            for (int j = 0; j < GRID_WIDTH; ++j) {
                grid[0][j] = 0;
            }
            // Move all lines above down by one
            for (int k = i; k > 0; --k) {
                for (int j = 0; j < GRID_WIDTH; ++j) {
                    grid[k][j] = grid[k - 1][j];
                }
            }
            linesCleared++;
        }
        
    }

    if (linesCleared > 0) {
        score += 100 * linesCleared; // Adds 100 points for every line cleared
        displayScore();
        updateHighScore("Player1", score); // Assuming a single player game
    }
}

void updateSpeed()
{
    if (score >= 20 && currentSpeedDelay > maxSpeedDelay + speedIncrement) {
        currentSpeedDelay -= speedIncrement; // Decrease delay to increase speed
        if (currentSpeedDelay == maxSpeedDelay + speedIncrement) {
            std::cout << "This is the fastest I can go, can you handle me?" << std::endl;
        }
        else {
            std::cout << "Let's make it faster!!!" << std::endl;
        }

    }
}

bool gameOver()
{
    if (tetrominoY == 0 && checkBottomCollision()) {
        std::cout << "Game Over!" << std::endl;
        std::cout << "Highest Score: " << highestScore << std::endl;
        return true; // Game over
    }
    return false; // Game is not over
}

int main(int argc, char* args[]) {
    // Initialize SDL system
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;

    initializeTetrominoQueue();
    if (!init(&gWindow, &gRenderer)) {
        std::cout << "Failed to initialize!" << std::endl;
    }
    else {
        // Main loop
        while (!quit) {
            SDL_Color randomColor = getRandomColor();
            // Handle events on the queue
            while (SDL_PollEvent(&e) != 0) {
                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                // Handle keyboard input
                else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_LEFT: {
                            
                            bool canMoveLeft = true;
                            for (int i = 0; i < TETROMINO_SIZE; ++i) {
                                for (int j = 0; j < TETROMINO_SIZE; ++j) {
                                    if (currentTetromino[i][j] != 0) {
                                        if (tetrominoX + j - 1 < 0) {
                                            canMoveLeft = false; // Cannot move left, part of the tetromino would be out of bounds
                                        }
                                    }
                                }
                            }
                            if (canMoveLeft) {
                                tetrominoX--; // Move left
                            }
                            break;
                        }
                        case SDLK_RIGHT: {
                            
                            bool canMoveRight = true;
                            for (int i = 0; i < TETROMINO_SIZE; ++i) {
                                for (int j = 0; j < TETROMINO_SIZE; ++j) {
                                    if (currentTetromino[i][j] != 0) {
                                        if (tetrominoX + j + 1 >= GRID_WIDTH) {
                                            canMoveRight = false; // Cannot move right, part of the tetromino would be out of bounds
                                        }
                                    }
                                }
                            }
                            if (canMoveRight) {
                                tetrominoX++; // Move right
                            }
                            break;
                        }
                        case SDLK_UP:
                            rotateTetromino(currentTetromino);
                            break;
                    }
                }
            }

            if (!checkBottomCollision()) {
                tetrominoY++;
            }
            else {
                placeTetrominoInGrid(); // Place tetromino in the grid and update score
                clearLines();

                updateCurrentTetrominoFromQueue();
                tetrominoY = 0;
                tetrominoX = GRID_WIDTH / 2 - TETROMINO_SIZE / 2;

                // Display score
                displayScore();
                updateSpeed();
            }

            // Check for game over condition, should I quit or not?
            quit = gameOver();


            // Clear screen
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            // Draw game grid and tetromino
            drawGrid(gRenderer);
            drawTetromino(gRenderer, currentTetromino, tetrominoX, tetrominoY, randomColor);
            renderGrid(gRenderer); // Draw placed tetromino blocks

            // Update screen
            SDL_RenderPresent(gRenderer);



            SDL_Delay(currentSpeedDelay); // Sets the delay for the loop depending on updateSpeed()
        }
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}


bool init(SDL_Window** gWindow, SDL_Renderer** gRenderer) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    else {

        *gWindow = SDL_CreateWindow("Tetris Inferno", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (*gWindow == NULL) {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
        else {

            *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (*gRenderer == NULL) {
                std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                return false;
            }
            else {

                SDL_SetRenderDrawColor(*gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }

    return true;
}