//--------------- definition of the variables ----------------------------
define ($MACARP 00:15:17:15:5d:74)
define ($IP 172.16.97.81)

//--------------- definition of the networking interfaces -----------------
in :: FromDevice(0);
out :: ToDevice(0);

//---------------- definition of classifier for arp/ip ---------------------
c :: Classifier(
	12/0806 20/0001, // ARP request
    12/0800,
    -);
in -> c;


//----------------- resolve ARP ------------------------
arpr :: ARPResponder($IP $MACARP)
c[0] -> arpr -> out;


//------------------ Start Testing --------------------

reasem :: IPReassembler();

c[1] -> CheckIPHeader(14) -> reasem[0] -> IPPrint("abc", CONTENTS HEX, LENGTH true) -> ICMPPingResponder() -> EtherMirror() -> out;

//reasem[1] -> ICMPError(ADDR, timeexceeded, reassembly);

c[2] -> Discard();