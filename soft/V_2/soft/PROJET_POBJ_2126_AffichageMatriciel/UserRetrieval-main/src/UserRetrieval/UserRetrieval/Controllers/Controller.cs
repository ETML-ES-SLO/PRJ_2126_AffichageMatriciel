/// ETML
/// Author      : Santiago Sugrañes
/// Date        : 10.01.2022
/// Description : Application controller, gets the data from model and sets it to the view

using System;
using System.Windows.Forms;
using UserRetrieval.Buisness;
using UserRetrieval.Models;
using View = UserRetrieval.Views.View;

namespace UserRetrieval.Controllers
{
    /// <summary>
    /// Controller Class
    /// </summary>
    class Controller
    {
        /// <summary>
        /// Attrivutes
        /// </summary>
        private readonly View _view = new View();
        private readonly Model _model = new Model();

        /// <summary>
        /// Default constructor
        /// </summary>
        public Controller()
        {
            this._view.Ctrler = this;
            SetViewText();
        }

        /// <summary>
        /// Runs the main view
        /// </summary>
        internal void RunMainView()
        {
            Application.Run(this._view);
        }

        /// <summary>
        /// Thets the View text to the user Name and Surname
        /// </summary>
        private void SetViewText()
        {
            string userInfos = this._model.GetUserInfos();



            // If user infos could not be retrieved, log it and exit the program
            if (userInfos == null || userInfos == "")
            {
                Logger.Log(Logger.LogLevel.WARNING, "User infos could not be retrieved");
                Environment.Exit(0);
            }
            else
            {
                this._view.SetText(userInfos);
                Logger.Log(Logger.LogLevel.INFO, $"{userInfos} connected successfully");
                Logger.AppendToUsers(userInfos);
            }
        } 
    }
}
