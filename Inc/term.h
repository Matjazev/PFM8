#ifndef TERM_H
#define TERM_H
#include "stm32f7xx_hal.h"
#include "ascii.h"
/*******************************************************************************
* Function Name	: 
* Description		: 
* Output				:
* Return				:
*******************************************************************************/
#define __CMDLEN	128

class _TERM {
	private:
		int			seq, timeout;
		char 		cmdbuf[__CMDLEN],*cmdp;
	public:
		_TERM() {
			seq=timeout=0;
			cmdp=cmdbuf;
		};
	int		error;
	bool	Cmd(int c);
	char	*Cmd(void);
	int		Escape(void);
	void	Repeat(int);
	void	*Parser(void *v);
		
	virtual void Prompt(void);
	virtual int Decode(char *) {return 0;};
};

#endif
