
#include <click/config.h>
#include "ipecho.hh"

#if 0
#include <click/glue.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/straccum.hh>
#endif

CLICK_DECLS



Packet *
IPEcho::simple_action(Packet *p)
{
    // Get paket data.
    uint8_t *pdata; 
    // caller must guarantee packet is an IP packet.
    // See clicknet/ip.h for struct "click_ip"
    const click_ip *iph = p->ip_header();
    int ip_len = ntohs(iph->ip_len);
    int payload_len = ip_len - (iph->ip_hl << 2);
 
    
    // see: http://en.wikipedia.org/wiki/List_of_IP_protocol_numbers
    // look the protocol number to see if this packet is a info packet.
    if ( iph->ip_p == 0x0fd && payload_len > 0) 
    {
        if ( NULL == 
            (pdata = (uint8_t *)malloc(sizeof(*pdata) * payload_len)) )
        {
            fprintf(stderr, "Memory fail: print info packets!\n");
            return p;
        }
        memcpy(pdata, (const uint8_t*)iph + (iph->ip_hl << 2), payload_len);
        for ( int i = 0; i  < payload_len; ++i )
        {
            printf("%c", pdata[i]); 
        }
        printf("\n"); 
    }

    return p;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(IPEcho)
//ELEMENT_MT_SAFE(IPEcho)
