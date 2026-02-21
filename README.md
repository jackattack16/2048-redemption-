# untitled

A small C++ project using **SFML 3.0** and **CMake**. The code demonstrates basic rendering, custom shapes and simple animation effects.

---

## 📋 Table of Contents

1. [Prerequisites](#prerequisites)
2. [Platform‑specific setup](#platform-specific-setup)
   - [Linux](#linux)
   - [macOS](#macos)
   - [Windows](#windows)
3. [Build the project](#build-the-project)
4. [Run the executable](#run-the-executable)
5. [Common issues & troubleshooting](#common-issues--troubleshooting)
6. [License](#license)

---

## Prerequisites

- **C++ compiler** with C++20 support (e.g., GCC ≥ 10, Clang ≥ 12, MSVC ≥ 19.30)
- **CMake** ≥ 3.20
- **SFML 3.0** (static linking is enabled in the CMakeLists.txt)
- Git (to clone the repository)

The project is configured to copy the bundled font (`Blue Winter.ttf`) into the build directory automatically.

---

## Platform‑specific setup

### Linux

```bash
# Install build tools and SFML (Debian/Ubuntu example)
sudo apt update
sudo apt install -y build-essential cmake libsfml-dev

# Clone the repository
git clone https://github.com/jackattack16/2048-redemption-.git
cd untitled
```

> **Note**: If your distribution provides an older SFML version, download the latest 3.0 binaries from the official site and point CMake to them:
>
> ```bash
> wget https://www.sfml-dev.org/files/SFML-3.0.0-linux-gcc-13.2.0.tar.gz
> tar xf SFML-3.0.0-linux-gcc-13.2.0.tar.gz
> export SFML_ROOT=$(pwd)/SFML-3.0.0
> ```
>
> The `SFML_ROOT` environment variable will be picked up by `find_package`.

### macOS

```bash
# Install Homebrew if you don't have it
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake sfml

# Clone the repository
git clone https://github.com/jackattack16/2048-redemption-.git
cd untitled
```

If you prefer a static build, you may need to rebuild SFML from source with `-DBUILD_SHARED_LIBS=OFF` and then set `SFML_ROOT` to the build directory.

### Windows

1. **Install a C++ toolchain** – Visual Studio 2022 (Desktop development with C++) or MSYS2 + MinGW.
2. **Install CMake** – download the installer from https://cmake.org/download/ and add it to your `PATH`.
3. **Obtain SFML 3.0**
   - **Option A – vcpkg** (recommended)
     ```powershell
     git clone https://github.com/microsoft/vcpkg.git
     .\vcpkg\bootstrap-vcpkg.bat
     .\vcpkg\vcpkg install sfml:x64-windows-static
     ```
     Then add the vcpkg toolchain file when configuring CMake:
     ```powershell
     cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
     ```
   - **Option B – pre‑compiled binaries**
     Download the Windows static binaries from https://www.sfml-dev.org/download/sfml/3.0/ and extract them. Set the environment variable `SFML_ROOT` to the extracted folder before running CMake.
4. **Clone the repository**
   ```powershell
   git clone https://github.com/jackattack16/2048-redemption-.git
   cd untitled
   ```

---

## Build the project

From the repository root (the same directory that contains `CMakeLists.txt`):

```bash
# Create a build directory (out‑of‑source build is recommended)
cmake -B build -S .

# Build (you can use --parallel to speed up)
cmake --build build --config Release -- -j$(nproc)   # Linux/macOS
# On Windows with Visual Studio generator:
# cmake --build build --config Release
```

The executable will be placed in `build/` (or `build/Release/` on Windows). The CMake script already copies `Blue Winter.ttf` into the binary directory, so the program can locate the font at runtime.

---

## Run the executable

```bash
./build/untitled   # Linux/macOS
# Windows (Release configuration)
build\Release\untitled.exe
```

If you encounter a missing DLL error on Windows, make sure the SFML static libraries were linked (the project sets `SFML_STATIC_LIBRARIES TRUE`). When using the dynamic version, copy the required `*.dll` files from the SFML `bin` folder next to the executable.

---

## Common issues & troubleshooting

| Symptom | Likely cause | Fix |
|---------|--------------|-----|
| `SFML::Graphics not found` | CMake cannot locate SFML | Set `SFML_ROOT` to the directory containing `SFMLConfig.cmake` or install the dev package (`libsfml-dev` on Linux, `brew install sfml` on macOS, vcpkg on Windows). |
| Linker errors about `sfml-graphics-s` | Static linking flag missing | Ensure `set(SFML_STATIC_LIBRARIES TRUE)` is present (it already is) and that you link against the static libraries (`sfml-graphics-s`, `sfml-window-s`, `sfml-system-s`). |
| Font not displayed | Font file not copied | Verify that `Blue Winter.ttf` exists in the build directory (CMake copies it). If you moved the binary, copy the font alongside it. |
| `CMake Error: Generator ... not known` | CMake version too old | Upgrade CMake to ≥ 3.20 (the project requires C++20). |

---

## License

This project is licensed under the **MIT License** – see the `LICENSE` file for details.
