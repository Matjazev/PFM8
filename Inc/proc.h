#ifndef _PROC_H
#define _PROC_H
#include "io.h"
#define 	_THREAD_BUFFER_SIZE 128

typedef	void *func(void *);
 
typedef	struct {
func			*f;
void			*arg;
char			*name;
int				t,dt,to;
} _proc;

extern		_buffer 	*_thread_buf;
void			_proc_loop(void),
					_proc_list(void),
					_proc_remove(void *,void *);
_proc		 *_proc_add(void *,void *,char *,int),
				 *_proc_find(void *,void *);
void			_wait(int,void (*)(void));
#endif
