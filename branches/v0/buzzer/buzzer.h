#ifndef BUZZER_H_
#define BUZZER_H_
#include <unistd.h>

struct buzzer_note {
	useconds_t duration;
	int freq;
};

/**
 * open the buzzer device
 * returns 0 on success 
 */
int buzzer_init(void);

/**
 * close the buzzer device
 */
void buzzer_cleanup(void);

/**
 * make the buzzer sound for a specific length of time in microseconds
 * returns 0 on success
 */
int buzzer_buzz(useconds_t duration, int freq);

/**
 * simply turns the buzzer on with a certain frequency
 * returns 0 on success
 */
int buzzer_on(int freq);

/**
 * turns off buzzer
 * returns 0 on success
 */
int buzzer_off(void);

/**
 * play a series of notes
 */
void buzzer_play(struct buzzer_note notes[], int notecount);

#endif /* BUZZER_H_ */
