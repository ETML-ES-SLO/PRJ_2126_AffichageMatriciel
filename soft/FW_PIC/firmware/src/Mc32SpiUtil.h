#ifndef Mc32SpiUtil_H
#define Mc32SpiUtil_H

/******************************************************************************/
// Project		: 2126 Affichage Matriciel Nom Etudiant
// Author 		: Ricardo Crespo
// Date 		: 12.06.2022
// Descrition   : Header du code de la communication SPI
/******************************************************************************/

//  Mc32SpiUtil.h
//	Description :	Utilitaire gestion SPI CCS like
//
//	Auteur 		: 	C. Huber
//	Version		:	V1.1
//	Compilateur	:	XC32 V1.40
//
/*--------------------------------------------------------*/

#include <stdint.h>                     // Library standard de types

// Écriture d'un byte sur le SPI1
void spi_write1( uint8_t Val);

// Lecture via une écriture pour décaler d'un byte sur le SPI1
uint8_t spi_read1( uint8_t Val);

#endif
