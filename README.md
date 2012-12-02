# Distributed Event Coordination System “virtualmem” in C on a UNIX-based platform.



### SYNOPSIS:
To run this program:
> **dec_server [−h] [-p port-number] [−l file]**

*	**−h** : Print a usage summary with all options and exit.
*	**−p** : *port-number* : Listen on the given port. If not provided, **dec_server** will listen on port 9090.
*	**−l** : *file* : Log all requests and responses to the given file. If not, print all to *stdout*.



### DESCRIPTION:
*dec_server* is the server component of the Distributed Event Coordination (DEC) System. It will listen on a certain port number for the incoming requests from DEC clients. There can be three different requests coming from clients:

####i) Insert Request:
This request from any client will enter new **event ordering** information to the server using the **happened–before** relationship. In this relationship, events will be represented with capital letters (i.e., A, B, C, D… etc), and the happened before relationship will be represented with **“->”** *(**dash** and **right angle bracket**)*. The event orderings will be separated with “white space” and will end with a semicolon. Example for an insert request:

	dec_client1$ insert A->B B->C C->D;
	dec_client1$ response from server: INSERT DONE.
	dec_client1$ _

This event insertion request is sent to the server from dec_client1, and it includes four events (A,B, C, D). Event A happened before event B; event B happened before event C; and event C happened before event D. After receiving the insert request, the DEC server will insert this event ordering information to its **global event ordering graph**. If the insertion is successful, it will return to the client an **“INSERT DONE”** message.

	dec_server$ request received from timberlake.cse.buffalo.edu: insert A->B B->C C->D;
	dec_server$ INSERT DONE.

If the server detects any conflicting information during insert (i.e. it had B->A before and now it receives A->B, which is not possible), then the server will return a **“CONFLICT DETECTED. INSERT FAILED”** message. It will also show the conflicting events, i.e. **“A->B and B->A cannot be true at the same time!”**.


####ii) Query Request:
This request from any client will ask the relative ordering between any two events. The queried events in this request will be separated with “white space” and will end with a semicolon. Example of query request:
	
	dec_client1$ query A D;
	dec_client1$ response from server: A happened before D.
	dec_client1$ _


There can be multiple requests sent on the same line:

	dec_client1$ insert E->F; query A E;
	dec_client1$ response from server: A concurrent to E.
	dec_client1$ _

If there is no information available about any particular event at the server, it should return an error message:

	dec_client1$ query A G;
	dec_client1$ response from server: Event not found: G.
	dec_client1$ _


####iii) Reset Request:
This request from any client will reset the global event ordering graph at the server. The reset request will not take any arguments and will be followed by a semicolon. Example of reset request:
	
	dec_client1$ reset;
	dec_client1$ response from server: RESET DONE.
	dec_client1$ _

### DEC_CLIENT:
> **dec_server [−h] [-s server-host] [-p port-number]**

*	**−h** : Print a usage summary with all options and exit.
*	**−s** : *server-host* : Connect to the specified host (by hostname or by IP address). If not provided, connect to the *localhost*.
*	**−p** : *port-number* : Connect to the server at the given port. If not provided, connect to 9090.

### DESCRIPTION:
*dec_client* is the client component of the Distributed Event Coordination (DEC) System. It will connect to the DEC server running on particular host and port number and will send the server one of the three requests mentioned above.