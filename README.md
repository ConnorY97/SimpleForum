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

### Committing without changes
1. Run `git commit --allow-empty -m "Your commit message here"`

### Reading website logs
1. run `tmux attach -t simpleforum`

### Set up virtual environment to render markdown
1. `python -m venv venv`
2. `venv\Scripts\activate`
3. `pip install beautifulsoup4 lxml markdown`
4. Windows = `./render.ps1`, Linux = `./render_markdown.py`