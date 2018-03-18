#include "Enclave_t.h"

const uint32_t debug_flush_cycle = 1000;
const uint32_t init_state_size = 65536;
const uint32_t tcp_expiration = 9;
const uint32_t syn_expiration = 2;
const uint32_t timer_cycle = 4;
const uint32_t rule_flush_cycle = 1000; 
const uint32_t rule_active_time = 5;
struct state_entry* _free=NULL;
struct state_entry* _use=NULL;
uint32_t _deleted_state_count=0;
struct state_entry* _delete_=NULL;
struct state_entry* _suspend_=NULL;
struct ipchain* ipt=NULL;

int getChainSize(ipchain *c)
{
	return c->size;
}
struct entry* allocEntry()
{
	entry *e = new entry();
    	e->delete_pre = NULL;
    	e->delete_next = NULL;
    	e->append_pre = NULL;
    	e->append_next = NULL;
    	e->connection = NULL;
    	e->d_a = 0;
    	e->seq = 0;
    	e->active_time = 0;
	return e;
}
int copyEntry(entry *e1, entry *e2)
{
	if (!(e1 && e2))
		return 0;
	e1->src_ip = e2->src_ip;
	e1->src_ip_mask = e2->src_ip_mask;
	e1->des_ip = e2->des_ip;
	e1->des_ip_mask = e2->des_ip_mask;
	e1->src_port_min = e2->src_port_min;
	e1->src_port_max = e2->src_port_max;
	e1->des_port_min = e2->des_port_min;
	e1->des_port_max = e2->des_port_max;

	e1->protocol = e2->protocol;
	e1->action = e2->action;
	
        return 1;
}
struct ipchain* allocChain()
{
	ipchain *c = new ipchain();
	struct entry* head = allocEntry();
	struct entry* tail = allocEntry();

	head->pre = NULL;
	head->next = tail;

	tail->pre = head;
	tail->next = NULL;

	c->head = head;
	c->tail = tail;
	c->size = 0;
    	c->_delete_ = NULL;
    	c->_delete_size_ = 0;
    	c->_append_ = NULL;
    	c->_append_size_ = 0;
    	c->delete_refs = 0;
    	c->delete_seq = 0;
	return c;
}
struct entry* getEntryByIndex(ipchain *c, int index)
{
	if (index > c->size - 1)
	{
		return NULL;
	}
	struct entry *cur = c->head->next;
	while(cur) {
		if (index == 0)
			return cur;
		else{
			cur = cur->next;
			index--;
		}
	}
	return NULL;
}
int addEntryAtTail(ipchain *c, entry *e)
{
    	if (!(c && e)) {
		return 0;
    	}
	struct entry* tmp = c->tail->pre;
    	e->next = c->tail;
    	e->pre = tmp;
	tmp->next = e;
	c->tail->pre = e;
	c->size++;
    	return 1;
}
int addEntryAtHead(ipchain *c, entry *e)
{
    	if (!(c && e))
		return 0;
	struct entry* tmp = c->head->next;
	e->next = tmp;
	e->pre = c->head;
	tmp->pre = e;
	c->head->next = e;
	c->size++;
   	return 1;
}
int delEntryByIndex(ipchain *c, int index)
{
	if (!c)
		return 0;
	if (index > c->size - 1)
	{
		return 0;
	}
	struct entry* tmp = getEntryByIndex(c, index);
	tmp->pre->next = tmp->next;
	tmp->next->pre = tmp->pre;
	free(tmp);
    	c->size--;
	return 1;
}
struct entry* nextEntry(ipchain *c, entry *e){
	return e->next;
}
int isEntryEqual(ipchain *c, entry *e1, entry *e2){
	if (e1->src_ip == e2->src_ip &&
		e1->src_ip_mask == e2->src_ip_mask &&
		e1->des_ip == e2->des_ip &&
		e1->des_ip_mask == e2->des_ip_mask &&
		e1->src_port_min == e2->src_port_min &&
		e1->src_port_max == e2->src_port_max &&
		e1->des_port_min == e2->des_port_min &&
		e1->des_port_max == e2->des_port_max &&
		e1->protocol == e2->protocol &&
		e1->action == e2->action)
		return 1;
	else
		return 0;
}
int isEntryMatch(ipchain *c, entry *e1, entry *e2){
	if (e2->src_port_min < e1->src_port_min ||
		e2->src_port_max > e1->src_port_max ||
		e2->des_port_min < e1->des_port_min ||
		e2->des_port_max > e1->des_port_max)
		return 0;
	if (ntohl(e2->des_ip) < ntohl(e1->des_ip) || (ntohl(e2->des_ip) > ntohl(e1->des_ip | e1->des_ip_mask)) ||
	    ntohl(e2->src_ip) < ntohl(e1->src_ip) || (ntohl(e2->src_ip) > ntohl(e1->src_ip | e1->src_ip_mask)))
        	return 0;
	if (e1->protocol != e2->protocol)
		return 0;
	return 1;
}
int add_to_delete(ipchain *c, entry *e) {
    e->delete_pre = NULL;
    e->delete_next = c->_delete_;
    if ( NULL != c->_delete_ ) {
        c->_delete_->delete_pre = e;
    }
    c->_delete_ = e;
    c->_delete_size_++;
    if ( 0x06 != e->protocol && NULL != e->connection ) {
        c->delete_refs++;
    }
    e->d_a = 1;
    c->delete_seq++;
    e->seq = c->delete_seq;
    return 1;   
}
uint32_t get_delete_size(ipchain *c) {
    return c->_delete_size_; 
}
uint16_t get_delete_seq(ipchain *c) {
    return c->delete_seq;
}
struct entry* find_appended_entry_by_seq(ipchain *c, uint16_t seq) {
    struct entry* ret = c->_append_;
    if ( NULL != ret ) {
        while ( NULL != ret->append_next ) {
            if ( seq == ret->seq ) {
                return ret;
            } else {
                ret = ret->append_next;
            }
        }
        if ( seq == ret->seq ) {
            return ret;
        }
    }
    return NULL;
}
struct entry* pop_from_delete(ipchain *c) {
    struct entry* ret = NULL;
    if ( c->_delete_size_ > 0 && NULL != c->_delete_) {
        ret = c->_delete_;
        c->_delete_ = ret->delete_next;
        ret->delete_next = NULL;
        ret->delete_pre = NULL;
        if ( NULL != c->_delete_ ) {
            c->_delete_->delete_pre = NULL;
        }
        c->_delete_size_--; 
        ret->pre->next = ret->next;
        ret->next->pre = ret->pre;
        c->size--;
        ret->next = NULL;
        ret->pre = NULL;
    }
    return ret;
}
int add_to_append(ipchain *c, entry *e) {
    e->append_pre = NULL;
    e->append_next = c->_append_;
    if ( NULL != c->_append_ ) {
        c->_append_->append_pre = e;
    }
    c->_append_ = e;
    c->_append_size_++;
    if ( addEntryAtTail(c, e) != 0 ) {
        return 1;
    } else {
        return 0;
    }
}
int firewallstate_t()
{
	int index = 0;
    	struct state_entry* tmp;
    	while ( index < (int)init_state_size ) {
        	tmp = new state_entry();
        	tmp->sib_pre = NULL;
        	tmp->sib_next = NULL;
        	tmp->rule = NULL;
        	tmp->store_pre = NULL;
        	tmp->store_next = _free;
        	if ( NULL != _free ) {
            		_free->store_pre = tmp;
        	}
        	_free = tmp;
        	index++;
    	}
	return 1;
}
int _firewallstate_t()
{
	struct state_entry* tmp;
    	struct state_entry* kill;
    	tmp = _use;
    	while ( NULL != tmp ) {
        	kill = tmp;
        	tmp = tmp->store_next;
        	delete kill;
    	}
    	tmp = _free;
    	while ( NULL != tmp ) {
        	kill = tmp;
        	tmp = tmp->store_next;
        	delete kill;
    	}
	return 1;
}
int fs_add_state_to_delete_t(entry* e)
{
	if ( NULL != e->connection ) {
        	struct ipchain* c = ipt;
        	struct state_entry *last = e->connection;
        	while ( NULL != last ) {
            		last->seq = e->seq;
            		last->delete_pre = NULL;
            		last->delete_next = _delete_;
            		if ( NULL != _delete_ ) {
                		_delete_->delete_pre = last;
            		}
            		_delete_ = last;
            		_deleted_state_count++;
            		// update delete_refs
            		if ( 0 != last->active ) {
                		c->delete_refs++;
            		}
            	last = last->sib_next;
        	}
    	}
	return 1;
}
int fs_update_state_by_tag_t(state_entry* e)
{
	struct state_entry* s;
    	s = _delete_;
    	while ( NULL != s ) {
        	if ( ((e->src_ip == s->src_ip)&&
             	 	(e->dst_ip == s->dst_ip)&&
              		(e->src_port == s->src_port)&&
              		(e->dst_port == s->dst_port)) || 
             		((e->src_ip == s->dst_ip)&&
              		(e->dst_ip == s->src_ip)&&
              		(e->src_port == s->src_port)&&
              		(e->dst_port == s->dst_port)) || 
             		((e->src_ip == s->src_ip)&&
             		 (e->dst_ip == s->dst_ip)&&
              		(e->src_port == s->dst_port)&&
            		  (e->dst_port == s->src_port)) || 
            		 ((e->src_ip == s->dst_ip)&&
             		 (e->dst_ip == s->src_ip)&&
             		 (e->src_port == s->dst_port)&&
              		(e->dst_port == s->src_port))
           	) {
            // If the matched state is active, then turn it off
            // and decrease delete_refs.
            		if ( 0 != s->active ) {
               			s->active = 0;
                		ipt->delete_refs--;
                		return 1;
            		}
        	}
        	s = s->sib_next;
    	}	
	return 1;
}
struct state_entry* fs_pop_from_deleted_state_t()
{
    	struct state_entry* ret = _delete_;
    	// pop from _delete_ link. 
    	if ( 0 == _deleted_state_count || NULL == _delete_ ) {
        	return NULL;  
    	}
    	_delete_ = ret->sib_next;
    	ret->sib_next = NULL;
    	if ( NULL != _delete_ ) {
        	_delete_->sib_pre = NULL;
    	} 
    	// pop from _use link. 
    	if ( NULL != ret->store_pre ) {
        	ret->store_pre->store_next = ret->store_next;
    	} else {
        	_use = ret->store_next;
    	}
    	if ( NULL != ret->store_next ) {
        	ret->store_next->store_pre = ret->store_pre;
    	}
    	return ret;
}
int fs_flush_timers_t()
{
// update timers in each state entry and delete if necessarry.
    	struct state_entry* tmp;
    	struct state_entry* kill;
    	int ret = 1;
    	tmp = _use;
    	while ( NULL != tmp ) {
        	tmp->life_time--;
        	if ( static_cast<signed int>(tmp->life_time) <= 0 ) {
            	kill = tmp;
            	tmp = tmp->store_next;
            	kill->state = CLOSE;
            	ret = fs_delete_entry_t(kill);
        	} else {
            		tmp = tmp->store_next;
        	}
    	}
    	return ret;
}
int fs_update_state_t(uint8_t mask, uint8_t th_flags, uint32_t th_seq, uint32_t th_ack, state_entry* s)
{
    int ret=1;
    switch ( th_flags & mask ) {
        // fin. 
        case 0x01: {
            // check. 
            if ( s->state == ESTABLISHED  ) {
                s->fin1_ack = th_seq;
                s->state = FIN_1;
                s->life_time = tcp_expiration;
            } else if ( s->state == FIN_1 ) {
                s->fin2_ack = th_seq;
                s->state = FIN_2;
                s->life_time = tcp_expiration;
            } else if ( s->state == CLOSING_WAIT ) {
                s->fin2_ack = th_seq; 
                s->state = LAST_ACK;
                s->life_time = syn_expiration;
            } else {
                // invalid fin. 
                ret = 0;
            }
            break;
        }
        // syn. 
        case 0x02: {
            // check. 
            if ( s->state == CLOSE ) {
                s->syn1_ack = th_seq;
                s->state = SYN_1;
                s->life_time = syn_expiration;
            } else {
                // invalid syn. 
                ret = 0;
            }
            break;
        }
        // ack. 
        case 0x10: {
            // check. 
            if ( s->state == ESTABLISHED ) {
                // pass this packet. 
                s->life_time = tcp_expiration;
            } else if ( s->state == SYN_2 && (th_ack == s->syn2_ack+1) ) {
                s->state = ESTABLISHED;
                s->life_time = tcp_expiration;
            } else if ( s->state == FIN_1 ) {
                // This ack is for the first FIN.  
                if ( th_ack == s->fin1_ack+1 ) {
                    s->state = CLOSING_WAIT;
                    s->life_time = tcp_expiration;
                }
            } else if ( s->state == FIN_2 ) {
                // This ack is for the second FIN. 
                if ( th_ack == s->fin2_ack+1 ) {
                    s->state = LAST_ACK;
                    s->life_time = syn_expiration;
                }
            } else if ( s->state == LAST_ACK && (th_ack == s->fin2_ack+1) ) {
                s->state = CLOSE;
                ret = fs_delete_entry_t(s);
            } else {
                // invalid ack. 
                ret = 0;
            }
            break;
        }
        // fin + ack 
        case 0x11: {
            // check. 
            if ( s->state == ESTABLISHED ) {
                s->fin1_ack = th_seq;
                s->state = FIN_1;
                s->life_time = tcp_expiration;
            } else if ( s->state == FIN_1 ) { 
                // reply to the previous fin. 
                if ( th_ack == s->syn1_ack+1 ) {
                    s->state = LAST_ACK;
                    s->life_time = syn_expiration;
                // this ack reply to other data segments, not previous fin. 
                } else {
                    s->state = FIN_2; 
                    s->life_time = tcp_expiration;
                }
                s->fin2_ack = th_seq;
            // this case, ack must reply to a data segment. 
            } else if ( s->state == CLOSING_WAIT ) {
                s->state = LAST_ACK;
                s->fin2_ack = th_seq;
                s->life_time = syn_expiration;
            } else {
                // invalid fin + ack.
                ret = 0;
            }
            break;
        }
        // syn + ack 
        case 0x12: {
            // check. 
            if ( s->state == SYN_1 && (th_ack == s->syn1_ack+1)) {
                s->syn2_ack = th_seq;
                s->state = SYN_2;
                s->life_time = syn_expiration;
            } else {
                // invalid syn+ack. 
                ret = 0;
            }
            break;
        }
        // rst (+ack)
        case 0x14: case 0x04: {
            // check. 
            s->state = CLOSE;
            // should delete this node.
            ret = fs_delete_entry_t(s);
            // reset this connection. 
            break;
        }
        // None of the above. 
        default: {
            // invalid tcp flag.  
            ret = 0;
        }
    }
    return ret;
}
int fs_add_entry_t(entry* e, entry* rule, uint8_t mask,uint8_t th_flags, uint32_t th_seq, uint32_t th_ack)
{
    struct state_entry *tmp = NULL; 
    // If free is not empty, grab a node from 'free'
    if ( NULL != _free ) {
        tmp = _free;
        _free = _free->store_next;
        if ( NULL != _free ) {
            _free->store_pre = NULL;
        }
    // free is empty. 
    } else {
        tmp = new state_entry(); 
        if ( NULL == tmp ) {
            fprintf(stderr, "Could not allocate state entry: out of memory!\n");
            fflush(stderr);
            return 0;
        }
    }
    // fill up new state node. 
    tmp->src_ip = e->src_ip;
    tmp->dst_ip = e->des_ip;
    tmp->src_port = e->src_port_min;
    tmp->dst_port = e->des_port_min;
    tmp->protocol = e->protocol;
    // initial state is set to CLOSE
    tmp->state = CLOSE;
    // Try to update the state of this state entry. 
    int ret = fs_update_state_t(mask, th_flags, th_seq, th_ack, tmp);
    // If updated successfully. 
    if ( 0 != ret ) {
        // mark this state_entry as active. 
        tmp->active = 1;    

        // append to 'use'
        tmp->store_pre = NULL;
        tmp->store_next = _use;
        if ( NULL != _use ) {
            _use->store_pre = tmp;
        }
        _use = tmp;
        // append to 'rule'
        tmp->rule = rule;
        tmp->sib_pre = NULL;
        tmp->sib_next = rule->connection;
        if ( NULL != rule->connection ) {
            rule->connection->sib_pre = tmp;
        }
        rule->connection = tmp;
        // if this rule is in '_delete_' link, 
        // add to _delete_, increase 'delete_refs'.
        if ( 1 == rule->d_a ) {
            tmp->delete_pre = NULL;
            tmp->delete_next = _delete_;
            if ( NULL != _delete_ ) {
                _delete_->delete_pre = tmp;
            }
            _delete_ = tmp;
            _deleted_state_count++;
            ipt->delete_refs++; 
        }
    // Otherwise, append it back to 'free'
    } else {
        tmp->active = 0;
        // append it to 'free' link. 
        tmp->rule = NULL;
        tmp->sib_pre = NULL;
        tmp->sib_next = NULL;
        tmp->store_pre = NULL;
        tmp->store_next = _free;
        tmp->delete_pre = NULL;
        tmp->delete_next = NULL;
        if ( NULL != _free ) {
            _free->store_pre = tmp;
        }
        _free = tmp; 
    }
    return ret;
}
int fs_delete_entry_t(state_entry* s)
{
    // This node is not in use or an error may occur to this node,
    // It's illegal that a state entry is associated to none of the rules. 
    if ( NULL == s->rule ) {
        return 0;
    }
    // if it's the first node. update rule->connection. 
    if ( NULL == s->sib_pre ) {
        s->rule->connection = s->sib_next;
    // else it's not first node. update its previous node. 
    } else {
        s->sib_pre->sib_next = s->sib_next;
    }
    // if it's not the last node. update its next node. 
    if ( NULL != s->sib_next ) {
        s->sib_next->sib_pre = s->sib_pre;
    }
    // garuantee it move clear from rule links.    
    s->sib_pre = s->sib_next = NULL;

    // if it's the first one, the update _use. 
    if ( NULL == s->store_pre ) {
        _use = s->store_next;
    // else it's not the first one. update its previous node. 
    } else {
        s->store_pre->store_next = s->store_next;
    }
    // if it's not the last one, update its next node. 
    if ( NULL != s->store_next ) {
        s->store_next->store_pre = s->store_pre;
    }

    // If the related TCP rule is in '_delete_' link. 
    // decrease delete_refs by 1. 
    if ( 1 == s->rule->d_a ) {
        // remove from _delete_
        if ( NULL != s->delete_pre ) {
            s->delete_pre->delete_next = s->delete_next;
        } else {
            _delete_ = s->delete_next;
        }
        if ( NULL != s->delete_next ) {
            s->delete_next->delete_pre = s->delete_pre;
        }
        s->delete_next = s->delete_pre = NULL;
        _deleted_state_count--;
        if ( 0 != s->active ) {
            ipt->delete_refs--;
        }
    }
    s->rule = NULL;
        
    // append it to 'free' link. 
    s->store_pre = NULL;
    s->store_next = _free;
    if ( NULL != _free ) {
        _free->store_pre = s;
    }
    _free = s; 
    return 1;
}
int fs_clear_states_t()
{
    int ret = 1; 
    struct state_entry* tmp = _use;
    while ( NULL != tmp ) {
        ret = fs_delete_entry_t(tmp);
        tmp = _use;
    }
    return ret;
}
struct state_entry* fs_check_entry_t(uint32_t src_ip,uint32_t dst_ip,uint16_t src_port,uint16_t dst_port)
{
    struct state_entry* s;
    s = _use;
    while ( NULL != s ) {
        if ( src_ip == s->src_ip ) {
            if ( dst_ip == s->dst_ip ) {
                if ( src_port == s->src_port ) {
                    if ( dst_port == s->dst_port ) {
                        // found;
                        return s;
                    }
                } else if ( dst_port == s->src_port ) {
                    if ( src_port == s-> dst_port ) {
                        // found; 
                        return s;
                    }
                }
            }
        } else if ( dst_ip == s->src_ip ) {
            if ( src_ip == s->dst_ip ) {
                if ( src_port == s->src_port ) {
                    if ( dst_port == s->dst_port ) {
                        // found;
                        return s;
                    }
                } else if ( dst_port == s->src_port ) {
                    if ( src_port == s-> dst_port ) {
                        // found; 
                        return s;
                    }
                }
            }
        }
        s = s->store_next;
    }
    return NULL;
}
int fs_suspend_state_t(state_entry* state)
{
    // grab a chunk of memory
    struct state_entry* s;
    if ( NULL != _free ) {
        s = _free;
        _free = _free->store_next;
    } else {
        s = new state_entry();
    }
    
    // fill up the memory. -- costly.
    s->src_ip=state->src_ip;
    s->dst_ip=state->dst_ip;
    s->src_port=state->src_port;
    s->dst_port=state->dst_port;
    s->protocol=state->protocol;
    s->state=state->state;
    s->life_time=state->life_time;
    s->syn1_ack=state->syn1_ack;
    s->syn2_ack=state->syn2_ack;
    s->fin1_ack=state->fin1_ack;
    s->fin2_ack=state->fin2_ack;
    s->sib_pre=state->sib_pre;
    s->sib_next=state->sib_next;
    s->delete_pre=state->delete_pre;
    s->delete_next=state->delete_next;
    s->suspend_pre=state->suspend_pre;
    s->suspend_next=state->suspend_next;
    s->rule=state->rule;
    s->seq=state->seq;
    s->active=state->active;
    s->store_next = s->store_pre = NULL;
    
    // append to _suspend_.
    if ( NULL == _suspend_ ) {
        state->suspend_pre = NULL;
        state->suspend_next = NULL;
        _suspend_ = state;
        return 1;
    }
    struct state_entry* tmp = _suspend_;
    while ( NULL != tmp->suspend_next ) {
        // suspend in ascending order.
        if ( s->seq > tmp->seq ) {
            tmp = tmp->suspend_next;
        } else {
            s->suspend_next = tmp;
            s->suspend_pre = tmp->suspend_pre;
            if ( NULL != tmp->suspend_pre ) {
                tmp->suspend_pre->suspend_next = s;
            }
            tmp->suspend_pre = s;
            return 1;
        }
    }
    if ( s->seq > tmp->seq ) {
        s->suspend_pre = tmp;
        s->suspend_next = tmp->suspend_next;
        tmp->suspend_next = s;
        return 1;
    } else {
        s->suspend_next = tmp;
        s->suspend_pre = tmp->suspend_pre;
        if ( NULL != tmp->suspend_pre ) {
            tmp->suspend_pre->suspend_next = s;
        }
        tmp->suspend_pre = s;
        return 1;
    }
}
int fs_attach_suspend_states_t(entry* e)
{
    if ( NULL == _suspend_ ) {
        return 1;
    }
    struct state_entry* state = _suspend_;
    while ( NULL != state ) {
        if ( e->seq < state->seq ) {
            state = state->suspend_next;
        } else if ( state->seq == e->seq ) {
            // append state to e->connection
            state->sib_next = e->connection;
            if ( NULL != e->connection ) {
                e->connection->sib_pre = state;
            }
            state->sib_pre = NULL;
            e->connection = state; 
            // append state to _use.
            state->store_next = _use;
            if ( NULL != _use ) {
                _use->store_pre = state;
            }
            state->store_pre = NULL;
            _use = state;
            // teardown from _suspend_.
            if ( NULL != state->suspend_pre ) {
                state->suspend_pre->suspend_next = state->suspend_next;
            }
            if ( NULL != state->suspend_next ) {
                state->suspend_next->suspend_pre = state->suspend_pre;
            }
            // set to active. 
            state->active = 0x01;
            state->seq = 0;
            // move to the next
            state = state->suspend_next;
        } else {
            return 1;
        }
    } 
    return 1;
}
int fs_attach_state_t(entry* e, state_entry* s)
{
    struct state_entry *tmp = NULL; 
    // can grab from _free?
    if ( NULL != _free ) {
        tmp = _free;
        _free = _free->store_next;
        if ( NULL != _free ) {
            _free->store_pre = NULL;
        }
    // can't grab from _free, allocate it yourself. 
    } else {
        tmp = new state_entry();
        if ( NULL == tmp ) {
            return 0;
        }
    }
    // fill up new state node. 
    tmp->src_ip=s->src_ip;
    tmp->dst_ip=s->dst_ip;
    tmp->src_port=s->src_port;
    tmp->dst_port=s->dst_port;
    tmp->protocol=s->protocol;
    tmp->state=s->state;
    tmp->life_time=s->life_time;
    tmp->syn1_ack=s->syn1_ack;
    tmp->syn2_ack=s->syn2_ack;
    tmp->fin1_ack=s->fin1_ack;
    tmp->fin2_ack=s->fin2_ack;
    tmp->sib_pre=s->sib_pre;
    tmp->sib_next=s->sib_next;
    tmp->delete_pre=s->delete_pre;
    tmp->delete_next=s->delete_next;
    tmp->suspend_pre=s->suspend_pre;
    tmp->suspend_next=s->suspend_next;
    tmp->rule=s->rule;
    tmp->seq=s->seq;
    tmp->active=s->active;
    // append to _use. storage management.
    tmp->store_pre = NULL;
    tmp->store_next = _use;
    if ( NULL != _use ) {
        _use->store_pre = tmp;
    }
    _use = tmp;
    // append to 'e', state management.  
    tmp->rule = e;
    tmp->sib_pre = NULL;
    tmp->sib_next = e->connection;
    if ( NULL != e->connection ) {
        e->connection->sib_pre = tmp;
    }
    // set to active. 
    tmp->active = 0x01;
    tmp->seq = 0;
    return 1;
}
