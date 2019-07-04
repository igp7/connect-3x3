#include "clientGame.h"

unsigned int readMove()
{
	int i, isValid, column;
	tString enteredMove;

	// Init...
	column = 0;

	// While player does not enter a correct move...
	do{

		// Init...
		bzero (enteredMove, STRING_LENGTH);
		isValid = TRUE;

		// Show input message
		printf ("Enter a move [0-%d]:", BOARD_WIDTH-1);

		// Read move
		fgets(enteredMove, STRING_LENGTH-1, stdin);

		// Remove new-line char
		enteredMove[strlen(enteredMove)-1] = 0;

		// Check if each character is a digit
		for (i=0; i<strlen(enteredMove) && isValid; i++){

			if (!isdigit(enteredMove[i]))
				isValid = FALSE;
		}

		// Entered move is not a number
		if (!isValid){
			printf ("Entered move is not correct. It must be a number between [0-%d]\n", BOARD_WIDTH-1);
		}

		// Entered move is a number...
		else{

			// Conver entered text to an int
			column = atoi (enteredMove);
		}

	}while (!isValid);

	return ((unsigned int) column);
}



int enviarMensajeCliente(int fdsocket, tString msg)
{
	int error = TRUE;
	int msgError;
	int size = strlen(msg);
	
	msgError = send(fdsocket, &size, sizeof(int), 0);
	// Comprobar si hay error en el envio
	if (msgError < 0){
		printf("Error: en el envio de un mensaje al server");
		error = FALSE;
	}
	
	msgError = send(fdsocket, msg, size, 0);
	// Comprobar si hay error en el envio
	if (msgError < 0){
		printf("Error: en el envio de un mensaje al server");
		error = FALSE;
	}
	
	return error;
}

int recibirMensajeCliente(int fdsocket, tString msg)
{
	int size;
	int error = TRUE;
	int msgError;
	
	msgError = recv(fdsocket, &size, sizeof(int), 0);
	if(msgError < 0){
		printf("Error: en la recepcio del size de un mensaje del server");
		error = FALSE;
	}
	
	memset(msg, 0, STRING_LENGTH);
	msgError = recv(fdsocket, msg, size, 0);
	if(msgError < 0){
		printf("Error: en la recepcio del contenido de un mensaje del server");
		error = FALSE;
	}
	
	strcat(msg, "\0");

	return error;
}

unsigned int recibirCodigoCliente(int fdsocket)
{
	unsigned int codigo;
	int msgError;

	msgError = recv(fdsocket, &codigo, sizeof(unsigned int), 0);
	if(msgError < 0){
		showError("Error: En la recepcion del codigo desde el servidor");
		codigo = FALSE;
	}

	return codigo;
}

int recibirBoardCliente(int fdsocket, tBoard board)
{
	int msgError;
	int error = TRUE;

	msgError = recv(fdsocket, board, sizeof(tBoard), 0);
	if(msgError < 0){
		printf("Error: En la recepcion del tablero desde el servidor \n");
		error = FALSE;
	}

	return error;
}

void recibirMensajeTurno(int fdsocket, tBoard board, unsigned int *codigo, tString mensaje)
{
	int error;
	tString msg;

	memset(msg, 0, STRING_LENGTH);
	error = recibirMensajeCliente(fdsocket, msg);

	if (error != TRUE)
		showError("ERROR msg de inicio partida en el mensaje");

	memset(board, 0, (BOARD_WIDTH * BOARD_HEIGHT));
	error = recibirBoardCliente(fdsocket, board);
	if (error != TRUE)
		showError("ERROR msg de inicio partida en el board");


	error = recibirCodigoCliente(fdsocket);
	if (error != TRUE)
		showError("ERROR msg de inicio partida en el codigo");

}

int enviarMoveCliente(int fdsocket, unsigned int move)
{
	int error = TRUE;
	int msgError;

	msgError = send(fdsocket, &move, sizeof(move), 0);
	// Comprobar si hay error en el envio
	if (msgError < 0){
		printf("Error: en el envio del move al server");
		error = FALSE;
	}

	return error;
}

int main(int argc, char *argv[])
{
	int socketfd;						/** Socket descriptor */
	unsigned int port;					/** Port number */
	struct sockaddr_in server_address;	/** Server address structure */
	char* serverIP;						/** Server IP */
	tString msg;						/** Message sent to the server side */
	int error;
	tBoard board;
	unsigned int codigo;
	unsigned int move;


	// Check arguments!
	if (argc != 3){
		fprintf(stderr,"ERROR wrong number of arguments\n");
		fprintf(stderr,"Usage:\n$>%s serverIP port\n", argv[0]);
		exit(0);
	}

	// Get the port
	port = atoi(argv[2]);

	// Create socket
	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check if the socket has been successfully created
	if (socketfd < 0)
		showError("ERROR while creating the socket");

	// Get the server address
	serverIP = argv[1];

	// Fill server address structure
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(serverIP);
	server_address.sin_port = htons(port);

	// Connect with server
	if (connect(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
		showError("ERROR while establishing connection");


	/** Recibir confirmacion de conexion */
	memset(msg, 0, STRING_LENGTH);
	error = recibirMensajeCliente(socketfd, msg);

	if (error != TRUE)
		showError("ERROR recepcion msg de conexion");
	else
		printf("%s\n",msg);


	/** Recibir ACK */
	memset(msg, 0, STRING_LENGTH);
	error = recibirMensajeCliente(socketfd, msg);

	if (error != TRUE)
		showError("ERROR msg de confirmacion");


	/** Mandar nombre del jugador */
	memset(msg, 0, STRING_LENGTH);
	printf("Enter player name: ");
	fgets(msg, STRING_LENGTH-1, stdin);
	msg[strlen(msg)-1] = 0;

	// Send message to the server side
	error = enviarMensajeCliente(socketfd, msg);

	// Check the number of bytes sent
	if (error != TRUE)
		showError("ERROR mandar nombre cliente");

	/** Recibir nombre de los jugadores a los que me enfrenta */
	memset(msg, 0, STRING_LENGTH);
	error = recibirMensajeCliente(socketfd, msg);

	if (error != TRUE)
		showError("ERROR msg de contrincantes");
	else
		printf("%s\n",msg);


	/** Desarrolllo del juego */

	/** Recibir comienzo de juego */
	memset(msg, 0, STRING_LENGTH);
	error = recibirMensajeCliente(socketfd, msg);

	if (error != TRUE)
		showError("ERROR msg de Game Starts");
	else
		printf("%s\n\n",msg);


	do
	{
		memset(msg, 0, STRING_LENGTH);
		error = recibirMensajeCliente(socketfd, msg);

		if (error != TRUE)
			showError("ERROR msg de inicio partida en el mensaje");

		memset(board, 0, (BOARD_WIDTH * BOARD_HEIGHT));
		error = recibirBoardCliente(socketfd, board);
		if (error != TRUE)
			showError("ERROR msg de inicio partida en el board");


		codigo = recibirCodigoCliente(socketfd);
		if (codigo == FALSE)
			showError("ERROR msg de inicio partida en el codigo");


		printBoard (board, msg);

		if(codigo == TURN_MOVE)
		{
			move = readMove();
			enviarMoveCliente(socketfd, move);
		}
	}while(codigo != GAMEOVER_WIN && codigo != GAMEOVER_DRAW && codigo != GAMEOVER_LOSE);

	printf("%s\n\n",msg);

	close(socketfd);

	return 0;
}
