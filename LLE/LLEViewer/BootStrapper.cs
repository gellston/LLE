using ConvMVVM2.Core.MVVM;
using LLEViewer.Services;
using LLEViewer.ViewModels;
using LLEViewer.Views;
using LLEViewer.Windows;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;

namespace LLEViewer
{
    public class BootStrapper : AppBootstrapper
    {
        protected override void OnStartUp(IServiceContainer container)
        {
            var localizeService = container.GetService<ILocalizeService>();
            localizeService.SetResourceManager(LLEViewer.Properties.Resource.ResourceManager);
        }

        protected override void RegionMapping(IRegionManager layerManager)
        {
            layerManager.Mapping<MainView>("MainView");
            layerManager.Mapping<HomeView>("ContentView");
        }

        protected override void RegisterModules()
        {

        }

        protected override void RegisterServices(IServiceCollection serviceCollection)
        {

            //Windows
            serviceCollection.AddSingleton<MainWindowView>();

            //Views
            serviceCollection.AddSingleton<MainView>();
            serviceCollection.AddSingleton<CameraView>();
            serviceCollection.AddSingleton<ImageView>();
            serviceCollection.AddSingleton<VideoView>();
            serviceCollection.AddSingleton<HomeView>();

            //ViewModels
            serviceCollection.AddSingleton<MainViewModel>();
            serviceCollection.AddSingleton<CameraViewModel>();
            serviceCollection.AddSingleton<ImageViewModel>();
            serviceCollection.AddSingleton<VideoViewModel>();



            //Service
            serviceCollection.AddSingleton<ILLEService, LLEService>();
        }

        protected override void ViewModelMapping(IViewModelMapper viewModelMapper)
        {

        }
    }
}
