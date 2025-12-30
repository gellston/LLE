
using ConvMVVM2.Core.Attributes;
using ConvMVVM2.Core.MVVM;
using LLEViewer.Models;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer.ViewModels
{
    public partial class ImageViewModel : ViewModelBase
    {

        #region Private Property
        private readonly IDialogService dialogService;
        #endregion

        #region Constructor
        public ImageViewModel(IDialogService dialogService)
        {
            this.dialogService = dialogService;
        }
        #endregion

        #region Collection

        [Property]
        private BatchObservableCollection<MediaFile> _ImageFileCollection = new BatchObservableCollection<MediaFile>();
        #endregion


        #region Public Property
        [Property]
        private MediaFile _SelectedMediaFile = null;
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
            try
            {
       

            }catch(Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }
        #endregion
    }
}
