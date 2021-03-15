#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

void clearScreen();
void delay(int speed);
void mirror(char displayBoard[250][250]);
void rotate90(char displayBoard[250][250]);
void rotate270(char displayBoard[250][250]);
void deleteBoard(char displayBoardChar[250][250]);
void display0(char displayBoardChar[250][250], int boardSizeRow, int boardSizeCol);
void display90(char displayBoardChar[250][250], int boardSizeRow, int boardSizeCol);
void display270(char displayBoardChar[250][250],int boardSizeRow, int boardSizeCol);
void displayMr(char displayBoardChar[250][250],int boardSizeRow, int boardSizeCol);
void compose(int isInception, string input, char symbol, char displayBoardChar[250][250],int inputSize, int coordY, int coordX, int boardSizeRow, int boardSizeCol);

int main()
{

	//----------------------------------INPUT FILE & GET INPUT FROM USER----------------------------------//

//declaration of variables
	ifstream inputFile;
	string filename;
	char symbol;
	int speed;
	string input;
	int coordX, coordY;
	int boardSizeRow, boardSizeCol;
	string direction;
	string wrapmou; 
	wrapmou = "noWr";															//initializes wrapmou as no wrap around will happen
	string rotatemou; 
	rotatemou = "noRot";														//initializes rotatemou as no rotation will happen
	string optional;
	int inputSize;
	int limit = 25;

	cout << "Welcome to this program!" << endl << "Please enter the name of the file that you want to process, including the type name of your file." << endl;
	cout << "Example: unagi.txt" << endl;
	cout << "Your filename: " ;
	cin >> filename;

	cout << endl;

//getting input from the user through the file and check is there any invalid input, if yes: exit from program
	inputFile.open(filename);
	if (inputFile)
	{
		while (inputFile >> symbol)
		{
    		if (symbol == ' ')
    		{
    			cout << "You have entered invalid symbol." << endl << "Exiting from program..." << endl;
    			return 0;
			}
    		else
    		{
    			cout << "You chose to use " << symbol << " to draw your characters." << endl;
			}
    		break;
		}
		while (inputFile >> speed)
		{
			if (speed < 0 || speed > 10 )
			{
				cout << "You have entered invalid speed. Speed can only be in between 1-10." << endl << "Exiting from program..." << endl;
				return 0;
			}
			else
			{
				cout << "You chose to use the speed of " << speed << " as your speed of refresh cycle." << endl;
			}
			break;
		}

		inputFile.ignore();
		getline(inputFile, input);
		cout << "You chose to display: " << input << endl;
		inputSize = input.length();

		if (input.length() > limit)
   		{
			cout << "You entered an input which is over 25 characters." << endl << "Exiting from program..." << endl;
	      	return 0;
   		}

		while (inputFile >> coordX >> coordY)
		{
			cout << "Your anchor point is: " << coordX << " , " << coordY << " (x, y)." << endl;
			break;
		}
		while (inputFile >> boardSizeRow >> boardSizeCol)
		{
			if (boardSizeRow >= 0 && boardSizeCol >=0 )
			{
				cout << "Your display board size will be: " << boardSizeRow << " x " << boardSizeCol <<endl;
			}
			else
			{
				cout << "You have entered invalid size for your display board." << endl << "Exiting from program..." << endl;
			}

			break;
		}
	
		while (inputFile >> direction)
		{
			if (direction == "lr")
			{
				cout << "You chose to make your input move from left to right." << endl;
			}
			else if (direction == "rl")
			{
				cout << "You chose to make your input move from right to left." << endl;
			}
			else if (direction == "ud")
			{
				cout << "You chose to make your input move from up to down." << endl;
			}
			else if (direction == "du")
			{
				cout << "You chose to make your input move from down to up." << endl;
			}
			else if (direction == "st")
			{
				cout << "You chose to make your input to stay stationary." << endl;
			}
			else
			{
				cout << "You have entered invalid direction." << endl << "Exiting from program..." << endl;
				return 0;
			}
			break;
		}
				while (inputFile >> optional)									//if the user want to do wrap/rotation, here will overwrite the input that declared as no wrap & no rotation
				{
					if (optional == "wr")
					{
						cout << "You chose to make your input to wrap around the edges of the display board." << endl;
						wrapmou = "wr";
					}
					if (optional == "rot90")
					{
						cout << "You chose to make your input to rotate clockwise by 90 degrees." << endl;
						rotatemou = "rot90";
					}
					if (optional == "rot-90")
					{	
						cout << "You chose to make your input to rotate anti-clockwise by 90 degrees." << endl;
						rotatemou = "rot-90";
					}
					if (optional == "mr")
					{
						cout << "You chose to make your input to have a vertical reflection which is the mirror effect." << endl;
						rotatemou = "mr";
					}
				}
		inputFile.close();
	}
	else													//when the filename can't be found/opened
	{
		cout << "Unable to open file." << endl;
		return 0;
	}


	if (inputFile >> coordX >> coordY)						//check the vailidity of anchor point
	{
		if( coordX < 0 || coordY < 0 || coordX > (boardSizeRow-1) || coordY > (boardSizeCol-1))
    	{
			cout << "You have entered invalid anchor point for your display board." << endl << "Exiting from program..." << endl;
			return 0;
		}
	}
	
	coordY = boardSizeRow - 1 - coordY;

//ask for input of timeSteps and to do the inception or not
	int timeSteps;
	cout << endl << "Please choose how many time steps you want to display the input." << endl << "Each time step represents one refresh cycle." << endl;
	cin >> timeSteps;
	if (!timeSteps || timeSteps <= 0 )
	{
		cout << "You have entered invalid time steps." << endl << "Exiting from program...";
		return 0;
	}
	cout << endl;

//BONUS: INCEPTION
	char incept;
	cout << "The 'inception' effect means that each character of your input is drawn using its own character. " << endl << "If you wish to implement it please enter Y, if no please enter any characters." << endl;
	cin >> incept;

	bool isInception;

	if (incept == 'Y' || incept == 'y')
	{
		isInception = true;
	}
	else
	{
		isInception = false;
	}

	//----------------------------------CREATE DISPLAY BOARD FOR CHARACTER (2D ARRAY) ----------------------------------//

	char displayBoardChar[250][250];

	for (int row = 0; row < 250; row++)										//Fill the whole displayBoardChar array with blanks(default value)
	{
		for (int col = 0; col < 250; col++)									//So that garbage values will not appear
		{
			displayBoardChar[row][col] = ' ';
		}
	}

	//----------------------------------LET CHAR TO BE DISPLAYED WITH DIRECTION AFTER ROTATION----------------------------------//

	int choice;								//Assigning number to direction choices to do switch case
	if (direction == "lr")
	{choice = 1;}
	else if (direction == "rl")
	{choice = 2;}
	else if (direction == "ud")
	{choice = 3;}
	else if (direction == "du")
	{choice = 4;}
	else if (direction == "st")														
	{choice = 5;}

	switch (choice)
	{
		case 1:													//when the string needs to move from left to right
		{
			for(int x = 0; x < timeSteps; x++)
            {
                if(rotatemou == "noRot")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    display0(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX++;
                }
                else if(rotatemou == "rot90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate90(displayBoardChar);
                    display90(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY--;
                }
                else if(rotatemou == "rot-90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate270(displayBoardChar);
                    display270(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY++;
                }
                else if(rotatemou == "mr")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    mirror(displayBoardChar);
                    displayMr(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX++;
                }
            }
            break;
		}

		case 2:													//when the string needs to move from right to left
		{
			for(int x = 0; x < timeSteps; x++)
            {
                if(rotatemou == "noRot")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    display0(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX--;
                }
                else if(rotatemou == "rot90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate90(displayBoardChar);
                    display90(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY++;
                }
                else if(rotatemou == "rot-90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate270(displayBoardChar);
                    display270(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY--;
                }
                else if(rotatemou == "mr")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    mirror(displayBoardChar);
                    displayMr(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX--;
                }
            }
            break;
		}

		case 3:													//when the string needs to move from up to down
		{
			for(int x = 0; x < timeSteps; x++)
            {
               if(rotatemou == "noRot")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    display0(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY++;
                }
                else if(rotatemou == "rot90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate90(displayBoardChar);
                    display90(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX++;
                }
                else if(rotatemou == "rot-90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate270(displayBoardChar);
                    display270(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX--;
                }
                else if(rotatemou == "mr")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    mirror(displayBoardChar);
                    displayMr(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY--;
                }
            }
            break;
		}

		case 4:													//when the string needs to move from down to up
		{
			for(int x = 0; x < timeSteps; x++)
            {
                 if(rotatemou == "noRot")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    display0(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY--;
                }
                else if(rotatemou == "rot90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate90(displayBoardChar);
                    display90(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX--;
                }
                else if(rotatemou == "rot-90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate270(displayBoardChar);
                    display270(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordX++;
                }
                else if(rotatemou == "mr")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    mirror(displayBoardChar);
                    displayMr(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                    delay(speed);
                    coordY++;
                }
            }
            break;
		}

		case 5:													//when the string needs to stay stationary
		{
			for(int x = 0; x < timeSteps; x++)
            {
                if(rotatemou == "noRot")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    display0(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                }
                else if(rotatemou == "rot90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate90(displayBoardChar);
                    display90(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                }
                else if(rotatemou == "rot-90")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    rotate270(displayBoardChar);
                    display270(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                }
                else if(rotatemou == "mr")
                {
                    deleteBoard(displayBoardChar);
                    compose(isInception, input, symbol, displayBoardChar, inputSize, coordY, coordX, boardSizeRow, boardSizeCol);
                    mirror(displayBoardChar);
                    displayMr(displayBoardChar, boardSizeRow, boardSizeCol);
                    clearScreen();
                }
            }
            break;
		}
	}
}

void delay(int speed)
{
    speed = (300000000 / speed);			//The bigger value of speed inputted by user, the smaller the value for the FOR LOOP to loop
    for( int i = 0; i < speed; i++ )		//So the speed will be fastest at speed of 10, slowest at speed of 1
	{}
}

void clearScreen()
{
   system( "cls" );
}

void mirror(char displayBoardChar[250][250])							//mirroring the whole display board of my string that will be printed in
{
	for (int i = 0; i < 250/2; i++)
	{
		for (int j = 0; j < 250; j++)
		{
			char temp = displayBoardChar[i][j];
			displayBoardChar[i][j] = displayBoardChar[250-1-i][j];
			displayBoardChar[250-1-i][j]= temp;
		}
	}
}

void rotate90(char displayBoardChar[250][250])							//rotating 90 degrees clockwise of the whole display board of my string that will be printed in
{
    for (int i = 0; i < 250/2; i++)
    {
        for (int j = i; j < 250 - i - 1; j++)
        {
            int temp = displayBoardChar[i][j];
            displayBoardChar[i][j] = displayBoardChar[250-1-j][i];
            displayBoardChar[250-1-j][i] = displayBoardChar[250-1-i][250-1-j];
            displayBoardChar[250-1-i][250-1-j] = displayBoardChar[j][250-1-i];
            displayBoardChar[j][250-1-i] = temp;
        }
    }
}

void rotate270(char displayBoardChar[250][250])							//rotating 90 degrees anticlockwise of the whole display board of my string that will be printed in
{
	for (int i = 0; i < 250 / 2; i++)
    {
        for (int j = i; j < 250 - i -1; j++)
        {
            int temp = displayBoardChar[i][j];
            displayBoardChar[i][j] = displayBoardChar[j][250-1-i];
            displayBoardChar[j][250-1-i] = displayBoardChar[250-1-i][250-1-j];
            displayBoardChar[250-1-i][250-1-j] = displayBoardChar[250-1-j][i];
            displayBoardChar[250-1-j][i] = temp;
        }
    }
}

void display0(char displayBoardChar[250][250],int boardSizeRow, int boardSizeCol)		//displaying the whole display board of my string that will be printed in (no rotation)
{
    for(int i = 0; i < boardSizeRow; i++)
    {
        for(int j = 0; j < boardSizeCol; j++)
        {
            cout << displayBoardChar[i][j];
        }
        cout << endl;
    }
    //clearScreen();
}

void display90(char displayBoardChar[250][250],int boardSizeRow, int boardSizeCol)		//displaying the whole display board of my string that will be printed in (rotation 90 degrees clockwise)
{
    for(int i = 0; i < boardSizeRow; i++)
    {
        for(int j = 0; j < boardSizeCol; j++)
        {
            cout << displayBoardChar[i][j + 250 - boardSizeCol];
        }
        cout << endl;
    }
}

void display270(char displayBoardChar[250][250],int boardSizeRow, int boardSizeCol)		//displaying the whole display board of my string that will be printed in (rotation 90 degrees anticlockwise)
{
    for(int i = 0; i < boardSizeRow; i++)
    {
        for(int j = 0; j < boardSizeCol; j++)
        {
            cout << displayBoardChar[i + 250 - boardSizeRow][j];
        }
        cout << endl;
    }
}

void displayMr(char displayBoardChar[250][250],int boardSizeRow, int boardSizeCol)		//displaying the whole display board of my string that will be printed in (mirror,flip)
{
    for(int i = 0; i < boardSizeRow; i++)
    {
        for(int j = 0; j < boardSizeCol; j++)
        {
            cout << displayBoardChar[i + 250 - boardSizeRow][j];
        }
        cout << endl;
    }
}

void deleteBoard(char displayBoardChar[250][250])									//putting in blanks in the 2d array to prevent garbage values
{
    for(int i = 0; i < 250; i++)
    {
        for(int j = 0; j < 250; j++)
        {
            displayBoardChar[i][j] = ' ';
        }
    }
}

void compose(int isInception, string input, char symbol, char displayBoardChar[250][250],int inputSize, int coordY, int coordX, int boardSizeRow, int boardSizeCol)
{
//assigning the input(string) inputted by user to its respective alphabets, change the symbol used(chose by the user)
    char a [7][9]={{"    #   "},{"   # #  "},{"  #   # "},{" #     #"},{" #######"},{" #     #"},{" #     #"}};
    char b [7][9]={{" ###### "},{" #     #"},{" #     #"},{" ###### "},{" #     #"},{" #     #"},{" ###### "}};
	char c [7][9]={{"   #### "},{"  #     "},{" #      "},{" #      "},{" #      "},{"  #     "},{"   #### "}};
	char d [7][9]={{" ###### "},{" #     #"},{" #     #"},{" #     #"},{" #     #"},{" #     #"},{" ###### "}};
	char e [7][9]={{" #######"},{" #      "},{" #      "},{" ###### "},{" #      "},{" #      "},{" #######"}};
	char f [7][9]={{" #######"},{" #      "},{" #      "},{" ###### "},{" #      "},{" #      "},{" #      "}};
	char g [7][9]={{"  ######"},{" #      "},{" #      "},{" #      "},{" #   ###"},{" #    ##"},{"  ######"}};
	char h [7][9]={{" #     #"},{" #     #"},{" #     #"},{" #######"},{" #     #"},{" #     #"},{" #     #"}};
	char I [7][9]={{" #######"},{"    #   "},{"    #   "},{"    #   "},{"    #   "},{"    #   "},{" #######"}};
	char J [7][9]={{"       #"},{"       #"},{"       #"},{"       #"},{"       #"},{" #     #"},{"  ##### "}};
	char k [7][9]={{" #    # "},{" #   #  "},{" #  #   "},{" ###    "},{" #  #   "},{" #   #  "},{" #    # "}};
	char l [7][9]={{" #      "},{" #      "},{" #      "},{" #      "},{" #      "},{" #      "},{" #######"}};
    char m [7][9]={{" #     #"},{" ##   ##"},{" # # # #"},{" #  #  #"},{" #     #"},{" #     #"},{" #     #"}};
	char n [7][9]={{" #     #"},{" ##    #"},{" # #   #"},{" #  #  #"},{" #   # #"},{" #    ##"},{" #     #"}};
	char o [7][9]={{"   ###  "},{"  #   # "},{" #     #"},{" #     #"},{" #     #"},{"  #   # "},{"   ###  "}};
	char p [7][9]={{" ###### "},{" #     #"},{" #     #"},{" ###### "},{" #      "},{" #      "},{" #      "}};
	char q [7][9]={{"  ##### "},{" #     #"},{" #     #"},{" #     #"},{" #   # #"},{" #    # "},{"  #### #"}};
	char r [7][9]={{" ###### "},{" #     #"},{" #     #"},{" ###### "},{" #   #  "},{" #    # "},{" #     #"}};
	char s [7][9]={{"  ##### "},{" #     #"},{" #      "},{"  ##### "},{"       #"},{" #     #"},{"  ##### "}};
	char t [7][9]={{" #######"},{"    #   "},{"    #   "},{"    #   "},{"    #   "},{"    #   "},{"    #   "}};
	char u [7][9]={{" #     #"},{" #     #"},{" #     #"},{" #     #"},{" #     #"},{" #     #"},{"  ##### "}};
	char v [7][9]={{" #     #"},{" #     #"},{" #     #"},{" #     #"},{"  #   # "},{"   # #  "},{"    #   "}};
	char w [7][9]={{" #     #"},{" #     #"},{" #     #"},{" #  #  #"},{" #  #  #"},{" ##   ##"},{" #     #"}};
	char x [7][9]={{" #     #"},{" #     #"},{"  #   # "},{"    #   "},{"  #   # "},{" #     #"},{" #     #"}};
	char y [7][9]={{" #     #"},{" #     #"},{"  #   # "},{"    #   "},{"    #   "},{"    #   "},{"    #   "}};
	char z [7][9]={{" #######"},{"      # "},{"     #  "},{"    #   "},{"   #    "},{"  #     "},{" #######"}};

    char zero  [7][9]={{" #######"},{" #     #"},{" #   ###"},{" #  #  #"},{" ###   #"},{" #     #"},{" #######"}};
	char one   [7][9]={{"    #   "},{"  ###   "},{"    #   "},{"    #   "},{"    #   "},{"    #   "},{" #######"}};
	char two   [7][9]={{"  ##### "},{" #     #"},{"      # "},{"    ##  "},{"   #    "},{"  #     "},{" #######"}};
	char three [7][9]={{" #######"},{"      # "},{"     #  "},{"    ### "},{"       #"},{" #     #"},{"  ##### "}};
	char four  [7][9]={{"     ## "},{"    # # "},{"   #  # "},{"  #   # "},{" #######"},{"      # "},{"      # "}};
	char five  [7][9]={{" #######"},{" #      "},{" #      "},{" ###### "},{"       #"},{" #     #"},{"  ##### "}};
	char six   [7][9]={{"   #### "},{"  #     "},{" #      "},{" ###### "},{" #     #"},{" #     #"},{"  ##### "}};
	char seven [7][9]={{" #######"},{"      # "},{"     #  "},{"    #   "},{"   #    "},{"   #    "},{"   #    "}};
	char eight [7][9]={{"  ##### "},{" #     #"},{" #     #"},{"  ##### "},{" #     #"},{" #     #"},{"  ##### "}};
	char nine  [7][9]={{"  ##### "},{" #     #"},{" #     #"},{"  ######"},{"       #"},{"      # "},{" #####  "}};
	char blank [7][9]={{"        "},{"        "},{"        "},{"        "},{"        "},{"        "},{"        "}};

    for (int i = 0; i < 7; i++) 														//Checking for the characters typed by user row by row (character size is 8 rows)
	{
		for(int charSeq = 0; charSeq < input.length(); charSeq++)						//Checking for every characters typed one by one in ascending order (1st,2nd,3rd,4th,5th,...)
    	{
			if (input[charSeq] == 'A' || input[charSeq] == 'a')							//Determine the characters typed are which characters "(A-Z)/(0-9)/(blank)"
        	{
	            for(int j=0;j<9;j++)
				{
					if (isInception == 0)												//if no inception effect
					{
						if (a[i][j] == '#')												//when the 2d array contains '#'
						a[i][j] = symbol;												//change it to the symbol inputted by user
					}
					else if (isInception == 1)											//if inception effect is needed
					{
						if(a[i][j] == '#')												//when the 2d array contains '#'
						a[i][j] = 'A';													//change it to A (when the character typed is A)
					}

					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=a[i][j];							//Assign displayBoardChar to the pattern of characters drew at first (2D array to 2D array)
          		} 																		//[i = rows] [j + (the initial coordinates of 1st/2nd/... char) = columns]
		    }

            else if (input[charSeq] == 'B' || input[charSeq] == 'b')
			{
           		for(int j=0;j<9;j++)
           		{
           			if (isInception == 0)
					{
						if(b[i][j] == '#')
						b[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if (b[i][j] == '#')
						b[i][j] = 'B';
					}
					else
					{}

				  	displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=b[i][j];
		  		}
           	}

            else if (input[charSeq] == 'C' || input[charSeq] == 'c')
          	{
           		for(int j=0;j<9;j++)
           		{
           			if (isInception == 0)
					{
						if(c[i][j] == '#')
						c[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if (c[i][j] == '#')
						c[i][j] = 'C';
					}
					else
					{}

				  	displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=c[i][j];
		  		}

           	}

            else if (input[charSeq] == 'D' || input[charSeq] == 'd')
           	{
              	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(a[i][j] == '#')
						d[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if (d[i][j] == '#')
						d[i][j] = 'D';
					}
					else
					{}
					displayBoardChar[coordY + i][j+(charSeq*8)]=d[i][j];
				}

           	}

			else if (input[charSeq] == 'E' || input[charSeq] == 'e')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(e[i][j] == '#')
						e[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(e[i][j] == '#')
						e[i][j] = 'E';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=e[i][j];
				}
           	}

            else if (input[charSeq] == 'F' || input[charSeq] == 'f')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(f[i][j] == '#')
						f[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(f[i][j] == '#')
						f[i][j] = 'F';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=f[i][j];
				}
           	}

			else if (input[charSeq] == 'G' || input[charSeq] == 'g')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(g[i][j] == '#')
					  	g[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(g[i][j] == '#')
						g[i][j] = 'G';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=g[i][j];
				}
           	}

            else if (input[charSeq] == 'H' || input[charSeq] == 'h')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(h[i][j] == '#')
						h[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(h[i][j] == '#')
						h[i][j] = 'H';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=h[i][j];
				}
           	}

			else if (input[charSeq] == 'I' || input[charSeq] == 'i')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(I[i][j] == '#')
						I[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(I[i][j] == '#')
						I[i][j] = 'I';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=I[i][j];
				}
           	}

			else if (input[charSeq] == 'J' || input[charSeq] == 'j')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(J[i][j] == '#')
						J[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(J[i][j] == '#')
						J[i][j] = 'J';
					}
					else
					{}
					displayBoardChar[coordY + i][ coordX + j+(charSeq*8)]=J[i][j];
				}
           	}

			else if (input[charSeq] == 'K' || input[charSeq] == 'k')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(k[i][j] == '#')
						k[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(k[i][j] == '#')
						k[i][j] = 'K';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=k[i][j];
				}
           	}

            else if (input[charSeq] == 'L' || input[charSeq] == 'l')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(l[i][j] == '#')
					  	l[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(l[i][j] == '#')
						l[i][j] = 'L';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=l[i][j];
				}
           	}

			else if (input[charSeq] == 'M' || input[charSeq] == 'm')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(m[i][j] == '#')
						m[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(m[i][j] == '#')
						m[i][j] = 'M';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=m[i][j];
				}
           	}

			else if (input[charSeq] == 'N' || input[charSeq] == 'n')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(n[i][j] == '#')
					  	n[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						n[i][j] = 'N';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=n[i][j];
				}
           	}

            else if (input[charSeq] == 'O' || input[charSeq] == 'o')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(o[i][j] == '#')
              			o[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(o[i][j] == '#')
						o[i][j] = 'O';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=o[i][j];
				}
           	}

			else if (input[charSeq] == 'P' || input[charSeq] == 'p')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(p[i][j] == '#')
              			p[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(p[i][j] == '#')
						p[i][j] = 'P';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=p[i][j];
				}
           	}

            else if (input[charSeq] == 'Q' || input[charSeq] == 'q')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(q[i][j] == '#')
              			q[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(q[i][j] == '#')
						q[i][j] = 'Q';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=q[i][j];
				}
           	}

            else if (input[charSeq] == 'R' || input[charSeq] == 'r')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(r[i][j] == '#')
              			r[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(r[i][j] == '#')
						r[i][j] = 'R';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=r[i][j];
				}
           	}

            else if (input[charSeq] == 'S' || input[charSeq] == 's')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(s[i][j] == '#')
              			s[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(s[i][j] == '#')
						s[i][j] = 'S';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=s[i][j];
				}
           	}

            else if (input[charSeq] == 'T' || input[charSeq] == 't')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(t[i][j] == '#')
              			t[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(t[i][j] == '#')
						t[i][j] = 'T';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=t[i][j];
				}
           	}

            else if (input[charSeq] == 'U' || input[charSeq] == 'u')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(u[i][j] == '#')
              			u[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(u[i][j] == '#')
						u[i][j] = 'U';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=u[i][j];
				}
           	}

            else if (input[charSeq] == 'V' || input[charSeq] == 'v')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(v[i][j] == '#')
              			v[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(v[i][j] == '#')
						v[i][j] = 'V';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=v[i][j];
				}
           	}

            else if (input[charSeq] == 'W' || input[charSeq] == 'w')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(w[i][j] == '#')
              			w[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(w[i][j] == '#')
						w[i][j] = 'W';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=w[i][j];
				}
           	}

            else if (input[charSeq] == 'X' || input[charSeq] == 'x')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(x[i][j] == '#')
              			x[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(x[i][j] == '#')
						x[i][j] = 'X';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=x[i][j];
				}
           	}

            else if (input[charSeq] == 'Y' || input[charSeq] == 'y')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(y[i][j] == '#')
              			y[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(y[i][j] == '#')
						y[i][j] = 'Y';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=y[i][j];
				}
           	}

            else if (input[charSeq] == 'Z' || input[charSeq] == 'z')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(z[i][j] == '#')
              			z[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(z[i][j] == '#')
						z[i][j] = 'Z';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=z[i][j];
				}
           	}

            else if(input[charSeq] == '0')
           	{
              	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(zero[i][j] == '#')
              			zero[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(zero[i][j] == '#')
						zero[i][j] = '0';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=zero[i][j];
				}
           	}

            else if(input[charSeq] == '1')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(one[i][j] == '#')
              			one[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(one[i][j] == '#')
						one[i][j] = '1';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=one[i][j];
				}
           	}

            else if(input[charSeq] == '2')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(two[i][j] == '#')
              			two[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(two[i][j] == '#')
						two[i][j] = '2';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=two[i][j];
				}
           	}

            else if(input[charSeq] == '3')
           	{
              	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(three[i][j] == '#')
              			three[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(three[i][j] == '#')
						three[i][j] = '3';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=three[i][j];
				}
           	}

            else if(input[charSeq] == '4')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(four[i][j] == '#')
              			four[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(four[i][j] == '#')
						four[i][j] = '4';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=four[i][j];
				}
           	}

            else if(input[charSeq] == '5')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(five[i][j] == '#')
              			five[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(five[i][j] == '#')
						five[i][j] = 'C';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=five[i][j];
				}
           	}

            else if(input[charSeq] == '6')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(six[i][j] == '#')
              			six[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(six[i][j] == '#')
						six[i][j] = '6';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=six[i][j];
				}
           	}

            else if(input[charSeq] == '8')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(seven[i][j] == '#')
              			seven[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(seven[i][j] == '#')
						seven[i][j] = '7';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=seven[i][j];
				}
           	}

			else if(input[charSeq] == '8')
            {
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(eight[i][j] == '#')
              			eight[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(eight[i][j] == '#')
						eight[i][j] = '8';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=eight[i][j];
				}
           	}

            else if(input[charSeq] == '9')
           	{
               	for(int j=0;j<9;j++)
              	{
              		if (isInception == 0)
              		{
              			if(nine[i][j] == '#')
              			nine[i][j] = symbol;
					}
					else if (isInception == 1)
					{
						if(nine[i][j] == '#')
						nine[i][j] = '9';
					}
					else
					{}
					displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=nine[i][j];
				}
           	}

			else
           	{
               	for(int j=0;j<9;j++)
		  		displayBoardChar[coordY + i][coordX + j+(charSeq*8)]=blank[i][j];
           	}
		}
	}
}
