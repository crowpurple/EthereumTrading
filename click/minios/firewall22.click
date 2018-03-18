//--------------- These values are for ARP ----------------------------
// xenbr00 traffic in. 
define ($br00MAC 00:00:00:00:02:00)
define ($tiMAC 00:00:00:00:01:00)
define ($tiIP 172.16.97.81)
// xenbr01 maintenance in. 
define ($br01MAC 00:00:00:00:02:01)
define ($miMAC 00:00:00:00:01:01)
define ($miIP 172.16.97.82)
// xenbr02 traffic out. 
define ($br02MAC 00:00:00:00:02:02)
define ($toMAC 00:00:00:00:01:02)
define ($toIP 172.16.97.83)
// xenbr03 maintenance out. Not used yet. No more than 3 bridges.   
define ($br03MAC 00:00:00:00:02:03)
define ($moMAC 00:00:00:00:01:03)
define ($moIP 172.16.97.83)

//--------------- definition of the networking interfaces -----------------
tIn :: FromDevice(0);
tInArp :: ToDevice(0); 
mIn :: FromDevice(1);
mInArp :: ToDevice(1); 
tOut :: ToDevice(2);

//--------------- definition of the filter (static firewall) --------------
//filter :: IPFilter(
//	allow src host 172.16.97.254,
//	allow src host 172.16.97.80,
//	deny all);

//---------------- definition of classifier for arp/ip ---------------------
ct      :: Classifier(
	12/0806 20/0001,    // ARP request
    12/0800,            // IP 
    -);
tIn -> ct;
cm      :: Classifier(
	12/0806 20/0001,    // ARP request
	12/0800 23/fd,      // IP test 
    -);
mIn -> cm;

//----------------- resolve ARP ------------------------
tiarpr :: ARPResponder($tiIP $tiMAC)
ct[0] -> tiarpr -> tInArp;
miarpr :: ARPResponder($miIP $miMAC)
cm[0] -> miarpr -> mInArp;


//------------------ Initialization --------------------
RandomSource(1) -> Initglobal() -> Discard();
//fwmatch :: firewallmatch();
fwman :: fwmanager(); 

filter :: IPFilter(
	allow src host 172.16.97.80,
	deny all);

//------------------ Start testing --------------------
// maintenance message 
cm[1] -> CheckIPHeader(14) -> fwman -> Discard(); 
cm[2] -> Discard();

// traffic. 
TrafficOut :: {
    input[0] -> StoreEtherAddress($br00MAC, 'src') -> StoreEtherAddress(fe:ff:ff:ff:ff:ff, dst) -> [0]output; 
}
ct[1] -> CheckIPHeader(14) -> filter;
filter[0] -> [0]TrafficOut[0] -> Print("matched!") -> tOut;
filter[1] -> Discard();
ct[2] -> Discard();



//firewall :: Firewall();

// messages for firewall maintenance.
//c1[1] -> CheckIPHeader(14) -> firewall -> messOut;
//c1[2] -> Discard();

// traffic 
//c[1] -> CheckIPHeader(14) -> filter;
// If it's an ICMP then reply, else do nothing.
//filter[0] -> ICMPPingResponder() -> EtherMirror() -> out;
//c[2] -> Discard();

