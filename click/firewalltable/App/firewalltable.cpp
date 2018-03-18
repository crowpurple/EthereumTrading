
#include <click/config.h>
#include "firewalltable.hh"
#include <click/glue.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/straccum.hh>
#include <click/etheraddress.hh>
#ifdef CLICK_USERLEVEL
#include <math.h>
#endif
#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include "sgx_urts.h"
#include "Enclave_u.h"
#define ENCLAVE_FILE "enclave.signed.so"
#define MAX_BUF_LEN 100

CLICK_DECLS

extern class firewalltable* g_ft;
sgx_enclave_id_t eid;
sgx_status_t ret=SGX_SUCCESS;
sgx_launch_token_t token ={25};
int updated = 0;

/*******************************************************************/
// Definition of class firewallstate. 
//
firewallstate::firewallstate():
    _fwt(g_ft),
{
	int enclave_ret = 0;
	firewallstate_t(eid,&enclave_ret);
	if(1!=enclave_ret)
		printf("firewallstate::firewallstate() fails\n");
}

firewallstate::~firewallstate() 
{
	int enclave_ret = 0;
	_firewallstate_t(eid,&enclave_ret);
	if(1!=enclave_ret)
		printf("firewallstate::~firewallstate() fails\n");
}

int firewallstate::fs_add_state_to_delete(struct entry* e) {
	int enclave_ret = 0;
	fs_add_state_to_delete_t(eid,&enclave_ret,e);
	if(1!=enclave_ret)
		printf("firewallstate::fs_add_state_to_delete(struct entry* e) fails\n");
    return 1;
}

int firewallstate::fs_update_state_by_tag(Packet* p) {
    const struct click_ip *iph = p->ip_header();
    const struct click_tcp *thd = p->tcp_header();
    struct state_entry *tmp= new state_entry();
    tmp->src_ip = (uint32_t)(iph->ip_src.s_addr);
    tmp->dst_ip = (uint32_t)(iph->ip_dst.s_addr);
    tmp->src_port = ntohs(thd->th_sport);
    tmp->dst_port = ntohs(thd->th_dport);
    int enclave_ret = 0;
    fs_update_state_by_tag_t(eid,&enclave_ret,tmp);
    if(1!=enclave_ret)
	printf("firewallstate::fs_update_state_by_tag(Packet* p) fails\n");
    return enclave_ret;
}

struct state_entry* firewallstate::fs_pop_from_deleted_state() {
    struct state_entry* enclave_ret = new state_entry();
    fs_pop_from_deleted_state_t(eid,&enclave_ret);
    if(NULL==enclave_ret)
	printf("firewallstate::fs_pop_from_deleted_state() fails\n");
    return enclave_ret;
}

int firewallstate::initialize(ErrorHandler * errh) {
    return 0;
}

int firewallstate::fs_flush_timers() {
    int enclave_ret = 0;
    fs_flush_timers_t(eid,&enclave_ret);
    if(1!=enclave_ret)
	printf("firewallstate::fs_update_state_by_tag(Packet* p) fails\n");
    return enclave_ret;
}

inline uint8_t firewallstate::fs_get_flag_by_packet(Packet* p) const {
    return static_cast<uint8_t>((p->tcp_header())->th_flags);
}

int firewallstate::fs_update_state(Packet* p, struct state_entry* s) {
    // get tcp header pointer.  
    const struct click_tcp* thd = p->tcp_header();
    // look packet's flags. 
    uint8_t th_flags = thd->th_flags;
    uint32_t th_seq=ntohl(thd->th_seq);
    uint32_t th_ack=ntohl(thd->th_ack);
    uint8_t mask = (TH_FIN) | (TH_SYN) | (TH_ACK) | (TH_RST);
    int enclave_ret = 0;
    // mask should be: 00010111
    fs_update_state_t(eid,&enclave_ret,mask,th_flags,th_seq,th_ack,s);
    if(1!=enclave_ret)
	printf("firewallstate::fs_update_state(Packet* p, struct state_entry* s) fails\n");
    return enclave_ret;
}

int firewallstate::fs_add_entry(Packet* p, struct entry* e, struct entry* rule) {
    // get tcp header pointer.  
    const struct click_tcp* thd = p->tcp_header();
    // look packet's flags. 
    uint8_t th_flags = thd->th_flags;
    uint32_t th_seq=ntohl(thd->th_seq);
    uint32_t th_ack=ntohl(thd->th_ack);
    uint8_t mask = (TH_FIN) | (TH_SYN) | (TH_ACK) | (TH_RST);
    int enclave_ret = 0;
    // mask should be: 00010111
    fs_add_entry_t(eid,&enclave_ret,e,rule,mask,th_flags,th_seq,th_ack);
    if(1!=enclave_ret)
	printf("firewallstate::fs_add_entry(Packet* p, struct state_entry* s) fails\n");
    return enclave_ret;
}

int firewallstate::fs_delete_entry(struct state_entry* s) {
    int enclave_ret = 0;
    fs_delete_entry_t(eid,&enclave_ret,s);
    if(1!=enclave_ret)
	printf("firewallstate::fs_delete_entry(struct state_entry* s) fails\n");
    return enclave_ret;
}

int firewallstate::fs_clear_states() {
    int enclave_ret = 0;
    fs_clear_states_t(eid,&enclave_ret);
    if(1!=enclave_ret)
	printf("firewallstate::fs_clear_states() fails\n");
    return enclave_ret;
}

struct state_entry* firewallstate::fs_check_entry(Packet* p)
{
    struct click_ip *iph = p->ip_header();
    struct click_tcp *thd = p->tcp_header();
    uint32_t src_ip = (uint32_t)(iph->ip_src.s_addr);
    uint32_t dst_ip = (uint32_t)(iph->ip_dst.s_addr);
    uint16_t src_port = (uint16_t)ntohs(thd->th_sport);
    uint16_t dst_port = (uint16_t)ntohs(thd->th_dport);
    struct state_entry* enclave_ret = new state_entry();
    fs_check_entry_t(eid,&enclave_ret,src_ip,dst_ip,src_port,dst_port);
    return enclave_ret;
}

int firewallstate::fs_suspend_state(struct state_entry* state) {
    int enclave_ret = 0;
    fs_suspend_state_t(eid,&enclave_ret,state);
    if(1!=enclave_ret)
	printf("firewallstate::fs_suspend_state(struct state_entry* state) fails\n");
    return enclave_ret;
}

int firewallstate::fs_attach_suspend_states(struct entry* e) {
    int enclave_ret = 0;
    fs_attach_suspend_states_t(eid,&enclave_ret,e);
    if(1!=enclave_ret)
	printf("firewallstate::fs_attach_suspend_states(struct entry* e) fails\n");
    return enclave_ret;
}

int firewallstate::fs_attach_state(struct entry* e, struct state_entry* s) {
    int enclave_ret = 0;
    fs_attach_state_t(eid,&enclave_ret,e,s);
    if(1!=enclave_ret)
	printf("firewallstate::fs_attach_state(struct entry* e, struct state_entry* s) fails\n");
    return enclave_ret;
}

////////////////////////print not used/////////////////
void firewallstate::fs_print_state(const struct state_entry* s, 
                              Packet *p = NULL, 
                              uint8_t purpose = 2 ) const 
{
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    // Called by other reason. 
    if ( NULL == p ) {
        src_ip = static_cast<uint32_t>(ntohl(s->src_ip));
        dst_ip = static_cast<uint32_t>(ntohl(s->dst_ip));
        src_port = s->src_port;
        dst_port = s->dst_port;
        printf("%hhu.%hhu.%hhu.%hhu:%hu + %hhu.%hhu.%hhu.%hhu:%hu [%s] (-)\n",
                static_cast<uint8_t>((src_ip>>24) & 0x0ff),
                static_cast<uint8_t>((src_ip>>16) & 0x0ff),
                static_cast<uint8_t>((src_ip>> 8) & 0x0ff),
                static_cast<uint8_t>(src_ip       & 0x0ff), src_port,
                static_cast<uint8_t>((dst_ip>>24) & 0x0ff),
                static_cast<uint8_t>((dst_ip>>16) & 0x0ff),
                static_cast<uint8_t>((dst_ip>> 8) & 0x0ff),
                static_cast<uint8_t>(dst_ip       & 0x0ff), dst_port,
                readable_state[static_cast<int>(s->state)] 
              );
    } else {
        const struct click_ip *iph = p->ip_header();
        const struct click_tcp *thd = p->tcp_header();
        src_ip = static_cast<uint32_t>(ntohl(iph->ip_src.s_addr)); 
        dst_ip = static_cast<uint32_t>(ntohl(iph->ip_dst.s_addr)); 
        src_port = static_cast<uint16_t>(ntohs(thd->th_sport));
        dst_port = static_cast<uint16_t>(ntohs(thd->th_dport));
        uint32_t seq = static_cast<uint32_t>(ntohl(thd->th_seq));
        uint32_t ack = static_cast<uint32_t>(ntohl(thd->th_ack));
        printf("%hhu.%hhu.%hhu.%hhu:%hu - %hhu.%hhu.%hhu.%hhu:%hu [%s] (%s) <%u>:<%u>\n",
                static_cast<uint8_t>((src_ip>>24) & 0x0ff),
                static_cast<uint8_t>((src_ip>>16) & 0x0ff),
                static_cast<uint8_t>((src_ip>> 8) & 0x0ff),
                static_cast<uint8_t>(src_ip       & 0x0ff), src_port,
                static_cast<uint8_t>((dst_ip>>24) & 0x0ff),
                static_cast<uint8_t>((dst_ip>>16) & 0x0ff),
                static_cast<uint8_t>((dst_ip>> 8) & 0x0ff),
                static_cast<uint8_t>(dst_ip       & 0x0ff), dst_port,
                readable_state[static_cast<int>(s->state)],
                (0 == purpose) ? ("s") : (1==purpose ? "f" : "-"),
                seq, ack);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct entry* firewalltable::ft_find_appended_entry_by_seq(uint16_t seq) {
    return find_appended_entry_by_seq(ipt, seq);
}

Packet *
print_SGX::simple_action(Packet *p)
{
	
	int num = 1;
	ret = sgx_create_enclave(ENCLAVE_FILE,1,&token,&updated,&eid,NULL);
	if(ret!=SGX_SUCCESS)
	{
		printf("App:error %#x,failed to create enclave.\n",ret);
                return p;
	}

	int enclave_ret = 0;
        foo(eid,&enclave_ret,num,ts1);

	if(SGX_SUCCESS != sgx_destroy_enclave(eid))
		printf("SGX_error!\n");
	printf("SGX_Success!\n");

  return p;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(print_SGX)
ELEMENT_REQUIRES(userlevel)


