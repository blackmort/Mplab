#include <htc.h>				// Biblioth�que HITECH

#ifndef _XTAL_FREQ				// D�finition de la fr�quence du quartz associ� au PIC. */
#define _XTAL_FREQ 20000000		//														*/
#endif	
						//			    										*/
#define	DelayUs(x)	{ unsigned int _dcnt; \
		_dcnt = (x)/3; \
		while(--_dcnt != 0) \
	continue; }
//****************************************************************************************************************************

// Equivalences LCD.

#define DON4 RD0		// Donn�es 4 bits associ�es au LCD.
#define DON5 RD1
#define DON6 RD2
#define DON7 RD3

#define EN RB2			// Validation de la donn�e sur front descendant (Horloge) 
#define RS RB3			// Coix du registre. RS = 0 Registre de commande, RS = 1 Registre d'affichage.

// Equivalences CLAVIER.

#define LGN1 RD7		// Lignes associ�es au clavier.
#define LGN2 RD6
#define LGN3 RD5
#define LGN4 RD4
#define COL0 RE0		// Colonnes associ�es au clavier.
#define COL1 RE1
#define COL2 RE2
#define RX_PIN RA0
#define TX_PIN RA1
//****************************************************************************************************************************

// D�finition des variables g�n�rales.

unsigned char MT81, MT82;					// M�moire travail 8 bits.	
unsigned short MT161,MT162,MTHEX,MTHEX1,MTHEX2,MTHEX3,MTHEX4;					// M�moire travail 16 bits.
unsigned char CPTCAR;					                                      	// Compteur de caract�res 8 bits.

//****************************************************************************************************************************

// D�finition des variables LCD.

// Ecriture de la table des messages (ROM)

// Chaque message se termine par un caract�re de 'FIN' de valeur '\0' (Caract�re de controle NUL valeur ASC2 =0, 8 bits.).

char ValeurAng[3];

const unsigned char TABMESS[ ]= "\0Moteur  1 ou 2 :\0Moteur 1:[0-365]\0Moteur 2:[0-365]\0Angle =  xxx    \0Erreur 1 ou 2   \0Reset Moteur 1  \0Reset Moteur 2"; // Table des messages (Le nombre des messages n'est pas limit�).

unsigned char MESSX[17] = {0}; // message (RAM 16 caract�res + '\0')

// D�finition des variables CLAVIER.

// Table associ�es au touches du clavier utilis�.

unsigned char MCLAV=0;			// Code ASC2 de la touche actionn�e du clavier

const unsigned char CODCLAV[12]="123456789*0#";

//****************************************************************************************************************************

// D�finition des variables g�n�rales.

int Angle1;
int Angle2;
int count;
int speed;

unsigned char a=33,b=0,udata;

//****************************************************************************************************************************

//D�claration des sous-fonctions LCD.


void INITLCD (void);			// Initialisation du LCD

void CHOIXMESS (char);			// Copie du message s�lectionn� en RAM MESSX 
void AFFILCDLG1 (void);			// Affichage du message MESSX ( ligne 1, 16 caract�res)
void AFFILCDLG2 (void);			// Affichage du message MESSX ( ligne 2, 16 caract�res)
			
void ENVOI (void);				// Copie des bits D0 RA1,D1 RA2,D2 RA3 ,D3 RA4 et validation de la donn�e (EN).

//D�claration des sous-fonctions CLAVIER.

void INITCLA (void);	// Initialisation du clavier
void SCRUTCLAV (void); 	// Scrutation du clavier 
void LECTCOL (void);	// Lecture des colones 

//D�claration des sous-fonctions MOTEUR.

void Moteur (void);

void putrs1USART(const char *data);




//************************************************************************************************************************
// MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR-MOTEUR
//************************************************************************************************************************
//************************************
// Initialisation du  MOTEUR
//************************************
void Moteur(void)
{
  CMCON = 0x07; // To turn off comparators
  ADCON1 = 0x06; // To turn off analog to digital converters
  TRISC = 0; // PORT B as output port
  PORTC = 0x0F;			//turn off portc (all off)			
}

void DelayMs(unsigned int cnt)
{
	do 
{
		DelayUs(1000);
} 
while(--cnt);
}
//****************************************************************************************************************************
// LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD-LCD
//****************************************************************************************************************************
//***********************
// Initialisation du LCD.
//***********************

void INITLCD (void)
{

// Initialisation PIC RD0, RD1, RD2, RD3, RB2, RB3 En sortie.

TRISD = TRISD & 0b11110000;		// RD0, RD1, RD2, RD3, en sorties.
TRISE = TRISE & 0b11101111;		// Mode I/O s�lectionn�.
TRISB = TRISB & 0b11110011;		// RB2, RB3 en sortie.
OPTION = OPTION | 0b10000000;	// Pas de r�sistances de PUL-UP.

RS = 0;	// Mode commande (Acc�s au registre de commande du LCD).
EN = 0;	// Initialisation horloge.

__delay_ms(20);	// Temps de mise en fonction du LCD apr�s alimentation de celui-ci.
__delay_ms(20);

PORTD = 0b0011 ;

EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fr�quence du signal d'horloge associ� au LCD.
EN = 0;
__delay_ms(10);		

EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fr�quence du signal d'horloge associ� au LCD.
EN = 0;
__delay_ms(10);

EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fr�quence du signal d'horloge associ� au LCD.
EN = 0;
__delay_ms(10);


PORTD = 0b0010 ;	// Mode 4 bits.
EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fr�quence du signal d'horloge associ� au LCD.
EN = 0;
__delay_ms(10);


EMILCD (0x28);			// Mode 4 bit, 2/16, font 5x7
EMILCD (0x08);			// affichage off
EMILCD (0x01);			// clear et home
EMILCD (0x0C);			// Affichage on, curseur clign off 
EMILCD (0x06);			// pas de decalage

RS = 1;					// Mode caract�re (Acc�s au registre d'affichage du LCD).

}

//****************************************************************************************************************************


//*************************************************************************
// Positionnement et �mission (caractere ou commande) au LCD, MSB puis LSB.
//*************************************************************************

void EMILCD (char DLCD)		// DLCD m�moire associ�e � la fonction EMILCD (Ne peut �tre utilis�e que dans cette fonction).
{
MT81=DLCD;				// Envoi du MSB de DLCD.
MT81 = MT81 / 16 ;		// Positionnement du MSB de DLCD en Bit0, Bit1, Bit2, Bit3.
ENVOI ();				// Envoi du MSB.

MT81=DLCD;				// Envoi du LSB de DLCD.
ENVOI ();				// Envoi du LSB.

}

//****************************************************************************************************************************

//*****************************************************************************
// Copie des bit0 RD0, bit1 RD1, bit2 RD2, bit3 RD3 et validation de la donn�e (EN).
//*****************************************************************************

void ENVOI (void)
{

MT81 = MT81 & 0b00001111;			// Mise � 0 des bits 7, 6, 5, 4.
PORTD = PORTD | MT81; 				// Recopie des 1 PORTD0 � PORTA3
MT81 = MT81 | 0b11110000;			// Mise � 1 des bits 7, 6, 5, 4.
PORTD = PORTD & MT81;				// Recopie des 0 PORTD0 � PORTD3

	// Validation de la donn�e

EN = 1;				// Front descendant

__delay_us(10);		// Temporisation de 10 us. Fr�quence du signal d'horloge associ� au LCD.

EN = 0;

__delay_ms(20);		// Temporisation de 20 ms. Avant le caract�re suivant.

}

//****************************************************************************************************************************

//*****************************************************
// Affichage du message MESSX ( ligne 2, 16 caract�res)
//*****************************************************

		void AFFILCDLG2 (void)

		{	// D�but.
			RS = 0; 			// Mode commandes
			EMILCD( 0xc0 ); 	// Positionnement du curseur.Ligne 2
			RS = 1; 			// Mode caract�res.
 			
			MT82=0;	
		
			while (MESSX[MT82] != 0)  		// Tant que le caract�re est diff�rent de '\0'

					{ 
					EMILCD(MESSX[MT82]); 	// Emission au LCD des caract�res 0 � 15 du message contenu dans MESSX.
					MT82++;
					}
 						
		}	// FIN.	

//****************************************************************************************************************************

//*****************************************************
// Affichage du message MESSX ( ligne 2, 16 caract�res)
//*****************************************************

		void AFFILCDLG1 (void)

		{	// D�but.
			RS = 0; 			// Mode commandes
			EMILCD( 0x80 ); 	// Positionnement du curseur.Ligne 2
			RS = 1; 			// Mode caract�res.
 			
			MT82=0;	
		
			while (MESSX[MT82] != 0)  		// Tant que le caract�re est diff�rent de '\0'

					{ 
					EMILCD(MESSX[MT82]); 	// Emission au LCD des caract�res 0 � 15 du message contenu dans MESSX.
					MT82++;
					}
 						
		}	// FIN.	

//****************************************************************************************************************************

//******************************************
// Copie du message s�lectionn� en RAM MESSX 
//******************************************

void CHOIXMESS (char PTMESS)
													
	{	// D�but.

	MT161=0;MT162=0;MT81=0;			// MT161 pointeur de caract�re, MT162 pointeur de message.							

	while ( MT162 != PTMESS)  		// Initialisation du pointeur de caract�res.(Premier caract�re du message s�lectionn�)

			{
			while ( TABMESS[MT161] != 0) MT161++;
 			MT161++;
			MT162++;
			}										

	while ( TABMESS[MT161] != 0) {MESSX[MT81]=TABMESS[MT161]; MT161++;MT81++;}		// Copie du message dans MESSX.																	


	MESSX[MT81]=0;		// Insertion du caract�re de fin de message '\0'.	
 																	
	}	// FIN.	

//************************************************************************************************************************
// CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER-CLAVIER
//************************************************************************************************************************

//**********************
// Sous fonction INITCLA
//**********************

void INITCLA (void)
{

TRISD = TRISD & 0b00001111;		// RD7,RD6;RD5,RD4, en sorties.
TRISE = TRISE & 0b11101111;		// Mode I/O port D.

ADCON1 = ADCON1 & 0b11110110;	// PORTE en mode I/O. ADCON1 Bit 3 � Bit0 "011x"
ADCON1 = ADCON1 | 0b00000110;

TRISE = TRISE | 0b00000111;			// RE0,RE1,RE2, en entr�es.

}

//************************
// Sous fonction Scrutclav
//************************

void SCRUTCLAV (void)
{

LGN1=1;LGN2=1;LGN3=1;LGN4=1;

LGN1=0;  			// S�lection de la premi�re ligne
MT81=0;				// MT81 est �gale � 0 (correspond au 1)
LECTCOL (); 		// Aller vers la sous-fonction

LGN1=1;LGN2=0; 		// S�lection de la deuxi�me ligne
MT81=3;				// MT81 est �gale � 3 (correspond au 4)
LECTCOL (); 		// Aller vers la sous-fonction	

LGN2=1;LGN3=0; 		// S�lection de la troisi�me ligne
MT81=6;				// MT81 est �gale � 6 (correspond au 7)
LECTCOL ();	    	// Aller vers la sous-fonction

LGN3=1;LGN4=0; 		// S�lection de la quatri�me ligne
MT81=9;				// MT81 est �gale � 9 (correspond au *)
LECTCOL (); 		// Aller vers la sous-fonction

}

//************************************
// Sous fonction lecture des colonnes.
//************************************
void LECTCOL (void)

{

if (!COL0) MCLAV = CODCLAV [MT81];		// Si Colonne 1 appuy�e alors MCLAV = MT81

if (!COL1) MCLAV = CODCLAV [MT81+1];	// Si Colonne 2 appuy�e alors MCLAV = MT81 + 1

if (!COL2) MCLAV = CODCLAV [MT81+2];	// Si Colonne 3 appuy�e alors MCLAV = MT81 + 2

}
void putrsUSART(const char *data)
{
do
{
while(!(TXSTA & 0x02));
TXREG = *data;
} while( *data++ );
}