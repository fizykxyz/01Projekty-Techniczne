

/* ----------------------------------------------------------
 * Program to control a T6963C-based 240x64 pixel LCD display
 *
 *
 *
 * Written by John P. Beale May 3-4, 1997  beale@best.com
 *
 * Converted by Nico Sachs Feb. 2006 for AVR-GCC
 *
 *  Based on information from Steve Lawther,
 *  "Writing Software for T6963C based Graphic LCDs", 1997 which is at
 *  http://ourworld.compuserve.com/homepages/steve_lawther/t6963c.pdf
 *
 *  and the Toshiba T6963C data sheet, also on Steve's WWW page
 *
 *  and info at: http://www.citilink.com/~jsampson/lcdindex.htm
 *               http://www.cs.colostate.edu/~hirsch/LCD.html
 *               http://www.hantronix.com/
 * ----------------------------------------------------------
 */

#include <avr/io.h>
#include <avr/delay.h>
#include <stdio.h>
#include <stdlib.h>     // rand()
#include <string.h>     // strlen()
#include <math.h>       // cos(),sin()

/*
 *  FG                (1)  frame ground
 *  GND               (2)  ground
 *  VDD               (3)  LCD logic supply  +5 DC
 *  VO                (4)  Contrast
 *  /WR               (5)  <-->
 *  /RD               (6)  <-->
 *  /CE or /CS        (7)  <-->
 *  CD or RS          (8)  <-->
 *  /RST              (9)  <-->

 *  D0                (10) <-->
 *  D1                (11) <-->
 *  D2                (12) <-->
 *  D3                (13) <-->
 *  D4                (14) <-->
 *  D5                (15) <-->
 *  D6                (16) <-->
 *  D7                (17) <-->

 *  FS                (18)           font select       HI : narrow            LOW : Wide   ( normal )
                                                       40 with 6x8 font       30 with 8x8 font
 *  Vout              (19)           Variable Resistor with GND and centre with VO ( Pin 4 )
 *  BLA               (20)           Back light +
 *  BLK               (21)           Back light -
 *  DSPON             (22)           I don't Know   but i think means display ON or ...
 */



/***************************************
									//**
           PIN DEFINITIONS			//**
									//**
****************************************/
									//**
#define glcd1_wr_PIN	0			//**
#define glcd1_wr_PORT	PORTA		//**
									//**
#define glcd1_rd_PIN	1			//**
#define glcd1_rd_PORT	PORTA		//**
									//**
#define glcd1_ce_PIN	2			//**
#define glcd1_ce_PORT	PORTA		//**
									//**
#define glcd1_cd_PIN	3			//**
#define glcd1_cd_PORT	PORTA		//**
									//**
#define glcd1_rst_PIN	4			//**
#define glcd1_rst_PORT	PORTA		//**
									//**
#define glcd1_fs_PIN	5			//**
#define glcd1_fs_PORT	PORTA		//**
									//**
#define glcd1_rv_PIN	6			//**
#define glcd1_rv_PORT	PORTA		//**
									//**
#define glcd1_DATA_PORT 	PORTD	//**
#define glcd1_DATA_PIN		PIND	//**
#define glcd1_DATADIR		DDRD	//**
									//**
//**************************************
//**************************************

/* ----- Definitions concerning LCD internal memory  ------ */

#define glcd1_G_BASE 0x0200            // base address of graphics memory
#define glcd1_T_BASE 0x0000            // base address of text memory
#define glcd1_BYTES_PER_ROW 30         // how many bytes per row on screen








#define glcd1_wr_high() glcd1_wr_PORT  |=  _BV(glcd1_wr_PIN);
#define glcd1_wr_low()  glcd1_wr_PORT  &= ~_BV(glcd1_wr_PIN);

#define glcd1_rd_high() glcd1_rd_PORT  |=  _BV(glcd1_rd_PIN);
#define glcd1_rd_low()  glcd1_rd_PORT  &= ~_BV(glcd1_rd_PIN);

#define glcd1_ce_high() glcd1_ce_PORT  |=  _BV(glcd1_ce_PIN);
#define glcd1_ce_low()  glcd1_ce_PORT  &= ~_BV(glcd1_ce_PIN);

#define glcd1_cd_high() glcd1_cd_PORT  |=  _BV(glcd1_cd_PIN);
#define glcd1_cd_low()  glcd1_cd_PORT  &= ~_BV(glcd1_cd_PIN);

#define glcd1_rst_high() glcd1_rst_PORT  |=  _BV(glcd1_rst_PIN);
#define glcd1_rst_low()  glcd1_rst_PORT  &= ~_BV(glcd1_rst_PIN);




#define glcd1_SET_DATADIR_OUT()	(glcd1_DATADIR = 0xFF)
#define glcd1_SET_DATADIR_IN()	(glcd1_DATADIR = 0x00)






#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
    /* on ATmega64/128 PINF is on port 0x00 and not 0x60 */
    #define PIN(x) ( &PORTF==&(x) ? _SFR_IO8(0x00) : (*(&x - 2)) )
#else
	#define PIN(x) (*(&x - 2))    /* address of input register of port x          */
#endif



/* ----------------------------------------------------------- */


void glcd1_dput(int byte); 			 // write data byte to LCD module
int  glcd1_dget(void);    			 // get data byte from LCD module
int  glcd1_sget(void);  			 // check LCD display status pbrt
void glcd1_cput(int byte);			 // write command byte to LCD module
void glcd1_setup();			 // make sure control lines are at correct levels
void glcd1_init();     			 // initialize LCD memory and display modes
void glcd1_print(int x,int y,const char *string);  	 // send string of characters to LCD
void glcd1_print_ram(int x,int y,char *string);  	 // send string of characters to LCD
void glcd1_set_address(unsigned int addr);
void glcd1_clear_graph();    		 // clear graphics memory of LCD
void glcd1_clear_text();  		 // clear text memory of LCD
void glcd1_xy(int x, int y); 		 // set memory pointer to (x,y) position (text)
void glcd1_clrpixel(int column, int row);  // set single pixel in 240x64 array
void glcd1_setpixel(int column, int row);  // set single pixel in 240x64 array
void glcd1_pixel(int column, int row,char show1);

void glcd1_show(const char *,int,int);
void glcd1_line(int x1, int y1, int x2, int y2, unsigned char show);
void glcd1_circle(int x, int y, int radius, unsigned char show);
void glcd1_circle_half(int x, int y, int radius, unsigned char show);
void glcd1_box(int x1, int y1, int x2, int y2, unsigned char show);
//void glcd1_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);
//void glcd1_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);
void glcd1_fill(int x1, int y1, int x2, int y2, unsigned char persent,char first);




#define glcd1_XMAX 240        // limits of (x,y) LCD graphics drawing
#define glcd1_XMIN 0
#define glcd1_YMAX 128
#define glcd1_YMIN 0



void main()
{
	glcd1_init();
	glcd1_circle(30,30,20,1);
}



void glcd1_set_address(unsigned int addr){
glcd1_dput(addr%256);
glcd1_dput(addr>>8);
glcd1_cput(0x24);
}



void glcd1_clear_graph()    // clear graphics memory of LCD
{
int i;
glcd1_set_address(glcd1_G_BASE);     // addrptr at address G_BASE1
for (i=0;i<3840;i++) {   // must be  3840 ?????????????????     5120
	glcd1_dput(0); glcd1_cput(0xc0);               // write data, inc ptr.
	}	 // end for(i)
} // end glcd1_clear_graph()

//----------------------------------------------------------------

void glcd1_clear_text()
{
 int i;
 glcd1_set_address(glcd1_T_BASE);     // addrptr at address T_BASE1

 for (i=0;i<480;i++) {  //  must be 480   ???????????     640
      glcd1_dput(0); glcd1_cput(0xc0);               // write data, inc ptr.
 } // end for(i)

} // glcd1_clear_text1()

//-------------------------------------------------------------------------------


void glcd1_print(int x,int y, const char *string)  // send string of characters to LCD
{
int i;
int c;
glcd1_xy(x,y);
  for (i=0;string[i]!=0;i++) {
      c = string[i] - 0x20;     // convert ASCII to LCD char address
      if (c<0) c=0;
      glcd1_dput(c);
      glcd1_cput(0xc0);               // write character, increment memory ptr.
  } // end for

} // end glcd1_string

//--------------------------------------------------------------------------------




void glcd1_print_ram(int x,int y,char *string)  // send string of characters to LCD
{
int i;
int c;
glcd1_xy(x,y);
  for (i=0;string[i]!=0;i++) {
      c = string[i] - 0x20;     // convert ASCII to LCD char address
      if (c<0) c=0;
      glcd1_dput(c);
      glcd1_cput(0xc0);               // write character, increment memory ptr.
  } // end for

} // end glcd1_string

//--------------------------------------------------------------------------------


void glcd1_setpixel(int column, int row)  // set single pixel in 240x64 array
{

int addr;       // memory address of byte containing pixel to write
  if( (column>=glcd1_XMAX) || (row>=glcd1_YMAX) )return;
  addr =  glcd1_G_BASE + (row*glcd1_BYTES_PER_ROW)  + (column/8);
  glcd1_set_address(addr);  // set LCD addr. pointer
  glcd1_cput(0xf8 | ((7-column%8)) );  // set bit-within-byte command
}

//--------------------------------------------------------------------------------

void glcd1_clrpixel(int column, int row)
{
int addr;       // memory address of byte containing pixel to write
  if( (column>=glcd1_XMAX) || (row>=glcd1_YMAX) )return;
  addr =  glcd1_G_BASE + (row*glcd1_BYTES_PER_ROW)  + (column/8);
  glcd1_set_address(addr);  // set LCD addr. pointer
  glcd1_cput(0xf0 | ((7-column%8)) );  // set bit-within-byte command


}

//---------------------------------------------------------------------------------

void glcd1_pixel(int column, int row,char show)
{
int addr;       // memory address of byte containing pixel to write
if( (column>=glcd1_XMAX) || (row>=glcd1_YMAX) )return;
addr =  glcd1_G_BASE + (row*glcd1_BYTES_PER_ROW)  + (column/8);
glcd1_set_address(addr);   // set LCD addr. pointer
if(show)
  glcd1_cput(0xf8 | ((7-column%8)) );  // set bit-within-byte command
else
  glcd1_cput(0xf0 | ((7-column%8)) );  // set bit-within-byte command
}

//---------------------------------------------------------------------------------


void glcd1_show(const char * s,int start_line,int how_many_line){
int addr,i;
addr =  glcd1_G_BASE +start_line*30;
glcd1_set_address(addr);   // set LCD addr. pointer
glcd1_cput(0xB0);
for(i=0;i<how_many_line*30;i++){
	glcd1_dput(s[i]);
	}
glcd1_cput(0xB2);
}



void glcd1_xy(int x, int y)  // set memory pointer to (x,y) position (text)
{
int addr;

  addr = glcd1_T_BASE + (y * glcd1_BYTES_PER_ROW) + x;
  glcd1_set_address(addr);   // set LCD addr. pointer

} // lcd_xy1()



/* ==============================================================
 * Low-level I/O routines to interface to LCD display
 * based on four routines:
 *
 *          glcd1_dput(): write data byte
 *          glcd1_cput(): write control byte
 *          glcd1_dget(): read data byte         (UNTESTED)
 *          glcd1_sget(): read status
 * ==============================================================
 */

void glcd1_setup()  // make sure control lines are at correct levels
{

 DDR(glcd1_ce_PORT) |= _BV(glcd1_ce_PIN);
 DDR(glcd1_rd_PORT) |= _BV(glcd1_rd_PIN);
 DDR(glcd1_wr_PORT) |= _BV(glcd1_wr_PIN);
 DDR(glcd1_cd_PORT) |= _BV(glcd1_cd_PIN);
 DDR(glcd1_rst_PORT) |= _BV(glcd1_rst_PIN);

glcd1_ce_high();
glcd1_rd_high();
glcd1_wr_high();
glcd1_cd_high();


 glcd1_SET_DATADIR_OUT();

 glcd1_rst_high();

 _delay_ms(5);

 glcd1_rst_low();

 _delay_ms(5);

 glcd1_rst_high();

} // end glcd1_setup()

//---------------------------------------------------------------------------

void glcd1_init()  // initialize LCD memory and display modes
{
 glcd1_dput(glcd1_G_BASE%256);
 glcd1_dput(glcd1_G_BASE>>8);
 glcd1_cput(0x42);       // set graphics memory to address glcd1_G_BASE

 glcd1_dput(glcd1_BYTES_PER_ROW%256);
 glcd1_dput(glcd1_BYTES_PER_ROW>>8);
 glcd1_cput(0x43);  // n bytes per graphics line

 glcd1_dput(glcd1_T_BASE%256);
 glcd1_dput(glcd1_T_BASE>>8);
 glcd1_cput(0x40);       // text memory at address glcd1_T_BASE1

 glcd1_dput(glcd1_BYTES_PER_ROW%256);
 glcd1_dput(glcd1_BYTES_PER_ROW>>8);
 glcd1_cput(0x41);  // n bytes per text line


 glcd1_cput(0x81);  // mode set: Graphics OR Text, ROM CGen

 glcd1_cput(0xa7);  // cursor is 8 lines high
 glcd1_dput(0x00);
 glcd1_dput(0x00);
 glcd1_cput(0x21);  // put cursor at (x,y) location

 glcd1_cput(0x9C);
	      // (For cursor to be visible, need to set up position

} // end glcd1_init()


// -------------------------------------------------------------------------------

int glcd1_sget(void)  // get LCD display status byte
{
int glcd1_status;

  glcd1_SET_DATADIR_IN();

  glcd1_cd_high();		// bring LCD C/D line high (read status byte)
  glcd1_rd_low();		// bring LCD /RD line low (read active)
  glcd1_ce_low();		// bring LCD /CE line low (chip-enable active)

  asm volatile ("nop"::);
  asm volatile ("nop"::);

  glcd1_status = glcd1_DATA_PIN;      // read LCD status byte

  glcd1_ce_high();         // bring LCD /CE line high, disabling it
  glcd1_rd_high();         // deactivate LCD read mode
  glcd1_SET_DATADIR_OUT(); // make 8-bit parallel port an output port

  return(glcd1_status);
} // sget1()

//------------------------------------------------------------------------------------------

void glcd1_dput(int byte) // write data byte to LCD module over par. port
		    // assume PC port in data OUTPUT mode
{
  do {} while ((0x03 & glcd1_sget()) != 0x03); // wait until display ready
  glcd1_cd_low();
  glcd1_wr_low();         // activate LCD's write mode

  glcd1_DATA_PORT = byte;          // write value to data port
  glcd1_ce_low();                       // pulse enable LOW > 80 ns (hah!)

  asm volatile ("nop"::);
  asm volatile ("nop"::);

  glcd1_ce_high();                       // return enable HIGH
  glcd1_wr_high();                       // restore Write mode to inactive

 // using my P5/75 MHz PC with ISA bus, CE stays low for 2 microseconds

} // end dput1()

//------------------------------------------------------------------------------------------

int glcd1_dget(void)      // get data byte from LCD module
{
int glcd1_byte;

  do {} while ((0x03 & glcd1_sget()) != 0x03); // wait until display ready
  glcd1_SET_DATADIR_IN(); // make PC's port an input port
  glcd1_wr_high();   // make sure WRITE mode is inactive
  glcd1_cd_low();   // data mode
  glcd1_rd_low();   // activate READ mode
  glcd1_ce_low();   // enable chip, which outputs data

  glcd1_byte = glcd1_DATA_PIN;  // read data from LCD

  glcd1_ce_high();   // disable chip
  glcd1_rd_high();   // turn off READ mode
  glcd1_SET_DATADIR_OUT(); // make 8-bit parallel port an output port

  return(glcd1_byte);
} // glcd1_dget1()

//----------------------------------------------------------------------------------------

void glcd1_cput(int byte) // write command byte to LCD module
		    // assumes port is in data OUTPUT mode
{
  do {} while ((0x03 & glcd1_sget()) != 0x03); // wait until display ready

  glcd1_DATA_PORT = byte;  // present data to LCD on PC's port pins

  glcd1_cd_high();         // control/status mode
  glcd1_rd_high();         // make sure LCD read mode is off
  glcd1_wr_low();         // activate LCD write mode
  glcd1_ce_low();         // pulse ChipEnable LOW, > 80 ns, enables LCD I/O
  glcd1_ce_high();         // disable LCD I/O
  glcd1_wr_high();         // deactivate write mode

} // glcd1_cput()










/***********************************************************************
Draws a line from x1,y1 go x2,y2. Line can be drawn in any direction.
Set show1 to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void glcd1_line(int x1, int y1, int x2, int y2, unsigned char show)
{
  int dy ;
  int dx ;
  int stepx, stepy, fraction;
  dy = y2 - y1;
  dx = x2 - x1;
  if (dy < 0)
  {
    dy = -dy;
    stepy = -1;
  }
  else
  {
    stepy = 1;
  }
  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
  {
    stepx = 1;
  }
  dy <<= 1;
  dx <<= 1;
  glcd1_pixel(x1,y1,show);
  if (dx > dy)
  {
    fraction = dy - (dx >> 1);
    while (x1 != x2)
    {
      if (fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;
      glcd1_pixel(x1,y1,show);
    }
  }
  else
  {
    fraction = dx - (dy >> 1);
    while (y1 != y2)
    {
      if (fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }
      y1 += stepy;
      fraction += dx;
      glcd1_pixel(x1,y1,show);
    }
  }
}







/***********************************************************************
Draws a circle with center at x,y with given radius.
Set show1 to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void glcd1_circle(int x, int y, int radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)
  {
    glcd1_pixel(x + xc, y + yc, show);
    glcd1_pixel(x + xc, y - yc, show);
    glcd1_pixel(x - xc, y + yc, show);
    glcd1_pixel(x - xc, y - yc, show);
    glcd1_pixel(x + yc, y + xc, show);
    glcd1_pixel(x + yc, y - xc, show);
    glcd1_pixel(x - yc, y + xc, show);
    glcd1_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}


void glcd1_circle_half(int x, int y, int radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)
  {
//    glcd1_pixel(x + xc, y + yc, show);
    glcd1_pixel(x + xc, y - yc, show);
//    glcd1_pixel(x - xc, y + yc, show);
    glcd1_pixel(x - xc, y - yc, show);
//    glcd1_pixel(x + yc, y + xc, show);
    glcd1_pixel(x + yc, y - xc, show);
//    glcd1_pixel(x - yc, y + xc, show);
    glcd1_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}








void glcd1_box(int x1, int y1, int x2, int y2, unsigned char show)
{
glcd1_line(x1,y1,x2,y1,show);  // up
glcd1_line(x1,y2,x2,y2,show);  // down
glcd1_line(x2,y1,x2,y2,show);  // right
glcd1_line(x1,y1,x1,y2,show);  // left
}


/*

//***********************************************************************
//Draws a line from x,y at given degree from inner_radius to outer_radius.
//Set show to 1 to draw pixel, set to 0 to hide pixel.
//***********************************************************************
void glcd1_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  glcd1_line(fx,fy,tx,ty,show);
}


void glcd1_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  glcd1_line(fx,fy,tx,ty,show);
  glcd1_line(fx+1,fy+1,tx+1,ty+1,show);
  glcd1_line(fx-1,fy-1,tx-1,ty-1,show);
}




*/



void glcd1_fill(int x1, int y1, int x2, int y2, unsigned char persent,char first)
{
char M,horizon_line,horizon_line2,i,str1[10];
if(persent>100)return;
if(!first){
	glcd1_line(x1,y2,x2,y2,1);  // down
	glcd1_line(x2,y1,x2,y2,1);  // right
	glcd1_line(x1,y1,x1,y2,1);  // left
	first=1;
	}
M=100/abs(y2-y1);

horizon_line=persent/M;
for(i=0;i<horizon_line;i++)
	glcd1_line(x1+2,y2-2-i,x2-2,y2-2-i,1);

horizon_line2=100/M;
for(i=horizon_line;i<horizon_line2;i++)
	glcd1_line(x1+2,y2-2-i,x2-2,y2-2-i,0);


sprintf(str1,"%02d%% ",persent);
glcd1_print_ram( (x2+x1)/16-1 , (y2+y1)/16,str1);
}




