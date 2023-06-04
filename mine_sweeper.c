#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Max_Board_Size 22
// Name: Moshe Yakubov
// Id: 323667832

//Creating a struct variable (to each cell in the board).
typedef struct Board
{
	int Close_Bombs_In_The_Area;
	bool Is_Bomb;
	int Visibility_Status;
} Board;
// Announcement of functions
void PrintGameOpening();
int FindBombsInTheArea(Board board[][Max_Board_Size], int row, int col, int rows, int cols);
void BoardPreperation(Board board[][Max_Board_Size], int rows, int cols);
void PrintBoard(Board board[][Max_Board_Size], int rows, int cols);
void PrintNextMove();
void OpenCellsWithoughtBombs(Board board[][Max_Board_Size], int rows, int cols, int number_of_cells_to_open);
void OpenCells(Board board[][Max_Board_Size], int rows, int cols);
int OpenCellsNearby(Board board[][Max_Board_Size], int rows, int cols, int current_row, int current_col, int opened_cells);
int OpenTheArea(Board board[][Max_Board_Size], int rows, int cols, int current_row, int current_col, int opened_cells);
bool IsChoiceInBound(int rows, int cols, int row, int col);
bool IsOpenOrFlag(char open_or_flag);
void NotInBoundMessages(int rows, int cols, int current_row, int current_col);




void main()
{
	Board board[Max_Board_Size][Max_Board_Size];
	int choose_the_boards_size, current_row, current_col, check_the_input, opened_cells = 0, number_of_rows = 0, number_of_cols = 0, i, j;
	bool game_over = false;
	char open_or_flag = '\0';
	// Setting variables
	PrintGameOpening();
	// printing the opening anouncment of the game
	check_the_input = scanf("%d", &choose_the_boards_size);
	// the user need to choose the size of the table in wich he wants to play
	while (choose_the_boards_size < 0 || choose_the_boards_size > 4)
	{
		PrintGameOpening();
		check_the_input = scanf("%d", &choose_the_boards_size);
	}
	// as long as the input is invalid, the program asks him to choose a valid input and printing the opening anouncment of the game
	switch (choose_the_boards_size)
	{
	case 1:
		number_of_rows = 8;
		number_of_cols = 8;
		BoardPreperation(board, number_of_rows, number_of_cols);
		break;
	case 2:
		number_of_rows = 12;
		number_of_cols = 12;
		printf("\n");
		BoardPreperation(board, number_of_rows, number_of_cols);
		break;
	case 3:
		number_of_rows = 15;
		number_of_cols = 15;
		BoardPreperation(board, number_of_rows, number_of_cols);
		break;
	case 4:
		printf("Please enter the size of the board (Lines <= 22 and Cols <= 22  ):");
		check_the_input = scanf("%d%d", &number_of_rows, &number_of_cols);
		BoardPreperation(board, number_of_rows, number_of_cols);
		break;
	case 0:
		printf("Hope you have enjoyed playing !!!!!");
		return;
	}
	/*the program is making the board according to the user's choice,
	unlesss he want to quit the game and then it's printing a breakup message and finish the game*/
	while (!game_over)
	{
		// the program runs unless the game is over (the user opened a cell that has a bomb)
		printf("\n");
		// the program does the following:
		PrintBoard(board, number_of_rows, number_of_cols);
		// print the updated board
		PrintNextMove();
		// print the "next move" message
		check_the_input = scanf("%d%d", &current_row, &current_col);
		if (current_row != -1)
			check_the_input = scanf(" %c", &open_or_flag);
		// ask him to put an 3 inputs for: the rows, the cols, if he wants to open it or mark it as a flag
		while (!IsChoiceInBound(number_of_rows, number_of_cols, current_row, current_col) || !IsOpenOrFlag(open_or_flag))
		{
			if (current_row == -1 && current_col >= 0)
			{
				OpenCellsWithoughtBombs(board, number_of_rows, number_of_cols, current_col);
				opened_cells += current_col;
				break;
			}
			// there is a shortcut function here for opening cells
			NotInBoundMessages(number_of_rows, number_of_cols, current_row, current_col);
			check_the_input = scanf("%d%d", &current_row, &current_col);
			if (current_row != -1)
				check_the_input = scanf(" %c", &open_or_flag);
			// the program checks if the inputs are valid and if not, it prints a proper message and asks him to choose again
		}

		if (board[current_row][current_col].Is_Bomb == true && (open_or_flag == 'o' || open_or_flag == 'O'))
		{
			printf(" You've hit a bomb! Game over!\n");
			OpenCells(board, number_of_rows, number_of_cols);
			game_over = true;
			continue;
		}
		// if the user hit a bomb, the game is over and the board is fully open with the locations the boms were located (and the program stops)
		else
		{
			if (board[current_row][current_col].Visibility_Status == 1)
			{
				printf("Invalid move, please enter valid choice!\n");
			}
			// if the user chose to open a cell already opened, the program tells him that he chose invalid choice
			else
			{
				if (open_or_flag == 'f' || open_or_flag == 'F')
					board[current_row][current_col].Visibility_Status = 2;
				// if the user chose to mark the cell as a flag, the program update the cell's status
				else
				{
					if (open_or_flag == 'o' || open_or_flag == 'O')
					{
						if (board[current_row][current_col].Close_Bombs_In_The_Area != 0)
						{
							board[current_row][current_col].Visibility_Status = 1;
							opened_cells++;
						}
						else
						{
							if (board[current_row][current_col].Visibility_Status == 2)
							{
								board[current_row][current_col].Visibility_Status = 1;
								opened_cells++;
							}
							opened_cells = OpenTheArea(board, number_of_rows, number_of_cols, current_row, current_col, opened_cells);
						}
						// if the user opened a cell that isn't a bomb, the program opens the empty cells around 
					}
				}
			}
		}
		if (opened_cells >= (number_of_rows * number_of_cols) - sqrt(number_of_rows * number_of_cols))
		{
			PrintBoard(board, number_of_rows, number_of_cols);
			printf("You won! congratulations! CHAMPION!");
			game_over = true;
		}
		// if all the cells (which are not bombs) were opened, the user won and the game is over (the program stops).
	}
}
// The function prints the opening message of the game
void PrintGameOpening()
{
	printf("Welcome to Minesweeper!\n\n");
	printf("Please choose one of the following options and enter it's number:\n\n1 - for size 8X8\n\n2 - for size 12X12\n\n3 - for size 15X15\n\n4 - for custom size\n\n0 - Exit\n\n");
}

/*The function sets a number of bombs (according to the chosen rows) in a different positions on the board.*/
void BoardPreperation(Board board[][Max_Board_Size], int rows, int cols)
{
	int i, j, k, bomb_location = 0, random_number, number_of_bombs, counter = 0;
	number_of_bombs = sqrt(rows * cols);
	srand(time(NULL));
	int bombs[Max_Board_Size];
	bombs[number_of_bombs - 1] = -1;
	bool all_numbers_are_different = true;
	while (bombs[number_of_bombs - 1] == -1)
	{
		random_number = rand() % (rows * cols);
		for (k = 0; k < number_of_bombs; k++)
		{
			if (bombs[k] == random_number)
				all_numbers_are_different = false;
		}
		if (all_numbers_are_different)
		{
			bombs[bomb_location] = random_number;
			bomb_location++;
		}
		all_numbers_are_different = true;
	}
	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
		{
			board[i][j].Visibility_Status = 0;
			board[i][j].Is_Bomb = false;
			for (k = 0; k < number_of_bombs; k++)
				if (bombs[k] == counter)
					board[i][j].Is_Bomb = true;
			counter++;
		}
	for (i = 0; i < rows; i++)
		for (j = 0; j < cols; j++)
			board[i][j].Close_Bombs_In_The_Area = FindBombsInTheArea(board, i, j, rows, cols);
}


// The function return the amount of the all bombs located around the cell we opened
int FindBombsInTheArea(Board board[][Max_Board_Size], int row, int col, int rows, int cols)
{
	int bombs_found_in_the_area = 0;
	int i = row;
	int j = col;
	if (i > 0 && j > 0 && board[i - 1][j - 1].Is_Bomb == true)
		bombs_found_in_the_area++;
	if (i > 0 && board[i - 1][j].Is_Bomb == true)
		bombs_found_in_the_area++;
	if (i > 0 && j < cols - 1 && board[i - 1][j + 1].Is_Bomb == true)
		bombs_found_in_the_area++;
	if (j > 0 && board[i][j - 1].Is_Bomb == true)
		bombs_found_in_the_area++;
	if (j < cols - 1 && board[i][j + 1].Is_Bomb == true)
		bombs_found_in_the_area++;
	if (j > 0 && i < rows - 1 && board[i + 1][j - 1].Is_Bomb == true)
		bombs_found_in_the_area++;
	if (i < rows - 1 && board[i + 1][j].Is_Bomb == true)
		bombs_found_in_the_area++;
	if (i < rows - 1 && j < cols - 1 && board[i + 1][j + 1].Is_Bomb == true)
		bombs_found_in_the_area++;
	return bombs_found_in_the_area;
}

// The function prints the board
void PrintBoard(Board board[][Max_Board_Size], int rows, int cols)
{
	int col_number = 0;
	int i;
	printf("     ");
	for (i = 0; i < cols; i++)
	{
		if (i >= 0 && i <= 9)
			printf("   %d", i);
		else
			printf("  %d", i);
	}
	printf("\n");
	printf("   ");
	for (i = 0; i <= (cols + 1) * 4 - 1; i++)
		printf("_");
	printf("\n");
	for (int row = 0; row < rows; row++)
	{
		if (col_number >= 0 && col_number <= 9)
			printf("    %d |", col_number);
		else
			printf("   %d |", col_number);

		col_number++;
		for (int col = 0; col < cols; col++)
		{
			if (board[row][col].Is_Bomb == false && board[row][col].Visibility_Status == 0)
				printf("%s", " X |");

			if (board[row][col].Is_Bomb == true && board[row][col].Visibility_Status == 0)
				printf("%s", " X |");

			if (board[row][col].Is_Bomb == true && board[row][col].Visibility_Status == 1)
				printf("%s", " * |");
			if (board[row][col].Visibility_Status == 2)
			{
				printf("%s", " F |");
			}
			if (board[row][col].Is_Bomb == false && board[row][col].Visibility_Status == 1)
			{

				if (board[row][col].Close_Bombs_In_The_Area == 0)
				{
					printf("%s", "   |");
				}
				else
				{
					printf(" %d |", board[row][col].Close_Bombs_In_The_Area);
				}

			}
		}
		if (row != rows)
			printf("\n");
	}
	printf("   ");
	for (i = 0; i <= (cols + 1) * 4 - 1; i++)
		printf("_");
	printf("\n");
}

// The function prints a "next move" message
void PrintNextMove()
{
	printf("\nPlease enter your move, row and column : ");
}
/*The function opens all the cells in the board*/
void OpenCells(Board board[][Max_Board_Size], int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			board[i][j].Visibility_Status = 1;
		}
	}
	PrintBoard(board, rows, cols);
}

/*The function gets a two-dimensional array of Board type and also an int type variable.
The function opens the number of cells we inserted not including the cells where the bombs are located.*/
void OpenCellsWithoughtBombs(Board board[][Max_Board_Size], int rows, int cols, int number_of_cells_to_open)
{
	int i, j, cells_already_opened = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (board[i][j].Visibility_Status != 1 && board[i][j].Is_Bomb == 0 && cells_already_opened != number_of_cells_to_open)
			{
				board[i][j].Visibility_Status = 1;
				cells_already_opened++;
			}
		}
	}
}

// this function opens the all empty cells nearby the cell that opened (until we get to a cell that has a bomb, a flag and so on (a cell that isn't empty)
int OpenTheArea(Board board[][Max_Board_Size], int rows, int cols, int current_row, int current_col, int opened_cells)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (IsChoiceInBound(rows, cols, current_row + i, current_col + j) && board[current_row + i][current_col + j].Visibility_Status == 0)
				if (board[current_row][current_col].Close_Bombs_In_The_Area == 0 && board[current_row + i][current_col + j].Visibility_Status == 0)
				{
					board[current_row + i][current_col + j].Visibility_Status = 1;
					opened_cells++;
					opened_cells = OpenCellsNearby(board, rows, cols, current_row, current_col, opened_cells);
					for (int i = -1; i <= 1; i++)
						for (int j = -1; j <= 1; j++)
							if (IsChoiceInBound(rows, cols, current_row + i, current_col + j))
								opened_cells = OpenTheArea(board, rows, cols, current_row + i, current_col + j, opened_cells);
				}
	return opened_cells;
}
// this function opens the empty cells around the cell that opened
int OpenCellsNearby(Board board[][Max_Board_Size], int rows, int cols, int current_row, int current_col, int opened_cells)
{
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (IsChoiceInBound(rows, cols, current_row + i, current_col + j) && board[current_row + i][current_col + j].Is_Bomb == false && board[current_row + i][current_col + j].Visibility_Status == 0)
			{
				board[current_row + i][current_col + j].Visibility_Status = 1;
				opened_cells++;
			}
	return opened_cells;
}
// this function checks if he row and the col that chosen are located in the boundaries of the board
bool IsChoiceInBound(int rows, int cols, int current_row, int current_col)
{
	return ((0 <= current_row) && (current_row < rows) && (0 <= current_col) && (current_col < cols));
}
// this function check if the user chose correctly to open a cell or to mark it as a flag
bool IsOpenOrFlag(char open_or_flag)
{
	return ((open_or_flag == 'o') || (open_or_flag == 'O') || (open_or_flag == 'f') || (open_or_flag == 'F'));
}
// this function prints a proper anouncement if the user put an invlid input of row or col
void NotInBoundMessages(int rows, int cols, int current_row, int current_col)
{
	if (current_row != -1 && !IsChoiceInBound(rows, cols, current_row, current_col))
	{
		if (current_row < 0 || current_row> rows - 1)
			printf("Number of lines is out of range!\n");
		if (current_col < 0 || current_col > cols - 1)
			printf("Number of columns is out of range!\n");
		printf("Please enter the size of the board(Lines <= 22 and Cols <= 22) :");
	}
	else
		printf("Please enter a valid choice!\n");
}


