#include "pintos_thread.h"

struct station {

	int seat_num; // remain  train seat
	int waiting_people; // waiting people
	int boarding_people; // boarding people
	
	struct lock *lock; //Mutex lock
	struct condition *cond_full; // train_num = 0;
	struct condition *cond_train; // train is arrived station
	
};

void
station_init(struct station *station)
{
	station -> seat_num = 0;
	station -> waiting_people = 0;
	station -> boarding_people = 0;
	
	station->lock = malloc(sizeof(struct lock));
	station->cond_full = malloc(sizeof(struct condition));
	station->cond_train = malloc(sizeof(struct condition));
	
	lock_init(station->lock);
	cond_init(station->cond_full);
	cond_init(station->cond_train);
}

void
station_load_train(struct station *station, int count)
{
	//train is arrived station
	lock_acquire(station->lock); // critical section 
	station->seat_num += count; // train remain seat_num
	
	
	while(station->seat_num > 0 && station->waiting_people > 0) {
		//new train is arrived
		cond_broadcast(station->cond_train,station->lock);
		//until train is full, train is stop
		cond_wait(station->cond_full,station->lock);
	}
	// if train remain seat number is zero and waiting people number is zero 
	// The train is leaving the station
	station -> seat_num = 0;
	station -> waiting_people = 0;
	lock_release(station->lock); // critical section
	
}

void
station_wait_for_train(struct station *station)
{
	//people are arrived station, waiting train is arrived station
	lock_acquire(station->lock); // critical section
	station->waiting_people++;
	// if boarding people are same train remain seat, people are waiting new train
	while(station->boarding_people == station->seat_num) {
		cond_wait(station->cond_train,station->lock); // waiting new train 
	}
	station->waiting_people--;
	station->boarding_people++;
	lock_release(station->lock); // critical section
	
}

void
station_on_board(struct station *station)
{
	//people are boarding train, 
	lock_acquire(station->lock); // critical section
	station->boarding_people--;
	station->seat_num--;
	// if remain seat is zero or boarding people is zero, then The train is leaving the station
	if(station->seat_num ==0 || station->boarding_people==0) {
		cond_signal(station->cond_full, station->lock); // train is full signal
	}
	lock_release(station->lock); // critical section
	
}
