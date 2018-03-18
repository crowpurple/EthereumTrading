#ifndef CLICK_NFV_GETTIMEVAL_HH
#define CLICK_NFV_GETTIMEVAL_HH
#include <click/element.hh>

CLICK_DECLS


/**
 *  This class sets time value for each passed packet.
 * */
class GetTimeVal : public Element { 

public:
    GetTimeVal(){}
    ~GetTimeVal(){} 

    const char *class_name() const		{ return "GetTimeVal"; }
    const char *port_count() const		{ return "2/2"; }
    // input 0 and output 0 is PULL, others are PUSH.
    const char *processing() const      { return "lh/lh"; }
    void push(int, Packet*);
    Packet* pull(int);
private:

};

CLICK_ENDDECLS
#endif
