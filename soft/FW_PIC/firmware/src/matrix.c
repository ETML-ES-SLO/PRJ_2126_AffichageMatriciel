/******************************************************************************/
// Project		: 2126 Affichage Matriciel Nom Etudiant
// Author 		: Ricardo Crespo
// Date 		: 12.06.2022
// Descrition   : Fichier du code des Matrix
/******************************************************************************/

#include <stdio.h>                      // Library standard input / output
#include "matrix.h"                     // Library des fonction des Matrix
#include "app.h"                        // Library du programme principal
#include "Mc32SpiUtil.h"                // Library de communication SPI
   
// Numérotation des lignes des Matrix
uint8_t tbDecodRow[8] = {MAX7221_REG_DIGIT0, MAX7221_REG_DIGIT1, 
                         MAX7221_REG_DIGIT2, MAX7221_REG_DIGIT3, 
                         MAX7221_REG_DIGIT4, MAX7221_REG_DIGIT5, 
                         MAX7221_REG_DIGIT6, MAX7221_REG_DIGIT7};

// Poind des colones des Matrix
uint8_t tbDecodCol[8] = {0x80, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

// Font de l'alphabet en majuscules
uint8_t tbAlphabet[27][8] = {{0x70,0x88,0x88,0xf8,0x88,0x88,0x88,0x00},  // A
                             {0xf0,0x88,0x88,0xf0,0x88,0x88,0xf0,0x00},  // B
                             {0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00},  // C
                             {0xf0,0x88,0x88,0x88,0x88,0x88,0xf0,0x00},  // D
                             {0xf8,0x80,0x80,0xf0,0x80,0x80,0xf8,0x00},  // E
                             {0xf8,0x80,0x80,0xf0,0x80,0x80,0x80,0x00},  // F
                             {0x70,0x88,0x80,0xb8,0x88,0x88,0x70,0x00},  // G
                             {0x88,0x88,0x88,0xf8,0x88,0x88,0x88,0x00},  // H
                             {0xf8,0x20,0x20,0x20,0x20,0x20,0xf8,0x00},  // I
                             {0x78,0x08,0x08,0x08,0x08,0x88,0x70,0x00},  // J
                             {0x88,0x88,0x90,0xe0,0x90,0x88,0x88,0x00},  // K
                             {0x80,0x80,0x80,0x80,0x80,0x80,0xf8,0x00},  // L
                             {0x88,0xd8,0xa8,0x88,0x88,0x88,0x88,0x00},  // M
                             {0x88,0x88,0xc8,0xa8,0x98,0x88,0x88,0x00},  // N
                             {0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00},  // O
                             {0xf0,0x88,0x88,0xf0,0x80,0x80,0x80,0x00},  // P
                             {0x70,0x88,0x88,0x88,0xa8,0x98,0x78,0x00},  // Q
                             {0xf0,0x88,0x88,0xf0,0x88,0x88,0x88,0x00},  // R
                             {0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00},  // S
                             {0xf8,0x20,0x20,0x20,0x20,0x20,0x20,0x00},  // T
                             {0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00},  // U
                             {0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00},  // V
                             {0x88,0x88,0x88,0x88,0xa8,0xd8,0x88,0x00},  // W
                             {0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00},  // X
                             {0x88,0x88,0x50,0x20,0x20,0x20,0x20,0x00},  // Y
                             {0xf8,0x08,0x10,0x20,0x40,0x80,0xf8,0x00},  // Z
                             {0xf8,0xf8,0xf8,0xf8,0xf8,0xf8,0xf8,0x00}}; // Fill

// Font de l'alphabet en minuscules
uint8_t tbLowAlphabet[27][8] = {{0x00,0x00,0x70,0x08,0x78,0x88,0x78,0x00},  // a
                                {0x80,0x80,0xb0,0xc8,0x88,0x88,0xf0,0x00},  // b
                                {0x00,0x00,0x70,0x80,0x80,0x80,0x70,0x00},  // c
                                {0x08,0x08,0x68,0x98,0x88,0x88,0x78,0x00},  // d
                                {0x00,0x00,0x70,0x88,0xf8,0x80,0x70,0x00},  // e
                                {0x30,0x48,0x40,0xe0,0x40,0x40,0x40,0x00},  // f
                                {0x00,0x00,0x78,0x88,0x78,0x08,0x70,0x00},  // g
                                {0x80,0x80,0xb0,0xc8,0x88,0x88,0x88,0x00},  // h
                                {0x00,0x20,0x00,0x20,0x60,0x20,0x70,0x00},  // i
                                {0x08,0x00,0x18,0x08,0x08,0x48,0x30,0x00},  // j
                                {0x80,0x80,0x90,0xa0,0xc0,0xa0,0x90,0x00},  // k
                                {0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00},  // l
                                {0x00,0x00,0xd0,0xa8,0xa8,0xa8,0xa8,0x00},  // m
                                {0x00,0x00,0xb0,0xc8,0x88,0x88,0x88,0x00},  // n
                                {0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00},  // o
                                {0x00,0x00,0xf0,0x88,0xf0,0x80,0x80,0x00},  // p
                                {0x00,0x00,0x68,0x98,0x78,0x08,0x08,0x00},  // q
                                {0x00,0x00,0xb0,0xc8,0x80,0x80,0x80,0x00},  // r
                                {0x00,0x00,0x70,0x80,0x70,0x08,0xf0,0x00},  // s
                                {0x40,0x40,0xe0,0x40,0x40,0x48,0x30,0x00},  // t
                                {0x00,0x00,0x88,0x88,0x88,0x98,0x68,0x00},  // u
                                {0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00},  // v
                                {0x00,0x00,0x88,0x88,0xa8,0xa8,0x50,0x00},  // w
                                {0x00,0x00,0x88,0x50,0x20,0x50,0x88,0x00},  // x
                                {0x00,0x00,0x88,0x88,0x78,0x08,0x70,0x00},  // y
                                {0x00,0x00,0xf8,0x10,0x20,0x40,0xf8,0x00},  // z
                                {0xf8,0xf8,0xf8,0xf8,0xf8,0xf8,0xf8,0x00}}; // Fill

// Font des chiffres
uint8_t tbNumber[10][8] = {{0x70,0x88,0x98,0xa8,0xc8,0x88,0x70,0x00},  // 0
                           {0x20,0x60,0x20,0x20,0x20,0x20,0x70,0x00},  // 1
                           {0x70,0x88,0x08,0x30,0x40,0x80,0xf8,0x00},  // 2
                           {0x70,0x88,0x08,0x30,0x08,0x88,0x70,0x00},  // 3
                           {0x10,0x30,0x50,0x90,0xf8,0x10,0x10,0x00},  // 4
                           {0xf8,0x80,0xf0,0x08,0x08,0x88,0x70,0x00},  // 5
                           {0x30,0x40,0x80,0xf0,0x88,0x88,0x70,0x00},  // 6
                           {0xf8,0x08,0x10,0x20,0x40,0x40,0x40,0x00},  // 7
                           {0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00},  // 8
                           {0x70,0x88,0x88,0x78,0x08,0x10,0x60,0x00}}; // 9

// Font du character d'un point '.'
uint8_t tbDot[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00};    // .

// Set le chip select à l'état bas pour commencer la communication SPI
void startSS()
{
    // État bas du chip select du SPI
    SS1_Off(); 
}

// Set le chip select à l'état haut pour stoper la communication SPI
void stopSS()
{
    // État haut du chip select du SPI
    SS1_On(); 
}

// Fait la détection du nombre de Matrix qui sont connectées
void FindNumberMatrix(uint8_t* _numberMatrix)
{
    uint8_t bufferReadKey;                  // Buffeur de lecture de la clé
    uint8_t keyMatrix = 123;                // Clé de rebouclement des Matrix
    static uint8_t counterNumberMatrix = 0; // Compteur du nomvre de Matrix connectées
    
    // Set le chip select à l'état bas pour commencer la communication SPI
    startSS();
    
    // Clear les registres d'un certain nombre imaginaire de Matrix
    FillRegisterMatrix();
    
    // Tant que l'on ne reçois pas la clé de rebouclement des matrix
    do{
        // Écriture de la clé dans les Matrix via SPI
        bufferReadKey = spi_read1(keyMatrix);   
        // Incémentation du comteur du nombre de Matrix
        counterNumberMatrix++; 
    }while(bufferReadKey != keyMatrix);
    
    // Set le chip select à l'état haut pour stoper la communication SPI
    stopSS();
    
    // Retourne le nombre de Matrix connectées
    *_numberMatrix = (counterNumberMatrix - 1) / 2;
}

// Clear les registres d'un certain nombre imaginaire de Matrix
void FillRegisterMatrix()
{
    uint8_t i;              // Compteur de la boucle de remplisage des registres
    
    // Envoie pour un certain nombre imaginaire de Matrix d'un remplisage de registre
    for(i = 0; i < NUM_MATRIX_CLEAR; i++)
    {
        // Évoie un remplisage de registre pour le clear sur les Matrix
        SendOneByteRaw(MAX7221_REG_NOOP);
    }
}

// Envoie le buffeur d'affichage sur les Matrix
void SendAllMatrixRow(uint8_t _pMatrix[][8])
{        
    int i = 0;              // Compteur de la boucle pour le nombre de lignes
    int j = 0;              // Compteur pour le nombre de Matix

    // Pour toutes les lignes qu'il fait afficher
    for(i = ROW_MATRIX-1; i >= 0; i--)
    {
        // Set le chip select à l'état bas pour commencer la communication SPI
        startSS();
        
        // Pour toutes les Matrix connectées
        for(j = (maxMatrix - 1); j >= 0; j--)
        {
            // Envoie tout le buffeur d'affichage sur les Matrix
            SendByteRaw(tbDecodRow[i], ROTATE_RIGHT_BYTE(_pMatrix[j][i]));
        }
        
        // Set le chip select à l'état haut pour stoper la communication SPI
        stopSS();
    }
}

// Annimation de défilement du nom sur les Matrix
void ShiftAllMatrixRow(uint8_t _pMatrix[][8])
{    
    uint8_t i = 0;                  // Compteur pour le MSB des lignes de la première collone
    uint8_t j = 0;                  // Compteur pour tous les caractères que l'on affiche
    uint8_t k = 0;                  // Compteur pour toutes les lifnes des Matrix
    uint8_t pMatrixBuffer[8];       // Buffeur des MSB des lignes de la première collone
    
    // Schift le text seulement s'il est plus grand que la surfece d'affichage
    if(countCar > maxMatrix)
    {
        // Récupére la valeur de tous les MSB des lignes de la première collone
        for(i = 0; i < ROW_MATRIX; i++)
        {
            // Récupération du MSB de la ligne actuelle
            pMatrixBuffer[i] = RECUP_MSB(_pMatrix[0][i]);
        }

        // Pour tous les character que l'on affiche
        for(j = 0; j < countCar - 1; j++)
        {
            // Pour toutes les lignes des Matrix
            for(k = 0; k < ROW_MATRIX; k++)
            {
                // Décale de un vers la gauche toutes les lignes du buffeur d'affichage
                _pMatrix[j][k] = _pMatrix[j][k] << 1;
                
                // Si on n'est pas à la dernière Matrix du buffer d'affichage
                if(j < (countCar - 3))
                {
                    // Récupére le MSB de la prochainne Matrix et le met dans le LSB de la Matrix actuel
                    _pMatrix[j][k] |= RECUP_MSB(_pMatrix[j + 1][k]);
                }
                // Si non on est à la dernière Matrix du buffer d'affichage
                else
                {
                    // Remplace le LSB de la dernière Matrix du buffer d'affichage dans le MSB de la Matrix actuel
                    _pMatrix[j][k] |= pMatrixBuffer[k];
                }
            }
        }
        
        // Envoie le buffeur d'affichage sur les Matrix
        SendAllMatrixRow(_pMatrix);
    }
}

// Met une suite de caractères dans le buffeur d'affichage
void SendText(char *_text, uint8_t _pMatrix[][8])
{
    int i;      // Compteur de la boucle pour le nombre de caractères traités
    
    // Pour tous les caractères du nom
    for (i = 0; i <= countCar -1; i++) 
    {
        // Ajoute le caracter actuel au bufeur d'affichage au format pour les Matrix
        AddCharacter(_text[i], _pMatrix, (i + 1));
    }
}

// Ajoute un caractére au bon format pour les Matrix dans le bufeur d'affichage
void AddCharacter(char _characteer, uint8_t _pMatrix[][8], uint8_t _witchPlace)
{
    int i;                          // Compteur de la boucle pour le nombre de lignes

    uint8_t numCar = _witchPlace;   // Pour la position du caractère que l'on place
    uint8_t placeMatrix = 0;        // Pour indiquer sur quel Matrix on écrit
    uint8_t startCar = 0;           // Pour la position sur la Matrix dans le quel on écrit
    
    // Pour le nombre de lignes de notre Matrix
    for (i = 0; i < ROW_MATRIX; i++) 
    {
        // Calcule sur quel Matrix est-ce que l'on doit écrir le caractère selectionné à la suite
        placeMatrix = (uint8_t)((numCar - 1) / 1.4);
        // Calcul sur quel LED de la Matrix actuel est ce que l'on doit commencer à écrir le caractère
        startCar = ((((numCar - 1) * 5) + (numCar - 1)) - (ROW_MATRIX * placeMatrix));
        
        // Majuscules - Si le caractère est compris entre l'alphabet en majuscules de la table ASCII
        if((_characteer >= 0x41) && (_characteer <= 0x5A))
        {
            // Converti le caractère majuscules en forme pour les LED de la Matrix actuelle
            _pMatrix[(placeMatrix)][i] |= (tbAlphabet[_characteer - 0x41][i]) >> (startCar);
            // Si le caractère majuscules déborde sur la prochaine Matrix
            if(startCar >= NEXT_CAR)
            {
                // Converti le caractère majuscules en forme pour les LED de la Matrix suivante
                _pMatrix[(placeMatrix + 1)][i] 
                |= (tbAlphabet[_characteer - 0x41][i]) << (ROW_MATRIX - (startCar));
            }
        }
        // Minuscules - Si le caractère est compris entre l'alphabet en minuscules de la table ASCII
        else if ((_characteer >= 0x61) && (_characteer <= 0x7A))
        {
            // Converti le caractère minuscules en forme pour les LED de la Matrix actuelle
            _pMatrix[(placeMatrix)][i] |= (tbLowAlphabet[_characteer - 0x61][i]) >> (startCar);
            // Si le caractère minuscules déborde sur la prochaine Matrix
            if(startCar >= NEXT_CAR)
            {
                // Converti le caractère minuscules en forme pour les LED de la Matrix suivante
                _pMatrix[(placeMatrix + 1)][i] 
                |= (tbLowAlphabet[_characteer - 0x61][i]) << (ROW_MATRIX - (startCar));
            }
        }
        // Chiffres - Si le caractère est un chiffre de la table ASCII
        else if ((_characteer >= 0x30) && (_characteer <= 0x57))
        {
            // Converti le caractère chiffre en forme pour les LED de la Matrix actuelle
            _pMatrix[(placeMatrix)][i] |= (tbNumber[_characteer - 0x30][i]) >> (startCar);
            // Si le caractère chiffre déborde sur la prochaine Matrix
            if(startCar >= NEXT_CAR)
            {
                // Converti le caractère chiffre en forme pour les LED de la Matrix suivante
                _pMatrix[(placeMatrix + 1)][i] 
                |= (tbNumber[_characteer - 0x30][i]) << (ROW_MATRIX - (startCar));
            }
        }
        // Point - Si le caractère est un point	de la table ASCII
		else if (_characteer == 0x2E)
        {
            // Converti le caractère point en forme pour les LED de la Matrix actuelle
            _pMatrix[(placeMatrix)][i] |= tbDot[i] >> (startCar);
            // Si le caractère point déborde sur la prochaine Matrix
            if(startCar >= NEXT_CAR)
            {
                // Converti le caractère point en forme pour les LED de la Matrix suivante
                _pMatrix[(placeMatrix + 1)][i] 
                |= tbDot[i] << (ROW_MATRIX - (startCar));
            }
        }
    }
}

// Initialisation des MAX7221 de toutes les Matrix connectées
void InitMatrix ()
{
    // Vide tous les registres des Matrix connectées
    ShiftNooMatrix();
    
    // Scan Limit
    SendByte(max7221_reg_scanLimit, 0x07);
    // Decode mode
    SendByte(max7221_reg_decodeMode, 0x00);
    // Display test
    SendByte(max7221_reg_displayTest, 0x01);
    // Display test
    SendByte(max7221_reg_displayTest, 0x00);
    // Intensity
    SendByte(max7221_reg_intensity, INTENSITY);
    // No ShutDown
    SendByte(max7221_reg_shutdown, 0x01);
    
    // Vide tous les registres des Matrix connectées 
    ShiftNooMatrix();
}

// Vide tous les registres des Matrix connectées
void ShiftNooMatrix()
{
    int i = 0;          // Compteur de la boucle du nombre de Matrix connectées
    
    // Pour toutes les Matrix qui sont connectées
    for(i = 0; i < maxMatrix; i++)
    {
        // Envoie des character nuls sur la Matrix actuel
        SendByte(MAX7221_REG_NOOP, 0x00);
    }
}

// Envoie un byte d'adresse et un byte de data avec la gestion des chip select du SPI
void SendByte(uint8_t _addr, uint8_t _data)
{    
    // Set le chip select à l'état bas pour commencer la communication SPI
    startSS();       
    // Écrit le byte d'adresse avec la fonction de lecture du SPI1 pour clear le buffer hardware
    spi_read1(_addr);
    // Écrit le byte de data avec la fonction de lecture du SPI1 pour clear le buffer hardware
    spi_read1(_data);  
    // Set le chip select à l'état haut pour stoper la communication SPI
    stopSS();
}

// Envoie uniquement un byte via le SPI
void SendOneByteRaw(uint8_t _data)
{     
    // Écrit le byte avec la fonction de lecture du SPI1 pour clear le buffer hardware
    spi_read1(_data); 
}

// Envoie uniquement un byte d'adresse et un byte de data sur le SPI
void SendByteRaw(uint8_t _addr, uint8_t _data)
{          
    // Écrit le byte d'adresse avec la fonction de lecture du SPI1 pour clear le buffer hardware
    spi_read1(_addr);
    // Écrit le byte de data avec la fonction de lecture du SPI1 pour clear le buffer hardware
    spi_read1(_data);  
}

//== FONCTION NON UTILISÉES MAIS UTILES POUR LA COMMANDE D'UNE SEUL MATRIX ==//
//====== ELLES N'UTILISENT PAS TOUTES LE SYSTEM D'AFFICHAGE PAR LIGNE ====== //
//===== ELLE N'ON PAS ÉTÉ SUPRIMÉE POUR FACILITER LA REPRISE DU PROJET =====// 

// Affiche un seul caractere sur la premiprère Matrix
void SetOneCharacter(char _character, uint8_t _pMatrix[][8], uint8_t _witchMatrix)
{
    int i;
    for (i = 0; i < 8; i++) 
    {
        // Maguscules
        if((_character >= 0x41) && (_character <= 0x5A))
        {
            _pMatrix[(_witchMatrix - 1)][i] |= (tbAlphabet[_character - 0x41][i]) >> 1;
        }
         // Minuscules
        else if ((_character >= 0x61) && (_character <= 0x7A))
        {
            _pMatrix[(_witchMatrix - 1)][i] |= (tbLowAlphabet[_character - 0x61][i]) >> 1;
        }
        // Chiffres
        else if ((_character >= 0x30) && (_character <= 0x57))
        {
            _pMatrix[(_witchMatrix - 1)][i] |= (tbNumber[_character - 0x30][i]) >> 1;
        }
        // Point
        else if (_character == '.')
        {
            _pMatrix[(_witchMatrix - 1)][i] |= (tbDot[i]) >> 1;
        }
    }
}

// Affiche le même byte sur toutes les Matrix connectées
void SendByteAllMatrix(uint8_t* pBytesToWrite)
{        
    int i = 0;
    startSS();
    for(i = 0; i < 3; i++)
    {
        spi_write1(tbDecodRow[i]);
        spi_write1((tbDecodCol[pBytesToWrite[i]]) >> 1);
    }
    stopSS();
}

// Passe en mode shutdown et reviens au mode normal
void RefreshMatrix()
{
    SendByte(max7221_reg_shutdown, 0x00);
    SendByte(max7221_reg_shutdown, 0x01);
}

// Allume une seul LED dans la première Matrix
void SetLed(uint8_t _row, uint8_t _col)
{
    SendByte(tbDecodRow[_row - 1], tbDecodCol[_col - 1]);
}

// Allume toute une ligne sur la première Matrix
void SetFullRow(uint8_t _row)
{
    SendByte(tbDecodRow[_row - 1], 0xFF);
}

// Allume une partie spécifique d'une nigne sur la première Matrix
void SetRow(uint8_t _row, uint8_t _colData)
{
    SendByte(tbDecodRow[_row - 1], _colData >> 1);
}

// Allume toute une collone sur la première Matrix
void SetFullCol(uint8_t _col)
{
    int i;
    for (i = 0; i < 8; i++) 
    {
        SendByte(tbDecodRow[i], tbDecodCol[_col - 1]);
    }
}

// Efface toute la première Matrix
void ClearMatrix()
{
    int i;
    for (i = 0; i < 8; i++) 
    {
        SendByte(tbDecodRow[8], 0x00);
    }
}
