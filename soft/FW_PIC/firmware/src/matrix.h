/******************************************************************************/
// Project		: 2126 Affichage Matriciel Nom Etudiant
// Author 		: Ricardo Crespo
// Date 		: 12.06.2022
// Descrition   : Header du code des Matrix
/******************************************************************************/

#include <stdint.h>                     // Library standard de types
#include <stdbool.h>                    // Library pour le type bool
#include <stddef.h>                     // Library standard de types implicites
#include <stdlib.h>                     // Library standard d'acces à la mémoire
#include "system_config.h"              // Library des configurations system
#include "system_definitions.h"         // Library des definitions system

#ifndef MATRIX_H
#define	MATRIX_H

#define MAX7221_REG_NOOP        0x00        // Registre No operation   
#define MAX7221_REG_DIGIT0      0x01        // Registre Ligne 0
#define MAX7221_REG_DIGIT1      0x02        // Registre Ligne 1
#define MAX7221_REG_DIGIT2      0x03        // Registre Ligne 2
#define MAX7221_REG_DIGIT3      0x04        // Registre Ligne 3
#define MAX7221_REG_DIGIT4      0x05        // Registre Ligne 4
#define MAX7221_REG_DIGIT5      0x06        // Registre Ligne 5
#define MAX7221_REG_DIGIT6      0x07        // Registre Ligne 6
#define MAX7221_REG_DIGIT7      0x08        // Registre Ligne 7
#define max7221_reg_decodeMode  0x09        // Registre Decode mode
#define max7221_reg_intensity   0x0A        // Registre Intencité
#define max7221_reg_scanLimit   0x0B        // Registre Scan Limit
#define max7221_reg_shutdown    0x0C        // Registre Shutdown
#define max7221_reg_displayTest 0x0F        // Registre Disply Test

#define NUM_MATRIX_CLEAR        32          // Nombre de Matrix à vider

#define INTENSITY               9           // Intencité d'éclairage des Natrix

#define ZISE_KEY                2           // Taille de la clé pour le Software

#define END_NAME_KEY_SIZE       3           // Taille de la clé de fin de nom

#define ROW_MATRIX              8           // Nombre de lignes des Matrix

#define WIDTH_FONT              5           // Largeur de la font de l'alphabet

#define NEXT_CAR                4           // Valeur max de début de caracter

#define TIMER_MAIN              1           // Valeur de butée pour la machine d'état

#define TIMER_SHIFT             5           // Valeur de butée pour l'animation

#define ROTATE_RIGHT_BYTE(x)    ((x >> 1) | (x << 7)) // Décalage à gauche circulaire de un

#define RECUP_MSB(x)            (x >> 7)    // Récupére le MSB d'un byte        


// Set le chip select à l'état bas pour commencer la communication SPI
void startSS(void);

// Set le chip select à l'état haut pour stoper la communication SPI
void stopSS(void);
             
// Fait la détection du nombre de Matrix qui sont connectées
void FindNumberMatrix(uint8_t* _numberMatrix);

// Clear les registres d'un certain nombre imaginaire de Matrix
void FillRegisterMatrix(void);

// Envoie le buffeur d'affichage sur les Matrix
void SendAllMatrixRow(uint8_t _pMatrix[][8]);

// Annimation de défilement du nom sur les Matrix
void ShiftAllMatrixRow(uint8_t _pMatrix[][8]);

// Met une suite de caractères dans le buffeur d'affichage
void SendText(char *_text, uint8_t _pMatrix[][8]);

// Ajoute un caractére au bon format pour les Matrix dans le bufeur d'affichage
void AddCharacter(char _characteer, uint8_t _pMatrix[][8], uint8_t _witchPlace);

// Initialisation des MAX7221 de toutes les Matrix connectées
void InitMatrix(void);

// Vide tous les registres des Matrix connectées
void ShiftNooMatrix(void);

// Envoie un byte d'adresse et un byte de data avec la gestion des chip select du SPI
void SendByte(uint8_t _addr, uint8_t _data);

// Envoie uniquement un byte via le SPI
void SendOneByteRaw(uint8_t _data);

// Envoie uniquement un byte d'adresse et un byte de data sur le SPI
void SendByteRaw(uint8_t _addr, uint8_t _data);

//== FONCTION NON UTILISÉES MAIS UTILES POUR LA COMMANDE D'UNE SEUL MATRIX ==//
//====== ELLES N'UTILISENT PAS TOUTES LE SYSTEM D'AFFICHAGE PAR LIGNE ====== //
//===== ELLE N'ON PAS ÉTÉ SUPRIMÉE POUR FACILITER LA REPRISE DU PROJET =====// 

// Affiche un seul caractere sur la premiprère Matrix
void SetOneCharacter(char _characteer, uint8_t _pMatrix[][8], uint8_t _witchMatrix);

// Affiche le même byte sur toutes les Matrix connectées
void SendByteAllMatrix(uint8_t* pBytesToWrite);

// Passe en mode shutdown et reviens au mode normal
void RefreshMatrix(void);

// Allume une seul LED dans la première Matrix
void SetLed(uint8_t _row, uint8_t _col);

// Allume toute une ligne sur la première Matrix
void SetFullRow(uint8_t _row);

// Allume une partie spécifique d'une nigne sur la première Matrix
void SetRow(uint8_t _row, uint8_t _colData);

// Allume toute une collone sur la première Matrix
void SetFullCol(uint8_t _col);

// Efface toute la première Matrix
void ClearMatrix(void);

#endif	/* MATRIX_H */
