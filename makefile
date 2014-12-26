all: TransitHub StopA StopB StopC StopD

TransitHub: transit_hub.cpp
	g++ -o TransitHub transit_hub.cpp -lsocket -lnsl -lresolv

StopD: casino_d.cpp
	g++ -o StopD casino_d.cpp -lsocket -lnsl -lresolv

StopC: casino_c.cpp
	g++ -o StopC casino_c.cpp -lsocket -lnsl -lresolv

StopB: casino_b.cpp
	g++ -o StopB casino_b.cpp -lsocket -lnsl -lresolv

StopA: casino_a.cpp
	g++ -o StopA casino_a.cpp -lsocket -lnsl -lresolv

clean:
	rm TransitHub
	rm StopD
	rm StopC
	rm StopB
	rm StopA 	
