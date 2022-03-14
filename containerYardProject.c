/* Container Yard */
/* Author: Tyler Kim 2021*/

#include <stdio.h>
#define NUM_ROWS 8
#define NUM_COLS 8

#define VACANT 0
#define BOUNDARY -1
#define ENTRY -2
#define EXIT -3

void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index);
void PrintFloor(int floor[NUM_ROWS][NUM_COLS]);
double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width);
void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction);
int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int *rowStart, int *colStart, int *rowEnd, int *colEnd);
int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked);

char GetMove(void)
{
	char move;
	printf("\nMove container: ");
	scanf("%c", &move);
	// Ignore non-capital letter inputs
	while ((move < 'A') || (move > 'Z')) {
		scanf("%c", &move);
	}
	return move;
}

int main(void)
{
	int gameOver = 0;
	int isBlocked = 0;
	int floor[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf("............**********************************............\n");
	printf("............* CONTAINER YARD GAME SIMULATION *............\n");
	printf("............**********************************............\n");

	/* Initialise the yard floor grid and add containers */
	InitialiseFloor(floor, 'R', 3);
	AddContainer(floor, 28, 2, 0);
	AddContainer(floor, 11, 3, 1);
	AddContainer(floor, 41, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 42, 2, 1);
	AddContainer(floor, 34, 2, 0);
	AddContainer(floor, 36, 3, 1);
	AddContainer(floor, 37, 2, 1);
	AddContainer(floor, 53, 2, 0);
	AddContainer(floor, 30, 3, 1);

	/* Print status */
	printf("C Project\nContainer Yard!  The containers are rushing in!\n");
	printf("In fact, %.2f sq ft of the yard floor is available for containers!\n\n", FloorAreaAvailable(floor, 20.5, 10.3));

	/* Main simulation loop */
	while (gameOver != 2) {
		PrintFloor(floor);
		input = GetMove();
		isBlocked = LocateContainer(floor, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveContainer(floor, rowStart, colStart, rowEnd, colEnd, isBlocked);
	}

	/* A container is ready to exit - the simulation is over */
	PrintFloor(floor);
	printf("\nCongratulations, you've succeeded!");

	return 0;
}

void InitialiseFloor(int floor[NUM_ROWS][NUM_COLS], char entryboundary, int index)
{
/*
The function InitialiseFloor initialise the 2D grids.
Author: Tyler Kim
Inputs:
floor[NUM_ROWS][NUM_COLS] - A 2D array that represents the floor (to be initialised/modified by the funciton)
entryboundary - indication of 4 boundaries (represented by a character 'T', 'R', 'L' or 'B') representing the position of entry and exit.
index - in integer indicating how many rows down from the top/ or columns across from the left for the entry position.
*/
	int i, j;
	// nested for loops to initialise elements at the outer boundary or within the grid
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			// if the element is at the outer boundary, initialise it as -1.
			if (i == 0 || i == NUM_ROWS - 1 || j == 0 || j == NUM_COLS - 1) {
				floor[i][j] = BOUNDARY;
			}
			// if not, the element will be initialised as 0.
			else {
				floor[i][j] = VACANT;
			}
		}
	}

	// initialise the entry and exit according to the input from the program. 
	if (entryboundary == 'T') {
		floor[0][index] = ENTRY;
		floor[NUM_ROWS - 1][index] = EXIT;
	}

	if (entryboundary == 'B') {
		floor[NUM_ROWS - 1][index] = ENTRY;
		floor[0][index] = EXIT;
	}

	if (entryboundary == 'L') {
		floor[index][0] = ENTRY;
		floor[index][NUM_COLS - 1] = EXIT;
	}

	if (entryboundary == 'R') {
		floor[index][NUM_COLS - 1] = ENTRY;
		floor[index][0] = EXIT;
	}

}

void PrintFloor(int floor[NUM_ROWS][NUM_COLS])
{
	/*
	This function takes the initialised floor grid from InitialiseFloor function and prints it for user.
	Author: Tyler Kim 2021
	Input:
	floor[NUM_ROWS][NUM_COLS] - A 2D array that represents the floor that was initialised by InitialiseFloor funciton.
	Output: It prints the stylised initialised floor.
	*/
	int i, j;

	// nested loop to stylise each component into characters
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			// Use ' ' to represent an element within the grid
			if (floor[i][j] == VACANT) {
				printf(" ");
			}
			// Use '@' to represent an element at the boundary
			else if (floor[i][j] == BOUNDARY) {
				printf("@");
			}
			// Use 'U' to represent the extry
			else if (floor[i][j] == ENTRY) {
				printf("U");
			}
			// Use 'X' to represent the exit
			else if (floor[i][j] == EXIT) {
				printf("X");
			}
			else {
				printf("%c", floor[i][j]);
			}
		}
		printf("\n");
	}
}

double FloorAreaAvailable(int floor[NUM_ROWS][NUM_COLS], double length, double width) {
/*
The function FloorAreaAvailable calculates the vacant space availabel in the container yard.
Author: Tyler Kim 2021
Inputs:
floor[NUM_ROWS][NUM_COLS]: 2D array containing the data for the yard floor
length: length of the cell (each cell has the same length)
width: width of the cell (each cell has the same width)
Ouput:
The function returns the available area in the container yard (double).
*/
	int i, j;
	int vacantCells = 0;

	// Nested for loops to count the number of vacant cells in the container yard.
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_ROWS; j++) {
			if (floor[i][j] == 0) {
				vacantCells++;
			}
		}
	}

	// Return the area available in the container yard
	return length * width * vacantCells;
}

void AddContainer(int floor[NUM_ROWS][NUM_COLS], int position, int size, int direction) {
	/*
	The function AddContainer adds a containers according to the input size, direction and position.
	Author: Tyler Kim 2021
	Input:
	floor[NUM_ROWS][NUM_COLS]: A 2D array representing the floor grid
	position: The position of the container
	size: the size of the container to add.
	direction: whether to add the container horizontally (0) or vertically (1).
	*/

	int i, j;
	int n = 0;
	// 65 is 'A' in ASCII.
	int Letter = 65;
	int Largest = 0;

	// Everytime the function is run and the input is valid, the sedond contianer added will be container 'B' and so on. 
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (floor[i][j] > Largest) {
				Largest = floor[i][j];
			}
		}
	}
	if (Largest >= 65) {
		Letter = Largest + 1;
	}

	// placing the container horizontally
	if (direction == 0) {
		for (n = 0; n < size; n++) {
			if (floor[0][position + n] != 0) {
				return;
			}
		}
		for (n = 0; n < size; n++) {
			floor[0][position + n] = Letter;
		}
	}

	// placing the container vertically
	if (direction == 1) {
		for (n = 0; n < size; n++) {
			if (floor[n][position] != 0) {
				return;
			}
		}
		for (n = 0; n < size; n++) {
			floor[n][position] = Letter;
		}
	}
}

int LocateContainer(int floor[NUM_ROWS][NUM_COLS], char move, int *rowStart, int *colStart, int* rowEnd, int *colEnd)
{
	/*
	The function LocateContainer locates the specified container and return a value on whether if it can be moved or not.
	Author: Tyler Kim
	Input:
	floor[NUM_ROWS][NUM_COLS]: the 2D array representing the container yard.
	move: the letter representing the container being searched for.
	4 pointer variable which will store the location of specified containers
	Output: the function returns 0 if the container can't be moved front or back and it returns 1 it the container can move front or back.
	*/
	int i, j;
	int n = 0;

	// nested for loops to identify the container
	for (i = 0; i < NUM_ROWS; i++) {
		for (j = 0; j < NUM_COLS; j++) {
			if (floor[i][j] == move) {
				*rowStart = i;
				*colStart = j;

				// if the container was located horizontally, locate the end point
				if (floor[*rowStart][*colStart + 1] == move) {
					while (floor[*rowStart][*colStart + n] == move) {
						*colEnd = *colStart + n;
						*rowEnd = *rowStart;
						n++;
					}
					// if the container can move return 1 and if not, return 0. 
					if (floor[*rowStart][*colStart - 1] == 0 || floor[*rowEnd][*colEnd + 1] == 0) {
						return 1;
					}
					else {
						return 0;
					}
				}
				// if the container was located vertically, locate the end point
				else if (floor[*rowStart + 1][*colStart] == move) {
					while (floor[*rowStart + n][*colStart] == move) {
						*rowEnd = *rowStart + n;
						*colEnd = *colStart;
						n++;
					}
					// if the container can move return 1 and if not, return 0. 
					if (floor[*rowStart - 1][*colStart] == 0 || floor[*rowEnd + 1][*colEnd] == 0) {
						return 1;
					}
					else {
						return 0;
					}
				}
			}
			else {
			}
		}
	}
	return -1;
}

int MoveContainer(int floor[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1, int isBlocked) {
	/*
	This function moves the specified containers. The container will move left or up if they can and if they can't they'll move the opposite way until it reaches the boundary or another container.
	Author: Tyler Kim
	Inputs:
	floor: 2D array representing the container yard
	4 integer values representing the position of the container to move.
	1 integer value representing whether the container is blocked from both front and back
	Output:
	This function has one integer output depending on where the container is placed.
	*/

	int Letter = floor[r0][c0];
	int sizeR = r1 - r0;
	int sizeC = c1 - c0;
	int i, j;
	int limit = 0;

	if (isBlocked == 0) {
		if (sizeR > 0) {
			if (floor[r0 - 1][c0] == ENTRY || floor[r1 + 1][c0] == ENTRY) {
				return 1;
			}
			else if (floor[r0 - 1][c0] == EXIT || floor[r1 + 1][c0] == EXIT) {
				return 2;
			}
			else return -1;
		}
		else if (sizeC > 0) {
			if (floor[r0][c0 - 1] == ENTRY || floor[r0][c1 + 1] == ENTRY) {
				return 1;
			}
			else if (floor[r0][c0 - 1] == EXIT || floor[r0][c1 + 1] == EXIT) {
				return 2;
			}
			else return -1;
		}
	}
	else {

		for (i = r0; i <= r1; i++) {
			for (j = c0; j <= c1; j++) {
				floor[i][j] = 0;
			}
		}

		if (sizeR > 0) {
			if (r0 > 1) {
				for (i = 1; i < r0; i++) {
					if (floor[i][c0] != 0) {
						limit = i;
					}
				}
				if (limit == r0 - 1) {
					limit = NUM_ROWS - 1;
					for (i = NUM_ROWS - 2; i > r1; i--) {
						if (floor[i][c0] != 0) {
							limit = i;
						}
					}
					r1 = limit - 1;
					r0 = limit - sizeR - 1;
					for (i = r0; i <= r1; i++) {
						floor[i][c0] = Letter;
					}
					if (floor[NUM_ROWS - 1][c0] == EXIT) {
						return 2;
					}
					else if (floor[NUM_ROWS - 1][c0] == ENTRY) {
						return 1;
					}
					else {
						return 0;
					}
				}
				r1 = limit + 1 + sizeR;
				r0 = limit + 1;
				for (i = r0; i <= r1; i++) {
					floor[i][c0] = Letter;
				}
				if (floor[0][c0] == EXIT) {
					return 2;
				}
				else if (floor[0][c0] == ENTRY) {
					return 1;
				}
				else {
					return 0;
				}
			}
			else if (r0 == 1) {
				limit = NUM_ROWS - 1;
				for (i = NUM_ROWS - 2; i > r1; i--) {
					if (floor[i][c0] != 0) {
						limit = i;
					}
				}
				r1 = limit - 1;
				r0 = limit - sizeR - 1;
				for (i = r0; i <= r1; i++) {
					floor[i][c0] = Letter;
				}
				if (floor[NUM_ROWS - 1][c0] == EXIT) {
					return 2;
				}
				else if (floor[NUM_ROWS - 1][c0] == ENTRY) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}
		if (sizeC > 0) {
			if (c0 > 1) {
				for (j = 1; j < c0; j++) {
					if (floor[r0][j] != 0) {
						limit = j;
					}
				}
				if (limit == c0 - 1) {
					limit = NUM_COLS - 1;
					for (j = NUM_COLS - 2; j > c1; j--) {
						if (floor[r0][j] != 0) {
							limit = j;
						}
					}
					c1 = limit - 1;
					c0 = limit - sizeC - 1;
					for (j = c0; j <= c1; j++) {
						floor[r0][j] = Letter;
					}
					if (floor[r0][NUM_COLS - 1] == EXIT) {
						return 2;
					}
					else if (floor[r0][NUM_COLS - 1] == ENTRY) {
						return 1;
					}
					else {
						return 0;
					}
				}
				c1 = 1 + limit + sizeC;
				c0 = 1 + limit;
				for (j = c0; j <= c1; j++) {
					floor[r0][j] = Letter;
				}
				if (floor[r0][0] == ENTRY) {
					return 1;
				}
				else if (floor[r0][0] == EXIT) {
					return 2;
				}
				else {
					return 0;
				}
			}
			else if (c0 == 1) {
				limit = NUM_COLS - 1;
				for (j = NUM_COLS - 2; j > c1; j--) {
					if (floor[r0][j] != 0) {
						limit = j;
					}
				}
				c1 = limit - 1;
				c0 = limit - sizeC - 1;
				for (j = c0; j <= c1; j++) {
					floor[r0][j] = Letter;
				}
				if (floor[r0][NUM_COLS - 1] == ENTRY) {
					return 1;
				}
				else if (floor[r0][NUM_COLS - 1] == EXIT) {
					return 2;
				}
				else {
					return 0;
				}
			}
		}
		return 0;
	}
	return -1;
}

