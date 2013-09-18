#include "delay.h"

void
DelayMs(unsigned char cnt) {
unsigned char i;
while (cnt--) {
i=4;
while(i--) {
DelayUs(uS_CNT);	/* Adjust for error */
} ;
} ;
}