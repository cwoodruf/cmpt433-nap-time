#ifndef NAP
#define NAP

/* maximum size that can be transfered atomically on a unix domain socket */
/* #define NAPBUFFSIZE 	4096 */
#define NAPBUFFSIZE 	80
/* known bug that this is not defined in sys/un.h */
#define UNIX_PATH_MAX   108

#define NAPSOCK 	"./napclient.sock"
#define NAPPWFILE 	"./nappassword"
#define NAPBASEDIR	"./files"
#define NAPINDEXFILE	"index.txt"
#define NAPPEERSFILE	"peers.txt"

/* drdata.co.dd */
#define NAPHOST		"184.71.163.218"
#define NAPPORT		29533

/* how long to wait before revalidating */
#define NAPKEEPALIVE	300

#define NAPCOMMAND_VALIDATE	"validate"
#define NAPCOMMAND_PEERS 	"peers"
#define NAPCOMMAND_INDEX 	"index"
#define NAPCOMMAND_GETFILE 	"getfile"
#define NAPCOMMAND_STREAM 	"stream"

/* generic response should be followed by an integer byte value */
#define NAPRESPONSE		"RESPONSE"

/* other single line response codes */
#define NAPRESPONSE_VALID 	"VALID"
#define NAPRESPONSE_FILEERR 	"FILEERR"
#define NAPRESPONSE_OK 		"OK"
#define NAPRESPONSE_NOFILE 	"NOFILE"

#endif /* NAP */
