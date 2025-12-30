
using ConvMVVM2.Core.Attributes;
using ConvMVVM2.Core.MVVM;
using LLEViewer.Services;
using LLEViewer.Windows;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer.ViewModels
{
    public partial class MainViewModel : ViewModelBase
    {

        #region Private Property
        private readonly IRegionManager regionManager;
        private readonly ILLEService lleService;
        #endregion

        #region Constructor
        public MainViewModel(IRegionManager regionManager,
                             ILLEService lleService)
        {
            this.regionManager = regionManager;
            this.lleService = lleService;

            this.lleService.OnShutdown += LleService_OnShutdown;
            this.lleService.OnSetup += LleService_OnSetup;
        }


        #endregion




        #region Public Property
        [Property]
        private string _SelectedDevice = "CPU";

        [Property]
        private bool _IsInitialized = false;
        #endregion

        #region Collection
        [Property]
        private ObservableCollection<string> _DeviceCollection = new ObservableCollection<string>()
        {
            "CPU", "CUDA"
        };
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

        [RelayCommand]
        private void Setup()
        {
            try
            {
                this.lleService.Setup(this.SelectedDevice);

            }catch(Exception ex)
            {

            }
        }

        [RelayCommand]
        private void Shutdown()
        {
            try
            {

                this.lleService.Shutdown();

            }
            catch (Exception ex)
            {

            }
        }
        #endregion

        #region Event Handler
        private void LleService_OnSetup()
        {

            this.IsInitialized = true;
        }

        private void LleService_OnShutdown()
        {
            this.IsInitialized = false;
        }
        #endregion
    }
}
