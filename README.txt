Anne Harris
CS 372
Program 1

README.txt

1. On first flip window, open files in flip3.engr.oregonstate.edu

2. Give files executable permissions with this line:
chmod +x chatserve

3. On flip3.engr.oregonstate.edu, start chatserve with this line:
./chatserve <PORTNUM>

<PORTNUM> is the port number of your choosing, I used 30111 when debugging. 

4. On second flip window, open files in flip2.engr.oregonstate.edu

5. Compile the c files with the line:
make chatclient

6. On flip2.engr.oregonstate.edu, start chatclient with this line:
./chatclient flip3.engr.oregonstate.edu <PORTNUM>

where <PORTNUM> is the SAME port number you used to start chatserve

7. The client will enter in their handle

8. The client will send the first chat, the server will send the second, it must go back and forth

9. If either the client or server type '\quit' the client will close and the server will remain open
	- If this, start from step 6 to reconnect new client

10. If the serve gets the Ctrl+C command it ends both the client and the server