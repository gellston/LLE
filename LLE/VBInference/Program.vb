Imports System

Module Program
    Sub Main(args As String())
        Try
            ' Create LLE Instance
            Dim lle = LLEAPI.V1.LLE.Create()

            ' load zerodce model and load on cpu
            ' its also support onnx model load from path
            lle.Setup(LLEAPI.V1.DlType.ZeroDCE, LLEAPI.V1.Device.Cpu)

            ' load color image
            Dim input = LLEAPI.V1.Image.Imread(
                    "C://github//dataset//lol_dataset//our485//low//low_15.png",
                    LLEAPI.V1.ColorType.Color
                )

            ' predict
            Dim output = lle.Predict(input)

            ' save image file on disk
            LLEAPI.V1.Image.Imwrite("C://github//LLE//LLE//x64//Debug//result1.jpg", output)

            ' cleanup internal instance
            lle.Shutdown()

        Catch ex As Exception
            System.Diagnostics.Debug.WriteLine(ex.ToString())
        End Try
    End Sub
End Module
