using CommunityToolkit.Mvvm.Input;
using ConvMVVM2.Core.MVVM;
using LLEViewer.Windows;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer.ViewModels
{
    public partial class MainViewModel : ViewModelBase
    {

        #region Private Property
        private readonly IRegionManager regionManager;
        #endregion

        #region Constructor
        public MainViewModel(IRegionManager regionManager)
        {
            this.regionManager = regionManager;
        }
        #endregion


        #region Public Property

        #endregion

        #region Command
        [RelayCommand]
        private void HomeView()
        {
            try
            {
                this.regionManager.Navigate("ContentView", "HomeView");
            }
            catch (Exception ex)
            {

            }
        }

        [RelayCommand]
        private void ImageView()
        {
            try
            {
                this.regionManager.Navigate("ContentView", "ImageView");
            }
            catch (Exception ex)
            {

            }
        }


        [RelayCommand]
        private void VideoView()
        {
            try
            {
                this.regionManager.Navigate("ContentView", "VideoView");
            }
            catch (Exception ex)
            {

            }
        }



        [RelayCommand]
        private void CameraView()
        {
            try
            {
                this.regionManager.Navigate("ContentView", "CameraView");
            }
            catch (Exception ex)
            {

            }
        }
        #endregion
    }
}
