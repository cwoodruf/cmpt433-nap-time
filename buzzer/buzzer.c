/**
 * buzzer module for cmpt433 assignment 3
 * @author Cal Woodruff <cwoodruf@sfu.ca> 301013983
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include "buzzer.h"

#ifndef BUZZER_Debug
#define BUZZER_Debug DEBUG
#endif

#define BUZZER_DEVICE "/dev/PWM-Test"

static int buzzer_fd = -1;

/**
 * open the buzzer device
 * returns 0 on success 
 */
int buzzer_init(void)
{
	buzzer_fd = open(BUZZER_DEVICE, 0);
	if (buzzer_fd < 0) {
#if BUZZER_Debug
		perror("buzzer: can't open device!");
#endif
		return 1;
	}
	return 0;
}

/**
 * close the buzzer device
 */
void buzzer_cleanup(void)
{
	close(buzzer_fd);
}

/**
 * make the buzzer sound for a specific length of time in microseconds
 * returns 0 on success
 */
int buzzer_buzz(useconds_t duration, int freq)
{
	int ret;

	/* if the duration is a ridiculous value don't do anything */
	if (duration < 0) {
#if BUZZER_Debug
		perror("buzzer: time must be greater than 0!");
#endif
		return -1;
	}
	if ((ret = buzzer_on(freq))) return ret;
	usleep(duration);
	return buzzer_off();
}

/**
 * simply turns the buzzer on with a certain frequency
 * returns 0 on success
 */
int buzzer_on(int freq)
{
	assert(buzzer_fd >= 0);

	if (freq <= 0) {
#if BUZZER_Debug
		perror("buzzer: frequency must be greater than 0!");
#endif
		return 1;

	}
	if (ioctl(buzzer_fd, 1, freq) < 0) {
#if BUZZER_Debug
		perror("buzzer: error setting frequency!");
#endif
		return 1;
	}
	return 0;
}

/**
 * turns off buzzer
 * returns 0 on success
 */
int buzzer_off(void)
{
	assert(buzzer_fd >= 0);

	if ((ioctl(buzzer_fd, 0)) < 0) {
#if BUZZER_Debug
		perror("buzzer: error setting frequency!");
#endif
		return 1;
	}
	return 0;
}

/**
 * play a series of notes
 */
void buzzer_play(struct buzzer_note notes[], int notecount)
{
	int i;
	assert(notes);
	if (notecount <= 0) return;
	for (i = 0; i<notecount; i++) {
		buzzer_buzz(notes[i].duration, notes[i].freq);
		usleep(10000);
	}
}

/* end buzzer.c */

