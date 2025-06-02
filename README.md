# Build
1. Clone the repo: https://github.com/ConnorY97/SimpleForum.git
2. `cd SimpleForum`
3. `mkdir build && cd build`
4. `cmake ..`
5. `cmake --build . && cd Debug && ./SimpleForum.exe`
6. In your browser of choice open `http://localhost:18080/`

## Notes:
### Build tests:
1. In the root of the project
2. run `cmake -S . -B build -DBUILD_TESTS=ON`
    - Run `-DBUILD_TESTS=OFF` again to improve compile time.

### Build and Run App
1. After a commit run the following cmd `git push live master`
2. Input the password
3. It should be live in a moment
4. Confirm logs: `"/home/XXX/Documents/PersonalProjects/SimpleForum/post-receive.log"`