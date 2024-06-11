/******************************************************************************/
// Project		        : User Retrival for 2126 Affichage Matriciel Nom Etudiant
// Author 		        : Ricardo Crespo
// Modifications        : Mélissa Perret
// Date 		        : 12.06.2022
// Date de modification : 14.05.2024
// Descrition           : Fichier du code principale
/******************************************************************************/

// Toutes les librairies (namespace) utilisées
using System.Windows.Forms;
using UserRetrieval.Controllers;
using System.IO.Ports;
using System.Linq;
using System;

namespace UserRetrieval.Views
{
    /// <summary>
    /// User Interface Class
    /// </summary>
    public partial class View : Form
    {
        /// <summary>
        /// Property
        /// </summary>
        internal Controller Ctrler { get; set; }        // Contrôleur pour la classe Controller

        private static readonly char[] SpaceSeparator = new char[] { ' ' }; // Définition d'un tableau de caractères contenant uniquement un espace comme séparateur

        private const string KEY = "C";                 // Clé de retour attendue
        private const string END_NAM_KEY = "XDR";       // Clé de fin de nom
        private const string ANNOUNCE_KEY = "x";        // Clé d'annoncement
        private const int MAX_NUMBER_OF_LETTERS = 11;   // Maximum de lettre pris en charge par le code du firmware 
        private const int MAX_FIRSTNAME_LENGTH = MAX_NUMBER_OF_LETTERS - 3; // On fait le max de lettre - 3, le 3 correspond à l'espace entre le prénom et le nom, la première du nom et le point

        string[] ports = new string[] { " " };          // Tableau de travail des ports actuels
        string[] portsNew = new string[] { " " };       // Tableau avec les nouveaux ports
        string[] portsOld = new string[] { " " };       // Tableau avec les anciens ports

        string useName = "Default";                     // Utilisateur par défaut si pas trouvé

        int curentSelectedPort = 0;                     // Le port COM selectionné actuellement
        int counterTimerEndCom = 0;                     // Temps d'attente de réponse

        bool canRetryOpen = true;                       // Peut réessayer un communication
        bool canEndCom = false;                         // Peut mettre fin à une communicaiton

        /// <summary>
        /// Default constructor
        /// </summary>
        public View()
        {
            // Initialisation des composants
            InitializeComponent();
            // Lecture des ports au démarrage 
            canRetryOpen = RefreshSerialPort();
            // Configuration des paramètres de la communicaiton UART
            InitSerialCom();
            // Démarrage du timer1 principal qui fait tourner tout le programme
            timer1.Start();
        }

        /// <summary>
        /// Sets the text to the text label
        /// </summary>
        /// <param name="userInfos"></param>
        internal void SetText(string userInfos)
        {
            // Récupération du nom de la séssion
            useName = userInfos;
            // Affiche le nom de l'étudiant sur la fenêtre
            this.lblUserName.Text = userInfos;

            textBox1.Text = useName; // Préremplit le champ de texte avec le nom de l'utilisateur récupéré
        }

        // Événement lors de la réception de données via la communication UART sur un port COM
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            // Buffeur de lecture des données reçues
            string dataRx = serialPort1.ReadExisting();

            // Si les données reçues sont égales à la clé secrète
            if (dataRx == KEY)
            {
                // Garde le port COM ouvert
                canEndCom = false;
                // Réinitialisation du compteur d'attente de réponse
                counterTimerEndCom = 0;

                // On envoie plus le nom de la session, on attend que l'utilisateur clic sur le bouton pour valider son nom
                //Seul le thread d’interface utilisateur peut mettre à jour les éléments d’interface utilisateur, les autres threads doivent utiliser BeginInvoke pour mettre à jour l'interface utilisateur
                button1.BeginInvoke(new Action(InitialiserBouton));
            }
        }

        private void InitialiserBouton()
        {
            button1.Enabled = !string.IsNullOrEmpty(textBox1.Text); // Activation du bouton si le champ de texte n'est pas vide (la communication est ouverte)
        }

        // Envoi du nom de la session par UART via le port COM
        private void SendMesageCom()
        {
            // Utilisation de StringSplitOptions.RemoveEmptyEntries pour gérer les cas avec plusieurs espaces ou une entrée commençant par des espaces
            string[] nameParts = useName.Split(SpaceSeparator, System.StringSplitOptions.RemoveEmptyEntries);
            string name = nameParts[0].Trim(); // Récupération du premier mot comme prénom, en supprimant les espaces au début et à la fin

            // Si la longueur du prénom dépasse la limite maximale, le prénom est tronqué et un point est ajouté à la fin
            if (name.Length > MAX_NUMBER_OF_LETTERS)
            {
                // Name = name.Substring(0, NEEDED_NUMBER_OF_CHARACTERS - 3) + "...";
                name = name.Substring(0, MAX_NUMBER_OF_LETTERS - 1) + ".";
            }

            // Si le nom est composé de plusieurs mots
            if (nameParts.Length > 1)
            {
                int numberOfAddedSpaces = MAX_FIRSTNAME_LENGTH - name.Length; // Calcul du nombre d'espaces à ajouter pour atteindre la longueur maximale du prénom
                for (int i = 0; i < numberOfAddedSpaces; i++)
                {
                    name = "_" + name; // Ajout des espaces au début du prénom
                }

                name += $" {nameParts[1].Trim()[0]}"; // Si pluseurs mots, première lettre du second mot utilisée comme initiale du nom.
            }
            else  
            {
                switch (name.Length)  // Adapte le code en fonction du nombre de caractère rentrés
                {
                    case 1:
                        name = $"{name}____{name}____{name}"; // Si le prénom a 1 caractère, il est répété trois fois avec des underscores entre les répétitions
                        break;
                    case 2:
                        name = $"{name}___{name}__{name}";  // Si le prénom a 2 caractères, il est répété trois fois avec des underscores entre les répétitions
                        break;
                    case 3:
                        name = $"{name}____{name}_";  // Si le prénom a 3 caractères, il est répété deux fois avec des underscores entre les répétitions
                        break;
                    case 4:
                        name = $"{name}___{name}";  // Si le prénom a 4 caractères, il est répété deux fois avec des underscores entre les répétitions
                        break;
                    default:
                        int numberOfAddedSpaces = MAX_NUMBER_OF_LETTERS - name.Length; // Ajout d'espaces si la longueur du prénom est inférieure à la longueur maximale
                        for (int i = 0; i < numberOfAddedSpaces; i++)
                        {
                            name = "_" + name;
                        }
                        break;
                }
            }

            Console.WriteLine(name);
            name += END_NAM_KEY; // Ajout d'une clé de fin

            // Conversion de tous les accents éventuels en lettres normales
            byte[] tempBytes = System.Text.Encoding.GetEncoding("ISO-8859-8").GetBytes(name);

            // Envoi du nom correctement encodé
            serialPort1.Write(System.Text.Encoding.UTF8.GetString(tempBytes));

            // Arrêt de la communication et fermeture du port COM actuel
            EndTryCom();
        }

        // Récupération des ports COM du système
        private bool RefreshSerialPort()
        {
            // Récupération de la liste des ports COM du système
            portsNew = SerialPort.GetPortNames();

            // Si la nouvelle liste de ports COM est différente de la précédente
            if (!Enumerable.SequenceEqual(portsNew, portsOld))
            {
                // Redimensionnement du tableau des ports COM à la taille du nouveau tableau COM
                Array.Resize<string>(ref ports, portsNew.Length);
                // Copie du tableau des nouveaux ports dans le tableau des ports de travail
                portsNew.CopyTo(ports, 0);
                // Redimensionnement du tableau des anciens ports COM à la taille du tableau de travail COM
                Array.Resize<string>(ref portsOld, ports.Length);
                // Copie du tableau des ports de travail dans le tableau des anciens ports COM
                ports.CopyTo(portsOld, 0);
                // Retourne une réponse vraie
                return true;
            }
            // Sinon, la liste des ports reste la même 
            else
            {
                // Retourne une réponse fausse
                return false;
            }
        }

        // Initialisation des paramètres UART du port COM
        private void InitSerialCom()
        {
            // Configuration du BaudRate
            serialPort1.BaudRate = 9600;
            // Pas de parité
            serialPort1.Parity = Parity.None;
            // Taille du message de 8 bits
            serialPort1.DataBits = 8;
            // Un seul bit de stop
            serialPort1.StopBits = StopBits.One;
            // Pas de Handshake
            serialPort1.Handshake = Handshake.None;

            // Set read timeouts
            serialPort1.ReadTimeout = 500;
            // Set write timeouts
            serialPort1.WriteTimeout = 500;
        }

        // Essaye d'ouvrir un port COM
        private bool TryOpenCom()
        {
            // Test tous les ports COM du tableau de travail des ports COM 
            for (int i = 0; i < ports.Length; i++)
            {
                // Essaye d'ouvrir le port COM actuel 
                try
                {
                    // Récupération du nom du port COM actuel
                    serialPort1.PortName = (string)ports[i];
                    // Ouverture du port COM actuel
                    serialPort1.Open();
                    // Compteur du port COM sélectionné actuellement
                    curentSelectedPort = i;
                    // Retourne une réponse vraie
                    return true;
                }
                catch { } //(System.Exception e)
            }
            // Retourne une réponse fausse
            return false;
        }

        // Commence l'essai de communication avec tous les ports COM
        private void StartTryCom()
        {
            // Si on arrive à ouvrir le port COM et qu'on a le droit d'en ouvrir un 
            if (TryOpenCom() && canRetryOpen)
            {
                // Essaye d'envoyer la clé d'envoi pour s'annoncer au dispositif qui l'attend
                try
                {
                    // Envoi de la clé d'annonce sur le port COM ouvert actuellement
                    serialPort1.Write(ANNOUNCE_KEY);
                }

                catch { } //(System.Exception e)

                // Démarre le délai d'attente de réception de la clé de retour sur le port COM actuel
                canEndCom = true;
                // Arrête d'essayer d'ouvrir un autre port COM
                canRetryOpen = false;
            }
            // Sinon, on n'arrive pas à ouvrir les ports COM et on n'a pas le droit d'en ouvrir
            else
            {
                // Récupération des ports COM du système
                canRetryOpen = RefreshSerialPort();
            }
        }

        // Termine la communication avec le port COM actuel et le ferme
        private void EndTryCom()
        {
            // Ferme le port COM actuel
            serialPort1.Close();
            // Efface le port COM actuel de la liste de travail des ports COM
            ports[curentSelectedPort] = " ";
            // Arrête la fermeture des ports et reprend le code normalement
            canEndCom = false;
            // Peut réessayer d'ouvrir un autre port
            canRetryOpen = true;
        }

        // Événement du Timer1 toutes les 200ms
        private void timer1_Tick(object sender, System.EventArgs e)
        {
            // Si on peut commencer le délai d'attente de la réception de la clé
            if (canEndCom)
            {
                // Si on attend 200ms 
                if (counterTimerEndCom == 1)
                {
                    // Termine la communication avec le port COM actuel et le ferme
                    EndTryCom();
                    // Réinitialisation du compteur d'attente de réponse
                    counterTimerEndCom = 0;
                }
                // Incrémentation du compteur d'attente de réponse
                counterTimerEndCom++;
            }
            // Sinon, on n'a pas une communication en cours et donc on peut essayer d'en commencer une
            else
            {
                // Commence l'essai de communication avec tous les ports COM
                StartTryCom();
            }
        }
        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {
            TextBox textBox = (TextBox)sender;  // On cast l'objet "sender" en TextBox 
            SetText(textBox.Text); // Copie le texte de "textBox" dans le label pour le mettre à jour 

            button1.Enabled = !canRetryOpen && !string.IsNullOrEmpty(textBox.Text); // Bouton de validation cliquable que si le nom n'est pas vide
        }
        private void button1_Click_1(object sender, EventArgs e)
        {
            SendMesageCom(); // Envoie du nom
            textBox1.Enabled = false; // Désactivation du champ de text (la communication est fermée)
            button1.Enabled = false; // Désactivation du bouton (la communication est fermée)
        }
    }
}
