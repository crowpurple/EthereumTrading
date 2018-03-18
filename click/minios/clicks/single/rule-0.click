define ($MACARP 00:15:17:15:5d:74)
define ($IP 172.16.97.81)
in :: FromDevice();
out :: ToDevice();

filter :: IPFilter(

	allow ack && src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 44167,
	allow ack && src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 44167 && dst port 22,
	deny all);

c      :: Classifier(
	12/0806 20/0001, // ARP request
	12/0800,
	-);

arpr :: ARPResponder($IP $MACARP)

in -> c;

c[0] -> arpr -> out;
c[1] -> CheckIPHeader(14) ->  Print("start") -> filter;
c[2] -> Discard();


filter[0] -> Print("end") -> out;


