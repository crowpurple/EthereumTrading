#include <click/config.h>
#include "settimeval.hh"

CLICK_DECLS
void
SetTimeVal::push(int port, Packet* p) {
    long int sec;
    long int nsec;
    struct timespec tvl;
    // get current time 
    clock_gettime(CLOCK_REALTIME, &tvl);
    sec = tvl.tv_sec;
    nsec = tvl.tv_nsec;
    // put the values into users annos.
    // byptes 32-39 are used for timestap (confilct with timestap element)
    // see packet_anno.hh
    p -> set_anno_u32(32, (uint32_t)sec);
    p -> set_anno_u32(36, (uint32_t)nsec);
    output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SetTimeVal)
