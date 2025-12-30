using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace LLEViewer.Utils
{
    public class ImageHelper
    {

        public static WriteableBitmap CreateFromRawPointer(IntPtr srcBuffer,
                                                           int width,
                                                           int height,
                                                           int srcStride)
        {
            if (srcBuffer == IntPtr.Zero) throw new ArgumentNullException(nameof(srcBuffer));
            if (width <= 0 || height <= 0) throw new ArgumentOutOfRangeException("width/height");
            if (srcStride <= 0) throw new ArgumentOutOfRangeException(nameof(srcStride));

            var wb = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgr24, null);

            int bufferSize = srcStride * height;
            wb.WritePixels(new Int32Rect(0, 0, width, height), srcBuffer, bufferSize, srcStride);

            return wb;
        }

        public static void UpdatePixelsSameSize(WriteableBitmap wb,
                                                IntPtr srcPtr,
                                                int width,
                                                int height,
                                                int srcStride)
        {
            if (wb == null) throw new ArgumentNullException(nameof(wb));
            if (srcPtr == IntPtr.Zero) throw new ArgumentNullException(nameof(srcPtr));

            if (wb.PixelWidth != width || wb.PixelHeight != height)
                throw new ArgumentException("WriteableBitmap size mismatch.");
            if (wb.Format != PixelFormats.Bgr24)
                throw new ArgumentException("PixelFormat mismatch.");

            int bufferSize = srcStride * height;

            wb.WritePixels(sourceRect: new Int32Rect(0, 0, width, height),
                           buffer: srcPtr,
                           bufferSize: bufferSize,
                           stride: srcStride);
        }
    }
}
