/* 
 * 
 * adapted from
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/example.html
 * Antony Courtney,	25/11/94
 */

#ifndef NAPMULTICAST
#define NAPMULTICAST

#define UNIX_PATH_MAX 108
#define NAP_PORT 22534
#define NAP_GROUP "225.0.0.34"
#define NAP_PEER_DIR "/tmp"
#define NAP_ACK "ACK"
#define PORTSTART 1024
#define PORTEND 65535
#define NAPMSGLEN 32
#define NAP_WAIT 8

#endif /* NAPMULTICAST */

