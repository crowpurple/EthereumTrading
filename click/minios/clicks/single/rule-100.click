define ($MACARP 00:15:17:15:5d:74)
define ($IP 172.16.97.81)
#define ($MAC 00:00:00:00:00:55);
in :: FromDevice();
out :: ToDevice();
#dropc :: Counter();
#ethh  :: StoreEtherAddress($MAC, 6);


filter :: IPFilter(
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 890 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 891 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 892 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 893 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 894 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 895 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 896 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 897 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 898 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 899 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 910 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 911 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 912 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 913 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 914 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 915 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 916 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 917 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 918 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 919 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 920 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 921 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 922 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 923 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 924 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 925 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 926 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 927 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 928 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 929 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 930 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 931 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 932 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 933 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 934 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 935 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 936 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 937 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 938 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 939 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 940 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 941 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 942 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 943 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 944 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 945 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 946 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 947 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 948 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 949 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 950 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 951 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 952 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 953 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 954 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 955 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 956 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 957 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 958 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 959 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 960 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 961 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 962 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 963 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 964 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 965 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 966 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 967 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 968 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 969 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 970 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 971 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 972 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 973 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 974 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 975 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 976 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 977 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 978 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 979 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 980 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 981 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 982 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 983 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 984 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 985 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 986 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 987 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 988 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 989 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 990 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 991 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 992 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 993 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 994 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 995 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 996 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 997 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 998 && ack,
allow src host 172.16.97.80 && dst host 172.16.97.81 && tcp && src port 22 && dst port 999 && ack,

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
c[1] -> CheckIPHeader(14) -> Print("start") -> filter
c[2] -> Discard();


//filter[0] -> Print("end") -> ICMPPingResponder() -> EtherMirror() -> out;
filter[0] -> Print("end") -> out;


