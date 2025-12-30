using ConvMVVM2.WPF.Extensions;
using ConvMVVM2.WPF.Host;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace LLEViewer
{
    public class Starter
    {

        #region Static Functions
        [STAThread]
        public static void Main(string[] args)
        {
            try
            {
                var host = ConvMVVM2Host.CreateHost<BootStrapper, Application>(args, "App");
                host.AddWPFDialogService();
                host.Build();
                host.RunApp("MainWindowView");


            }
            catch(Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex);
            }
        }
        #endregion
    }
}
