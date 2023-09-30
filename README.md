## Setup

- Download MSYS2

- Inside MSYS2 run these commands:

```shell
pacman -Syuu
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-glm
pacman -S mingw-w64-x86_64-glew
pacman -S mingw-w64-x86_64-glfw
pacman -S mingw-w64-x86_64-stb
pacman -S mingw-w64-x86_64-opencv
```

- Open Settings -> Build, Execution, Deployment -> Toolchains
- Add new, and set compiler folder C:\msys64\mingw64 (or wherever you downloaded)
- Try to build