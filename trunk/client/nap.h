#ifndef NAP
#define NAP

/* maximum size that can be transfered atomically on a unix domain socket */
#define BUFFSIZE 	4096
/* known bug that this is not defined in sys/un.h */
#define UNIX_PATH_MAX   108

#define NAPSOCK 	"./napclient.sock"
#define NAPHOST		"drdata.co.cc"
#define NAPPORT		29533

#endif /* NAP */
