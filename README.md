<h1 align="center">Simplified Abalone Game</h1>
<p align="center">A simplified version of the abalone boardgame with a GUI and an AI to play against (Linux environment)</p>
<p align="center">This was a 9-person group project coded in less than two weeks</p>
<br>

## Installing gcc, gtk3 and make
<p align="left">You need to install gcc to compile the c source files</p>
<p align="left">You need to install gtk3 to compile the gui source files</p>
<p align="left">You need to install make to use the makefile so it compiles everything using gcc and gtk3</p>

```bash
#gcc
sudo apt install gcc # Linux
brew install gcc # macOS

#gtk3
sudo apt install libgtk-3-dev # Linux
brew install gtk+3 # macOS

#make
sudo apt install make # Linux
brew install make # macOS

# doxygen (optional, used to generate the documentation)
sudo apt install doxygen # Linux
brew install doxygen # macOS
```
## MakeFile options

```bash
make # Compiles all the files and makes a main executable called " abalone ", then deletes all the useless compiled files
make clean # Deletes all the executable files, if there are any (" abalone " file included)
make docs # Creates a doxygen folder using the doxygen config file
make tests # Used for testing purposes
```

## Compile and start the game

```bash
make # To compile the game (if not already done)
./abalone -l noir # Play against the AI (you start)
./abalone -l blanc # Play against the AI (AI starts)
./abalone -pvp # Play against another player locally (black starts)
./abalone -s <port> # Host a server on the given port number (no GUI)
./abalone -c <IP_address>:<port> # Connect to a server with its port and IP address (no GUI)
```
