//these numbers INCLUDE the borders (i.e. val of 12 will give play-space of 10)
const uint8_t GRID_MAX_X = 13;
const uint8_t GRID_MAX_Y = 13;
const char EMPTY_SPACE = '-';
const char BORDER_PIECE = '#';

//these are the Joystick Analog Pins
const uint8_t XPIN = 0;
const uint8_t YPIN = 1;
const uint16_t UPPER_DEADBAND = 640; //512+128
const uint16_t LOWER_DEADBAND = 384; //512-128

//8 is up, 6 is right, 2 is down, 4 is left
const uint8_t   UP_CONST    = 8;
const uint8_t   DOWN_CONST  = 2;
const uint8_t   LEFT_CONST  = 4;
const uint8_t   RIGHT_CONST = 6;

//honestly no idea if im using this data type right

//these are the starting pos for the snake head, which should be right in the middle of the grid
uint8_t headPosX = GRID_MAX_X/2;
uint8_t headPosY = GRID_MAX_Y/2;

//rememeber 0, 0 is the upper left corner
char grid[GRID_MAX_Y][GRID_MAX_X];




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
        grid[y][GRID_MAX_X-1] = BORDER_PIECE;
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


uint16_t GetDir()
{
    //on the joystick im using, 0, 0 is upper left; that means down is 1023Y and right is 1023X
    uint16_t x = analogRead(XPIN);
    uint16_t y = analogRead(YPIN);
    bool xDown  = (x > UPPER_DEADBAND);
    bool xUp    = (x < LOWER_DEADBAND);
    bool yLeft  = (y > UPPER_DEADBAND);
    bool yRight = (y < LOWER_DEADBAND);

    //if someone presses up and left, horizontal movement is going to take priority
    if(xDown) return DOWN_CONST;
    else if(xUp) return UP_CONST;
    else if(yLeft) return LEFT_CONST;
    else if(yRight) return RIGHT_CONST;
    else return 0;
}






void setup()
{
    Serial.begin(9600);
    delay(100);

    ClearGrid();
    PrintGrid();
}


void loop() 
{
    Serial.println(GetDir());
    delay(200);
}