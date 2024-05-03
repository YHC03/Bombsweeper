#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

#define MIN_VALUE 4 // MIN_VALUE can't be under 4, as the game runs oddly in 1-3
#define MAX_VALUE 26 // MAX_VALUE can't be over 26, as it uses A-Z to set map location

char value[MAX_VALUE][MAX_VALUE]; // Stores the value of the map.
char isUncover[MAX_VALUE][MAX_VALUE]; // Stores if uncovered

void reset()
{
	for (int i = 0; i < MAX_VALUE; i++)
	{
		for (int j = 0; j < MAX_VALUE; j++)
		{
			value[i][j] = 0;
			isUncover[i][j] = 1;
		}
	}

	return;
}

int findRemaining(int size, int bombNum)
{
	int result = bombNum;
	for(int i=0 ; i<size ; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (isUncover[i][j] == -1)
			{
				result--;
			}
		}
	}
	
	return result;
}

// isEnd for indicating if the game ends
void printBoard(int size, int remaining, int isEnd)
{
	system("cls");
	char naming = 'A';
	printf("\\");
	for (int i = 0; i < size; i++)
	{
		printf("%c", naming);
		naming++;
	}
	printf("\n");
	naming = 'A';
	for (int i = 0; i < size; i++)
	{
		printf("%c", naming);
		for (int j = 0; j < size; j++)
		{
			if (isUncover[i][j] && !isEnd)
			{
				if (isUncover[i][j] == -1)
				{
					printf("!");
				}else{
					printf("#");
				}
			}else{
				if (value[i][j] == -1)
				{
					printf("@");
				}
				else if (value[i][j] == 0) {
					printf("_");
				}else{
					printf("%d", value[i][j]);
				}
			}
		}
		printf("%c", naming);
		naming++;
		printf("\n");
	}

	printf(" ");
	naming = 'A';
	for (int i = 0; i < size; i++)
	{
		printf("%c", naming);
		naming++;
	}
	printf("\n%d Left\n", remaining);
	return;
}

// Create Map
void makeBoard(int size, int num, int eliminateX, int eliminateY)
{
	int location[MAX_VALUE * MAX_VALUE], tmpNum = 0, tmpCount = 0;

	const char xPos[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const char yPos[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	srand(time(NULL));
	for (int i = 0; i < num; i++)
	{
getNumber:
		tmpNum = rand() % (size * size);
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
		}
		else {
			goto getNumber;
		}

		for (int j = 0; j < i; j++)
		{
			if (location[j] == tmpNum)
			{
				goto getNumber;
			}
		}
		location[i] = tmpNum;
	}

	// save the bomb's location
	for (int i = 0; i < num; i++)
	{
		value[(int)location[i] / size][location[i] % size] = -1;
	}

	// save the map
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (value[i][j] != -1)
			{
				tmpCount = 0;
				for (int k = 0; k < 8; k++)
				{
					if (i + yPos[k] >= 0 && i + yPos[k] < size && j + xPos[k] >= 0 && j + xPos[k] < size)
					{
						if (value[i + yPos[k]][j + xPos[k]] == -1) // if bomb was found
						{
							tmpCount++;
						}
					}
				}
				value[i][j] = tmpCount;
			}
		}
	}

	return;
}

// Check if Opened
int isUncovered(int xValue, int yValue)
{
	return !isUncover[yValue][xValue];
}

// discover a value
// isFirst to run a recursive function
// if fails, returns 1
int discover(int xValue, int yValue, int size, int isFirst, int isFlag)
{
	const char xPos[4] = { 1, 0, -1, 0};
	const char yPos[4] = { 0, 1, 0, -1};

	if (!isUncover[yValue][xValue]) // Prevent re-open the value
	{
		if(isFirst)
		{
			printf("Already Opened\n");
			system("PAUSE");
		}
		return 0;
	}

	if (isFlag)
	{
		isUncover[yValue][xValue] = -isUncover[yValue][xValue];

		return 0;
	}

	if (isUncover[yValue][xValue] == -1)
	{
		printf("The site has flag.\n");
		system("PAUSE");

		return 0;
	}

	isUncover[yValue][xValue] = 0;

	if (value[yValue][xValue] == -1)
	{
		if (isFirst)
		{
			return 1;
		}else{
			isUncover[yValue][xValue] = 1; // now re-cover it
		}

		return 0;
	}

	// if the bomb number is 0, find the near value
	if (value[yValue][xValue] == 0)
	{
		for (int k = 0; k < 4; k++)
		{
			if (yValue + yPos[k] >= 0 && yValue + yPos[k] < size && xValue + xPos[k] >= 0 && xValue + xPos[k] < size)
			{
				discover(xValue + xPos[k], yValue + yPos[k], size, 0, 0);
			}
		}
	}

	return 0;
}

int checkUncoverNum(int size)
{
	int result = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (isUncover[i][j]) // 1 or -1(flag)
			{
				result++;
			}
		}
	}
	
	return result;
}

// Play the game
void play(int size, int bombNum)
{
	int isEnd = 0, tmpX, tmpY, isFlag = 0;
	int remaining = size;
	char inputValue[20];

retryFirst:
	printBoard(size, bombNum, isEnd);
	printf("Which Point to Discover?(row, col, put ! last to change flags) : ");

	isFlag = 0;
	scanf("%s", inputValue);
	tmpY = inputValue[0] - 'A';
	if (tmpY > 'Z' - 'A') { tmpY -= 'a' - 'A'; }
	tmpX = inputValue[1] - 'A';
	if (tmpX > 'Z' - 'A') { tmpX -= 'a' - 'A'; }
	if (inputValue[2] == '!')
	{
		isFlag = 1;
	}

	if (tmpX >= size || tmpY >= size || tmpX < 0 || tmpY < 0)
	{
		printf("Wrong Data Inputed.\n");
		system("PAUSE");
		goto retryFirst;
	}
	makeBoard(size, bombNum, tmpX, tmpY);
	discover(tmpX, tmpY, size, 1, isFlag);

	while (!isEnd)
	{
		remaining = findRemaining(size, bombNum);

		if (checkUncoverNum(size) == bombNum && !isEnd)
		{
			printBoard(size, remaining, 1);
			printf("Success!\n");
			system("PAUSE");

			return;
		}
retry:
		printBoard(size, remaining, isEnd);
		printf("Which Point to Discover?(row, col, put ! last to change flags) : ");

		isFlag = 0;
		scanf("%s", inputValue);
		tmpY = inputValue[0] - 'A';
		if (tmpY > 'Z' - 'A') { tmpY -= 'a' - 'A'; }
		tmpX = inputValue[1] - 'A';
		if (tmpX > 'Z' - 'A') { tmpX -= 'a' - 'A'; }
		if (inputValue[2] == '!')
		{
			isFlag = 1;
		}

		if (tmpX >= size || tmpY >= size || tmpX < 0 || tmpY < 0)
		{
			printf("Wrong Data Inputed.\n");
			system("PAUSE");
			goto retry;
		}

		isEnd = discover(tmpX, tmpY, size, 1, isFlag);

		
	}
	printBoard(size, remaining, isEnd);
	printf("Fail\n");
	system("PAUSE");

	return;
}

int main()
{
	int boardSize = MIN_VALUE;
	int bombsNum = -1;
	
	while (1)
	{
		system("cls");

		boardSize = -1;
		while (boardSize < MIN_VALUE || boardSize > MAX_VALUE)
		{
			printf("Map Size(n*n) : ");
			scanf("%d", &boardSize);
			if (boardSize < MIN_VALUE || boardSize > MAX_VALUE)
			{
				system("cls");
				printf("Wrong data Inputed.\n");
			}
		}

		system("cls");
		printf("Map Size(n*n) : %d\n", boardSize);

		bombsNum = -1;
		while (bombsNum < 0 || bombsNum > boardSize * boardSize / 8 + 1)
		{
			printf("How may bombs in map? : ");
			scanf("%d", &bombsNum);
			if (bombsNum < 0 || bombsNum > boardSize * boardSize / 8 + 1)
			{
				system("cls");
				printf("Wrong data Inputed.\n");
				printf("Map Size(n*n) : %d\n", boardSize);
			}
		}
		getchar(); // Clear Buffer

		reset();
		play(boardSize, bombsNum);
	}
	return 0;
}