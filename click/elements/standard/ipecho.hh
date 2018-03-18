#ifndef CLICK_IPECHO_HH
#define CLICK_IPECHO_HH
#include <click/element.hh>
CLICK_DECLS

class IPEcho : public Element { public:

    IPEcho() {}
    ~IPEcho() {} 

    const char *class_name() const		{ return "IPEcho"; }
    const char *port_count() const		{ return PORTS_1_1; }
    // 'push' action will automatically call simple_action.
    Packet *simple_action(Packet *);


};

CLICK_ENDDECLS
#endif
