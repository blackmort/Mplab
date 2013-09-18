#include <htc.h>				// Bibliothéque HITECH

#ifndef _XTAL_FREQ				// Définition de la fréquence du quartz associé au PIC. */
#define _XTAL_FREQ 20000000		//														*/
#endif	
						//			    										*/
#define	DelayUs(x)	{ unsigned int _dcnt; \
		_dcnt = (x)/3; \
		while(--_dcnt != 0) \
	continue; }
//****************************************************************************************************************************

// Equivalences LCD.

#define DON4 RD0		// Données 4 bits associées au LCD.
#define DON5 RD1
#define DON6 RD2
#define DON7 RD3

#define EN RB2			// Validation de la donnée sur front descendant (Horloge) 
#define RS RB3			// Coix du registre. RS = 0 Registre de commande, RS = 1 Registre d'affichage.

// Equivalences CLAVIER.

#define LGN1 RD7		// Lignes associées au clavier.
#define LGN2 RD6
#define LGN3 RD5
#define LGN4 RD4
#define COL0 RE0		// Colonnes associées au clavier.
#define COL1 RE1
#define COL2 RE2
#define RX_PIN RA0
#define TX_PIN RA1
//****************************************************************************************************************************

// Définition des variables générales.

unsigned char MT81, MT82;					// Mémoire travail 8 bits.	
unsigned short MT161,MT162,MTHEX,MTHEX1,MTHEX2,MTHEX3,MTHEX4;					// Mémoire travail 16 bits.
unsigned char CPTCAR;					                                      	// Compteur de caractéres 8 bits.

//****************************************************************************************************************************

// Définition des variables LCD.

// Ecriture de la table des messages (ROM)

// Chaque message se termine par un caractére de 'FIN' de valeur '\0' (Caractére de controle NUL valeur ASC2 =0, 8 bits.).

char ValeurAng[3];

const unsigned char TABMESS[ ]= "\0Moteur  1 ou 2 :\0Moteur 1:[0-365]\0Moteur 2:[0-365]\0Angle =  xxx    \0Erreur 1 ou 2   \0Reset Moteur 1  \0Reset Moteur 2"; // Table des messages (Le nombre des messages n'est pas limité).

unsigned char MESSX[17] = {0}; // message (RAM 16 caractéres + '\0')

// Définition des variables CLAVIER.

// Table associées au touches du clavier utilisé.

unsigned char MCLAV=0;			// Code ASC2 de la touche actionnée du clavier

const unsigned char CODCLAV[12]="123456789*0#";

//****************************************************************************************************************************

// Définition des variables générales.

int Angle1;
int Angle2;
int count;
int speed;

unsigned char a=33,b=0,udata;

//****************************************************************************************************************************

//Déclaration des sous-fonctions LCD.


void INITLCD (void);			// Initialisation du LCD

void CHOIXMESS (char);			// Copie du message sélectionné en RAM MESSX 
void AFFILCDLG1 (void);			// Affichage du message MESSX ( ligne 1, 16 caractéres)
void AFFILCDLG2 (void);			// Affichage du message MESSX ( ligne 2, 16 caractéres)
			
void ENVOI (void);				// Copie des bits D0 RA1,D1 RA2,D2 RA3 ,D3 RA4 et validation de la donnée (EN).

//Déclaration des sous-fonctions CLAVIER.

void INITCLA (void);	// Initialisation du clavier
void SCRUTCLAV (void); 	// Scrutation du clavier 
void LECTCOL (void);	// Lecture des colones 

//Déclaration des sous-fonctions MOTEUR.

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
TRISE = TRISE & 0b11101111;		// Mode I/O sélectionné.
TRISB = TRISB & 0b11110011;		// RB2, RB3 en sortie.
OPTION = OPTION | 0b10000000;	// Pas de résistances de PUL-UP.

RS = 0;	// Mode commande (Accés au registre de commande du LCD).
EN = 0;	// Initialisation horloge.

__delay_ms(20);	// Temps de mise en fonction du LCD aprés alimentation de celui-ci.
__delay_ms(20);

PORTD = 0b0011 ;

EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fréquence du signal d'horloge associé au LCD.
EN = 0;
__delay_ms(10);		

EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fréquence du signal d'horloge associé au LCD.
EN = 0;
__delay_ms(10);

EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fréquence du signal d'horloge associé au LCD.
EN = 0;
__delay_ms(10);


PORTD = 0b0010 ;	// Mode 4 bits.
EN = 1;				// Front descendant (VALIDATION)
__delay_us(10);		// Temporisation de 10 us. Fréquence du signal d'horloge associé au LCD.
EN = 0;
__delay_ms(10);


EMILCD (0x28);			// Mode 4 bit, 2/16, font 5x7
EMILCD (0x08);			// affichage off
EMILCD (0x01);			// clear et home
EMILCD (0x0C);			// Affichage on, curseur clign off 
EMILCD (0x06);			// pas de decalage

RS = 1;					// Mode caractère (Accés au registre d'affichage du LCD).

}

//****************************************************************************************************************************


//*************************************************************************
// Positionnement et émission (caractere ou commande) au LCD, MSB puis LSB.
//*************************************************************************

void EMILCD (char DLCD)		// DLCD mémoire associée à la fonction EMILCD (Ne peut étre utilisée que dans cette fonction).
{
MT81=DLCD;				// Envoi du MSB de DLCD.
MT81 = MT81 / 16 ;		// Positionnement du MSB de DLCD en Bit0, Bit1, Bit2, Bit3.
ENVOI ();				// Envoi du MSB.

MT81=DLCD;				// Envoi du LSB de DLCD.
ENVOI ();				// Envoi du LSB.

}

//****************************************************************************************************************************

//*****************************************************************************
// Copie des bit0 RD0, bit1 RD1, bit2 RD2, bit3 RD3 et validation de la donnée (EN).
//*****************************************************************************

void ENVOI (void)
{

MT81 = MT81 & 0b00001111;			// Mise à 0 des bits 7, 6, 5, 4.
PORTD = PORTD | MT81; 				// Recopie des 1 PORTD0 à PORTA3
MT81 = MT81 | 0b11110000;			// Mise à 1 des bits 7, 6, 5, 4.
PORTD = PORTD & MT81;				// Recopie des 0 PORTD0 à PORTD3

	// Validation de la donnée

EN = 1;				// Front descendant

__delay_us(10);		// Temporisation de 10 us. Fréquence du signal d'horloge associé au LCD.

EN = 0;

__delay_ms(20);		// Temporisation de 20 ms. Avant le caractére suivant.

}

//****************************************************************************************************************************

//*****************************************************
// Affichage du message MESSX ( ligne 2, 16 caractéres)
//*****************************************************

		void AFFILCDLG2 (void)

		{	// Début.
			RS = 0; 			// Mode commandes
			EMILCD( 0xc0 ); 	// Positionnement du curseur.Ligne 2
			RS = 1; 			// Mode caractéres.
 			
			MT82=0;	
		
			while (MESSX[MT82] != 0)  		// Tant que le caractére est différent de '\0'

					{ 
					EMILCD(MESSX[MT82]); 	// Emission au LCD des caractéres 0 à 15 du message contenu dans MESSX.
					MT82++;
					}
 						
		}	// FIN.	

//****************************************************************************************************************************

//*****************************************************
// Affichage du message MESSX ( ligne 2, 16 caractéres)
//*****************************************************

		void AFFILCDLG1 (void)

		{	// Début.
			RS = 0; 			// Mode commandes
			EMILCD( 0x80 ); 	// Positionnement du curseur.Ligne 2
			RS = 1; 			// Mode caractéres.
 			
			MT82=0;	
		
			while (MESSX[MT82] != 0)  		// Tant que le caractére est différent de '\0'

					{ 
					EMILCD(MESSX[MT82]); 	// Emission au LCD des caractéres 0 à 15 du message contenu dans MESSX.
					MT82++;
					}
 						
		}	// FIN.	

//****************************************************************************************************************************

//******************************************
// Copie du message sélectionné en RAM MESSX 
//******************************************

void CHOIXMESS (char PTMESS)
													
	{	// Début.

	MT161=0;MT162=0;MT81=0;			// MT161 pointeur de caractére, MT162 pointeur de message.							

	while ( MT162 != PTMESS)  		// Initialisation du pointeur de caractéres.(Premier caractére du message sélectionné)

			{
			while ( TABMESS[MT161] != 0) MT161++;
 			MT161++;
			MT162++;
			}										

	while ( TABMESS[MT161] != 0) {MESSX[MT81]=TABMESS[MT161]; MT161++;MT81++;}		// Copie du message dans MESSX.																	


	MESSX[MT81]=0;		// Insertion du caractére de fin de message '\0'.	
 																	
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

ADCON1 = ADCON1 & 0b11110110;	// PORTE en mode I/O. ADCON1 Bit 3 à Bit0 "011x"
ADCON1 = ADCON1 | 0b00000110;

TRISE = TRISE | 0b00000111;			// RE0,RE1,RE2, en entrées.

}

//************************
// Sous fonction Scrutclav
//************************

void SCRUTCLAV (void)
{

LGN1=1;LGN2=1;LGN3=1;LGN4=1;

LGN1=0;  			// Sélection de la première ligne
MT81=0;				// MT81 est égale à 0 (correspond au 1)
LECTCOL (); 		// Aller vers la sous-fonction

LGN1=1;LGN2=0; 		// Sélection de la deuxième ligne
MT81=3;				// MT81 est égale à 3 (correspond au 4)
LECTCOL (); 		// Aller vers la sous-fonction	

LGN2=1;LGN3=0; 		// Sélection de la troisième ligne
MT81=6;				// MT81 est égale à 6 (correspond au 7)
LECTCOL ();	    	// Aller vers la sous-fonction

LGN3=1;LGN4=0; 		// Sélection de la quatrième ligne
MT81=9;				// MT81 est égale à 9 (correspond au *)
LECTCOL (); 		// Aller vers la sous-fonction

}

//************************************
// Sous fonction lecture des colonnes.
//************************************
void LECTCOL (void)

{

if (!COL0) MCLAV = CODCLAV [MT81];		// Si Colonne 1 appuyée alors MCLAV = MT81

if (!COL1) MCLAV = CODCLAV [MT81+1];	// Si Colonne 2 appuyée alors MCLAV = MT81 + 1

if (!COL2) MCLAV = CODCLAV [MT81+2];	// Si Colonne 3 appuyée alors MCLAV = MT81 + 2

}
void putrsUSART(const char *data)
{
do
{
while(!(TXSTA & 0x02));
TXREG = *data;
} while( *data++ );
}