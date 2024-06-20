/// ETML
/// Author      : Santiago Sugrañes
/// Date        : 10.01.2022
/// Description : Application model, retrieves all the data for the app

using System.DirectoryServices.AccountManagement;

namespace UserRetrieval.Models
{
    class Model
    {
        /// <summary>
        /// Default constructor
        /// </summary>
        public Model()
        {

        }

        /// <summary>
        /// Gets the user Name and Surname
        /// </summary>
        /// <returns>String containing Name and Surname</returns>
        internal string GetUserInfos()
        {
            string name = UserPrincipal.Current.DisplayName;
            if(string.IsNullOrEmpty(name)) name = UserPrincipal.Current.Name;
            return name;
        }
    }
}
