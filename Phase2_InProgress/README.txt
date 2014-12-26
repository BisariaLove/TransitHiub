Name - Love Bisaria
ID - 6840172822

Description:
We have 5 C++ Files namely, transit_hub.cpp , casino_a.cpp , casino_b.cpp , casino_c.cpp , casino_d.cpp. 'transit_hib.cpp' is the server for our system whose port number is 21922. The port is static.
Then we have 4 clients casino_a.cpp , casino_b.cpp , casino_c.cpp , casino_d.cpp. These clients communicate with the server using dynamic TCP ports. At every client we read from a file and prepare a vector saying the number of passengers at those who want to travel from the client to other casinos.
We communicate this information to the server i.e transit_hub.cpp.

Code Files:
5 Code File-
	- transit_hub.cpp: Server that gathers all the vectors from all the clients.
	- casino_a.cpp: Client file and sends the vector to transit_hub.
	- casino_b.cpp: Client file and sends the vector to transit_hub.
	- casino_c.cpp: Client file and sends the vector to transit_hub.
	- casino_d.cpp: Client file and sends the vector to transit_hub.
	
How to Run the code:
	- make clean
	- make
	- ./TransitHub
	- ./StopA
	- ./StopB
	- ./StopC
	- ./StopD
	
Reused Code:
	- I have used the code from 'Client.c' and 'Server.c' files mentioned in Beej's guide to network programming.