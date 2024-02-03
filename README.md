# webserv

WebServ is a simple HTTP server written in C++ that adheres to HTTP/1.1 standards. It is designed to be non-blocking, supporting multiple clients simultaneously using efficient I/O multiplexing. The server configuration is provided through a configuration file, allowing users to customize various aspects of the server's behavior.

This project was developed in colaboration with [jscaetano ](https://github.com/jscaetano).

## Features
- **Host static websites**: Host static websites with HTML, CSS, and JavaScript files.
- **Host multiple websites**: Host multiple websites simultaneously, each with its own configuration.
- **Non-blocking I/O**: Uses efficient I/O multiplexing to handle multiple clients simultaneously without blocking.
- **Customizable configuration**: The server configuration is provided through a configuration file, allowing users to customize various aspects of the server's behavior.
- **HTTP/1.1 compliant**: Adheres to the HTTP/1.1 standard, supporting common HTTP methods such as GET, POST, and DELETE.
- **Error handling**: Returns appropriate error codes and messages for various client requests.
- **File uploads**: TSupports file uploads using the POST method.
- **Directory listing**: The server can automatically generate a directory listing for a requested directory.
- **Redirects**: The server can redirect clients to a different URL based on the request.
- **Error pages**: The server can return custom error pages for various error codes.
- **CGI scripts**: Supports CGI scripts for dynamic content generation.

### Server Block Keywords
| Keyword | Description | Example | Must have |
| --- | --- | --- | --- |
| **server** | Server block. | *server { ... }* | Yes |
| **listen** | Port number to listen on. | *8080* | Yes |
| **host** | Host name. | *localhost* | Yes |
| **root** | Root directory for the server. | */path/to/root/on/your/machine* | Yes |
| **index** | Default index file. | *index.html* | Yes |
| **client_max_body_size** | Maximum size of the client request body. | *1M* | Yes |
| **server_name** | Server name. | *my_server* | Optional |
| **error_page** | Error page to return for 404 Not Found errors. | *404.html* | Optional |

### Location Block Keywords
| Keyword | Description | Example | Must have |
| --- | --- | --- | --- |
| **location** | URL path to match. | */upload { ... }* | Yes |
| **allow_methods** | Allowed HTTP methods. | *GET POST DELETE* | Optional |
| **try_file** | File to try when a directory is requested. | *file.html* | Optional |
| **return** | URL to redirect to. | */index.html* | Optional |
| **autoindex** | Enable directory listing. | *on* | Optional |
| **root** | Root directory for the location. | */path/to/root/on/your/machine* | Optional |
| **upload_to** | Directory to upload files to. | */path/to/upload/to* | Optional |
| **cgi_path** | Path to the CGI scripts. | */path/to/cgi/scripts* | Optional |
| **cgi_extension** | File extension for CGI scripts. | *.py* | Optional |

## Usage

To compile the webserv program, use the provided Makefile by running `make` in the project directory. This will generate an executable named `webserv`.

```
make
```

To run the webserv program, simply execute the generated `webserv` binary. This will start the server using the default configuration file `./conf/default.conf`.

```
./webserv
```
If you want to use a different configuration file, you can provide the path to the configuration file as an argument to the program.

```
./webserv /path/to/your/config/file.conf
```

Once running, you can interact with the server sending requests using `curl` or a web browser. The server will respond to requests based on the configuration file provided.

### Grade: 110/100

## 1. Concepts and Tests

- [1.1. How it Works](#11-how-it-works)
- [1.2. Which Multiplexing I/O](#12-which-multiplexing-io)
- [1.3. How the poll is working](#13-how-the-poll-is-working)
- [1.4. How is checked the Client read and write](#14-how-is-checked-the-client-read-and-write)
- [1.5. Polling Requests Error Handling](#15-polling-requests-error-handling)

2. [**Check the Config File**](#2-check-the-config-file)
   - [2.1. Add different hosts](#21-add-different-hosts)
   - [2.2. Simple server response test](#22-simple-server-response-test)
   - [2.3. Setup an error page](#23-setup-an-error-page)
   - [2.4. Simple Check max body size test](#24-simple-check-max-body-size-test)
   - [2.5. Setup file when a directory is requested](#25-setup-file-when-a-directory-is-requested)

3. [**Basic Checks**](#3-basic-checks)
   - [3.1. Check GET](#31-check-get)
   - [3.2. Check POST](#32-check-post)
   - [3.3. Check DELETE](#33-check-delete)
   - [3.4. Check UNKNOWN](#34-check-unknown)
   - [3.5. Download the uploaded file](#35-download-the-uploaded-file)

4. [**Check with Browser**](#4-check-with-browser)
   - [4.1. Check Network](#41-check-network)
   - [4.2. See a directory content](#42-see-a-directory-content)
   - [4.3. Try to redirect URL](#43-try-to-redirect-url)

5. [**Check Ports**](#5-check-ports)
   - [5.1. Using different ports and hosts](#51-using-different-ports-and-hosts)
   - [5.2. Using the same port and hosts](#52-using-the-same-port-and-hosts)
   - [5.3. Using the same port and different hosts](#53-using-the-same-port-and-different-hosts)
   - [5.4. Disclaimer](#54-disclaimer)

6. [**Stress Tests**](#6-stress-tests)
   - [6.1. About Sudo on the School](#61-about-sudo-on-the-school)
   - [6.2. Recommendations to Solve the Problem](#62-recommendations-to-solve-the-problem)
   - [6.3. Siege test](#63-siege-test)
   - [6.4. Memory Leak Test](#64-memory-leak-test)
   - [6.5. Check Hanging Connections](#65-check-hanging-connections)

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
To define more hosts besides localhost, you need to add the host IP to the `/etc/hosts' file from the operating system using sudo.

### 2.2. Simple server response test
To check the server response, use the following command:

```bash
curl --resolve localhost:8080:127.0.0.1 localhost:8080
```

### 2.3. Setup a error page
To setup a error page, you need to setup the ./conf file with the `error_page` directive in the server block, for example:

```conf
server {
	server_name my_server;
	listen 8080;
	host localhost;
	root /home/wcorrea-/workplace/common_core/webserv/webserv/websites/main_site;
	index index.html;
	client_max_body_size 1M;
	error_page 404.html;
}
```
This page will be returned when the server returns the error `404 Not Found`. If a error page is not setup, the server will return a default error page.

### 2.4. Simple Check max body size test
To check if the client's body size limiter is working , use the following command:

Set the .conf `client_max_body_size` to 9 and location`/`to allow_methods` post` and `get`:

```bash
curl -X POST -H "Content-Type: plain/text" --data "1234567890" localhost:8080/curl_post.py
```
In this case, the server must return error `413 Payload is too large`, but if it removes the `0` from the body end, the body will have 9 bytes, then it will not give error 413.

### 2.5. Setup file when a directory is requested
To setup the file when a directory is requested, you need to setup the ./conf file with the `try_file` directive, for example:

```conf
location /upload {
	allow_methods GET POST;
	try_file file.html;
}
```
In this case, the server will try to find the file `file.html` in the directory `/upload` and return it if it exists, else, the server will return the error `404 Not Found`.

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

	if ((pos = requestedServer.find(":")))
		requestedServer = requestedServer.substr(0, pos);

	Server	defaultServer = this->_clients.at(this->_tmp.clientID).getServer();
	serverVector::iterator server = this->_servers.begin();

	for (; server != this->_servers.end(); server++)
	{
		if (requestedServer == server->getServerName() && server->getHost() == defaultServer.getHost())
			this->_clients.at(this->_tmp.clientID).changeServer(*server);
	}
}
```

To test this, use the `same_host_port.conf` file and the following command:

```bash
curl --resolve localhost:8080:127.0.0.1 localhost:8080
```
This command will return the default server page.

```bash
curl --resolve black:8080:127.0.0.1 black:8080
```
This command will return the non-default server page.

## 6. Stress Tests

### 6.1. About Sudo on the School
Because the school has a security policy, the students can't use the sudo command. It means that the students can't run the specific features:
- Run the server if in ports less than 1024
- Set more custom hosts in the `/etc/hosts` file
- Install the `siege` program for the stress test

### 6.2. Recommendations to Solve the Problem
1. Create a VM, or use a VM that already exists of your previus projects like `born2beroot` or `inception`.
2. Share your project folder with the VM using the `Virtual Box Shared Folder` feature.
3. Access your VM through the terminal with the `ssh` command. 

**Tip**: We recommend using the terminal called `terminitor` for this tests. With this, you can open multiple terminals at the same window.

**Terminator Shortcuts:**
- Open terminator: `Ctrl` + `Alt` + `T`
- Split terminal horizontally: `Ctrl` + `Shift` + `O`
- Split terminal vertically: `Ctrl` + `Shift` + `E`
- Close terminal: `Ctrl` + `Shift` + `W`
- Change terminal: `Ctrl` + `Shift` + `Tab`

And to set more hosts in the `/etc/hosts` file, we need to use sudo too, for example:

```bash
sudo nano /etc/hosts
```
### 6.3. Siege test

To check the server with the stress test, you need to install the `siege` program:

```bash
sudo apt-get install siege
```
Now you need to add a new host to the VM's `/etc/hosts` file, for example:

```bash
127.0.0.1 	testing
```
After this, create a file called `empty.html` in your server root directory and keep it empty, then add this configuration to the ./conf file:

```conf
server {
	server_name my_server;
	listen 8080;
	host testing;
	root /your/vm/shared/folder/with/your/website;
	index empty.html;
	client_max_body_size 1M;
}

Now, run the following command:

```bash
siege -b http://testing:8080/empty.html -t 1m
```
`-b` defines the test as a benchmark.
`-t` defines the time of the test. It can `s` for seconds, `m` for minutes or `h` for hours.

If the availability is above 99.5% for a simple request on an empty page, the server is working fine.

### 6.4. Memory Leak Test
To check the server with the memory leak test, you can use the `top`. The `top` program shows the memory usage of each process. the `webserv` program need to be running. Then, run the following command:

```bash
pgrep webserv
```
This command will return the process ID of the `siege` program, then run the following command:

```bash
top -p <process ID>
```
Now you can see the memory usage of your `webserv`. The column `RES` shows the phisisical memory usage of the process. If the memory usage increases indefinitely, the server has a memory leak.

For a better test you can check the memory usage of the `webserv` program while the `siege` program is running.

### 6.5. Check Hanging Connections

To check the server is with hanging connections, you can use the `netstat` program. While the siege test is running, run the following command:

```bash
watch -n 1 netstat -tuln
```
If the state of the connection is different from `ESTABLISHED` or `LISTEN`, the connection is hanging.