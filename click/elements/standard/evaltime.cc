/*
 * print.{cc,hh} -- element prints packet contents to system log
 * John Jannotti, Eddie Kohler
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 * Copyright (c) 2008 Regents of the University of California
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include "evaltime.hh"
#include <click/glue.hh>
#include <click/args.hh>
#include <click/error.hh>
#include <click/straccum.hh>

CLICK_DECLS

long int my_time_second;
long int my_time_vsecond;
struct timespec my_tvl;

EvalTime::EvalTime()
{}

int
EvalTime::configure(Vector<String> &conf, ErrorHandler* errh)
{
    String label; 

    if (Args(conf, this, errh)
    .read_p("LABEL", label)
    .complete() < 0)
        return -1;
    
    _label = label;
    return 0;
}

void
EvalTime::push(int k, Packet*p)
{

    if ( strcmp(_label.c_str(), "start") == 0 ) {
        //    gettimeofday(&my_tvl, NULL);
        clock_gettime(CLOCK_REALTIME, &my_tvl);
        //    my_time_second = my_tvl.tv_sec;
        my_time_vsecond = my_tvl.tv_nsec;        
        //    printf("STARTING...\n");
    }
    else if ( strcmp(_label.c_str(), "end") == 0 ) {
        clock_gettime(CLOCK_REALTIME, &my_tvl);
        //my_time_second = my_tvl.tv_sec;
        //my_time_vsecond = my_tvl.tv_nsec;        
        //gettimeofday(&my_tvl, NULL);
        //my_time_second = my_tvl.tv_sec - my_time_second;
        my_time_vsecond = my_tvl.tv_nsec - my_time_vsecond;
        printf("%ld\n", my_time_vsecond);    
    }
    output(0).push(p);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(EvalTime)
ELEMENT_MT_SAFE(EvalTime)
