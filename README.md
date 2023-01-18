<h1 align="center">Simplified Abalone Game</h1>
<p align="center">A simplified version of the abalone boardgame with a GUI and an AI to play against (Linux environment)</p>
<p align="center">This was a 9-person group project coded in less than two weeks</p>
<br>

## Installing GTK3
<p align="left">You need to install GTK3 to compile the source files</p>

```bash
apt install libgtk-3-dev # Linux
brew install gtk+3 # macOS
```
## MakeFile options

```bash
make # Compiles all the files and makes an executable called " abalone "
make clean # Deletes all the executable files
make docs # Creates a doxygen folder using the doxygen config file
make tests # Used for testing purposes
```

## Compile and start the game

```bash
make # To compile the game (if not already done)
./abalone -l noir # Play against the AI (you start)
./abalone -l blan # Play against the AI (AI starts)
./abalone -s <port> # Host a server on the given port number (no GUI)
./abalone -c <IP_address>:<port> # Connect to a server with its port and IP address (no GUI)
```
