#ifndef NAP
#define NAP

/* maximum size that can be transfered atomically on a unix domain socket */
#define NAPBUFFSIZE 	4096
/* known bug that this is not defined in sys/un.h */
#define UNIX_PATH_MAX   108

#define NAPSOCK 	"/home/cal/cmpt433/private/myApps/cmpt433-nap-time/client/napclient.sock"
#define NAPPWFILE 	"./nappassword"
/* drdata.co.dd */
#define NAPHOST		"184.71.163.218"
#define NAPPORT		29533
/* how long to wait before revalidating */
/* #define NAPKEEPALIVE	300 */
#define NAPKEEPALIVE	30

#define NAPCOMMAND_VALIDATE 		"validate"
#define NAPRESPONSE_VALID 		"VALID"
/* generic response should be followed by an integer byte value */
#define NAPRESPONSE			"RESPONSE"

#endif /* NAP */
