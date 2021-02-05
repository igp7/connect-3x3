#include "game.h"

pthread_mutex_t m;

/* Envia un mensaje al clinte */
int enviarMensajeServer(int fdsocket, tString msg);
/* Envia un tablero al clinte */
int enviarTableroServer(int fdsocket, tBoard tablero);
/* Envia un Codigo al clinte */
int enviarCodigoServer(int fdsocket, unsigned int codigo);
/* Recibe un mensaje del clinte */
void recibirMensajeServer(int fdsocket, tString msg);
/* Envia el mensaje de conexion establecida a los clientes */
void mensajeACCEPTS(int fdsocket, int player);
/* Envia el mensaje de ACK a los clientes */
void mensajeACK(int fdsocket);
/* Envia el mensaje de los rivales contra los que se enfrentan a los clientes */
void mensajeRIVALES(int fdsocket, tString rival1, tString rival2);
/* Envia el mensaje de game start a los clientes */
void mensajeSTART(int fdsocket);
/* Seleciona de quien es el turno siguiente en la partida */
tPlayer selecionTurno(int *fdsocket, tPlayer turnoActual, char *chip, tThreadArgs players);
/* Envia mensajes al cliente que le toca jugar en ese turno */
void enviarMensajeTurnoJugar(int fdsocket, char playerChip, tBoard board);
/* Envia mensajes al cliente que no le toca jugar en ese turno */
void enviarMensajeTurnoEspera(int fdsocket, char playerChip, tBoard board);
/* Encia el pensaje de judar al cliente que tenga el turno y el de espera al resto de clientes en la partida */
void enviarMensajesTurno(tPlayer jugadorActual, tThreadArgs players, tBoard board);
/* Recibe el movimiento selecionado por el cliente */
unsigned int recibirMove(int fdsocket);
/* Manda un mensaje al ganador de la partida y a los perdedores */
void mensajeGanador(tPlayer jugadorActual, tThreadArgs players, tBoard board);
/* Manda un mensaje de enpate a los clientes de la partida */
void enviarMensajeEmpate(int fdsocket, tBoard board);
/* Manda un mensaje al perdidor de la partida */
void enviarMensajePerdedor(int fdsocket, tBoard board);
/* Manda un mensaje al ganador de la partida */
void enviarMensajeGanador(int fdsocket, tBoard board);
/* Guarda el resultado de la partida en el archivo scores.txt */
void actualizaScore(tPlayer jugadorActual, int ganador, int boardFull, tString player_1, tString player_2, tString player_3);
/* Tiene el desarollo del juego */
void *juego(void *arg);
