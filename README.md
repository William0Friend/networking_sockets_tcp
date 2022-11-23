# networking_sockets_tcp
stream socket, both client and server

py folder is working test to make sure everyting is open and not being blocked at all, bascally a way to rule out connectivity issues when testing.

At moment allows message up to 100kb and verifies with a sha256 

In the future will be adding client ip to server out put, maybe allow users to choose size of the message
Also nee to fix make file, used to work for server and client seperatly, but I want the option to do both at once so I need to fix it.
Also need to stop typing all breakpoints into gdb. Need to make a gdb file.
