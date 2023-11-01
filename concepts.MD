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

<!-- Verificações básicas

- Solicitações DELETE devem funcionar.
- Solicitações DESCONHECIDAS não devem causar nenhum travamento.
- Para cada teste, o código de status deve estar correto.
- Faça o upload de um arquivo para o servidor e recupere-o.

Verifique com um navegador

- Use o navegador de referência da equipe, abra a parte de rede dele e tente se conectar ao servidor.
- Observe o cabeçalho da solicitação e o cabeçalho da resposta.
- Deve ser compatível para servir um site totalmente estático.
- Tente um URL incorreto no servidor.
- Tente listar um diretório.
- Tente um URL redirecionado.
- Tente coisas.

Problemas de porta

- No arquivo de configuração, configure várias portas e use sites diferentes. Use o navegador para verificar se a configuração está funcionando conforme o esperado e exibindo o site correto.
- Na configuração, tente configurar a mesma porta várias vezes. Isso não deve funcionar.
- Inicie vários servidores ao mesmo tempo com configurações diferentes, mas com portas comuns. Isso está funcionando? Se estiver funcionando, pergunte por que o servidor deve funcionar se uma das configurações não estiver funcionando. Continue.

Teste de cerco e teste de estresse

- Use o Siege para executar alguns testes de estresse.
- A disponibilidade deve estar acima de 99,5% para uma solicitação simples em uma página vazia com "siege -b" nessa página. Verifique se não há vazamento de memória (monitore o uso de memória do processo, que não deve aumentar indefinidamente).
- Verifique se não há conexões penduradas.
- Você deve ser capaz de usar o Siege indefinidamente sem reiniciar o servidor (verifique "siege -b"). -->

<!-- English eval

Basic checks 

Using telnet, curl, prepared files demonstrates that the following features work properly:
- GET requests should work
- POST requests -› should work
- DELETE requests should work
- UNKNOWN requests -> should not produce any crash
- For every test the status code must be good
- upload some file to the server and get it bock 


Check with a browser

- Use the reference browser of the team, open the network part of it and try to connect to the server with it
- Look at the request header and response header
- It should be compatible to serve a fully static website
- Try o wrong URL on the server
- Try to list a directory
- Try a redirected URL
- Try things 


Port issues

- In the configuration file setup multiple ports and use different websites, use the browser to check that the configuration is working as expected, and show the right website.
- In the configuration try to setup the sense port multiple times. It should not work.
- Launch multiple servers at the scene time with different configurations but with common ports. Is it working? if it is working, ask why the server should work if one of the configurations isn't working. keep going 


Siege & stress test

- Use Siege to run some stress tests.
- Availability should be above 99.5% For a simple get on an empty poge with o siege -b on that page Check if there is no memory leak (monitor the process memory usage it should not go up indefinitely)
- Check if there is no hanging connection
- You should be able to use siege indefinitely without restarting the server (look at siege -b)  -->