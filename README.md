# Ray Tracer Project!
Reed College, CSI221 fall Final Project

## Table of Contents
- [Overview](#overview)
- [Requirements](#requirements)
- [Installation](#installation)
- [Installing Requirements](<#Installing Requirements>)
    - [Mac/IOS](#Mac/IOS)
    - [Linux](#linux)
    - [Windows](#Windows)
- [Useage](#Useage)
    - [Interfacing with the ray tracer](<#Interfacing with the ray tracer>)
    - [Compiling, Testing and cleaning](<#Compiling, Testing and cleaning>)
-  [Contributors](#Contributors)

## Overview
### What is a ray tracer?
idk man but I think it will look nice to have this here

### What about this project in particular
specifics about this project, may delete later if not much to say

## Requirements
<!---1. A working installation of C++-->
2. g++ compiler
    - run c++ up to at least to C++ standard 17
    - other compilers should work, you just may have to compile by hand instead of using the makefile
3. git installed on your terminal
If you dont meet these requirments, Follow the system specific steps in [here](#installing requirements)

## Installation 
> [!WARNING]
> Please make sure you meet base requiremtns before preceeding. (See Installing Requirements section for instructions)

Install using git clone
```
git clone https://github.com/michaeldunnpro/cppraytracer
```

## Installing Requirements 
All installation is done though your systems terminal.
### Mac/IOS 
<!---If you dont already, install command tools: 
```
xcode-select --install
```-->
First install brew as a package manager:
``` 
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
taken from the [homebrew website](https://brew.sh/), follow the instructions as prompted by the terminal. 
git
```
brew install git
```
g++ compiler
```
brew install gcc
```

### linux

*Fedora*:
git
``` 
sudo dnf install git 
```
g++ compiler
``` 
sudo dnf install git 
```

_Arch:_
git
``` 
sudo pacman -s git 
```
g++ compiler
``` 
sudo pacman -s gcc
```

### Windows
WIP



## Useage
WIP

### Compiling, Testing and cleaning
Here are the commands that you can run from the projects makefile

Compiling the test code: 
```
make test
```

Running the test code:
```
make run
```

Checking for leaks:
```
make leaks
```

Cleaning away old executables:
```
make clean
```

### Interfacing with the ray tracer
TBD/WIP

## Contributors
Frank Y, Micheal B, Micheal D, Xanthe N


