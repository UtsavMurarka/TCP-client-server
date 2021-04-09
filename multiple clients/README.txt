STEPS TO RUN:
	1. Compile server.c by running : gcc -o server server.c
	2. Compile client.c by running : gcc -o client client.c
	3. Open two separate terminals, one from the Server directory, and other from the Client directory
	4. Run ./server <port no.> in the server terminal
	5. Enter valid port number as command line argument.
	6. Run ./client <Server IP> <Server Port> in the client terminal
	7. Do step 6. on multiple terminals to try multiple simultaneous connections.
	8. Enter correct IP (127.0.0.1 - localhost) and port number
	9. Enter string on client
	10. reversed string will be printed on server, and server will ask for string input
	11. Type string on server and press enter.
	12. Client will print string in reverse order and wait for new string.
	13. type "exit" in client to exit.

Notes:
	- After every new connections or upon closing a connection, server prints the active connections.
	- Clients after 4 active connectios will not be able to establish connections.
	- In case multiple clients have sent their strings and are waiting for response, server will respond in the order in which the clients sent the messages.
