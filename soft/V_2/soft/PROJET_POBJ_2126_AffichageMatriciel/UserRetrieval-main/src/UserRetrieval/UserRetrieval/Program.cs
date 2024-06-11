using System;
using System.Windows.Forms;
using UserRetrieval.Buisness;
using UserRetrieval.Controllers;
/// ETML
/// Author      : Santiago Sugrañes
/// Date        : 10.01.2022
/// Description : Main program entry

namespace UserRetrieval
{
    static class Program
    {
        /// <summary>
        /// Point d'entrée principal de l'application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            // LOGGER : Application Log path
            Logger.AppLogPath = "./appLog.txt";
            Logger.UserLogPath = "./users.txt";

            // MVC : Controller instance
            Controller ctrler = new Controller();

            // MVC : App run main view
            ctrler.RunMainView();
        }
    }
}
