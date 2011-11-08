#ifndef NAP
#define NAP

/* maximum size that can be transfered atomically on a unix domain socket */
#define BUFFSIZE 	4096
/* known bug that this is not defined in sys/un.h */
#define UNIX_PATH_MAX   108

#define NAPSOCK 	"./napclient.sock"
#define NAPPWFILE 	"./nappassword"
/* drdata.co.dd */
#define NAPHOST		"184.71.163.218"
#define NAPPORT		29533
/* how long to wait before revalidating */
#define NAPKEEPALIVE	300

#define NAPCOMMAND_VALIDATE 		"validate"
#define NAPRESPONSE_VALID 		"VALID"

#endif /* NAP */
