using LLEAPI.V1;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer.Services
{
    public class LLEService
    {
        #region Private Property
        private readonly LLEAPI.V1.MemoryPool pool = null;
        private readonly LLEAPI.V1.LLE lle = LLEAPI.V1.LLE.Create();
        #endregion

        #region Constructor
        public LLEService()
        {


           
        }
        #endregion

        #region Public Functions
        public Image Predict(string path)
        {
            try
            {
                return lle.Predict(path);
            }
            catch
            {
                throw;
            }
        }
        public Image Predict(Image input)
        {
            try
            {
                return lle.Predict(input);
            }
            catch
            {
                throw;
            }
        }
        public void Setup(string device)
        {
            try
            {
                switch (device)
                {
                    case "CPU":
                        lle.Setup(DlType.ZeroDCE, Device.Cpu);
                        break;

                    case "CUDA":
                        lle.Setup(DlType.ZeroDCE, Device.Cuda);
                        break;
                }
            }
            catch
            {
                throw;
            }
        }
        public void Shutdown()
        {
            try
            {
                lle.Shutdown();
            }
            catch
            {
                throw;
            }
        }
        #endregion
    }
}
