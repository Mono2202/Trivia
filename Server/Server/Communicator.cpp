#include "Communicator.h"
Communicator* Communicator::m_communicatorInstance = nullptr;

// C'tors:

Communicator::Communicator(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
{
	// Creating the socket:
	m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// Condition: error while creating the socket
	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception("Could not create server socket\n");
}

Communicator* Communicator::getInstance(RequestHandlerFactory& handlerFactory)
{
	if (m_communicatorInstance == nullptr) {
		m_communicatorInstance = new Communicator(handlerFactory);
	}

	return m_communicatorInstance;
}

// D'tors:

Communicator::~Communicator()
{
	// Trying to close the socket:
	try {
		::closesocket(m_serverSocket);
	}

	// Catching exceptions:
	catch (...) {   }

	for (auto& client : m_clients)
	{
		try {
			delete client.second;
		} catch (...) {	}
	}

	delete m_communicatorInstance;
}


// Methods:

/*
Starting to handle client requests
Input : < None >
Output: < None >
*/
void Communicator::startHandleRequests()
{
	// Binding and listening to the client socket:
	bindAndListen();

	while (true)
	{
		// Creating a new client socket:
		SOCKET newClientSock = accept(m_serverSocket, NULL, NULL);

		// Condition: new socket creation failed
		if (newClientSock == INVALID_SOCKET) {
			throw std::exception("Can't connect to new client socket");
		}

		// Inserting the new client to the client map:
		m_clients.insert(std::pair<SOCKET, IRequestHandler*>(newClientSock, m_handlerFactory.createLoginRequestHandler()));
		
		// Creating the client thread:
		thread newClientThread(&Communicator::handleNewClient, this, newClientSock);

		// Detaching the client thread:
		newClientThread.detach();
	}
}


// Private Methods:

/*
Binding and listening to the client socket
Input : < None >
Output: < None >
*/
void Communicator::bindAndListen()
{
	// Inits:
	struct sockaddr_in sa = { 0 };

	// Socket address inits:
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;

	// Binding the socket:
	if (::bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
		throw std::exception("Failed in server socket binding\n");
	}

	// Listening to the socket:
	if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		throw std::exception("Failed in initiating server socket listening\n");
	}
}

/*
Handling the new client
Input : sock - the client socket
Output: < None >
*/
void Communicator::handleNewClient(SOCKET sock)
{
	// Setting random seed:
	srand(time(0));

	try
	{
		while (true)
		{
			// Inits:
			Buffer buffer(SIZE);
			RequestInfo rqInfo;
			RequestResult rqResult;
			int bufBytes = 0;
			int sizeMsg = 0;

			// Receiving status code into buffer
			bufBytes = recv(sock, (char*)&buffer[0], 1, 0);
			
			if (bufBytes <= 0) {
				throw std::exception("Could not receive message from client\n");
			}

			// Receiving 4 bytes into int(for size of message)
			bufBytes = recv(sock, (char*)&sizeMsg, 4, 0);

			buffer.resize(SIZE + 1);

			if (sizeMsg != 0)
			{
				// Receiving message from client:
				bufBytes = recv(sock, (char*)&buffer[1], SIZE, 0);

				// Condition: error while receiving message from client
				if (!bufBytes) {
					throw std::exception("Could not receive message from client\n");
				}
			}

			// Creating the RequestInfo struct:
			rqInfo.buffer = Buffer(buffer.begin(), buffer.end());
			rqInfo.id = buffer[0];
			rqInfo.receivalTime = time(NULL);
			std::cout << "ID:" << rqInfo.id << "\n";

			// Condition: relevant request
			if (m_clients.at(sock)->isRequestRelevant(rqInfo)) {
				// Building the RequestResult struct:
				try {
					rqResult = m_clients.at(sock)->handleRequest(rqInfo);
				}
				// Catching Request Error:
				catch (const std::exception& e)
				{
					// Inits:
					RequestResult errResult;
					ErrorResponse response = { e.what() };

					// Returning the result:
					errResult.buffer = JsonResponsePacketSerializer::serializeResponse(response);
					errResult.newHandler = nullptr;
					rqResult = errResult;
				}
			}

			// Condition: irrelevant request
			else {
				// Creating the error response:
				ErrorResponse response = { "ERROR: Irrelevant request" };
				rqResult.buffer = JsonResponsePacketSerializer::serializeResponse(response);
				rqResult.newHandler = nullptr;
			}

			// Condition: updating the handler
			if (rqResult.newHandler != nullptr) {
				delete m_clients.find(sock)->second;
				m_clients.find(sock)->second = rqResult.newHandler;
			}

			// Sending the a message to the client:
			if (!send(sock, (char*)&rqResult.buffer[0], rqResult.buffer.size(), 0)) {
				throw std::exception("Could not send message back to client\n");
			}
		}
	}

	// Catching exceptions:
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		
		// Inits:
		RequestResult requestResult;

		// Condition: Close Room
		if (dynamic_cast<RoomAdminRequestHandler*>(m_clients.find(sock)->second))
		{
			// Inits:
			RequestInfo rqInfo;
			string length = "0000";
			int i = 0;

			// Creating the RequestInfo struct:
			rqInfo.buffer = Buffer();
			rqInfo.buffer.push_back(CLOSE_ROOM_REQUEST);
			for (i = 0; i < length.length(); i++) {
				rqInfo.buffer.push_back(length[i]);
			}
			rqInfo.id = CLOSE_ROOM_REQUEST;
			rqInfo.receivalTime = time(NULL);

			// Closing the room:
			requestResult = m_clients.find(sock)->second->handleRequest(rqInfo);

			delete m_clients.find(sock)->second;
			m_clients.find(sock)->second = requestResult.newHandler;
			
		}

		// Condition: Leave Room
		if (dynamic_cast<RoomMemberRequestHandler*>(m_clients.find(sock)->second))
		{
			// Inits:
			RequestInfo rqInfo;
			string length = "0000";
			int i = 0;

			// Creating the RequestInfo struct:
			rqInfo.buffer = Buffer();
			rqInfo.buffer.push_back(LEAVE_ROOM_REQUEST);
			for (i = 0; i < length.length(); i++) {
				rqInfo.buffer.push_back(length[i]);
			}
			rqInfo.id = LEAVE_ROOM_REQUEST;
			rqInfo.receivalTime = time(NULL);

			// Leaving the room:
			requestResult = m_clients.find(sock)->second->handleRequest(rqInfo);

			delete m_clients.find(sock)->second;
			m_clients.find(sock)->second = requestResult.newHandler;
		}

		// Condition: In Game
		if (dynamic_cast<GameRequestHandler*>(m_clients.find(sock)->second))
		{
			// Inits:
			RequestInfo rqInfo;
			string length = "0000";
			int i = 0;

			// Creating the RequestInfo struct:
			rqInfo.buffer = Buffer();
			rqInfo.buffer.push_back(LEAVE_GAME_REQUEST);
			for (i = 0; i < length.length(); i++) {
				rqInfo.buffer.push_back(length[i]);
			}
			rqInfo.id = LEAVE_GAME_REQUEST;
			rqInfo.receivalTime = time(NULL);

			// Leaving the game:
			requestResult = m_clients.find(sock)->second->handleRequest(rqInfo);

			delete m_clients.find(sock)->second;
			m_clients.find(sock)->second = requestResult.newHandler;
		}

		// Condition: Logging-out
		if (dynamic_cast<MenuRequestHandler*>(m_clients.find(sock)->second))
		{
			// Inits:
			RequestInfo rqInfo;
			string length = "0000";
			int i = 0;

			// Creating the RequestInfo struct:
			rqInfo.buffer = Buffer();
			rqInfo.buffer.push_back(LOGOUT_REQUEST);
			for (i = 0; i < length.length(); i++) {
				rqInfo.buffer.push_back(length[i]);
			}
			rqInfo.id = LOGOUT_REQUEST;
			rqInfo.receivalTime = time(NULL);

			// Logging-out:
			m_clients.find(sock)->second->handleRequest(rqInfo);
			delete m_clients.find(sock)->second;
			m_clients.find(sock)->second = nullptr;
		}

		if (m_clients.find(sock)->second != nullptr)
			delete m_clients.find(sock)->second;
	}

	m_clients.erase(sock);
	::closesocket(sock);
}
