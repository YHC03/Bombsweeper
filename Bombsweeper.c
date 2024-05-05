#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

// Define the constants
#define MIN_VALUE 4 // MIN_VALUE can't be under 4, as the game runs oddly in 1-3
#define MAX_VALUE 26 // MAX_VALUE can't be over 26, as it uses A-Z to set map location

// Define the variables to store the map's data
char value[MAX_VALUE][MAX_VALUE]; // Stores the value of the map
char isUncover[MAX_VALUE][MAX_VALUE]; // Stores if uncovered

// Declaring Functions

// Mainly Operating Function
void play(int size, int bombNum);

// Data Modifying Function
int discover(int xValue, int yValue, int size, int isFirst, int isFlag);

// Data Printing Function
void printBoard(int size, int remaining, int isEnd);

// Resetting and Creating Map Function
void reset();
void makeBoard(int size, int num, int eliminateX, int eliminateY);

// Supplementary Function
int findRemaining(int size, int bombNum);
int checkUncoverNum(int size);

// End Declaring Functions


/* reset() Function
* 
* Operation : Reset the map 
* No input, output variable
* Last Comment Modify Date : 2024/5/5
*/
void reset()
{
	// Reset all of the map variable
	for (int i = 0; i < MAX_VALUE; i++)
	{
		for (int j = 0; j < MAX_VALUE; j++)
		{
			// Reset the value, and uncover data
			value[i][j] = 0;
			isUncover[i][j] = 1;
		}
	}

	return;
}


/* findRemaining() Function
* 
* Operation : find the remaining targets, subtracting the flaged area(it does not means the flaged area with target)
* Input variable : size(map size), bombNum(numbers of the targets)
* Output variable : Number of remaining targets
* Last Comment Modify Date : 2024/5/5
*/
int findRemaining(int size, int bombNum)
{
	// First, the result is the number of original number of targets
	int result = bombNum;
	for(int i=0 ; i<size ; i++)
	{
		for (int j = 0; j < size; j++)
		{
			// If flaged, decrease the remaining number of targets
			if (isUncover[i][j] == -1)
			{
				result--;
			}
		}
	}
	
	// Return the number of remaining targets
	return result;
}


/* printBoard() Function
* 
* Operation : Printing the board
* Input variable : size(size of the board), remaining(remaining number of the targets), isEnd(if the game ends, isEnd is 1)
* No output variable
* Last Comment Modify Date : 2024/5/5
*/
void printBoard(int size, int remaining, int isEnd)
{
	// Clear the screen
	system("cls");

	// Reset the Alphabet indicates the column
	char naming = 'A';

	printf("\\");
	// Print the Alphabet indicates the column
	for (int i = 0; i < size; i++)
	{
		printf("%c", naming);
		naming++;
	}
	printf("\n");

	// Reset the Alphabet indicates the row
	naming = 'A';


	for (int i = 0; i < size; i++)
	{
		// Print the Alphabet indicates the row
		printf("%c", naming);

		// Print the value of the specific area
		for (int j = 0; j < size; j++)
		{
			if (isUncover[i][j] && !isEnd)
			{
				// If the area is now flaged, print !
				// Else print =, indicates it is now covered.
				if (isUncover[i][j] == -1)
				{
					printf("!");
				}else{
					printf("=");
				}
			}else{
				// If the area has target, print *.
				if (value[i][j] == -1)
				{
					printf("*");
				}else if (value[i][j] == 0){ // If the value is 0(no targets near the area), print _.
					printf("_");
				}else{ // Else print the number of the near targets.
					printf("%d", value[i][j]);
				}
			}
		}

		// Print the Alphabet indicates the row
		printf("%c", naming);

		// Increase the Alphabet indicates the row
		naming++;

		// Move to next line
		printf("\n");
	}

	printf(" ");
	// Reset the Alphabet indicates the column
	naming = 'A';

	// Print the Alphabet indicates the column
	for (int i = 0; i < size; i++)
	{
		printf("%c", naming);
		naming++;
	}

	// Print the remaining number of the target
	printf("\n%d Left\n", remaining);

	return;
}


/* makeBoard() Function
* 
* Operation : Creating the map, with not deploying targets near [eliminateX, eliminateY]
* Input variable : size(size of the map), num(number of the targets), eliminateX, eliminateY(location not to deploy targets near the location)
* No output variable
* Last Comment Modify Date : 2024/5/5
*/
void makeBoard(int size, int num, int eliminateX, int eliminateY)
{
	// Variable to save location(Y_axis * num + X_axis), tmpNum(temporary save random location), tmpCount(temporary save number of the targets near the area)
	int location[MAX_VALUE * MAX_VALUE], tmpNum = 0, tmpCount = 0;

	// X, Y location to move
	const char xPos[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const char yPos[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	srand(time(NULL));
	for (int i = 0; i < num; i++)
	{
getNumber:
		tmpNum = rand() % (size * size);

		// Find if the number is near the [eliminateX, eliminateY] area
		if (tmpNum != eliminateY * size + eliminateX)
		{
			for (int k = 0; k < 8; k++)
			{
				if ((int)(tmpNum / size) + yPos[k] >= 0 && (int)(tmpNum / size) + yPos[k] < size && tmpNum % size + xPos[k] >= 0 && tmpNum % size + xPos[k] < size)
				{
					if ((int)(tmpNum / size) + yPos[k] == eliminateY && tmpNum % size + xPos[k] == eliminateX)
					{
						goto getNumber;
					}
				}
			}
		}else{
			goto getNumber;
		}

		// Find if the target is already deployed on that site
		for (int j = 0; j < i; j++)
		{
			if (location[j] == tmpNum)
			{
				goto getNumber;
			}
		}

		// Save the area
		location[i] = tmpNum;
	}

	// Save the target's location
	for (int i = 0; i < num; i++)
	{
		value[(int)location[i] / size][location[i] % size] = -1;
	}

	// Save the map
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			// If the target is not on that site
			if (value[i][j] != -1)
			{
				// Reset the count of the number of targets
				tmpCount = 0;
				for (int k = 0; k < 8; k++)
				{
					if (i + yPos[k] >= 0 && i + yPos[k] < size && j + xPos[k] >= 0 && j + xPos[k] < size)
					{
						if (value[i + yPos[k]][j + xPos[k]] == -1) // If target was found
						{
							tmpCount++;
						}
					}
				}

				// Save it
				value[i][j] = tmpCount;
			}
		}
	}

	return;
}


/* discover() Function
* 
* Operation : Discover specific site, and if the target found, return 1 to indicate the game is now over.
* Input variable : xValue, yValue(specific site to discover), size(size of the map), isFirst(indicate if the function call is not recursive), isFlag(indicate if this input is Flag setting)
* Output variable : Indicating if the game is now over(1 means that the game is now over)
* Last Comment Modify Date : 2024/5/5
*/
int discover(int xValue, int yValue, int size, int isFirst, int isFlag)
{
	// X, Y location to move
	const char xPos[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const char yPos[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	// Prevent Re-opening the value
	if (!isUncover[yValue][xValue])
	{
		if(isFirst)
		{
			// Print the area is already opened
			printf("Already Opened\n");
			system("PAUSE");
		}
		return 0;
	}

	// If the operation is flag setting
	if (isFlag)
	{
		isUncover[yValue][xValue] = -isUncover[yValue][xValue];

		return 0;
	}

	// If the operation is uncover at flag area
	if (isUncover[yValue][xValue] == -1)
	{
		printf("The site has flag.\n");
		system("PAUSE");

		return 0;
	}

	// Uncover the area
	isUncover[yValue][xValue] = 0;

	// If the target was found at the area
	if (value[yValue][xValue] == -1)
	{
		// If this time is first time(not clearing near zero)
		if (isFirst)
		{
			// Return that this turn is fail
			return 1;
		}else{
			// If this tims is not the first time(now clearing near zero)
			isUncover[yValue][xValue] = 1; // Re-cover the value
		}

		return 0;
	}

	// If the number of the near target is 0, find the near value
	if (value[yValue][xValue] == 0)
	{
		for (int k = 0; k < 8; k++)
		{
			if (yValue + yPos[k] >= 0 && yValue + yPos[k] < size && xValue + xPos[k] >= 0 && xValue + xPos[k] < size)
			{
				// If this function is not recursive call, or the next value is not 0, or the next location is not diagonal(if k % 2 == 1, the next location is diagonal)
				if (isFirst || value[yValue + yPos[k]][xValue + xPos[k]] != 0 || k % 2 == 0)
				{
					// Call recursive function
					discover(xValue + xPos[k], yValue + yPos[k], size, 0, 0);
				}
			}
		}
		
	}

	return 0;
}

/* checkUncoverNum() Function
* 
* Operation : Find number of the covered area
* Input variable : size(size of the map)
* Output variable : Number of the covered area
* Last Comment Modify Date : 2024/5/5
*/
int checkUncoverNum(int size)
{
	// Cleat the count variable
	int result = 0;

	// Find all area
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			// If the area is now covered(1), or has flags(-1)
			if (isUncover[i][j])
			{
				// Increase the counting
				result++;
			}
		}
	}
	
	// Return the counting
	return result;
}

/* play() Function
* 
* Operation : Manage the game play
* Input variable : size(size of the map), bombNum(number of the target)
* No output variable
* Last Comment Modify Date : 2024/5/5
*/
void play(int size, int bombNum)
{
	/* Create the variables
	* 
	* isEnd(indicate if the game ends, tmpX, tmpY(input value to discover it), isFlag(input value to flag/unflag the site)
	* remaining(indicate the remaining of the target)
	* inputValue(raw input data to get the value to discover)
	*/
	int isEnd = 0, tmpX, tmpY, isFlag = 0;
	int remaining = size;
	char inputValue[20];

	// Retry Label for first time input
retryFirst:
	// Disable flag operation
	isFlag = 0;

	// Get input for target to discover
	printBoard(size, bombNum, isEnd);
	printf("Which Point to Discover?(row, col, put ! last to change flags) : ");
	scanf("%s", inputValue);

	// Process the input
	tmpY = inputValue[0] - 'A'; // Get row data
	if (tmpY > 'Z' - 'A') { tmpY -= 'a' - 'A'; } // If input is small letter, change to capital letter input
	tmpX = inputValue[1] - 'A'; // Get column data
	if (tmpX > 'Z' - 'A') { tmpX -= 'a' - 'A'; } // If input is small letter, change to capital letter input
	if (inputValue[2] == '!') // If flag set mode
	{
		isFlag = 1;
	}

	// If the input is wrong
	if (tmpX >= size || tmpY >= size || tmpX < 0 || tmpY < 0)
	{
		// Print that the input is wrong, and retry
		printf("Wrong Data Inputed.\n");
		system("PAUSE");
		goto retryFirst;
	}

	// Make board avoid to set values on first input
	makeBoard(size, bombNum, tmpX, tmpY);

	// Discover at the input
	discover(tmpX, tmpY, size, 1, isFlag);

	// Loop until the game ends
	while (!isEnd)
	{
		// Get the remaining targets to find
		remaining = findRemaining(size, bombNum);

		// If uncovered number is the same as the number of the target
		if (checkUncoverNum(size) == bombNum && !isEnd)
		{
			// Print success
			printBoard(size, remaining, 1);
			printf("Success!\n");
			system("PAUSE");

			// Return to main() Function
			return;
		}

		// Retry Label for continuous input
	retry:
		// Disable flag operation
		isFlag = 0;

		// Get input for target to discover
		printBoard(size, remaining, isEnd);
		printf("Which Point to Discover?(row, col, put ! last to change flags) : ");
		scanf("%s", inputValue);

		// Process the input
		tmpY = inputValue[0] - 'A'; // Get row data
		if (tmpY > 'Z' - 'A') { tmpY -= 'a' - 'A'; } // If input is small letter, change to capital letter input
		tmpX = inputValue[1] - 'A'; // Get column data
		if (tmpX > 'Z' - 'A') { tmpX -= 'a' - 'A'; } // If input is small letter, change to capital letter input
		if (inputValue[2] == '!') // If flag set mode
		{
			isFlag = 1;
		}

		// If the input is wrong
		if (tmpX >= size || tmpY >= size || tmpX < 0 || tmpY < 0)
		{
			// Print that the input is wrong, and retry
			printf("Wrong Data Inputed.\n");
			system("PAUSE");
			goto retry;
		}

		// Discover the target number, and return if the game ends
		isEnd = discover(tmpX, tmpY, size, 1, isFlag);
	}

	// If the game ends with failure, print the data
	printBoard(size, remaining, isEnd);
	printf("Fail\n");
	system("PAUSE");

	// Return to main() Function
	return;
}

/* main() Function
* 
* Operation : Get the size of the map, numbers of the target, and call play() Function, forever
* No input and output variable
* Last Comment Modify Date : 2024/5/5
*/
int main()
{
	// Create the boardSize(size of the board for specific game), and bombsNum(number of the target for specific game) variable
	int boardSize = -1;
	int bombsNum = -1;
	
	// Infinite Loop
	while (1)
	{
		// Clear the screen
		system("cls");

		// Reset the boardSize variable
		boardSize = -1;

		// Loop until the input is correct
		while (boardSize < MIN_VALUE || boardSize > MAX_VALUE)
		{
			// Get the size of the board
			printf("Map Size(n*n) : ");
			scanf("%d", &boardSize);

			// If the input is wrong
			if (boardSize < MIN_VALUE || boardSize > MAX_VALUE)
			{
				// Clear the screen, and notify the input is wrong
				system("cls");
				printf("Wrong data Inputed.\n");
			}
		}

		// Reprint the map size data to clear the statement that the input is wrong
		system("cls");
		printf("Map Size(n*n) : %d\n", boardSize);

		// Reset the bombsNum variable
		bombsNum = -1;

		// Loop until the input is correct
		while (bombsNum < 0 || bombsNum > boardSize * boardSize / 2 + 1)
		{
			// Get the number of the target
			printf("How may bombs in map? : ");
			scanf("%d", &bombsNum);

			// If the input is wrong
			if (bombsNum < 0 || bombsNum > boardSize * boardSize / 2 + 1)
			{
				// Clear the screen, and notify the input is wrong
				system("cls");
				printf("Wrong data Inputed.\n");
				printf("Map Size(n*n) : %d\n", boardSize);
			}
		}
		// Clear stdin buffer
		getchar();

		// Reset the board
		reset();

		// Call play() Function to run the game
		play(boardSize, bombsNum);
	}

	return 0;
}