using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer.Models
{
    public class MediaFile
    {

        #region Constructor
        public MediaFile(string path)
        {
            this.FilePath = path;
            this.FileName = Path.GetFileName(path);
        }
        #endregion

        #region Public Property
        public string FileName { get; set; } = "";

        public string FilePath { get; set; } = "";
        #endregion
    }
}
