//Snake on an arduino using the serial monitor

//these numbers INCLUDE the borders (i.e. val of 12 will give play-space of 10)
const uint8_t GRID_MAX_X = 13;
const uint8_t GRID_MAX_Y = 13;
const char EMPTY_SPACE = '-';
const char BORDER_PIECE = '#';
const char HEAD_PIECE = 'S';
const char SNAKE_BODY = 'X';
const char APPLE_CHAR = 'A';

//these are the Joystick Analog Pins
const uint8_t OPEN_ANALOG_PIN = 5;
const uint8_t XPIN = 0;
const uint8_t YPIN = 1;
const uint16_t UPPER_DEADBAND = 640; //512+128
const uint16_t LOWER_DEADBAND = 384; //512-128

//this is how long every "frame" lasts
const uint8_t REFRESH_RATE = 75;
const uint8_t GROWTH_RATE = 3;

//8 is up, 6 is right, 2 is down, 4 is left; these are just ambiguous numbers i picked
const uint8_t   UP_CONST    = 8;
const uint8_t   DOWN_CONST  = 2;
const uint8_t   LEFT_CONST  = 4;
const uint8_t   RIGHT_CONST = 6;

//these are the starting positions for the snake head, which should be right in the middle of the grid
uint8_t headX = GRID_MAX_X/2;
uint8_t headY = GRID_MAX_Y/2;

uint8_t appleX = 3;
uint8_t appleY = 3;

bool noApple = true;
bool gameOver = false;
uint16_t snakeLength = 3;


//rememeber 0, 0 is the upper left corner
char grid[GRID_MAX_Y][GRID_MAX_X];

int tailX[((GRID_MAX_X)*(GRID_MAX_Y))]; //total area of the grid, can't get any bigger than that youd win or lose first
int tailY[((GRID_MAX_X)*(GRID_MAX_Y))]; //should be GRID_MAX_X-2 and GRID_MAX_Y-2 but im making it bigger just incase


//this function will COMPLETLY wipe the grid, then add borders
void ClearGrid()
{
    //clearing the grid
    for(uint8_t yCoor = 0; yCoor < GRID_MAX_Y; yCoor++)
    {
        for(uint8_t xCoor = 0; xCoor < GRID_MAX_X; xCoor++)
        {
            grid[yCoor][xCoor] = EMPTY_SPACE;
        }
    }

    //border work
    for(uint8_t y = 0; y < GRID_MAX_Y; y++)
    {
        grid[y][0] = BORDER_PIECE;
        grid[y][GRID_MAX_X-1] = BORDER_PIECE;   //minus one because the GRID_MAX_X const is the length, the coords are 0 to MAX-1
    }
    for(uint8_t x = 0; x < GRID_MAX_X; x++)
    {
        grid[0][x] = BORDER_PIECE;
        grid[GRID_MAX_Y-1][x] = BORDER_PIECE;
    }
}



void PrintGrid()
{
    for(uint8_t yCoor = 0; yCoor < GRID_MAX_Y; yCoor++)
    {
        for(uint8_t xCoor = 0; xCoor < GRID_MAX_X; xCoor++)
        {
            Serial.print(grid[yCoor][xCoor]);
            Serial.print(" ");  //to make the grid less cramped when in terminal
        }
        Serial.println();
    }
}


uint8_t GetDir()
{
    //on the joystick im using, 0, 0 is upper left; that means down is 1023Y and right is 1023X
    uint16_t x = analogRead(XPIN);
    uint16_t y = analogRead(YPIN);

    bool xRight  = (x > UPPER_DEADBAND);
    bool xLeft    = (x < LOWER_DEADBAND);
    bool yDown  = (y > UPPER_DEADBAND);
    bool yUp = (y < LOWER_DEADBAND);

    //if someone presses up and left, horizontal movement is going to take priority
    if(yDown) return DOWN_CONST;
    else if(yUp) return UP_CONST;
    else if(xLeft) return LEFT_CONST;
    else if(xRight) return RIGHT_CONST;
    else return 0;
}


//this function gets the direction passed in then uses it to change the headX and headY variables
void MoveSnake(uint8_t dir)
{
    static uint8_t prevDir, currDir;
    
    //if there is no input, use the previous one
    if(dir==0) currDir = prevDir;
    //if there is input, use it
    if(dir !=0) currDir = dir;


    if(currDir == UP_CONST && prevDir != DOWN_CONST)
    {
        headY--;    //remmeber 0 is up, bigger num is down
        prevDir = currDir; //movement was successful
    }
    else if(currDir == DOWN_CONST && prevDir != UP_CONST)
    {
        headY++;    //remember 0 is up, big number down
        prevDir = currDir;
    }
    else if(currDir == LEFT_CONST && prevDir != RIGHT_CONST)
    {
        headX--;
        prevDir = currDir;
    }
    else if(currDir == RIGHT_CONST && prevDir != LEFT_CONST) 
    {
        headX++;
        prevDir = currDir;
    }
    else    //if there is a contradicting direction (pressing up after going down) just go in the previous direction
    {
        if(prevDir == UP_CONST) headY--;
        if(prevDir == DOWN_CONST) headY++;
        if(prevDir == LEFT_CONST) headX--;
        if(prevDir == RIGHT_CONST)headX++;
    }
    
    for(uint16_t i = snakeLength; i > 0 ; i--)
    {
        //scooch the coords up one since the snake is moving; this makes room for the head in the histroy
        tailX[i] = tailX[i-1];
        tailY[i] = tailY[i-1];
    }

    //check if we just ate an apple, look where the head is going to be
    if(grid[headY][headX] == APPLE_CHAR)
    {
        snakeLength += GROWTH_RATE;
        noApple = true;
    }

    if((grid[headY][headX] == BORDER_PIECE) or (grid[headY][headX] == SNAKE_BODY))
    {
        gameOver = true;
    }

    //these set the snakes head to the 0th position after room has been made
    tailY[0] = headY;
    tailX[0] = headX;
}


void WriteSnake()
{    
    grid[tailY[0]][tailX[0]] = HEAD_PIECE;
    
    //runs up the list of coords and makes those positions the body to print the snake
    for(uint8_t i = 1; i < snakeLength; i++)
    {
        grid[tailY[i]][tailX[i]] = SNAKE_BODY;
    }
}


//function places a new apple on the grid by assigning to the variables which are then read in the PrintGrid function
void WriteNewApple()
{
    bool pickingApple = true;
    while(pickingApple)
    {
        uint8_t reqAppleX = random(1, GRID_MAX_X);
        uint8_t reqAppleY = random(1, GRID_MAX_Y);
        if(grid[reqAppleY][reqAppleX] == EMPTY_SPACE)
        {
            appleX = reqAppleX;
            appleY = reqAppleY;
            pickingApple = false;
        }
    }
    noApple = false;

}



void setup()
{
    Serial.begin(9600);
    delay(REFRESH_RATE);

    ClearGrid();
    PrintGrid();
    //picks up noise and uses that noise for the seed for the randomization, reccomended way to get randomness in code
    randomSeed(analogRead(OPEN_ANALOG_PIN));
}



void loop() 
{
    bool gameStarted;
    if(GetDir() != 0)
    {
        gameStarted = true;
    }
    else
    {
        gameStarted = false;
    }
    

    while(gameStarted && !gameOver)
    {
        delay(1000);
        
        ClearGrid();    //makes the grid blank
        if(noApple)     //if theres no apple make one
        {
            WriteNewApple();
        }    
        grid[appleY][appleX] = APPLE_CHAR;
        MoveSnake(GetDir());            //get the direction and move the snake
        WriteSnake();                   //writes snake to grid
        PrintGrid();                    //this is what does the serial outs and prints the grid with the snake and everything
    }
}
