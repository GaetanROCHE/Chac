/*
 * goutte.c:
 
 
 
 
 
 
 
 
 
 
 
 
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
//#include <softPwm.h>


  
//#define	GPIO_SENSOR		3



//  globalCounter:
//	Global variable to count interrupts
//	Should be declared volatile to make sure the compiler doesn't cache it.



unsigned int GPIO_Num;		// GPIO pin used for plus output
unsigned int Pulse_number;	// number of pulse
unsigned int Freq;			// Frequency in Hertz
unsigned int T_ON;			// Durée niveau haut 
unsigned int T_OFF;			// Durée niveau bas
char chaine_tmp[100];		// Stockage temporaire


 
void DiplayUsage(char * argv)
{
printf("Envoi d'une salve d'impultion sur une sortie GPIO \n\r");
printf("Exemple : %s 0 100 400 \n\r", argv);
printf("Génrère 100 impultion à une fréquense de 400 Hertz sur la broche GPIO 0 \n\r");
}
 

 
 



//============================================================
// Thread destiné à gerer le séquencement
//
//=============================================================
PI_THREAD (Task1) 
{
//static unsigned char etapeGoutte1=0;
//static unsigned char etapeShot_1=0;

	//(void)piHiPri (10) ;	// Set this thread to be high priority
	for (;;)
	{
	// Ajouter code ici !!!!


	//delayMicroseconds(100);
	delay (100);	
	}
};



 
  


/*
 * main
 *********************************************************************************
 */
int main (int argc, char *argv[])
{
unsigned char command;
int i;

// default init
GPIO_Num = 0;
Pulse_number = 10;
Freq = 100;
	

	
wiringPiSetup(); // init obligatoire en début de prog.
 
 
 
 

 for (i=0; i < argc; i++)
		{
        printf("Argument %d : %s \n", i+1, argv[i]);
		}
	
  if(argc == 4)
	{
	GPIO_Num = atoi(argv[1]);	// on récupère le num de gpio
	sprintf  (chaine_tmp, "gpio mode %d out", GPIO_Num   );
	system (chaine_tmp);	  

	Pulse_number= atoi(argv[2]); // number of pulses

	Freq = atoi(argv[3]); 			// Freq.
	
    T_OFF = 1000000 / Freq /2;		// en usecondes
    T_ON = 	1000000 / Freq /2;		// en usecondes
	//if(T_OFF < 100) T_OFF = 100;
	//if(T_ON < 100) T_ON = 100;
	
	printf("Using GPIO %d for %d pulse(s) at %d Hertz \n\r", GPIO_Num, Pulse_number, Freq );
	printf("T_ON = %d µs, T_OFF= %d µs \n\r", T_ON, T_OFF );
	
	printf("Sending burst ....\n\r");
	
	for(i=0; i!= Pulse_number; i++)
		{
		digitalWrite (GPIO_Num, 1);
		delayMicroseconds (T_OFF);
		digitalWrite (GPIO_Num, 0);
		delayMicroseconds (T_ON);
		}
	
	
  
  
  
	printf("Done. Exiting Programm. \n\r");
	}	
  else 
	{
	printf("Error Bad Argument\n\r");
	DiplayUsage(argv[0]);
	}	  
  
  
  
  
  
  return 0 ;
}
