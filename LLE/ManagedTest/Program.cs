namespace ManagedTest
{
    internal class Program
    {
        static void Main(string[] args)
        {
            try
            {
                //// Create LLE Instance
                var lle = LLEAPI.V1.LLE.Create();
                //// load zerodce model and load on cpu
                //// its also support onnx model load from path
                lle.Setup(LLEAPI.V1.DlType.ZeroDCE, LLEAPI.V1.Device.Cuda);
                //// load color image
                var input = LLEAPI.V1.Image.Imread("C://github//dataset//lol_dataset//our485//low//low_15.png", LLEAPI.V1.ColorType.Color);
                //// predict 
                var output = lle.Predict(input);
                //// save image file on disk
                LLEAPI.V1.Image.Imwrite("C://github//LLE//LLE//x64//Debug//result1.jpg", output);
                //// cleanup internal instance
                lle.Shutdown();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }
    }
}
