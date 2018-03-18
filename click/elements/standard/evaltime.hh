#ifndef CLICK_NFV_EVALTIME_HH
#define CLICK_NFV_EVALTIME_HH
#include <click/element.hh>
#include <click/string.hh>
CLICK_DECLS

class EvalTime: public Element { public:

    EvalTime() CLICK_COLD;

    const char *class_name() const		{ return "EvalTime"; }
    const char *port_count() const		{ return PORTS_1_1; }

    int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;
    bool can_live_reconfigure() const		{ return true; }
    void push(int, Packet*);

    //Packet *simple_action(Packet *);

 private:
    String _label;
};

CLICK_ENDDECLS
#endif
