# Title of the Game – Tetris Inferno 

Tetris: a popular puzzle game where players must strategically rotate, move and place tetrominoes as they fall. Tetromino is a shape composed of four-square blocks each. The goal is to complete a horizontal line of blocks without any gaps, which then clears those lines from the grid and gives points to the player. It also prevents the blocks from being tetrominoes If the Tetrominoes stack up to the top of the grid, the game is over. 

Implementation Details: 

Tetromino Movement: implemented the functionality for moving Tetrominoes left, right, and down within the game grid. This involves updating the position of the current Tetromino based on user input and automatic downward movement. 

Collision Detection: developed the collision detection logic to prevent Tetrominoes from moving beyond the grid boundaries or into already placed blocks. This ensures that Tetrominoes stop falling when they reach the bottom of the grid or land on top of other Tetrominoes 

Game Grid: set up the game grid, a 2D array representing the playing field where the Tetrominoes are placed. The grid tracks filled and empty cells, allowing for the placement of Tetrominoes and line clearing. 

Store Tetromino Shapes Using a Queue: queue used to store upcoming Tetromino shapes, allowing the game to display the next shapes to the player. This was achieved by using a data structure like std::queue to enqueue new Tetromino shapes and dequeue them when it's time to introduce the next shape to the grid. 


Scoring function: The score is incremented by a fixed amount each time a tetromino is placed on the grid. This is a simple use of an integer variable to keep track of the score, with a straightforward addition operation to update the score. 

Random color generating: the function ‘getRandomColor’ selects a random index within the bounds of the array and returns the color at that index. This uses an array data structure to store the possible colors and a random number generation algorithm to select one. 

Rotating tetromino: The rotation function modifies the 2D array representing the current tetromino. It creates a temporary 2D array to hold the current state of the tetromino, then it rotates the tetromino  90 degrees clockwise by reassigning the values from the temporary array to the original one in a rotated manner. 

Clear Lines function: There’s a loop to go from rows from bottom to top, and then a loop to check whether the whole row is filled with a Tetromino, if yes, then it clears up the line and brings down the line up top. It also adds 100 to the score per lines cleared. 

Update Speed function: It increases the speed of the Tetromino that is coming down every time the score is more than or equal to 20 and if it satisfies a limit speed. It decreases the delay by 20, which means speed is increased by 20. 

Game over function: Checks if Tetromino reaches the top of the grid, by checking if check Bottom Collision function is, and if tetromino placed in y exists. If one of these is false, the game is over. 

 

 
How to Play 

Step 1: 
When you see the first Tetromino comes, you can manipulate using the arrow buttons: up, left and right. The UP button will flip the tetromino in order to help the player to fit better shapes together. LEFT will move the piece to the left and RIGHT will move it right.  

Step 2:  
The tetromino will stack on top of one another, so player will need to manipulate it in the way to create a horizontal line out of different shapes will give double points for clearing line.  
After the line is filled, it will clear making the stacked tetromino on the top to go down by 1 row. For clearing the row, the player receives double points.  

Step 3: Watch your score, the speed and tetromino 

As the player progresses, the tetromino go faster and the score goes higher. Watch the tetromino  so it won’t reach the top of the grid, otherwise the game is over, and you need to restart. After the game is over, there will be the highest score you achieved. 

 
