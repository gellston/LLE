<div align="center">

<img src="https://raw.githubusercontent.com/gellston/LLE/main/icon.png" alt="LLE Icon" width="140" />

  # LLE (Low Light Enhancement)

  AI-based Low-Light Enhancement inference API for **Windows x64**.

  [![NuGet](https://img.shields.io/nuget/v/LLE.Native.Cu118.svg?style=for-the-badge&logo=nuget)](https://www.nuget.org/packages/LLE.Native.Cu118)
  ![C++](https://img.shields.io/badge/C%2B%2B-Used-00599C?style=for-the-badge&logo=c%2B%2B)
  ![C++/CLI](https://img.shields.io/badge/C%2B%2B%2FCLI-Used-512BD4?style=for-the-badge)
  ![C%23](https://img.shields.io/badge/C%23-Used-512BD4?style=for-the-badge&logo=csharp)
  ![Python](https://img.shields.io/badge/Python-Model%20Training-3776AB?style=for-the-badge&logo=python)
</div>

---

## Overview

**LLE** is a low-light image enhancement library that provides an **inference API** for an AI model trained in **Python**.

- Name: **LLE (Low Light Enhancement)**
- Author / Maintainer: **gellston**
- Examples:
  * [`CppInference`](https://github.com/gellston/LLE/tree/main/LLE/CppInference)

### Model Support (Now / Next)
- **Current**: Low-light enhancement inference (native runtime)
- **Planned**: Support **improved low-light enhancement models** over time (quality / speed / size trade-offs),
  and expand model options as the project evolves.

> LLE is not a one-off release. Model quality and available variants may improve through updates.
### Training Scripts
- Training scripts used for model development are available here:
  * https://github.com/gellston/LLE/tree/main/python


### Dataset
- This project used the **LOLI-Street** low-light street image dataset:
  * https://www.kaggle.com/datasets/tanvirnwu/loli-street-low-light-image-enhancement-of-street


---

## Platform

- ✅ **Windows x64 only**
  - Even if you use **C#** or **C++**, this library only works on **Windows x64**.

---

## Runtime (CPU / CUDA)

### CPU
- **CPU inference**: no special runtime constraints (beyond standard Windows x64 requirements).

### CUDA (GPU)
- **CUDA inference** requires matching GPU runtimes.
- For `LLE.Native.Cu118`, you must match:
  - **CUDA Toolkit 11.8**
  - **cuDNN 8.5.0.96**

### CPU + CUDA Mixed Usage (Important)
- `LLE.Native.Cu118` can be used in a **mixed mode**:
  - You can run **CPU inference** regardless of CUDA availability.
  - When CUDA/cuDNN is correctly installed, you can also run **CUDA inference**.
  - This enables flexible usage such as **CPU fallback** or **choosing CPU/CUDA per workload**.

> If CUDA inference fails to load (DLL not found / entry point not found), verify your installed **CUDA** and **cuDNN** versions match the package.

#### Downloads (CUDA / cuDNN)
CUDA / cuDNN downloads are provided by NVIDIA. cuDNN downloads may require an NVIDIA Developer account.

```text
CUDA Toolkit 11.8 (Windows x86_64):
https://developer.nvidia.com/cuda-11-8-0-download-archive

CUDA Toolkit Archive (if you need other versions later):
https://developer.nvidia.com/cuda-toolkit-archive

cuDNN Archive (select cuDNN 8.5.0.x, matching CUDA 11.x build):
https://developer.nvidia.com/rdp/cudnn-archive

cuDNN 8.5.0 Release Notes:
https://docs.nvidia.com/deeplearning/cudnn/archives/cudnn-850/release-notes/rel_8.html
```

---

## Development Environment

- **Visual Studio 2026**

---

## Runtime Dependency (Required)

This library requires a separate **redistribution package** to run (native runtime DLLs, etc.).
Download and install the redistribution package before using LLE.

- Microsoft Visual C++ Redistributable (Latest Supported):
  https://learn.microsoft.com/ko-kr/cpp/windows/latest-supported-vc-redist?view=msvc-170

---

## NuGet Packages

LLE is not a “single one-off release”. The NuGet packages can be **updated over time**
(bug fixes, performance improvements, new runtime variants, model upgrades).

Current / planned package list:

- **`LLE.Native.Cu118`** (Windows x64, CPU, CUDA 11.8 + cuDNN 8.5.0.96)  
  https://www.nuget.org/packages/LLE.Native.Cu118
- **(planned)** `LLE.Managed.Cu118` (Windows x64, improved .NET experience)

> The list may expand (e.g., different CUDA versions) and existing packages may receive updates.

---

## Installation

### Package Manager
```powershell
Install-Package LLE.Native.Cu118
```

### .NET CLI
```bash
dotnet add package LLE.Native.Cu118
```

---

## Usage in C++

```cpp
#include <lle/memoryPool.h>
#include <lle/image.h>
#include <lle/lle.h>
#include <iostream>

int main()
{
    try {
        // create lle instance
        auto lle = lleapi::v1::lle::create();

        // load Zero-DCE++ model on CPU
        // (also supports loading an ONNX model from a file path)
        lle->setup(lleapi::v1::dlType::zeroDCE, lleapi::v1::device::cpu);

        // load color image
        auto input = lleapi::v1::image::imread(
            "C://github//dataset//lol_dataset//our485//low//low_15.png",
            lleapi::v1::colorType::color
        );

        // predict
        auto output = lle->predict(input);

        // save result image
        lleapi::v1::image::imwrite(
            "C://github//LLE//LLE//x64//Debug//result1.jpg",
            output
        );

        // cleanup internal instance
        lle->shutdown();
    }
    catch (std::exception ex) {
        std::cout << ex.what() << std::endl;
    }
}
```

---

## Roadmap

- [ ] Add first-class **C# support** (planned)
- [ ] Add additional runtime variants (e.g., different CUDA versions)
- [ ] Improve low-light enhancement model quality and provide more model options/variants

---

## Research References / Acknowledgements

This project uses ideas and/or model architectures from academic research.
If you use **LLE** in research, demos, or publications, please consider **citing the original papers**.

We sincerely thank the authors and contributors of these works for advancing low-light enhancement research:

- **Zero-DCE (CVPR 2020)**  
  Chunle Guo, Chongyi Li, Jichang Guo, Chen Change Loy, Junhui Hou, Sam Kwong, Runmin Cong  
  *Zero-Reference Deep Curve Estimation for Low-Light Image Enhancement*  
  Paper (CVF Open Access): https://openaccess.thecvf.com/content_CVPR_2020/html/Guo_Zero-Reference_Deep_Curve_Estimation_for_Low-Light_Image_Enhancement_CVPR_2020_paper.html  
  Project page: https://li-chongyi.github.io/Proj_Zero-DCE.html

- **Zero-DCE++ (TPAMI 2021/2022)** *(used by this API)*  
  Chongyi Li, Chunle Guo, Chen Change Loy  
  *Learning to Enhance Low-Light Image via Zero-Reference Deep Curve Estimation*  
  arXiv: https://arxiv.org/abs/2103.00860  
  Project page: https://li-chongyi.github.io/Proj_Zero-DCE%2B%2B.html  
  DOI: https://doi.org/10.1109/TPAMI.2021.3063604

> Note: Please also comply with the licenses/terms of any upstream code, weights, and third-party libraries you use or redistribute.

---

## License

MIT License

Copyright (c) 2025–present gellston

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
