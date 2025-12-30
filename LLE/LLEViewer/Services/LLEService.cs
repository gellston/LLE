using LLEAPI.V1;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer.Services
{
    public class LLEService : ILLEService
    {
        #region Private Property
        private readonly LLEAPI.V1.MemoryPool pool = LLEAPI.V1.MemoryPool.Create();
        private readonly LLEAPI.V1.LLE lle = null;
        #endregion

        #region Constructor
        public LLEService()
        {


            this.lle = LLEAPI.V1.LLE.Create(this.pool);
           
        }
        #endregion

        #region Event 
        public event Action OnSetup;
        public event Action OnShutdown;
        #endregion

        #region Public Functions
        public Image Predict(string path)
        {
            try
            {

                lock (this.lle)
                {
                    return lle.Predict(path);
                }

   
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
                lock (this.lle)
                {
                    return lle.Predict(input);
                }


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
                lock (this.lle)
                {
                    switch (device)
                    {
                        case "CPU":
                            lle.Setup(DlType.ZeroDCE, Device.Cpu);
                            this.OnSetup();
                            break;

                        case "CUDA":
                            lle.Setup(DlType.ZeroDCE, Device.Cuda);
                            this.OnSetup();
                            break;
                    }
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
                lock (this.lle)
                {
                    lle.Shutdown();
                    this.OnShutdown();
                }
            }
            catch
            {
                throw;
            }
        }
        #endregion


    }
}
