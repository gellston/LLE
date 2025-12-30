using LLEAPI.V1;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer.Services
{
    public interface ILLEService
    {
        #region Public Functions
        public Image Predict(string path);
        public Image Predict(Image input);
        public void Setup(string device);
        public void Shutdown();
        #endregion

        #region Event 
        public event Action OnSetup;
        public event Action OnShutdown;
        #endregion
    }
}
