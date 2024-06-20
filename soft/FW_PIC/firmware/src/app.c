/******************************************************************************/
// Project		: 2126 Affichage Matriciel Nom Etudiant
// Author 		: Ricardo Crespo
// Date 		: 12.06.2022
// Descrition   : Fichier du code principal
/******************************************************************************/

/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"                        // Library du programme principal
#include "Mc32SpiUtil.h"                // Library de communication SPI
#include "matrix.h"                     // Library des fonction des Matrix

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;               // Structure de la machine d'état principale

uint8_t countCar = 0;           // Compteur du nombre de characters d'un nom
uint8_t maxMatrix = 0;          // Nombre de Matrix max sur le quel on peut afficher

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// Fonction Callback du Timer1 pour commander la machinne d'état pricipale
void MainTimer_Callback()
{
    // Compteur du temps de cycle principale
    static uint32_t mainTimerCount = 0;
    // Compteur du temps de défilement de l'annimation
    static uint32_t shiftTimerCount = 0;
    
    // Pour du débug (default commented)
    //DLED_Toggle();
    
    // Incrémentation compteur du temps de cycle principale
    mainTimerCount++;

    // Si le compteur du temps de cycle principale a attent la valeut voulue
    if (mainTimerCount >= TIMER_MAIN)
    {
        // Reset le compteur du temps de cycle principale
        mainTimerCount = 0;
        // Active le flag qui appelle la machine d'état principale
        appData.mainTimerHasOcurred = true;
    }

    // Compteur du temps de défilement de l'annimation
    shiftTimerCount++;

    // Si le compteur du temps de défilement de l'annimationa a attent la valeur voulue
    if (shiftTimerCount >= TIMER_SHIFT)
    {
        // Reset le compteur du temps de défilement de l'annimation
        shiftTimerCount = 0;
        // Active le flag qui effectue la prichaine étape de l'animation
        appData.shiftTimerHasOcurred = true;
    }
}
    
// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks ( void )
{
    static uint8_t tbMatrix[32][8];      // Buffeur d'afficahge sur le Matrix
    static uint8_t numberChar = 0;       // Compteir de la taille de la clé d'envoi

    char key[] = {'C', '\0'};            // Clé de confirmaiton pour le software
    char keyCom = 'x';                   // Clé de communicaiton de la part du software
    char keyEndName[] = {'X', 'D', 'R'}; // Clé de fin de nom complet
    char character;                      // Buffeur du character actuellement lu
    char reciveCharacter = ' ';          // Buffeur de réception des caracters de l'UART
    static char buffReadName[20] = {' '};// Buffer de reception de l'UART

    static bool startReadName = false;   // Permet de commencer à lire le nom
    static bool canShift = false;        // Peut commencer l'animation
    
    // Si le flag d'appel de la machine d'état principale est actif
    if(appData.mainTimerHasOcurred)
    {
        // Reset le flag de la machine d'état principale 
        appData.mainTimerHasOcurred = false;
        // Passe dans l'état TASKS
        APP_UpdateState(APP_STATE_SERVICE_TASKS);
    }

    // Machine d'état principale
    switch ( appData.state )
    {
        // État d'initialiation
        case APP_STATE_INIT:
        {                 
            // Démare le timeur 1
            DRV_TMR0_Start();
            
            // Active le SPI
            PLIB_SPI_Enable(SPI_ID_1);
  
            // Fait la détection du nombre de Matrix qui sont connectées
            FindNumberMatrix(&maxMatrix);
            
            // Initialise les matrix, donc setup les MAX7221
            InitMatrix();
            
            // Clear toutes les matrix connectée, donc éteint toutes les LEDs
            SendAllMatrixRow(tbMatrix);
            
            //======== START ONLY FOR LOCAL TEST (default commented) ========//
            
            // Écrit le prénom et la première létre du nom du créateur dans le buffeur d'affichage
            //SendText("Ricardo C", tbMatrix);
            
            // Envoie le buffeur d'affichage sur les Matrix
            //SendAllMatrixRow(tbMatrix);
            
            // Active l'animation de défilement
            //canShift = true;
            
            //======== END ONLY FOR LOCAL TEST (default commented) ========//
            
            // Passe dans l'état WAIT
            APP_UpdateState(APP_STATE_WAIT);
            
            break;
        }

        // État d'attente
        case APP_STATE_WAIT:
        {
            // Ne fait rien
            break;       
        }
                
        // État d'éxecution
        case APP_STATE_SERVICE_TASKS:
        {
            // Tant que l'on reçois des datas dans le RX buffeur ET pas l'animation 
            while(PLIB_USART_ReceiverDataIsAvailable(USART_ID_1) && !canShift)
            {
                // Récupération du charactere dpuis le RX buffeur
                character = PLIB_USART_ReceiverByteReceive(USART_ID_1);
                
                // Si on a pas reçu la clé de communication et qu'on a pas encore lu le nom 
                if((character != keyCom) && (!startReadName))
                {
                    // Reset du compteur du nombre de characteres du nom
                    countCar = 0;
                    // Peut démarer la lecture du nom complete
                    startReadName = true;
                }
                // Si non on stoque la clé de communicaiton avec le software
                else
                {
                    // Sauvegarde de la clé de communicaiton avec le software
                    reciveCharacter = character;
                }
                
                // Si on peut faire la lecture du nom complete
                if(startReadName)
                {
                    // Stockage du charactere actuel dans le buffer de stockage du nom
                    buffReadName[countCar] = character;
                    // Incrémentation du nombre de characteres du nom
                    countCar++;
                }
            }
            
            // Si on a reçu la clé du Software ET que l'on lit pas le nom
            if((reciveCharacter == keyCom) && !startReadName)
            {
                // Tant qu'on a pas fini la chaine ET que l'on a pas plus de 8 caracteres
                while ((key[numberChar] != 0) && (numberChar < ZISE_KEY)) 
                {
                    // Attent que le TX buffeur soit disponible
                    while(PLIB_USART_TransmitterBufferIsFull(USART_ID_1));
                    // Envoi de la clé de confirmation au software
                    PLIB_USART_TransmitterByteSend(USART_ID_1, key[numberChar]);
                    // Incrémentation du compteur de nombre de characteres
                    numberChar++;
                }
                // Reset du beuffer de réception des characteres
                reciveCharacter = ' ';
                // Reset du compteur de nombre de characteres
                numberChar = 0;
            } 
            
            // Si on a reçu un nom d'éléve et qu'on a pas encore d'annimation
            if((buffReadName[0] != 0x20)
            && (buffReadName[0] != NULL)
            && (buffReadName[0] != keyCom)
            && (buffReadName[countCar - 3] == keyEndName[0])
            && (buffReadName[countCar - 2] == keyEndName[1])
            && (buffReadName[countCar - 1] == keyEndName[2])
            && (!canShift))
            {
                // Enléve les position de la clé de réception du nom 
                countCar -= (END_NAME_KEY_SIZE - 1);
                // Insére un '.' aprés la lettre maguscule du nom de l'éléve
                buffReadName[countCar - 1] = '.';
                // Écrit le nom réceptionné dans le buffeur d'affichage
                SendText(buffReadName, tbMatrix);     
                // Envoie le buffeur d'affichage sur les Matrix
                SendAllMatrixRow(tbMatrix);
                // Peut démarer l'animation
                canShift = true;
            }
            
            // Si le flag qui effectue la prichaine étape de l'animation est actif
            if(appData.shiftTimerHasOcurred && canShift)
            {
                // Reset du flag qui effectue la prichaine étape de l'animation
                appData.shiftTimerHasOcurred = false;
                // Effectue l'animation de défilement
                ShiftAllMatrixRow(tbMatrix);
            }

            // Passe dans l'ï¿½tat WAIT
            APP_UpdateState(APP_STATE_WAIT);
            break;
        }

        // État par défaut
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

// Permet de changer l'état de la machine d'état principale
void APP_UpdateState(APP_STATES newState)
{
    // Affectation du nouveau état de la machine d'état principale
    appData.state = newState;       
}

/*******************************************************************************
 End of File
 */
