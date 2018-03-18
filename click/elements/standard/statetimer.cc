#include <click/config.h>
#include "statetimer.hh"

CLICK_DECLS

int 
StateTimer::initialize(ErrorHandler *errh) {
    _timer.initialize(this);
    _timer.schedule_now();
    return 0;
}

void 
StateTimer::run_timer(Timer *timer) {
    assert(timer == &_timer);
    if ( NULL != g_ft ) {
        g_ft->ft_flush_state_timers();
    }
    _timer.schedule_after_sec(timer_cycle);
} 

void 
StateTimer::push(int port, Packet* p) {
    p->kill();
}


CLICK_ENDDECLS
EXPORT_ELEMENT(StateTimer)
