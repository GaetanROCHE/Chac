/*
 * goutte.c:
 *	Wait for Interrupt test program
 *
 *	This program demonstrates the use of the waitForInterrupt()
 *	function in wiringPi. It listens to a button input on
 *	BCM_GPIO pin 17 (wiringPi pin 0)
 *
 *	The biggest issue with this method is that it really only works
 *	well in Sys mode.
 *
 *	Jan 2013: This way of doing things is sort of deprecated now, see
 *	the wiringPiISR() function instead and the isr.c test program here.
 *
 * Copyright (c) 2012-2013 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>


  
#define	GPIO_SENSOR		1



//  globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.
static volatile int IntrCounter = 0;
static volatile int Task1Counter = 0;


  




//============================================================
// Thread destiné à gerer le séquencement
//
//=============================================================
PI_THREAD (Task1) 
{
//static unsigned char etapeGoutte1=0;
//static unsigned char etapeShot_1=0;

	(void)piHiPri (10) ;	// Set this thread to be high priority
	for (;;)
	{
	Task1Counter ++;



	delayMicroseconds(100);
	delay (1);	
	}
}


//==================== Pour savoir si un car est dispo
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}



 /*
 * myInterrupt:
 *********************************************************************************
 */
void Interrupt_F1 (void)
{
  ++IntrCounter ;
  }

 
  


/*
 * main
 *********************************************************************************
 */
int main (int argc, char *argv[])
{
unsigned char command;
// Trouver une manière plus élégante de faire cela 
system ("gpio mode 1 input") ;


		
wiringPiSetup ();


// installation du gestionaire d'interruption 
if (wiringPiISR (GPIO_SENSOR, INT_EDGE_FALLING, &Interrupt_F1) < 0)
  {
    fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno)) ;
    return 1 ;
  }
 
  for (;;)
  {
	printf("Pulse counter = %d, \n", IntrCounter );
	fflush (stdout) ;
	delay(500);
  }

  return 0 ;
}
