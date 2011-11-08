#ifndef NAP
#define NAP

/* maximum size that can be transfered atomically on a unix domain socket */
#define BUFFSIZE 	4096
/* known bug that this is not defined in sys/un.h */
#define UNIX_PATH_MAX   108

#define NAPSOCK 	"./napclient.sock"
/* drdata.co.dd */
#define NAPHOST		"184.71.163.218"
#define NAPPORT		29533

#define NAPRESPONSE_VALID 		"VALID"
#define NAPCOMMAND_VALIDATE 		"validate"
#define NAPPASSWORD_FILE_LOCATION 	"./passwd"

#endif /* NAP */
