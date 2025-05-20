# Compiling
## Requirements:
- VCPKG: https://vcpkg.io/en/
    - Crow: [Windows Install](https://crowcpp.org/master/getting_started/setup/windows/)
    - SQLite: `vcpkg install sqlite`

## Build:
1. Clone the repo: https://github.com/ConnorY97/SimpleForum.git
2. `cd SimpleForum`
3. `mkdir build && cd build`
4. `cmake ..`
5. `cmake --build . && cd Debug && ./SimpleForum.exe`
6. In your browser of choice open `http://localhost:18080/`

## Notes:
To build tests:
- `cmake -S . -B build -DBUILD_TESTS=ON`
    - Run `-DBUILD_TESTS=OFF` again to improve compile time.