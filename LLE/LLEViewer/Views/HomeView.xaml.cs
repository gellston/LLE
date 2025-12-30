using Microsoft.Web.WebView2.Core;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

namespace LLEViewer.Views
{
    public partial class HomeView : UserControl
    {
        // ✅ 허용할 호스트(virtual host 쓸 때 or 외부 허용하고 싶을 때)
        private readonly HashSet<string> _allowedHosts = new(StringComparer.OrdinalIgnoreCase)
        {
            "appassets.local",       // virtual host를 쓰면 여기에
            // "docs.mycompany.com",  // 외부 문서 허용하려면 추가
        };

        // ✅ (선택) 허용할 경로 prefix (같은 host라도 이 경로만 허용)
        private readonly string[] _allowedPathPrefixes =
        {
            "/readme.html",
            "/docs/",
        };

        private bool _firstNavigationAllowed = false;

        public HomeView()
        {
            InitializeComponent();
        }

        private async void WebView2_Loaded(object sender, RoutedEventArgs e)
        {
            await Web.EnsureCoreWebView2Async();

            // ✅ 먼저 이벤트부터 걸어두는 게 안전
            Web.CoreWebView2.NewWindowRequested += (s, ev) =>
            {
                ev.Handled = true; // 새 창 완전 차단
            };

            Web.CoreWebView2.NavigationStarting += (s, ev) =>
            {
                // 1) 첫 로드는 무조건 허용(문서가 안 보이는 문제 방지)
                if (!_firstNavigationAllowed)
                {
                    _firstNavigationAllowed = true;
                    return;
                }

                // 2) 사용자 클릭에 의한 이동만 통제
                if (!ev.IsUserInitiated) return;

                if (!Uri.TryCreate(ev.Uri, UriKind.Absolute, out var uri))
                {
                    ev.Cancel = true;
                    return;
                }

                // 3) ✅ file:// 로드라면: 같은 폴더(임시폴더) 안에서만 허용
                if (uri.IsFile)
                {
                    // 허용할 루트 폴더: 임시폴더 MyAppDocs
                    var allowedDir = Path.Combine(Path.GetTempPath(), "MyAppDocs") + Path.DirectorySeparatorChar;
                    var targetPath = uri.LocalPath;

                    // 같은 폴더 하위 파일만 허용(외부 파일로 점프 차단)
                    if (!targetPath.StartsWith(allowedDir, StringComparison.OrdinalIgnoreCase))
                        ev.Cancel = true;

                    return;
                }

                // 4) ✅ https/http 로드라면: host(+path) 화이트리스트
                if (uri.Scheme == "http" || uri.Scheme == "https")
                {
                    var hostAllowed = _allowedHosts.Contains(uri.Host);

                    // path 정책이 필요 없으면 아래 allowedByPath를 true로 두면 됨
                    var allowedByPath = _allowedPathPrefixes.Any(p =>
                        uri.AbsolutePath.StartsWith(p, StringComparison.OrdinalIgnoreCase));

                    if (!(hostAllowed && allowedByPath))
                        ev.Cancel = true;

                    return;
                }

                // 5) 그 외 스킴(mailto:, javascript: 등)은 전부 차단
                ev.Cancel = true;
            };

            // --- 여기부터 문서 로드 ---

            // 리소스 HTML을 임시 폴더로 풀기
            var uriPack = new Uri("pack://application:,,,/Resources/readme.html");
            var sri = Application.GetResourceStream(uriPack);
            if (sri?.Stream == null) return;

            var dir = Path.Combine(Path.GetTempPath(), "MyAppDocs");
            Directory.CreateDirectory(dir);

            var htmlPath = Path.Combine(dir, "readme.html");
            using (var fs = File.Create(htmlPath))
                sri.Stream.CopyTo(fs);

            // ✅ 파일로 로드
            Web.Source = new Uri(htmlPath);
        }
    }
}