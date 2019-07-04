#include "game.h"


/**
 * Reads a move entered by the player
 * @return A number between [0-BOARD_WIDTH) that represents the column where the chip is going to be inserted
 */
unsigned int readMove ();
/** Envia un mensaje al sevidor */
int enviarMensajeCliente(int fdsocket, tString msg);
/** Recibe un mensaje del sevidor */
int recibirMensajeCliente(int fdsocket, tString msg);
/** Recibe el tablero del sevidor */
int recibirBoardCliente(int fdsocket, tBoard board);
/** Recibe el codigo del sevidor */
unsigned int recibirCodigoCliente(int fdsocket);
/** Recibe el mensaje de que turno sel servidor */
void recibirMensajeTurno(int fdsocket, tBoard board, unsigned int *codigo, tString mensaje);
/** Envia el movimiento selecionado al server */
int enviarMoveCliente(int fdsocket, unsigned int move);
