#include <TCB8000C.h>
#include <stdlib.h>

#define ALIVE 1
#define DEAD 0

#define bheight 30
#define bwidth 40 // must be devisable by 8

#define ALIVEch 219
#define DEADch 255

char cdisplay = 0;
char cbuff = 1;

unsigned int generation = 0;
unsigned int buffer[16];
int count = 0;
unsigned char board[2][bheight][bwidth/8] = {
//board 0
//0         8         16        24        32
{{B00000000,B00000000,B00000000,B00000000,B00000000}, //0
{B00000000,B00000000,B00000000,B01000000,B00000000}, //1
{B00000000,B00000000,B00000001,B01000000,B00000000}, //2
{B00000000,B00000110,B00000110,B00000000,B00011000}, //3
{B00000000,B00001000,B10000110,B00000000,B00011000}, //4
{B01100000,B00010000,B01000110,B00000000,B00000000}, //5
{B01100000,B00010001,B01100001,B01000000,B00000000}, //6
{B00000000,B00010000,B01000000,B01000000,B00000000}, //7
{B00000000,B00001000,B10000000,B00000000,B00000000}, //8
{B00000000,B00000110,B00000000,B00000000,B00000000}, //9
{B00000000,B00000000,B00000000,B00000000,B00000000}, //10
{B00000000,B00000000,B00000000,B00000000,B00000000}, //11
{B00000000,B00000000,B00000000,B00000000,B00000000}, //12
{B00000000,B00000000,B00000000,B00000000,B00000000}, //13
{B00000000,B00000000,B00000000,B00000000,B00000000}, //14
{B00001110,B00111000,B00000000,B00000000,B00000000}, //15
{B00000000,B00000000,B00000000,B00000000,B00000000}, //16
{B00100001,B01000010,B00000000,B00000000,B00000000}, //17
{B00100001,B01000010,B00000000,B00000000,B00000000}, //18
{B00100001,B01000010,B00000000,B00000000,B00000000}, //19
{B00001110,B00111000,B00000000,B00000000,B00000000}, //20
{B00000000,B00000000,B00000000,B00000000,B00000000}, //21
{B00001110,B00111000,B00000000,B00000000,B00000000}, //22
{B00100001,B01000010,B00000000,B00000000,B00000000}, //23
{B00100001,B01000010,B00000000,B00000000,B00000000}, //24
{B00100001,B01000010,B00000000,B00000000,B00000000}, //25
{B00000000,B00000000,B00000000,B00000000,B00000000}, //26
{B00001110,B00111000,B00000000,B00000000,B00000000}, //27
{B00000000,B00000000,B00000000,B00000000,B00000000}, //28
{B00000000,B00000000,B00000000,B00000000,B00000000}}, //29
//board 1
//0         8         16        24        32
{{B00000000,B00000000,B00000000,B00000000,B00000000}, //0
{B00000000,B00000000,B00000000,B00000000,B00000000}, //1
{B00000000,B00000000,B00000000,B00000000,B00000000}, //2
{B00000000,B00000000,B00000000,B00000000,B00000000}, //3
{B00000000,B00000000,B00000000,B00000000,B00000000}, //4
{B00000000,B00000000,B00000000,B00000000,B00000000}, //5
{B00000000,B00000000,B00000000,B00000000,B00000000}, //6
{B00000000,B00000000,B00000000,B00000000,B00000000}, //7
{B00000000,B00000000,B00000000,B00000000,B00000000}, //8
{B00000000,B00000000,B00000000,B00000000,B00000000}, //9
{B00000000,B00000000,B00000000,B00000000,B00000000}, //10
{B00000000,B00000000,B00000000,B00000000,B00000000}, //11
{B00000000,B00000000,B00000000,B00000000,B00000000}, //12
{B00000000,B00000000,B00000000,B00000000,B00000000}, //13
{B00000000,B00000000,B00000000,B00000000,B00000000}, //14
{B00000000,B00000000,B00000000,B00000000,B00000000}, //15
{B00000000,B00000000,B00000000,B00000000,B00000000}, //16
{B00000000,B00000000,B00000000,B00000000,B00000000}, //17
{B00000000,B00000000,B00000000,B00000000,B00000000}, //18
{B00000000,B00000000,B00000000,B00000000,B00000000}, //19
{B00000000,B00000000,B00000000,B00000000,B00000000}, //20
{B00000000,B00000000,B00000000,B00000000,B00000000}, //21
{B00000000,B00000000,B00000000,B00000000,B00000000}, //22
{B00000000,B00000000,B00000000,B00000000,B00000000}, //23
{B00000000,B00000000,B00000000,B00000000,B00000000}, //24
{B00000000,B00000000,B00000000,B00000000,B00000000}, //25
{B00000000,B00000000,B00000000,B00000000,B00000000}, //26
{B00000000,B00000000,B00000000,B00000000,B00000000}, //27
{B00000000,B00000000,B00000000,B00000000,B00000000}, //28
{B00000000,B00000000,B00000000,B00000000,B00000000}} //29
};

// drawing functions:

void display();
void printStatus();

// life game functions:
char isAlive(int x, int y);
void update();
char neighbors(int x, int y);
void setCell(int x, int y, int b, char status);

void setup() {
     init_lcd();
}

void loop() {
     display();
     printStatus();
     update();
}

char isAlive(int x, int y) {
     int xindex = x >> 3; // shift operation is is the same as (x-1)/8
     int bit = x & B00000111; // same as
     char mask = B10000000 >> bit;
     
     return board[cdisplay][y][xindex] & mask;
}

void update() {
     char count;
     for (int y = 0; y < bheight; y++) {
           for (int x = 0; x < bwidth; x++) {
                 count = neighbors(x,y);

                 if ( count == 3 )
                       setCell(x,y,cbuff,ALIVE);
                 else if ( isAlive(x,y) && count == 2 )
                       setCell(x,y,cbuff,ALIVE);
                 else
                       setCell(x,y,cbuff,DEAD);
           }
     }
     if ( cdisplay == 1) {
           cdisplay = 0;
           cbuff = 1;
     }
     else {
           cdisplay = 1;
           cbuff = 0;
     }
     generation++;
}

char neighbors( int x, int y ) {
     char count = 0;
     if ( x > 0 && x < bwidth-1 && y > 0 && y < bheight-1 ) {
           // 3 above
           if ( isAlive(x-1,y-1) ) count++;
           if ( isAlive(x,y-1) ) count++;
           if ( isAlive(x+1,y-1) ) count++;
           
           // 2 to each side
           if ( isAlive(x-1,y) ) count++;
           if ( isAlive(x+1,y) ) count++;
           
           // 3 below
           if ( isAlive(x-1,y+1) ) count++;
           if ( isAlive(x,y+1) ) count++;
           if ( isAlive(x+1,y+1) ) count++;
     }
     else if ( x == 0 && y == 0 ) {
           if ( isAlive(x+1,y) ) count++;
           if ( isAlive(x,y+1) ) count++;
           if ( isAlive(x+1,y+1) ) count++;
     }
     else if ( x == bwidth-1 && y == bheight-1 ) {
           if ( isAlive(x-1,y-1) ) count++;
           if ( isAlive(x,y-1) ) count++;
           if ( isAlive(x-1,y) ) count++;
     }
     else if ( x == 0 ) {
           if ( isAlive(x,y-1) ) count++;
           if ( isAlive(x+1,y-1) ) count++;
           if ( isAlive(x+1,y) ) count++;
           if ( isAlive(x,y+1) ) count++;
           if ( isAlive(x+1,y+1) ) count++;
     }
     else if ( x == bwidth-1 ) {
           if ( isAlive(x-1,y-1) ) count++;
           if ( isAlive(x,y-1) ) count++;
           if ( isAlive(x-1,y) ) count++;
           if ( isAlive(x-1,y+1) ) count++;
           if ( isAlive(x,y+1) ) count++;
     }
     else if ( y == 0 ) {
           if ( isAlive(x-1,y) ) count++;
           if ( isAlive(x+1,y) ) count++;
           if ( isAlive(x-1,y+1) ) count++;
           if ( isAlive(x,y+1) ) count++;
           if ( isAlive(x+1,y+1) ) count++;
     }
     else {
           if ( isAlive(x-1,y-1) ) count++;
           if ( isAlive(x,y-1) ) count++;
           if ( isAlive(x+1,y-1) ) count++;
           if ( isAlive(x-1,y) ) count++;
           if ( isAlive(x+1,y) ) count++;
     }
     return count;
}

void setCell(int x, int y, int b, char status) {
     int xindex = x >> 3; // shift operation is is the same as (x-1)/8
     int bit = x & B00000111; // same as
     char mask = B10000000 >> bit;
     
     if (status)
           board[b][y][xindex] = board[b][y][xindex] | mask;
     else
           board[b][y][xindex] = board[b][y][xindex] & ~mask;
}

void display() {
     char string_line[41];
     
     set_color_fg_font(RED);
     set_color_bg(BLACK);
     string_line[40] = '\0';
     
     for (int y = 0; y < bheight; y++) {
           for (int x = 0; x < bwidth; x++) {
                 if( isAlive(x,y) )
                       string_line[x] = ALIVEch;
                 else
                       string_line[x] = DEADch;
           }
           print_string(0,y*8,(unsigned char *)string_line, 's');
     }
}

void printStatus() {
     char buf[6];
     set_color_fg(WHITE);
     set_color_bg(BLACK);
     print_string(0,232,(unsigned char*)"Generation:",'s');
     itoa(generation, buf, 10);
     print_string(96,232,(unsigned char*)buf,'s');
}