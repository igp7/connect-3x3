#include "serverGame.h"
#include <pthread.h>


int enviarMensajeServer(int fdsocket, tString msg)
{
	int error = TRUE;
	int msgError;
	int size = strlen(msg);
	
	msgError = send(fdsocket, &size, sizeof(int), 0);
	// Comprobar si hay error en el envio
	if (msgError < 0){
		printf("Error: en el envio de un mensaje al cliente");
		error = FALSE;
	}
	
	msgError = send(fdsocket, msg, size, 0);
	// Comprobar si hay error en el envio
	if (msgError < 0){
		printf("Error: en el envio de un mensaje al cliente");
		error = FALSE;
	}
	
	return error;
}

void recibirMensajeServer(int fdsocket, tString msg)
{
	int size;
	int msgError;
	
	memset(msg, 0, STRING_LENGTH);
	msgError = recv(fdsocket, &size, sizeof(int), 0);
	if(msgError < 0)
		printf("Error: en la recepcio del size de un mensaje del cliente");

	msgError = recv(fdsocket, msg, size, 0);
	if(msgError < 0)
		printf("Error: en la recepcio del contenido de un mensaje del cliente");
}

int enviarTableroServer(int fdsocket, tBoard tablero)
{
	int error = TRUE;
	int msgError;
	
	msgError = send(fdsocket, tablero, sizeof(tBoard), 0);
	// Comprobar si hay error en el envio
	if (msgError < 0){
		printf("Error: en el envio del tablero al cliente");
		error = FALSE;
	}
	
	return error;
}

int enviarCodigoServer(int fdsocket, unsigned int codigo)
{
	int error = TRUE;
	int msgError;
	
	msgError = send(fdsocket, &codigo, sizeof(codigo), 0);
	// Comprobar si hay error en el envio
	if (msgError < 0){
		printf("Error: en el envio del codigo al cliente");
		error = FALSE;
	}
	
	return error;
}

tPlayer selecionTurno(int *fdsocket, tPlayer turnoActual, char *chip, tThreadArgs players)
{
	tPlayer nextTurn;

	if(turnoActual == player1){
		nextTurn = player2;
		*fdsocket = players.socketPlayer2;
		*chip = PLAYER_2_CHIP;
	}
	else if(turnoActual == player2){
		nextTurn = player3;
		*fdsocket = players.socketPlayer3;
		*chip = PLAYER_3_CHIP;
	}
	else if(turnoActual == player3){
		nextTurn = player1;
		*fdsocket = players.socketPlayer1;
		*chip = PLAYER_1_CHIP;
	}

	return nextTurn;
}

void enviarMensajeEmpate(int fdsocket, tBoard board)
{
	int error;
	tString msg;

	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "The game is Draw!!");

	error = enviarMensajeServer(fdsocket, msg);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR enviarMensajeEmpate en MENSAJE");


	error = enviarTableroServer(fdsocket, board);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR enviarMensajeEmpate en BOARD");


	error = enviarCodigoServer(fdsocket, GAMEOVER_DRAW);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR enviarMensajeEmpate en CODIGO");
}

void mensajeGanador(tPlayer jugadorActual, tThreadArgs players, tBoard board)
{
	if(jugadorActual == player1)
	{
		enviarMensajeGanador(players.socketPlayer1, board);
		enviarMensajePerdedor(players.socketPlayer2, board);
		enviarMensajePerdedor(players.socketPlayer3, board);
	}
	else if(jugadorActual == player2)
	{
		enviarMensajePerdedor(players.socketPlayer1, board);
		enviarMensajeGanador(players.socketPlayer2, board);
		enviarMensajePerdedor(players.socketPlayer3, board);
	}
	else if(jugadorActual == player3)
	{
		enviarMensajePerdedor(players.socketPlayer1, board);
		enviarMensajePerdedor(players.socketPlayer2, board);
		enviarMensajeGanador(players.socketPlayer3, board);
	}
}

void enviarMensajeGanador(int fdsocket, tBoard board)
{
	int error;
	tString msg;
	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "Congratulations you win the game !!");

	error = enviarMensajeServer(fdsocket, msg);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensajeGanador en MENSAJE");


	error = enviarTableroServer(fdsocket, board);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensajeGanador en BOARD");


	error = enviarCodigoServer(fdsocket, GAMEOVER_WIN);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensajeGanador en CODIGO");
}

void enviarMensajePerdedor(int fdsocket, tBoard board)
{
	int error;
	tString msg;

	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "You lose the game !!");

	error = enviarMensajeServer(fdsocket, msg);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensajeGanador en MENSAJE");


	error = enviarTableroServer(fdsocket, board);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensajeGanador en BOARD");


	error = enviarCodigoServer(fdsocket, GAMEOVER_LOSE);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensajeGanador en CODIGO");
}

unsigned int recibirMove(int fdsocket)
{
	unsigned int move;
	int msgError;

	msgError = recv(fdsocket, &move, sizeof(unsigned int), 0);
	if(msgError < 0){
		showError("Error: En la recepcion del codigo desde el servidor");
	}

	return move;
}

void enviarMensajesTurno(tPlayer jugadorActual, tThreadArgs players, tBoard board)
{
	if(jugadorActual == player1)
	{
		enviarMensajeTurnoJugar(players.socketPlayer1, PLAYER_1_CHIP, board);
		enviarMensajeTurnoEspera(players.socketPlayer2, PLAYER_2_CHIP, board);
		enviarMensajeTurnoEspera(players.socketPlayer3, PLAYER_3_CHIP, board);
	}
	else if(jugadorActual == player2)
	{
		enviarMensajeTurnoEspera(players.socketPlayer1, PLAYER_1_CHIP, board);
		enviarMensajeTurnoJugar(players.socketPlayer2, PLAYER_2_CHIP, board);
		enviarMensajeTurnoEspera(players.socketPlayer3, PLAYER_3_CHIP, board);
	}
	else if(jugadorActual == player3)
	{
		enviarMensajeTurnoEspera(players.socketPlayer1, PLAYER_1_CHIP, board);
		enviarMensajeTurnoEspera(players.socketPlayer2, PLAYER_2_CHIP, board);
		enviarMensajeTurnoJugar(players.socketPlayer3, PLAYER_3_CHIP, board);
	}
}

void enviarMensajeTurnoJugar(int fdsocket, char playerChip, tBoard board)
{
	int error;
	tString msg;

	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "Its your turn. you play with:");
	strcat(msg, &playerChip); 
		
	error = enviarMensajeServer(fdsocket, msg);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensaje de turno jugar en MENSAJE");


	error = enviarTableroServer(fdsocket, board);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensaje de turno jugar en BOARD");


	error = enviarCodigoServer(fdsocket, TURN_MOVE);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensaje de turno jugar en CODIGO");
}

void enviarMensajeTurnoEspera(int fdsocket, char playerChip, tBoard board)
{
	int error;
	tString msg;

	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "Your rival is thinking... please, wait! You play with:");
	strcat(msg, &playerChip);

	error = enviarMensajeServer(fdsocket, msg);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensaje de turno esperar en MENSAJE");


	error = enviarTableroServer(fdsocket, board);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensaje de turno esperar en BOARD");


	error = enviarCodigoServer(fdsocket, TURN_WAIT);
	// Check bytes sent
	if (error != TRUE)
		showError("ERROR mensaje de turno esperar en CODIGO");
}

void mensajeACCEPTS(int fdsocket, int player)
{
	int error;
	tString msg;

	// Check  result
	if (fdsocket < 0)
	  showError("ERROR en mensajeACCEPTS accept");

	// Get the msg length
	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "Connection established with server!");

	error = enviarMensajeServer(fdsocket, msg);

	if(error == FALSE)
		showError("ERROR en el envio de conexion establecida");
	else
		printf("Player %d is connected!\n", player);
}

void mensajeACK(int fdsocket)
{
	int error;
	tString msg;

	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "ack");

	error = enviarMensajeServer(fdsocket, msg);

	// Check bytes sent
	if (error != TRUE)
		showError("ERROR en mensajeACK");
}

void mensajeRIVALES(int fdsocket, tString rival1, tString rival2)
{
	int error;
	tString msg;

	// Get the msg length
	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "You are playing against ");
	strcat (msg, rival1);
	strcat (msg, " and ");
	strcat (msg, rival2);
	error = enviarMensajeServer(fdsocket, msg);

	// Check bytes sent
	if (error != TRUE)
		showError("ERROR en mensajeRIVALES");
}

void mensajeSTART(int fdsocket)
{
	int error;
	tString msg;

	// Get the msg length
	memset(msg, 0, STRING_LENGTH);
	strcpy (msg, "Game starts!");

	error = enviarMensajeServer(fdsocket, msg);

	// Check bytes sent
	if (error != TRUE){
		showError("ERROR en mensajeSTART en el player");
	}
}

void actualizaScore(tPlayer jugadorActual, int ganador, int boardFull, tString player_1, tString player_2, tString player_3)
{
	FILE *fp;
	pthread_mutex_lock(&m);
	fp = fopen(SCORES_FILE_NAME, "a");

	if(ganador == TRUE){
		if(jugadorActual == player1){
			fprintf(fp, "%s->WINNER	", player_1);
			fprintf(fp, "%s->LOSER	", player_2);
			fprintf(fp, "%s->LOSER\n", player_3);
		}
		else if(jugadorActual == player2){
			fprintf(fp, "%s->WINNER	", player_2);
			fprintf(fp, "%s->LOSER	", player_1);
			fprintf(fp, "%s->LOSER\n", player_3);
		}
		else if(jugadorActual == player3){
			fprintf(fp, "%s->WINNER	", player_3);
			fprintf(fp, "%s->LOSER	", player_1);
			fprintf(fp, "%s->LOSER\n", player_2);
		}
	}

	if(boardFull == TRUE && ganador != TRUE){
		fprintf(fp, "%s->DRAW	", player_1);
		fprintf(fp, "%s->DRAW	", player_2);
		fprintf(fp, "%s->DRAW\n", player_3);
	}

	fclose(fp);
	pthread_mutex_unlock(&m);
}

void *juego(void *arg)
{
	int ganador = FALSE;
	int boardFull = FALSE;
	tPlayer jugadorActual;
	int fdsocketPlayerTurn;
	unsigned int columna;
	tMove move;
	char chipPlayerTurn;
	tBoard board;						/** Board of game */
	tString player_1;					/** Name of the player 1 */
	tString player_2;					/** Name of the player 2 */
	tString player_3;					/** Name of the player 3 */
	tThreadArgs *players;
	players = (tThreadArgs*)arg;


	pthread_detach(pthread_self());

	mensajeACK(players->socketPlayer1);
	recibirMensajeServer(players->socketPlayer1, player_1);
	printf("Name of Player 1 received: %s\n", player_1);

	mensajeACK(players->socketPlayer2);
	recibirMensajeServer(players->socketPlayer2, player_2);
	printf("Name of Player 2 received: %s\n", player_2);

	mensajeACK(players->socketPlayer3);
	recibirMensajeServer(players->socketPlayer3, player_3);
	printf("Name of Player 3 received: %s\n", player_3);


	mensajeRIVALES(players->socketPlayer1, player_2, player_3);
	mensajeRIVALES(players->socketPlayer2, player_1, player_3);
	mensajeRIVALES(players->socketPlayer3, player_1, player_2);

	// Mensaje de Comienza el juego
	mensajeSTART(players->socketPlayer1);
	mensajeSTART(players->socketPlayer2);
	mensajeSTART(players->socketPlayer3);

	memset(board, 0, (BOARD_WIDTH * BOARD_HEIGHT));
	initBoard (board);
	jugadorActual = player1;
	fdsocketPlayerTurn = players->socketPlayer1;

	memset(&chipPlayerTurn, 0, sizeof(char));
	chipPlayerTurn = PLAYER_1_CHIP;


	while(ganador != TRUE && boardFull != TRUE)
	{
		enviarMensajesTurno(jugadorActual, *players, board);
		columna = recibirMove(fdsocketPlayerTurn);
		move = insertChip (board, jugadorActual, columna);

		while(move != OK_move)
		{
			enviarMensajeTurnoJugar(fdsocketPlayerTurn, chipPlayerTurn, board);
			columna = recibirMove(fdsocketPlayerTurn);
			move = insertChip (board, jugadorActual, columna);
		}

		ganador = checkWinner (board, jugadorActual);
		boardFull = isBoardFull (board);
		if(ganador == TRUE){
			mensajeGanador(jugadorActual, *players, board);
		}
		else if(boardFull == TRUE){
			enviarMensajeEmpate(players->socketPlayer1, board);
			enviarMensajeEmpate(players->socketPlayer2, board);
			enviarMensajeEmpate(players->socketPlayer3, board);
		}
		else {
			jugadorActual = selecionTurno(&fdsocketPlayerTurn, jugadorActual, &chipPlayerTurn, *players);
		}
	}

	// Modificar fichero SCORES_FILE_NAME
	actualizaScore(jugadorActual, ganador, boardFull, player_1, player_2, player_3);

	// Cierra conexiones
	close(players->socketPlayer1);
	close(players->socketPlayer2);
	close(players->socketPlayer3);
	
	
	return (NULL);
}



int main(int argc, char *argv[])
{
	int socketfd;						/** Socket descriptor */
	unsigned int port;					/** Listening port */
	struct sockaddr_in serverAddress;	/** Server address structure */
	struct sockaddr_in clientAddress;	/** Client address structure */
	unsigned int clientLength;			/** Length of client structure */
	tThreadArgs *players;				/** Sockets of the players */
	pthread_t thread;


	// Check arguments
	if (argc != 2) {
		fprintf(stderr,"ERROR wrong number of arguments\n");
		fprintf(stderr,"Usage:\n$>%s port\n", argv[0]);
		exit(1);
	}

	// Creacion mutex
	pthread_mutex_init(&m, NULL);


	// Create the socket
	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check
	if (socketfd < 0)
	showError("ERROR while opening socket");

	// Init server structure
	memset(&serverAddress, 0, sizeof(serverAddress));

	// Get listening port
	port = atoi(argv[1]);

	// Fill server structure
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	// Bind
	if (bind(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		showError("ERROR while binding");

	// Listen
	listen(socketfd, N);

	// Get length of client structure
	clientLength = sizeof(clientAddress);

	//**** Establecimiento de conexion con los clientes ****/

	while(1)
	{
		if((players = (tThreadArgs *) malloc(sizeof(tThreadArgs))) == NULL)
			showError("ERROR, reserva de memoria");
		
		players->socketPlayer1 = accept(socketfd, (struct sockaddr *) &clientAddress, &clientLength);
		mensajeACCEPTS(players->socketPlayer1, 1);

		players->socketPlayer2 = accept(socketfd, (struct sockaddr *) &clientAddress, &clientLength);
		mensajeACCEPTS(players->socketPlayer2, 2);

		players->socketPlayer3 = accept(socketfd, (struct sockaddr *) &clientAddress, &clientLength);
		mensajeACCEPTS(players->socketPlayer3, 3);


		// Crear Thread
		if (pthread_create(&thread, NULL, juego, (void *)players) == -1){
			printf("Error creating thread\n");
			exit(1);
		}
	}
	
	return 0;
}
