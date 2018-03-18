#ifndef CLICK_NFV_SETTIMEVAL_HH
#define CLICK_NFV_SETTIMEVAL_HH
#include <click/element.hh>

CLICK_DECLS


/**
 *  This class sets time value for each passed packet.
 * */
class SetTimeVal : public Element { 

public:
    SetTimeVal(){}
    ~SetTimeVal(){} 

    const char *class_name() const		{ return "SetTimeVal"; }
    const char *port_count() const		{ return "1/1"; }
    // input 0 and output 0 is PULL, others are PUSH.
    const char *processing() const      { return "h/h"; }
    void push(int, Packet*);
    
private:

};

CLICK_ENDDECLS
#endif
