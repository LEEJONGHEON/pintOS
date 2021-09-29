#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	// FILL ME IN
	int h; // Hydrogen
	struct lock *lock; // mutex lock
	struct condition *h_cond_need_o; // h need o
	struct condition *o_cond_need_h; // o need h

};

void
reaction_init(struct reaction *reaction)
{
	reaction -> h = 0;
	reaction->h_cond_need_o = malloc(sizeof(struct condition));
	reaction->o_cond_need_h = malloc(sizeof(struct condition));
	reaction->lock = malloc(sizeof(struct lock));
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(reaction->lock);
	reaction->h ++; // make h
	// if h >=2 can make water
	if(reaction->h>=2) 
		cond_signal(reaction->h_cond_need_o,reaction->lock); // need o
	
	// h less than 2 waiting o need h signal 
	cond_wait(reaction->o_cond_need_h,reaction->lock);
	lock_release(reaction->lock);
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(reaction->lock);
	
	//if h less than 2 can't make water
	if(reaction->h < 2) 
		cond_wait(reaction->h_cond_need_o,reaction->lock); //waiting h
		
	//if h is more than 2 can make H2O water
	reaction->h -=2; 
	make_water(); // h2 + o -> water , making water
	
	//oxygen need H2
	cond_signal(reaction->o_cond_need_h, reaction->lock);
	cond_signal(reaction->o_cond_need_h, reaction->lock);
	lock_release(reaction->lock);
}
