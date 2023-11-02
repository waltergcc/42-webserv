# Mandatory Part

## 1. Check code and Questions

### 1.1. How it Works
An http server is nothing more than input and output reader that uses file descriptors to read and write on a socket. The server is in an infinite loop waiting for a Client's request, when it receives the request it makes the request parse and sends the response to the Client.

### 1.2. Which Multiplexing I/O
The function that makes this type of multiplexing chosen was `poll`, which is a more modern version of what the `select` function does.

### 1.3. How the poll is working
Only one vector poll is used to store all information from servers and clients. It is declared in the class [Service](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/include/Service.hpp#L42)

```c++
class Service
{
	private:
		pollfdVector	_pollingRequests;
```

And the function `poll()` is called each on time in the main loop of the function [Service::launch](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L67) receiving the same vector of Polls `_pollingRquests`, then check all sockets. It's called every time on while because Clients can be added or removed on each time. The poll function is called in the function [_initPollingRequests](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L89)

```c++
void Service::_initPollingRequests()
{
	if (poll(this->_pollingRequests.data(), this->_pollingRequests.size(), POLL_TIME_OUT) < 0 && g_shutdown == false)
		throw std::runtime_error(ERR_POLL_FAIL + std::string(std::strerror(errno)));
}
```

The first sockets of poll are filled with the information of the servers in the function [Service::Setup](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L42), where at the end of each server, the function is called[_addSocketInPollingRequests](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L314) which adds the server socket to poll.

```c++
void Service::_addSocketInPollingRequests()
{
	pollfd request;

	if (this->_tmp.launch == true)
	{
		request.fd = this->_tmp.connectionSocket;
		request.events = POLLIN | POLLOUT;
	}
	else
	{
		request.fd = this->_tmp.socket;
		request.events = POLLIN;
	}
	request.revents = 0;
	this->_pollingRequests.push_back(request);
}
```

To accept Clients, after the setup of all servers, within the function [Service::launch](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L67), When the Client has a request of [read data](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L124), The function is called [_acceptClient](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L148) that accepts the client connection and adds the Client's socket in Poll.

```c++
void Service::_acceptConnection()
{
	this->_tmp.connectionSocket = accept(this->_tmp.socket, NULL, NULL);

	if (this->_tmp.connectionSocket < 0)
		throw std::runtime_error(ERR_ACCEPT_SOCKET);
	
	fcntl(this->_tmp.connectionSocket, F_SETFL, O_NONBLOCK);	// set socket to non-blocking
	this->_clients.push_back(Client(this->_servers.at(this->_tmp.id), this->_tmp.connectionSocket));

	this->_addSocketInPollingRequests();
}
```

### 1.4. How is checked the Client read and write
On each iteration of the loop in the function [Service::launch](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L67), é chamada a função [_pollingManager](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L95) that checks the type of polling request.

```c++
void Service::_pollingManager()
{
	for (size_t i = 0; i < this->_pollingRequests.size(); i++)
	{
		this->_getLaunchInfo(i);

		if (this->_hasDataToRead())
			continue;
		if (this->_hasBadRequest())
			continue;
		if (this->_isServerRequest())
			continue;
		if (this->_hasDataToSend())
			continue;

		this->_resetInfo();
	}
}
```

### 1.5. Polling Requests Error Handling
In the internal functions of [_pollingManager](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L95), is made the error treatment of each type of polling request and then called the function [_closeConnection](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L172) that closes the connection and then deletes that client.

```c++
void Service::_closeConnection(std::string const &msg)
{
	close(this->_tmp.socket);
	this->_pollingRequests.erase(this->_pollingRequests.begin() + this->_tmp.id);
	this->_clients.erase(this->_clients.begin() + this->_tmp.clientID);
	printInfo(msg, RED);
}
```
The function `_closeConnection` is called in functions [_readData](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L161), [_hasBadRequest](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L180) and [_hasDataToSend](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L205). Specifically in the function [_readData](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L161), The error is throw when the `recv` function returns a value less than 1. that 0 and -1 are treated as a factor to close the connection.

```c++
void Service::_readData()
{
	char	buffer[BUFFER_SIZE] = {0};
	int		bytes = recv(this->_tmp.socket, buffer, BUFFER_SIZE, 0);

	if (bytes > 0)
		this->_clients.at(this->_tmp.clientID).appendRequest(buffer, bytes);
	else
		this->_closeConnection(CLOSE_MSG);
}
```
## 2. Check the Config File
[List of HTTP status codes](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes)

### 2.1. Add diferent hosts
To define more hosts besides localhost, you need to add the host IP to the `/etc/hosts' file from the operating system using sudo. **So far I don't know what to do about it.**

### 2.2. Simple server response test
To check the server response, use the following command:

```bash
curl --resolve localhost:8080:127.0.0.1 localhost:8080
```

### 2.3. Simple Check max body size test
To check if the client's body size limiter is working , use the following command:

Set the .conf `client_max_body_size` to 9 and location`/`to allow_methods` post` and `get`:

```bash
curl -X POST -H "Content-Type: plain/text" --data "1234567890" localhost:8080/curl_post.py
```
In this case, the server must return error `413 Payload is too large`, but if it removes the `0` from the body end, the body will have 9 bytes, then it will not give error 413.

## 3. Basic Checks

### 3.1. Check GET
To check the GET request, use the following command:

```bash
curl localhost:8080/index.html
```

### 3.2. Check POST
To check the POST request, use the following command:

```bash
curl -X POST -H "Content-Type: plain/text" --data "1234567890" localhost:8080/curl_post.py
```
This command will upload a file called `upload.txt` to the server with the content `1234567890`.

### 3.3. Check DELETE
To check the DELETE request, use the following command:

```bash
curl -X DELETE localhost:8080/upload/upload.txt

```
This command will delete the file `upload.txt` from the server.

### 3.4. Check UNKNOWN
To check the UNKNOWN request, use the following command:

```bash
curl -X UNKNOWN localhost:8080
```
This command will return the error `501 Not Implemented`.

### 3.5. Download the uploaded file
To download the uploaded file, After using the command to upload the file, use the following command:

```bash
curl -O localhost:8080/upload/upload.txt
```
This command will download the file `upload.txt` from the server to the current directory.

## 4. Check with Browser

### 4.1. Check Network
To check the network with `firefox`, flow the steps:

1. Open the `firefox` browser
2. Click on the page with the right mouse button and select `inspect element`
3. Click on the `network` tab
4. On the navigation bar, type the URL `localhost:8080`
5. On the `network` tab, click on the request to see more details

### 4.2. See a directory content
To see a directory content, you need to type the URL with the directory name at the end, for example:

```bash
localhost:8080/upload/
```
If the autoindex is enabled, the server will return the directory content, else, the server will return the error `403 Forbidden`.

### 4.3. Try to redirect URL
To try to redirect URL, you must setup the ./conf file location to redirect to another URL, for example:

```conf
location /redirect {
	allow_methods GET;
	return /index.html;
}
```
In this case, the server will redirect the URL `localhost:8080/redirect` to `localhost:8080/index.html`.

### 5. Check Ports

### 5.1. Using diferent ports and hosts
To check the server with diferent ports and sites, you need to setup the ./conf file with diferent ports and sites, for example:

```conf
server {
	server_name main_site;
	listen 8080;
	host localhost;
	root /home/wcorrea-/workplace/common_core/webserv/webserv/websites/main_site;
	index index.html;
	client_max_body_size 1M;
}

server {
	server_name blue;
	listen 8081;
	host blue.42.fr;
	root /home/wcorrea-/workplace/common_core/webserv/webserv/websites/blue;
	index index.html;
	client_max_body_size 1M;
}
```
In this case, the servers will be listening on ports 8080 and 8081, and the sites will be `main_site` and `blue` respectively.

### 5.2. Using the same port and  hosts
To check the server with the same port, you need to setup the ./conf file with the same port, for example:

```conf
server {
	server_name main_site;
	listen 8080;
	host localhost;
	root /home/wcorrea-/workplace/common_core/webserv/webserv/websites/main_site;
	index index.html;
	client_max_body_size 1M;
}

server {
	server_name blue;
	listen 8080;
	host localhost;
	root /home/wcorrea-/workplace/common_core/webserv/webserv/websites/blue;
	index index.html;
	client_max_body_size 1M;
}
```
In this case, the only server that will be listening on port 8080 will be the `main_site` server, because the `blue` server will not be able to listen on the same port.

### 5.3. Using the same port and different hosts
To check the server with the same port and different hosts, you need to setup the ./conf file with the same port and different hosts, for example:

```conf
server {
	server_name main_site;
	listen 8080;
	host localhost;
	root /home/wcorrea-/workplace/common_core/webserv/webserv/websites/main_site;
	index index.html;
	client_max_body_size 1M;
}

server {
	server_name blue;
	listen 8080;
	host blue.42.fr;
	root /home/wcorrea-/workplace/common_core/webserv/webserv/websites/blue;
	index index.html;
	client_max_body_size 1M;
}
```
In this case, the servers will be listening on port 8080, and the sites will be `main_site` and `blue` respectively.

### 5.4. Disclaimer
The server isn't able to listen on the same port and host simultaneously because the bind work only with one socket per port and host. When the server is listening on the same port and different hosts it's setup because the hosts are different. If it'll find a host that is already listening on the same port and host, this server will be ignored.

If the request received has a server_name and a host that is on the .conf but isn't the default, the server will replaced by this server temporarily on the function [_checkRequestedServer](https://github.com/waltergcc/42-webserv/blob/b8e300043c589f7439f70d5bb0e1eac04b0308d5/webserv/src/Service.cpp#L226)

```c++
void Service::_checkRequestedServer()
{
	std::string	request = this->_clients.at(this->_tmp.clientID).getRequest();
	std::string	requestedServer;
	size_t 		pos;

	if ((pos = request.find(REQUEST_HOST)))
	{
		requestedServer = request.substr(pos + std::strlen(REQUEST_HOST));
		if ((pos = requestedServer.find(NEWLINE)))
			requestedServer = requestedServer.substr(0, pos);
	}
	else
		return;

	Server	defaultServer = this->_clients.at(this->_tmp.clientID).getServer();
	serverVector::iterator server = this->_servers.begin();

	for (; server != this->_servers.end(); server++)
	{
		if (requestedServer == server->getServerName() && server->getHost() == defaultServer.getHost())
			this->_clients.at(this->_tmp.clientID).changeServer(*server);
	}
}
```
<!-- 

Teste de cerco e teste de estresse

- Use o Siege para executar alguns testes de estresse.
- A disponibilidade deve estar acima de 99,5% para uma solicitação simples em uma página vazia com "siege -b" nessa página. Verifique se não há vazamento de memória (monitore o uso de memória do processo, que não deve aumentar indefinidamente).
- Verifique se não há conexões penduradas.
- Você deve ser capaz de usar o Siege indefinidamente sem reiniciar o servidor (verifique "siege -b").

-->
