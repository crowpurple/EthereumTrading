__author__ = 'zhizhong pan'


import subprocess
import os
import time
import sys


def do_collector(p_size, rate, r_size, itera):
    loop = str(r_size / 50)
    file_name = str(itera) + '_raw_' + p_size + '_' + rate + '_' + str(r_size)

    print "===============  " + str(itera) + " ===================  "

    click_id = os.fork()
    if click_id == 0:
        print '++++++++++++   create click0   !++++++++++++++'
        subprocess.call(['xl', 'create', '/local/nfv-exp/config.xen'])
        print '++++++++++++   start  firewall   !++++++++++++++'
        subprocess.call(['cosmos', 'start', 'click0', '/local/nfv-exp/firewall00.click'])
        print '++++++++++++   set console   !++++++++++++++'
        subprocess.call('xl console click0 > ./evaldata/rawdata/' + file_name, shell=True)
        sys.exit()
    else:
        time.sleep(10)
        subprocess.call(['/local/work/clickos/nfv/pypsender/batchsender.sh', str(loop)])
        time.sleep(1)
        subprocess.call(['ssh', 'hongdal@130.127.133.32', 'sudo iperf -c server -p 5001 -b ' + rate + ' -l ' + p_size +' > /dev/null 2>&1'])
        time.sleep(30)
        subprocess.call(['xl', 'destroy', 'click0'])
        time.sleep(5)
        print '+++++++++++++   destroy click0!   +++++++++++++'

def collector_data():

    packet_size_list = ['64B', '128B', '256B', '512B', '1024B']
    rate_list = ['10MB', '20MB', '40MB', '60MB', '90MB']
    firewall_rules_min = 0
    firewall_rules_max = 3300
    step = 300
    itera = 0

    for rule_size in range(firewall_rules_min, firewall_rules_max, step):
        for rate in rate_list:
            for packet_size in packet_size_list:
                itera += 1
                do_collector(packet_size, rate, rule_size, itera)
                time.sleep(2)

def collector_data_test():
    do_collector('64B', '40MB', 1200, 111)
   # time.sleep(2)
   # do_collector('128B', '20MB', 1000, 2)
   # time.sleep(20)
   # do_collector('256B', '40MB', 1500, 3)
   # time.sleep(20)
   # do_collector('512B', '60MB', 2000, 4)

if __name__ == '__main__':
   #collector_data()
   collector_data_test()
