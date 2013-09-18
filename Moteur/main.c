/********************************************************************\
|          Pierre de coubertin : Stepper Motor Control               |
**********************************************************************
|              Controle de 2 moteur unipolaire                       | 
|    Via une console equiper d'un ecran LCD et d'un clavier  3x4     | 
|           Utilisation d'un UNLN2003A et d'un PIC16F877A            | 
|               Compiler avec Hi-Tech C compilers 	             	 | 
*********************************************************************|
|	Program: WP-AFF-CLAV-NV.c										 |-
|	Version: 1.0													 |
|-------------------------------------------------------------------*/

#include <htc.h>				// Bibliothéque HITECH
#include "lcdclamot.c"	

#ifndef _XTAL_FREQ				// Définition de la fréquence du quartz associé au PIC. */
#define _XTAL_FREQ 20000000		//														*/
#endif							//	

__CONFIG(HS & WDTDIS &  BORDIS & LVPDIS & DUNPROT & WRTEN & DEBUGEN & UNPROTECT);		// Configuration du PIC 16F877A


//Fonction Principale



void main (void)	

{
Moteur();       //Inititialisation du Moteur
INITLCD ();     //Initialisation LCD
INITCLA ();		// Utilisation de la fonction SCRUTCLAV. Scrutation du clavier.
				// MCLAV = 0 Pas de touche actionnée.
				// MCLAV != 0 MCLAV Contient le code ASCII de la touche actionnée. (Voir CODECLAV []). 
CHOIXMESS (1);  // Affichage message d'accueil : Moteur 1 ou 2:
AFFILCDLG1 ();
CPTCAR = 0;
count = 0;
speed=87,5;	
MCLAV = 0; 
SPBRG = 129; // 9600 baud @ 20MHz
TXSTA = 0x24; // setup USART transmit
RCSTA = 0x90; // setup USART receive
PORTC = 0; // Clear PORTC
TRISC = 0x80; //

while (MCLAV == 0) SCRUTCLAV ();			// Scutation permanente du clavier tant qu'aucunne touche actionnée.

//-----------------------------------------------------------------------------------------------------------------------------------------------
while (1)
{			
if (MCLAV=='1')									// Si moteur 1 selectionner

		{	
putrsUSART("\r\n\ Moteur 1:");		
CHOIXMESS (2);  // Affichage message Moteur 1:
AFFILCDLG1 ();
angle1 ();

		}

//-------------------------------------------------------------------------------------------------------------------------------------------------
if (MCLAV=='2')									// Si moteur 2 selectionner

		{	
putrsUSART("\r\n\ Moteur 2");
CHOIXMESS (3);  // Affichage message  Moteur  2:
AFFILCDLG1 ();
angle2 ();
		}

if (MCLAV=='3')									// Si resetmoteur 1 selectionner

		{	
putrsUSART("\r\n\ reset Moteur 1");	
CHOIXMESS (6);  // Affichage message Reset Moteur 2:
AFFILCDLG1 ();
resetmoteur1 ();
		}
if (MCLAV=='4')									// Si resertmoteur 2 selectionner

		{	
putrsUSART("\r\n\ reset Moteur 2");	
CHOIXMESS (7);  // Affichage message Reset Moteur 1:
AFFILCDLG1 ();
resetmoteur1 ();
		}



}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
			
void angle1 (void)

{
while (1)
{
if (MCLAV!='*')									// Si moteur 2 selectionner

		{			
Angle1=0;
MCLAV=0; while (MCLAV == 0) SCRUTCLAV ();	
	CPTCAR++;				// Incrémentation du compteur de caractéres.

		switch (CPTCAR) 		// Sélection du message à afficher.

			{ 
				case 1 :  CHOIXMESS (4); ValeurAng[0] = MCLAV; MESSX[9] = ValeurAng[0]; AFFILCDLG2 (); break;
				case 2 :  ValeurAng[1] = MCLAV; MESSX[10] = ValeurAng[1]; AFFILCDLG2 (); break;
				case 3 :  ValeurAng[2] = MCLAV; MESSX[11] = ValeurAng[2]; AFFILCDLG2 ();
                Angle1=((ValeurAng[0]-0x30)*100+ (ValeurAng[1]-0x30)*10+(ValeurAng[2]-0x30));
                putrsUSART('Angle1');
				default: CPTCAR = 0; 
                moteur1 ();

            
         
			}
		}
}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------

void angle2 (void)
{
while (1)
{
if (MCLAV!='*')									// Si moteur 2 selectionner

		{			
Angle2=0;
MCLAV=0; while (MCLAV == 0) SCRUTCLAV ();	
	CPTCAR++;				// Incrémentation du compteur de caractéres.

		switch (CPTCAR) 		// Sélection du message à afficher.

			{ 
				case 1 :  CHOIXMESS (4); ValeurAng[0] = MCLAV; MESSX[9] = ValeurAng[0]; AFFILCDLG2 (); break;
				case 2 :  ValeurAng[1] = MCLAV; MESSX[10] = ValeurAng[1]; AFFILCDLG2 (); break;
				case 3 :  ValeurAng[2] = MCLAV; MESSX[11] = ValeurAng[2]; AFFILCDLG2 ();
                Angle2=((ValeurAng[0]-0x30)*100+ (ValeurAng[1]-0x30)*10+(ValeurAng[2]-0x30));
                putrsUSART('Angle2');
				default: CPTCAR = 0;
                moteur2 ();



			}
		}
}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------

void  moteur1 (void)
{
while (1)
 {
            {
			PORTC=0b00001100;	//Step 0
			DelayMs(speed);
			PORTC=0b00000110;	//Step 1
			DelayMs(speed);
			PORTC=0b00000011;	//Step 2
			DelayMs(speed);
			PORTC=0b00001001;	//Step 3
			DelayMs(speed);
	     	}

	count++;				//counter + 1
		if(count==Angle1)
            {	
            PORTC=0b00001111;
           MCLAV = 0; 
            main ();
            
			}
}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------

void  moteur2 (void)
{
while (1)
 {
            {
			PORTC=0b11000000;	//Step 0
			DelayMs(speed);
			PORTC=0b01100000;	//Step 1
			DelayMs(speed);
			PORTC=0b00110000;	//Step 2
			DelayMs(speed);
			PORTC=0b10010000;	//Step 3
			DelayMs(speed);
	     	}

	count++;				//counter + 1
		if(count==Angle2)
            {	
            PORTC=0b11110000;
           MCLAV = 0; 
            main ();
			}
}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------

void  resetmoteur1 (void)
{
while (1)
 {
            {
			PORTC=0b00000011;	//Step 0
			DelayMs(speed);
			PORTC=0b00000110;	//Step 1
			DelayMs(speed);
			PORTC=0b00001100;	//Step 2
			DelayMs(speed);
			PORTC=0b00001001;	//Step 3
			DelayMs(speed);
	     	}

	count++;				//counter + 1
		if(count==Angle1)
            {	
            PORTC=0b00001111;
            MCLAV = 0;
            Angle1=0;
            main ();
            
			}
}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------

void  resetmoteur2 (void)
{
while (1)
 {
            {
			PORTC=0b00000011;	//Step 0
			DelayMs(speed);
			PORTC=0b00000110;	//Step 1
			DelayMs(speed);
			PORTC=0b00001100;	//Step 2
			DelayMs(speed);
			PORTC=0b00001001;	//Step 3
			DelayMs(speed);
	     	}

	count++;				//counter + 1
		if(count==Angle2)
            {	
            PORTC=0b11110000;
            MCLAV = 0; 
            Angle2=0;
            main ();
			}
}
}





