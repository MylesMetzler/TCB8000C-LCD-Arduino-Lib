#include "TCB8000C.h"
#include <avr/io.h>
#include <wiring.h>
#include <binary.h>

#define us_delay 0
#define us_scale 750
#define shape_delay 3      //spec: 20ms
#define package_delay 2 //spec: 2ms
#define LINEDELAY 1 // spec: 5

//----------------------------------------------------------------------------------
// Communication Functions
//----------------------------------------------------------------------------------

void setLCDBUS(unsigned char command) {
     PORTD &= ~0xFC;
     PORTB &= ~0x03;
     PORTD |= (command & 0xFC);
     PORTB |= (command & 0x03);
}


//send a byte to the lcd
void send_byte ( unsigned char command ) {
     A1_0;
     setLCDBUS(command);
     CS_0;
     WR0_0;
     WR0_1;
     CS_1;
}// end send_byte


//close the byte stream of commands
void close_byte() {
     A1_1;
     setLCDBUS(0x01);
     CS_0;
     WR0_0;
     WR0_1;
     CS_1;
}// end close_byte


//send a full command package
void send_package(unsigned char *package) {
     unsigned char i;
     for (i = *package; i ; i--)
     send_byte( *(++package));
     close_byte();
#if us_delay
     delayMicroseconds(package_delay*us_scale);
#else
     delay(package_delay);
#endif
}// end send_package


//----------------------------------------------------------------------------------
// Color Set Functions
//----------------------------------------------------------------------------------

//set the forground color of shapes (the color of the shape
void set_color_fg(unsigned int color) {
     unsigned char Buffer[4];

     Buffer[0]=3;
     Buffer[1]=0x20;
     Buffer[2]=color;
     Buffer[3]=color>>8;
     send_package(Buffer);
}// end SetFgColor


// set the font forground color (the color of the text)
// setting the regular fg will also set the fg for the font.
void set_color_fg_font(unsigned int color) {
     unsigned char Buffer[4];

     Buffer[0]=3;
     Buffer[1]=0x14;
     Buffer[2]=color;
     Buffer[3]=color>>8;
     send_package(Buffer);
}// end SetFontFgColor


// set the font background color (the color backdrop behind the text)
void set_color_bg(unsigned int color) {
     unsigned char Buffer[4];

     Buffer[0]=3;
     Buffer[1]=0x15;
     Buffer[2]=color;
     Buffer[3]=color>>8;
     send_package(Buffer);
}// end SetFontBgColor


//----------------------------------------------------------------------------------
// Drawing functions
//----------------------------------------------------------------------------------

//set a pixel
void set_pixel(unsigned int X, unsigned int Y) {
     unsigned char Buffer[6];

     Buffer[0]=5;
     Buffer[1]=0x23;
     Buffer[2]=X;
     Buffer[3]=X>>8;
     Buffer[4]=Y;
     Buffer[5]=Y>>8;
     send_package(Buffer);
}// end of set_pixel


//Draw a line
void draw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
     unsigned char Buffer[10];

     Buffer[0]=9;
     Buffer[1]=0x24;
     Buffer[2]=x1;
     Buffer[3]=x1>>8;
     Buffer[4]=y1;
     Buffer[5]=y1>>8;
     Buffer[6]=x2;
     Buffer[7]=x2>>8;
     Buffer[8]=y2;
     Buffer[9]=y2>>8;
     send_package(Buffer);
#if us_delay
     delayMicroseconds(5*us_scale);
#else
     delay(LINEDELAY);
#endif
}//end draw_line


//Draw an empty rectangle
void draw_rect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
     unsigned char Buffer[10];

     Buffer[0]=9;
     Buffer[1]=0x26;
     Buffer[2]=x1;
     Buffer[3]=x1>>8;
     Buffer[4]=y1;
     Buffer[5]=y1>>8;
     Buffer[6]=x2;
     Buffer[7]=x2>>8;
     Buffer[8]=y2;
     Buffer[9]=y2>>8;
     send_package(Buffer);
#if us_delay
     delayMicroseconds(shape_delay*us_scale);
#else
     delay(shape_delay);
#endif
}//end draw_rect


//draw an empty circle
void draw_circle(unsigned int X, unsigned int Y, unsigned char R) {
     unsigned char Buffer[7];

     Buffer[0]=6;
     Buffer[1]=0x28;
     Buffer[2]=X;
     Buffer[3]=X>>8;
     Buffer[4]=Y;
     Buffer[5]=Y>>8;
     Buffer[6]=R;
     send_package(Buffer);
#if us_delay
     delayMicroseconds(shape_delay*us_scale);
#else
     delay(shape_delay);
#endif
}//end draw_circle


// fill a rectangle
void fill_rect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
     unsigned char Buffer[10];

     Buffer[0]=9;
     Buffer[1]=0x27;
     Buffer[2]=x1;
     Buffer[3]=x1>>8;
     Buffer[4]=y1;
     Buffer[5]=y1>>8;
     Buffer[6]=x2;
     Buffer[7]=x2>>8;
     Buffer[8]=y2;
     Buffer[9]=y2>>8;
     send_package(Buffer);
#if us_delay
     delayMicroseconds(shape_delay*us_scale);
#else
     delay(shape_delay);
#endif
}// end Fill_Rect


//draw a filled circle
void fill_circle(unsigned int X, unsigned int Y, unsigned char R) {
     unsigned char Buffer[7];

     Buffer[0]=6;
     Buffer[1]=0x29;
     Buffer[2]=X;
     Buffer[3]=X>>8;
     Buffer[4]=Y;
     Buffer[5]=Y>>8;
     Buffer[6]=R;
     send_package(Buffer);
#if us_delay
     delayMicroseconds(shape_delay*us_scale);
#else
     delay(shape_delay);
#endif
}


//-----------------------------------
// String Routine
// max 64 byte in string
// size s = 8x8 font
//      m = 16x16 GB2312-80 (External ROM)
//      b = 16x16 BIG5 (External ROM)
//-----------------------------------
//BROKEN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void print_string(unsigned int X, unsigned int Y, unsigned char *pstr, unsigned char size) {
     unsigned char TempData[3], Buffer[6], NoOfChar = 0, pos = 0;

     TempData[0]=2;            // use internal 8x8 font
     TempData[1]=0x10;
     if (size == 'm') TempData[2]=0x04;
     if (size == 'b') TempData[2]=0x05;
     else TempData[2] = 0x00;
     send_package(TempData);

     Buffer[0]=5;            // set the location
     Buffer[1]=0x12;
     Buffer[2]=X;
     Buffer[3]=X>>8;
     Buffer[4]=Y;
     Buffer[5]=Y>>8;
     send_package(Buffer);

     //NoOfChar = strlen((const char *)pstr); // send the string
     while ( pstr[NoOfChar] != '\0' && pos < 64)
     NoOfChar++;
     
     send_byte(0x17);
     send_byte(NoOfChar);
     while(*pstr>0) {
           send_byte(*pstr++);
     }
     close_byte();
}


void printchar(unsigned int X, unsigned int Y, unsigned char character) {
     unsigned char TempData[3], Buffer[6];

     TempData[0]=2;            // use internal 8x8 font
     TempData[1]=0x10;
     TempData[2]=0x00;
     send_package(TempData);

     Buffer[0]=5;            // set the location
     Buffer[1]=0x12;
     Buffer[2]=X;
     Buffer[3]=X>>8;
     Buffer[4]=Y;
     Buffer[5]=Y>>8;
     send_package(Buffer);

     send_byte(0x17);
     send_byte(1);
     send_byte(character);
     close_byte();
}

//-------------------------------------------
// Prints a 16-bit bitmap
//-------------------------------------------
void show_bmp( unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int * bmp) {
     unsigned char Buffer[5];
     unsigned int i,j,k;
     unsigned long p = 0;
     unsigned long addr = y*640 + 2*x;

     for (i = 0; i < h; i++) {
           Buffer[0] = 4;
           Buffer[1] = 0x81;
           Buffer[2] = addr;
           Buffer[3] = addr >> 8;
           Buffer[4] = addr >> 16;
           send_package(Buffer);
           j = w;
           while(j >= 16) {
                 send_byte(0x84);
                 send_byte(32);
                 for(k = 0; k < 16; k++) {
                       send_byte(bmp[p]);
                       send_byte(bmp[p]>>8);
                       p++;
                 }
                 close_byte();
                 j -= 16;
           }
           if (j > 0) {
                 send_byte(0x84);
                 send_byte(2*j);
                 for(k = 0; k < j; k++) {
                       send_byte(bmp[p]);
                       send_byte(bmp[p]>>8);
                       p++;
                 }
                 close_byte();
           }
           addr += 640;
     }
}


//---------------------------------------
// initialize a full screen direct draw 1 frame.
//---------------------------------------
void fdd_init(unsigned int line) {
     // the following 2 lines are the equivalent of addr = line * 640
     // however that overflows due to the size of the variable line?
     unsigned long addr = line*5;
     addr = addr << 7;
     
     unsigned char Buffer[5];
     Buffer[0] = 4;
     Buffer[1] = 0x81;
     Buffer[2] = addr;
     Buffer[3] = addr >> 8;
     Buffer[4] = addr >> 16;
     send_package(Buffer);
}

//--------------------------------------
// send a direct draw package consisting of 16 pixels
//--------------------------------------
void fdd_sendpackage( unsigned int *pkg ) {
     
     send_byte(0x84);
     send_byte(32);
     
     for (int i = 0; i < 16; i++) {
           send_byte(pkg[i]);
           send_byte(pkg[i]>>8);
     }
     close_byte();
     delayMicroseconds(110);
}


void init_lcd() {

     //-----------------------------------
     // init routine
     //-----------------------------------
     unsigned char F500[]={
           4,0x83,0x00,0xf5,0x00  };      // Reg[f500]=00 (internal MCS0 cycle pulse width)
     unsigned char F504[]={
           4,0x83,0x04,0xf5,0x04  };      // Reg[f504]=04 (internal MCS1 pulse width) 
     unsigned char F505[]={
           4,0x83,0x05,0xf5,0x80  };      // Reg[f505]=80 (internal MCS1 memory accessing setting)

     unsigned char F6C4[]={
           4,0x83,0xc4,0xf6,0x10  };      // Reg[f505]=80 Set Memory Clock Divide

     unsigned char F080[]={
           4,0x83,0x80,0xf0,0xfc  };      // Reg[f080]=fc (16bpp TFT)
     unsigned char F08E[]={
           4,0x83,0x8e,0xf0,0x32  };      // Reg[f08e]=32 (set pixel clock and LCD_ON)

     // set the LCD charactics
     unsigned char F090[]={
           4,0x83,0x90,0xf0,0x14  };
     unsigned char F091[]={
           4,0x83,0x91,0xf0,0x25  };      
     unsigned char F092[]={
           4,0x83,0x92,0xf0,0x1e  };      
     unsigned char F094[]={
           4,0x83,0x94,0xf0,0x05  };      
     unsigned char F095[]={
           4,0x83,0x95,0xf0,0x0e  };      
     unsigned char F096[]={
           4,0x83,0x96,0xf0,0x03  };

     

     // setup the port directions
     DDRD |= 0xFC; // set pins 2-7 on port D to be output
     DDRB |= 0x07; // set pins 0 and 1 on port B to be output
     //DDRD = 0xFF;
     DDRC = B00110111;


     //Initialize the ports
     //set command to 0xFF
     setLCDBUS(0xFF);
     CS_1;
     RES_0;
     A1_0;
     RD_1;
     WR0_1;

     RES_1;
     delay(1200);
     RES_0;
     delay(2);
     RES_1;
     delay(300);

     send_package(F500);
     send_package(F504);
     send_package(F505);

     send_package(F6C4);

     send_package(F080);
     send_package(F08E);

     send_package(F090);
     send_package(F091);
     send_package(F092);
     send_package(F094);
     send_package(F095);
     send_package(F096);
}