define ($IP 172.16.97.81);
define ($MAC 00:15:17:15:5d:74);

source :: FromDevice();
sink   :: ToDevice();

c :: Classifier(
	12/0806 20/0001,
	12/0806 20/0002,
	12/0800,
	-);

arpq :: ARPQuerier($IP, $MAC);
arpr :: ARPResponder($IP $MAC);

source -> c;
c[0] -> ARPPrint(OK) -> arpr -> sink;
c[1] -> [1]arpq;
Idle -> [0]arpq;
arpq -> ARPPrint(OK) -> sink;
c[2] -> CheckIPHeader(14) -> IPPrint(s) -> sink;
c[3] -> Discard;



