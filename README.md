**./ircserv 'port' 'password'**
	*port* is the port number our server will be listening to for incoming connections.
	*password* is the connection password needed by the client to connect to the server.

Develop an IRC server in c++

# Technical terms

### Server :
Central part of an IRC system: Receives the messages from the clients and relay them to other clients.
### Client:
User connected to the IRC server. Each user has a unique identifier: the nickname
### Channel :
A virtual meeting place where multiple clients can communicate. A message sent by a client is visible from all the clients in this channel.
### Nickname :
Each client gets a nickname when connecting to a server. There are restrictions on a nickname creation:
- 9 chars max
- Non-empty string
- They MUST NOT contain any of the following characters: space `(' ', 0x20)`, comma `(',', 0x2C)`, asterisk `('*', 0x2A)`, question mark `('?', 0x3F)`, exclamation mark `('!', 0x21)`, at sign `('@', 0x40)`.
- They SHOULD NOT contain any dot character `('.', 0x2E)`.
- They MUST NOT start with any of the following characters: dollar `('$', 0x24)`, colon `(':', 0x3A)`.
- They MUST NOT start with a character listed as a [channel type] (https://modern.ircdocs.horse/#channel-types)`('#', 0x23)` `('&', 0x26)`, [channel membership prefix](https://modern.ircdocs.horse/#channel-membership-prefixes) `('~', 0x7E)` `('+', 0x2B)`, or prefix listed in the IRCv3 [`multi-prefix` Extension](https://ircv3.net/specs/extensions/multi-prefix).
- To sum up, you can use letters, digits, hyphens `('-', 0x2D)`, underscores `('_', 0x5F)`, square brackets `('[', 0x5B)` `(']', 0x5D)`, curly braces `('{', 0x7B)` `('}', 0x7D)`, backslash `('\', 0x5C` and pipe `('|', 0x7C)`
If the nickname already exists, you won't be able to enter.
### Commands :
Instructions sent by a client to the server to perform certain actions. (See below)
### Concurrency :
The ability of the server to handle multiple clients simultaneously.

### Port :
The port number on TCP/UDP protocols are bidirectionnal.
Ports from ***0 to 1023*** are **system ports**, they need superuser privileges to work, so we avoid them.
***1024 to 49151*** are **registered ports**, they are assigned to certain protocols/applications, but we can use them if they are not in use when launching our program. But there's a small chance it doesn't work.
***49152 to 65535*** are private ports, free to use for private/temporary services.
### Socket :
Structure in the network that allows communication between the server and a client 
(See more below)

# Socket programming
https://www.geeksforgeeks.org/socket-programming-cc/

A socket works like a file descriptor : I/O actions are done by writing and reading from a file descriptor. The usage is:

### Coding the socket
#### 1. Socket creation
```c++
int sockfd = socket(domain, type, protocol)
```
- _sockfd_: socket descriptor (similar to file descriptor)
- _domain:_ integer, specifies communication domain.
    - _AF_LOCAL_: communication between processes on the same host
    - _AF_INET_ - _AF_INET6_: communication between different hosts (IPV4 - 6)
- _type_: communication type
    - _SOCK_STREAM_: TCP
    - _SOCKER_DGRAM_: UDP
- _protocol_: protocol value for Internet Protocol (IP), which is 0
    - Same number which appears on protocol field in the IP header of a packer (man protocols)

#### 2. Setsockopt
This helps in manipulating options for the socket referred by the file descriptor sockfd. This is completely optional, but it helps in reuse of address and port. Prevents error such as: “address already in use”.

```c++
int setsockopt(int sockfd, int level, int optname,  const void *optval, socklen_t optlen);
```

#### 3. Bind
```c++
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

After the creation of the socket, the bind function binds the socket to the address and port number specified in addr(custom data structure). In the example code, we bind the server to the localhost, hence we use *INADDR_ANY* to specify the IP address.

#### 4. Listen
```c++
int listen(int sockfd, int backlog);
```

It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection. The backlog, defines the maximum length to which the queue of pending connections for sockfd may grow. If a connection request arrives when the queue is full, the client may receive an error with an indication of *ECONNREFUSED*.

#### 5. Accept
```c++
int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, the connection is established between client and server, and they are ready to transfer data.

### Functions 

#### send()
This function is used to transmit message to another socket.
```c
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```
The argument _sockfd_ is the file descriptor of the sending socket. On success, these calls return the number of characters sent. On error, -1 is returned, and errno is set appropriately.

#### recv()
This function is used to receive message from a socket.
```c
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

#### getaddrinfo()
This function is used to obtain a list of socket addresses that match the specified node and service.

#### freeaddrinfo()
Used to free the allocated memory by *getaddrinfo()*

# Commands

## Mandatory

### PASS
##### Parameter : "password"
First command to do before the USER/NICK connection.
### USER
##### Parameters: "username" "mode" "unused" "realname"
Second command to enter. 
***mode*** will be '0' and ***unused*** wil l be ' * '. These parameters may be different depending on the version of the IRC protocol, so we'll use these.
***realname*** can have spaces in it
##### Example
**USER guest 0 * :Ronnie Reagan** : Connect as guest

### NICK
##### Parameters : "nickname"
Should respect the rules above.

### KICK
##### *Parameters : "channel" "user" "[comment]"*
Eject a client from the channel (<u>chanop only</u>)
##### Examples
***KICK &Melbourne Matthew*** : kicks Matthew from Melbourne channel
***KICK #Finnish John :Speaking English*** : Kick John from #Finnish using "Speaking English" as the reason (comment).
### INVITE
##### *Parameters : "nickname" "channel"*
Invite a client to a channel. If the channel is on invitation only (<u>+i mode</u>) the client inviting must be <u>operator</u> of the channel.
##### Examples
***:Angel INVITE Wiz #Dust*** : User Angel inviting WiZ to channel #Dust
***INVITE Wiz #Twilight_Zone*** : Command to invite WiZ to #Twilight_zone

### JOIN
##### Parameters : "channel"  "[key]"
Used by client to listen to a channel. When the client joins the server, it must send to the client in this order:
- A msg with client as msg "source" and the channel name as 1st parameter of the msg
- The channel's topic (*RPL_TOPIC(332)*) / No msg if there is no topic.
- A list of users currently joined to this channel (with one or more *RPL_NAMREPLY(353)* numerics followed by a single *RPL_ENDOFNAMES(366)*  numeric).

### TOPIC
##### Parameters : "channel" "[topic]"
Change or view the channel topic: used without "topic" parameter, return the topic.
if there are both parameters, changes the channel topic to "topic". (<u>chanop only</u>)
##### Examples
***:Wiz TOPIC #test :New topic*** : User Wiz setting the topic.
***TOPIC #test :another topic*** : set the topic on #test to "another topic".
***TOPIC #test*** : check the topic for #test.
### MODE
==Only for channel modes in our project==
The <u>chanops</u> can change the channel modes
##### Parameters : "channel" "+/- o|i|t|l|k" "[limit]" "[user]" 
Change the channel's mode
	- **o** : Give/take channel operator privilege
	- **i** : Set/remove Invite-only channel
	- **t** : Set/remove the restrictions of the TOPIC command to channel operators
	- **l** : Set/remove the user limit to channel
	- **k** : Set/remove the channel key (password)
##### Examples
***MODE #Finnish +i*** : Makes channel #Finnish invite only.
***MODE #Finnish +o Kilroy*** : Gives 'chanop' privileges to Kilroy on #Finnish channel.
***MODE #eu-opers +l 10*** : Set the limit for the number of users on channel to 10.
### PRIVMSG
##### Parameters: "receiver" "text"
To send private messages between users.
There can be multiple receivers: "xx", "yy", "zz"
##### Examples
***:Angel PRIVMSG Wiz :Hello are you receiving this message ?*** : Msg from Angel to Wiz.
## Others

### QUIT
##### Parameters: "[Quit message]"
End the client session with a quit msg. The server must close the connection to the client. If we use a quit msg, it is sent instead of the default msg (nickname).
##### Example
***QUIT :Gone to have lunch*** : 
### LIST
##### Parameters: "[channel(s)]"
To list the channels and their topics. If the channel parameter is used, only this channel is displayed.
There can be multiple channels: "xx", "yy", "zz"
##### Examples
***LIST*** : List all channels.
***LIST  #Twilight_zone, #42*** : List channels #Twilight_Zone and #42.

# Design

### 3 sections : 
- Chat page in use
- Channels list
- Channel users 

### Pseudos
- **~ and/or purple dott:** Channel owner. Highest rank. Can choose admins.
- **& and/or red dott :** Admin. Can choose chanops.
- **@ and/or green dott :** Chanops / channel operators. Can ban users, even others chanops.
- **% and/or blue dott :** Semi-operators. Same role as chanops, but can't act on others chanops/semi-operators.
- **+ and/or yellow dott :** Regular users.

# Usefull links

https://www.rfc-editor.org/rfc/rfc1459 (official doc)
https://modern.ircdocs.horse/ (other doc)
https://www.geeksforgeeks.org/socket-programming-cc/ (socket)
https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/ (multi-clients server socket)
https://www.linuxtricks.fr/wiki/nc-netcat-la-boite-a-outils-reseau (nc doc)
http://chi.cs.uchicago.edu/chirc/index.html (IRC implementation project example)
https://github.com/marineks/Ft_irc (ft_irc implementation project example)
