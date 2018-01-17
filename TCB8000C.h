#ifndef TCB8000C_H_
#define TCB8000C_H_

//port definitions

#define WAIT    0x08
#define A1_1  PORTC |= 0x01
#define A1_0  PORTC &= ~0x01
#define CS_1  PORTC |= 0x02
#define CS_0  PORTC &= ~0x02
#define RES_1 PORTC |= 0x04
#define RES_0 PORTC &= ~0x04
#define RD_1  PORTC |= 0x10
#define RD_0  PORTC &= ~0x10
#define WR0_1 PORTC |= 0x20
#define WR0_0 PORTC &= ~0x20

// data bus is defined as
// high 6 bits are on PORTD.2 through PORTD.7
// low 2 bits are on PORTB.0 and PORTB.1

// color definitions
#define      RED            0xf800
#define      GREEN      0x07e0
#define      BLUE      0x001f
#define      YELLOW      0xffe0
#define      CYAN      0x07ff
#define      MAGENTA      0xf81f
#define      BLACK      0x0000
#define      WHITE      0xffff
#define GRAY    0xdefb


//-----------------------------------------------------------------------------
// Communication Functions
//-----------------------------------------------------------------------------

// put the 8 bit word onto the bus
void setLCDBUS(unsigned char command);

//send a byte to the lcd
void send_byte ( unsigned char command );

//close the byte stream of commands
void close_byte();

//send a full command package
void send_package(unsigned char *package);


//------------------------------------------------------------------------------
// Color Set Functions
//------------------------------------------------------------------------------

//set the foreground color of shapes (the color of the shape
void set_color_fg(unsigned int color);

// set the font foreground color (the color of the text)
// setting the regular fg will also set the fg for the font.
void set_color_fg_font(unsigned int color);

// set the font background color (the color backdrop behind the text)
void set_color_bg(unsigned int color);


//------------------------------------------------------------------------------
// Drawing functions
//------------------------------------------------------------------------------

//set a pixel
void set_pixel(unsigned int X, unsigned int Y);

//Draw a line
void draw_line(unsigned int x1, unsigned int y1, unsigned int x2,
                    unsigned int y2);

//Draw an empty rectangle
void draw_rect(unsigned int x1, unsigned int y1,
                    unsigned int x2, unsigned int y2);

//draw an empty circle
void draw_circle(unsigned int X, unsigned int Y, unsigned char R);

// fill a rectangle
void fill_rect(unsigned int x1, unsigned int y1,
                    unsigned int x2, unsigned int y2);

//draw a filled circle
void fill_circle(unsigned int X, unsigned int Y, unsigned char R);

//-----------------------------------
// String Routine
// max 64 byte in string
// size s = 8x8 font
//      m = 16x16 GB2312-80 (External ROM)
//      b = 16x16 BIG5 (External ROM)
//-----------------------------------
void print_string(unsigned int X, unsigned int Y,
                         unsigned char *pstr, unsigned char size);

// print a character at X,Y
void printchar(unsigned int X, unsigned int Y, unsigned char character);

//-----------------------------------
// Prints a 16-bit bitmap
// x,y = coordinates of the top left corner of the bmp
// w = the width in pixels of the bmp
// h = the height in pixels of the bmp
//-----------------------------------
void show_bmp( unsigned int x, unsigned int y,
                    unsigned int w, unsigned int h, unsigned int * bmp);


//-----------------------------------
// Initilizes required values for a full screen direct draw
// This is used when a full frame is to be written directy to the
// the LCD. It is faster then primative shape drawing.
//-----------------------------------
void fdd_init(unsigned int line);


//----------------------------------
// draw 16 pixels to the screen as a package
// pkg = the 16 pixels to be drawn, anything beyond the 16 will be ignored.
//
// fdd_init() MUST be called first
//----------------------------------
void fdd_sendpackage(unsigned int *pkg);


//-----------------------------------------------------------------------------
// initialization function
//-----------------------------------------------------------------------------
void init_lcd();

#endif /* TCB8000_H_ */