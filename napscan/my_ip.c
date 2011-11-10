#include <stdio.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>

/* get our ip address so we can send it back to node who pinged us */
/* from http://www.geekpage.jp/en/programming/linux-network/get-ipaddr.php */
char * my_ip(void)
{
     int fd;
     static struct ifreq ifr;

     fd = socket(AF_INET, SOCK_DGRAM, 0);

     /* I want to get an IPv4 IP address */
     ifr.ifr_addr.sa_family = AF_INET;

     /* I want IP address attached to "eth0" */
     strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

     ioctl(fd, SIOCGIFADDR, &ifr);

     close(fd);

     /* display result */
     return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}


