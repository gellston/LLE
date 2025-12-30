
using ConvMVVM2.Core.Attributes;
using ConvMVVM2.Core.MVVM;
using LLEViewer.Models;
using LLEViewer.Services;
using LLEViewer.Utils;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace LLEViewer.ViewModels
{
    public partial class ImageViewModel : ViewModelBase
    {

        #region Private Property
        private readonly IDialogService dialogService;
        private readonly ILLEService lleService;
        #endregion

        #region Constructor
        public ImageViewModel(IDialogService dialogService,
                              ILLEService lleService)
        {
            this.dialogService = dialogService;
            this.lleService = lleService;
        }
        #endregion

        #region Collection

        [Property]
        private BatchObservableCollection<MediaFile> _ImageFileCollection = new BatchObservableCollection<MediaFile>();
        #endregion


        #region Public Property
        [Property]
        private MediaFile _SelectedMediaFile = null;


        [Property]
        private WriteableBitmap _OriginalImage = null;

        [Property]
        private WriteableBitmap _EnhancedImage = null;
        #endregion

        #region Command

        [RelayCommand]
        private void OpenFolder()
        {

            try
            {

                var files = this.dialogService.OpenFileDialog("", "Open Folder", "JPEG (*.jpg)|*.jpg|PNG (*.png)|*.png");
                var models = files.Select(file => new MediaFile(file));

                this.ImageFileCollection.Clear();
                this.ImageFileCollection.AddRange(models);
            }
            catch (Exception ex)
            {

                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }


        [RelayCommand]
        private void ClearImages()
        {
            try
            {
                this.ImageFileCollection.Clear();
            }
            catch (Exception ex)
            {

                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }
        #endregion

        #region Partial Callback

        partial void OnSelectedMediaFileChanged(MediaFile value)
        {
            if (value == null) return;

            try
            {


                using var inputImage = LLEAPI.V1.Image.Imread(value.FilePath, LLEAPI.V1.ColorType.Color);

                if(this.OriginalImage == null ||
                   this.OriginalImage.Width != inputImage.Width ||
                   this.OriginalImage.Height != inputImage.Height)
                {
                    this.OriginalImage = ImageHelper.CreateFromRawPointer(inputImage.Data,
                                                                          (int)inputImage.Width,
                                                                          (int)inputImage.Height,
                                                                          (int)inputImage.Stride);
                }
                else
                {
                    ImageHelper.UpdatePixelsSameSize(this.OriginalImage,
                                                     inputImage.Data,
                                                     (int)inputImage.Width,
                                                     (int)inputImage.Height,
                                                     (int)inputImage.Stride);
                }



                using var enhancedImage = this.lleService.Predict(inputImage);

                if (this.EnhancedImage == null ||
                    this.EnhancedImage.Width != enhancedImage.Width ||
                    this.EnhancedImage.Height != enhancedImage.Height)
                {
                    this.EnhancedImage = ImageHelper.CreateFromRawPointer(enhancedImage.Data,
                                                                          (int)enhancedImage.Width,
                                                                          (int)enhancedImage.Height,
                                                                          (int)enhancedImage.Stride);
                }
                else
                {
                    ImageHelper.UpdatePixelsSameSize(this.EnhancedImage,
                                                     enhancedImage.Data,
                                                     (int)enhancedImage.Width,
                                                     (int)enhancedImage.Height,
                                                     (int)enhancedImage.Stride);
                }


            }
            catch(Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }
        #endregion
    }
}
