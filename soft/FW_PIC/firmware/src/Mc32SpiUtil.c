/******************************************************************************/
// Project		: 2126 Affichage Matriciel Nom Etudiant
// Author 		: Ricardo Crespo
// Date 		: 12.06.2022
// Descrition   : Fichier du code de la communication SPI
/******************************************************************************/

// Mc32SpiUtil.C
// Utilitaire  SPI
//
//	Description : 	fonctions SPI CCS like
//	Auteur 		: 	C. HUBER
//      Cr�ation	: 	08.04.2014
//      Modifications   :       09.02.2015 CHR

//  LISTE DES MODIFICATIONS :
//      Utilisation des stdint                  09.02.2015 CHR
//      Adaptation � la plib_spi                09.02.2015 CHR
//      maj version compilateur et Harmony      24.05.2016 CHR
//      Correction de la s�quence de lecture    25.05.2016 CHR
//      Version KIT     PCB 11020_B
//	Version		:	V1.2
//	Compilateur	:	XC32 V1.40 + Harmony 1.06
//
/*--------------------------------------------------------*/

#include "app.h"                        // Library du programme principal
#include "Mc32SpiUtil.h"                // Library de communication SPI
#include "peripheral\SPI\plib_spi.h"    // Library PLIB pour le SPI

// �criture d'un byte sur le SPI1
void spi_write1( uint8_t Val){
   int SpiBusy;
   
   PLIB_SPI_BufferWrite(SPI_ID_1, Val);

   do {
     SpiBusy =  PLIB_SPI_IsBusy(SPI_ID_1) ;
   } while (SpiBusy == 1);
}

// Lecture via une �criture pour d�caler d'un byte sur le SPI1
uint8_t spi_read1( uint8_t Val){
   int SpiBusy;  
   uint32_t lu;
   
   PLIB_SPI_BufferWrite(SPI_ID_1, Val);  
   // Attends fin transmission
   do {
        SpiBusy =  PLIB_SPI_IsBusy(SPI_ID_1) ;
   } while (SpiBusy == 1);
   
   // Attend arriv�e dans fifo
   while (PLIB_SPI_ReceiverFIFOIsEmpty(SPI_ID_1));
#ifdef MARKER_READ
   LED3_W  = 1;
#endif
   lu = PLIB_SPI_BufferRead(SPI_ID_1);
#ifdef MARKER_READ
   LED3_W  = 0;
#endif
   return lu;
}
