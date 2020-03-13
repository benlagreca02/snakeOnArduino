//these numbers INCLUDE the borders (i.e. val of 12 will give play-space of 10)
//hopefully I can impliment it so that it works with varying grid sizes
const uint8_t GRID_MAX_X = 12;
const uint8_t GRID_MAX_Y = 12;
//honestly no idea if im using this data type right
const uint8_t headPosX = 6;
const uint8_t headPosY = 5;

//rememeber 0, 0 is the upper left corner
char grid[GRID_MAX_Y][GRID_MAX_X];


//this function will COMPLETLY wipe the grid, then add borders, i haven't done the latter yet tho :((
void ClearGrid()
{
    //sets whole grid to " - "
    for(uint8_t yCoor = 0; yCoor <= GRID_MAX_Y; yCoor++)
    {
        for(uint8_t xCoor = 0; xCoor <= GRID_MAX_X; xCoor++)
        {
            grid[yCoor][xCoor] = "-";
        }
    }
    //***************** PUT CODE FOR ADDING BORDERS HERE
}


void PrintGrid()
{
    for(uint8_t yCoor = 0; yCoor <= GRID_MAX_Y; yCoor++)
    {
        for(uint8_t xCoor = 0; xCoor <= GRID_MAX_X; xCoor++)
        {
            Serial.print(grid[yCoor][xCoor]);
        }
        Serial.println();
    }
}



void setup()
{
    Serial.begin(9600);
    delay(100);
    ClearGrid();
    PrintGrid();
}





void loop() {}
