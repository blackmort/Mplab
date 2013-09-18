#include <htc.h>
#include "st7565-config.h"
#include "st7565.h"
#include "delay.h"
#include <pic18f4550.h>

void main()
{
    ADCON1 = 0;                      // Configure digital I/O
    PORTA = 0;                       // Set CS high (inactive)
    TRISA = 0;                       // Configure PORTA I/O
    GLCD_RESET = 0;
    DelayMs(50);
    GLCD_RESET = 1;
    DelayMs(100);
    GLCD_CS1 = 1;
    DelayMs(10);
    GLCD_SDA = 1;
    DelayMs(10);
    glcd_init();
    DelayMs(10);
    glcd_blank();
    glcd_contrast(5, 25);
    CLR_ALL();
    GOTO_XY (0,0);
    SdTexte("        Menu:");
    GOTO_XY (0,2);
    SdTexte("-Moteur 1");
    GOTO_XY (0,3);
    SdTexte("-Moteur 2");
    GOTO_XY (0,5);
    SdTexte("-Reset du Moteur 1");
    GOTO_XY (0,6);
    SdTexte("-Reset du Moteur 2");

while(1);
}