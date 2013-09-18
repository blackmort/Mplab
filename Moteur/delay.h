#define	MHZ	*1000L			/* number of kHz in a MHz */
#define	KHZ	*1			/* number of kHz in a kHz */

if	XTAL_FREQ == 20MHZ

	#define	DelayUs(x)	{ unsigned int _dcnt; \
		_dcnt = (x)/3; \
		while(--_dcnt != 0) \
	continue; }
	
		void DelayMs(unsigned int cnt){
	do {
		DelayUs(1000);
	} while(--cnt);
}