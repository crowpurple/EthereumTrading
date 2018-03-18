#include <click/config.h>
#include "gettimeval.hh"

CLICK_DECLS
void
GetTimeVal::push(int port, Packet* p) {
    if ( 0 == port ) {
        printf("port_0 must be connected to pull, packet dropped!\n");
        fflush(stdout);
        p -> kill();
        return;
    }
    long int sec, dsec;
    long int nsec, dnsec;
    struct timespec tvl;
    // get the values from users annos.
    sec = (long int)(p->anno_u32(32));
    nsec =(long int)(p->anno_u32(36));
    // get current time 
    clock_gettime(CLOCK_REALTIME, &tvl);
    dsec = (tvl.tv_sec - sec) * 1000000000;
    dnsec = tvl.tv_nsec - nsec + dsec;
    printf("@,%ld,#\n", dnsec);
    // don't need flush()
    output(1).push(p);
}

Packet*
GetTimeVal::pull(int port) {
    if ( 1 == port ) {
        printf("port_1 must be connected to push, packet dropped!\n");
        fflush(stdout);
        return 0;
    }
    // Grab packet from input 0
    Packet* p = input(0).pull();
    if ( 0 == p ) {
        return 0;
    }
    long int sec, dsec;
    long int nsec, dnsec;
    struct timespec tvl;
    // get the values from users annos.
    sec = (long int)(p->anno_u32(32));
    nsec =(long int)(p->anno_u32(36));
    // get current time 
    clock_gettime(CLOCK_REALTIME, &tvl);
    dsec = (tvl.tv_sec - sec) * 1000000000;
    dnsec = tvl.tv_nsec - nsec + dsec;
    printf("@,%ld,#\n", dnsec);
    // don't need flush()
    return p;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(GetTimeVal)
