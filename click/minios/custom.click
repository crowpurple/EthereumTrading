define ($IP 172.16.97.81);
define ($MAC 00:15:17:15:5d:75);

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
c[0] -> ARPPrint -> arpr -> sink;
c[1] -> [1]arpq;
Idle -> [0]arpq;
arpr -> ARPPrint -> sink;
c[2] -> CheckIPHeader(14) -> ICMPPingResponder() -> EtherMirror() -> sink;
c[3] -> Discard;



