// robot.h

#ifndef _ROBOT_H_
#define _ROBOT_H_

// Prototypes
void setSens(short roueA, short roueB);
void parseTrame();

extern unsigned char buffer[15];
extern unsigned int index;


#define TRAME_CAR_START '-'             // Caractere debut de trame
#define TRAME_CAR_SEPARCMD ':'          // caractere separation commande
#define TRAME_CAR_END ';'               // caractere fin de trame
#define TRAME_CMD_INDEX 1               // index du caractere de commande
#define TRAME_SENSA_INDEX 3             // index signe roue A
#define TRAME_SENSB_INDEX 8             // index signe roue B
#define TRAME_CAR_SEPAR ','
#define TRAME_CAR_SENS_POS '+'
#define TRAME_CAR_SENS_NEG '-'
#define TRAME_CAR_VITESSE 'v'   // -v:+100,+100;
#define TRAME_CAR_LED 'l'       // -l:g,d;
#define TRAME_LEDG_INDEX 3             // index led gauche
#define TRAME_LEDD_INDEX 5             // index led droite
#define TRAME_CAR_AU 's'        // Arret Urgence

#endif