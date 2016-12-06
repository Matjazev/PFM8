#include "proc.h"
#include "stm32f7xx_hal.h"
_buffer 	*_thread_buf=NULL;
//______________________________________________________________________________
_proc	*_proc_add(void *f,void *arg,char *name, int dt) {
_proc	*p=malloc(sizeof(_proc));
			if(p != NULL) {
				p->f=(func *)f;
				p->arg=arg;
				p->name=name;
				p->t=HAL_GetTick();
				p->dt=dt;
				if(!_thread_buf)
					_thread_buf=_buffer_init(_THREAD_BUFFER_SIZE*sizeof(_proc *));
				_buffer_push(_thread_buf,&p,sizeof(_proc *));
			}
			return p;
}
//______________________________________________________________________________
void	_proc_loop(void) {
_proc	*p;
			if(_thread_buf)
				if(_buffer_pull(_thread_buf,&p,sizeof(_proc *))) {
					if(HAL_GetTick() >= p->t) {
						p->to = HAL_GetTick() - p->t;
						p->f(p->arg);
						p->t = HAL_GetTick() + p->dt;
					}
					_buffer_push(_thread_buf,&p,sizeof(_proc *));
				}
}
//______________________________________________________________________________
void	_proc_remove(void  *f,void *arg) {
_proc	*p;
int		i=_buffer_count(_thread_buf)/sizeof(_proc *);
			while(i--) {
				_buffer_pull(_thread_buf,&p,sizeof(_proc *));
				if(f == p->f && arg == p->arg)
					free(p);
				else
					_buffer_push(_thread_buf,&p,sizeof(_proc *));
			}
}
//______________________________________________________________________________
_proc	*_proc_find(void  *f,void *arg) {
_proc	*p,*q=NULL;
int		i=_buffer_count(_thread_buf)/sizeof(_proc *);
			while(i--) {
				_buffer_pull(_thread_buf,&p,sizeof(_proc *));
				if(f == p->f && (arg == p->arg || !arg))
					q=p;
				_buffer_push(_thread_buf,&p,sizeof(_proc *));
			}
			return q;
}
//______________________________________________________________________________
void	_proc_list(void) {
_proc	*p;	
int		i	=_buffer_count(_thread_buf)/sizeof(_proc *);
			printf("...\r\n");
			while(i--) {
				_buffer_pull(_thread_buf,&p,sizeof(_proc *));
				printf("%08X,%08X,%s,%d\r\n",(int)p->f,(int)p->arg,p->name,p->to);
				_buffer_push(_thread_buf,&p,sizeof(_proc *));
			}
}
//___________________________________________________________________________
void	_wait(int t,void (*f)(void)) {
int		to=HAL_GetTick()+t;
			while(to > HAL_GetTick()) {
				if(f)
					f();
			}
}
