# Mandatory Part

## 1. Check code and Questions

### 1.1. How it Works
Um servidor http nada mais é que leitor de entrada e saida que usa file descriptors para ler e escrever em um socket. O servidor fica em loop infinito esperando por uma requisição de um cliente, quando recebe a requisição ele faz o parse da requisição e envia a resposta para o cliente. 
Explique os conceitos básicos de um servidor HTTP.

### 1.2. Which Multiplexing I/O
A função que faz essa tipo de multiplexação escolhida foi a `poll`, que é uma versão mais moderna do que faz a função select.

### 1.3. How the poll is working
É utilizado apenas um poll vector para guardar todas as informações dos servidores e clientes. Ele é declarado na classe [Service](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/include/Service.hpp#L42)

```c++
class Service
{
	private:
		pollfdVector	_pollingRequests;
```

E a função `poll()` é chamada a cada passagem no loop principal da função [Service::launch](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L67) recebendo o mesmo vector de polls `_pollingRequests`, para então fazer a verificação de todos os sockets. Ela é chamada todas as vezes dentro do while, pois a cada verificação dos requests, clientes podem ser adicionados os removidos. A função poll é chamada na função [_initPollingRequests](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L89)

```c++
void Service::_initPollingRequests()
{
	if (poll(this->_pollingRequests.data(), this->_pollingRequests.size(), POLL_TIME_OUT) < 0 && g_shutdown == false)
		throw std::runtime_error(ERR_POLL_FAIL + std::string(std::strerror(errno)));
}
```

Os primeiros sockets do poll são preenchidos com a informação dos servidores na função [Service::Setup](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L42), onde ao final de cada servidor, é chamada a função [_addSocketInPollingRequests](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L314) que adiciona o socket do servidor no poll.

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

Já para aceitar os clientes, após o setup de todos os servidores, dentro da função [Service::launch](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L67), quando o cliente tem um Polling Request de [envio de dados](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L124), é chamada a função [_acceptClient](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L148) que aceita o cliente e adiciona o socket do cliente no poll.

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
A cada iteração do loop na função [Service::launch](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L67), é chamada a função [_pollingManager](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L95) que verifica o tipo de polling request.

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
Nas funções internas de [_pollingManager](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L95), é feito o tratamento de erros de cada tipo de polling request e então chamada a função [_closeConnection](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L172) que fecha a conexão e em seguida deleta aquele cliente.

```c++
void Service::_closeConnection(std::string const &msg)
{
	close(this->_tmp.socket);
	this->_pollingRequests.erase(this->_pollingRequests.begin() + this->_tmp.id);
	this->_clients.erase(this->_clients.begin() + this->_tmp.clientID);
	printInfo(msg, RED);
}
```
A função `_closeConnection` é chamada nas funções [_readData](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L161), [_hasBadRequest](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L180) and [_hasDataToSend](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L205). Escificamente na função [_readData](https://github.com/waltergcc/42-webserv/blob/0201fbccef7be28ef80c92b7ebda00fe832c464f/webserv/src/Service.cpp#L161), o erro é lançado quando a função `recv` retorna um valor menor que 1. Ou seja, 0 e -1 são tratados como um fator para fechar a conexão.

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
Para definir mais hosts além do localhost, é necessário adicionar o ip do host no arquivo `/etc/hosts` do sistema operacional usando o sudo. **Até o momento não sei o que fazer quanto a isso.**

### 2.2. Simple server response test
Para verificar a resposta do servidor, use o seguinte comando:

```bash
curl --resolve localhost:8080:127.0.0.1 localhost:8080
```

### 2.3. Simple Check max body size test
Para verificar o tamanho do corpo do cliente, use o seguinte comando:

Set the .conf `client_max_body_size` to 9 and location `/` to allow_methods `POST` and `GET`:

```bash
curl -X POST -H "Content-Type: plain/text" --data "1234567890" localhost:8080/
```
Nesse caso, o servidor deve retornar o erro 413 Request Entity Too Large, mas se remove o `0` do final do corpo, o body terá 9 bytes, então não dará o erro 413.

<!-- - Configure rotas em um servidor para diretórios diferentes

Verificações Básicas
Usando telnet, curl, arquivos preparados, demonstre que as seguintes funcionalidades funcionam corretamente:
- Solicitações GET -> devem funcionar
- Solicitações POST -> devem funcionar
- Solicitações DELETE -> devem funcionar
- Solicitações DESCONHECIDAS -> não devem produzir nenhum travamento -->
