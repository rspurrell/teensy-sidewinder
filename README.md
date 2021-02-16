# Teensy 3.x/4.x Project Template
A comprehensive Teensy 3.x/4.x project template for Linux, Windows Subsystem for Linux (WSL), and Windows. Inspired by [apmorton/teensy-template](https://github.com/apmorton/teensy-template.git) referencing the Teensy cores from [PaulStoffregen/cores](https://github.com/PaulStoffregen/cores.git). Configurations for VS Code are included, but not required to use this project template.

## Tools Setup
After cloning this project you'll need the appropriate tools for your OS to compile and upload your code to your Teensy. You have two options (for WSL you must install Teensyduino for Windows from option 2 regardless of which tools option you choose):

### Option 1. Subtree the Standalone Teensy Tools from this Repository
Enables your project to compile standalone, but increases your project size. Run these two Git commands from the terminal within your project:
1. `git remote add tools https://github.com/rspurrell/teensy-template.git`
1. Then either:
    - Linux/WSL: `git subtree add --squash --prefix=tools/ tools linux-tools`
    - Windows: `git subtree add --squash --prefix=tools/ tools windows-tools`

### Option 2. Install Arduino and Teensyduino
Consumes a smaller footprint, but has a dependency on having Teensyduino installed to compile.
#### Linux/WSL
1. Download and extract an [Arduino IDE](https://www.arduino.cc/en/software) tarball:
```
wget https://downloads.arduino.cc/arduino-1.8.13-linux64.tar.xz
tar xvf arduino-1.8.13-linux64.tar.xz -C /path/to/parent
```
2. Download and install [Teensyduino](https://www.pjrc.com/teensy/td_download.html):
```
wget https://www.pjrc.com/teensy/td_153/TeensyduinoInstall.linux64
chmod 755 TeensyduinoInstall.linux64
./TeensyduinoInstall.linux64 --dir=/path/to/parent/arduino-1.8.13
```

#### WSL/Windows
1. Install the [Arduino IDE](https://www.arduino.cc/en/software)
1. Install [Teensyduino](https://www.pjrc.com/teensy/td_download.html) for Windows

## Environment Setup
### Linux
1. Install the Teensy udev rule: `sudo cp 49-teensy.rules /etc/udev/rules.d/`
1. Unplug your Teensy and plug it back in

### WSL
Create a convenient shortcut to the Teensy loader executable at `%PROGRAMFILES(x86)%\Arduino\hardware\tools\teensy.exe`. 

<span style="font-size:0.95em">Please note that since there is no USB support in WSL, `make upload` will not work.</span>

### Windows
Install **GNU Make** for Windows. I highly recommended using the Windows package manager, [Chocolately](https://chocolatey.org/). Once installed, simply type `choco install make` on an elevated command line.

## Using the Template
1. Put your code in `src/main.cpp` and its folder
1. Put any libraries you need in `libraries`
1. Set the TEENSY variable in `Makefile` according to your teensy version
	- You may need to update the `TOOLSPATH` variable depending what you choose for your tools setup
1. Build your code `make`. Alternatively, use `make TEENSY=**` where `**` is your Teensy version [`LC`, `30`, `31`, `32`, `35`, `36`, `40`, `41`]
1. Upload your code:
    - Linux/Windows: `make upload` (*does not work with WSL*)
    - WSL: Using Teensy loader, locate and load the compiled hex file located in your WSL project folder. (eg. `\\wsl$\Ubuntu-20.04\home\rspurrell\workspaces\teensy\teensy-template\bin`)

### Using VS Code
1. Install the [Microsoft C/C++ Extensions](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
1. Open this project folder in VS Code
1. Adjust the `toolsPath` and `compilerPath` in `.vscode/c_cpp_properties.json` as necessary
1. Select the configuration matching your Teensy. `Ctrl+Shift+P` and type `Select a Configuration`

## References
- The `cores` sub-folder is a `git subtree` of [PaulStoffregen/cores](https://github.com/PaulStoffregen/cores.git)
- The `linux-tool` and `windows-tools` branches are taken from [Teensyduino](http://www.pjrc.com/teensy/td_download.html) v1.5.3
- The `src/main.cpp` file is a copy of `teensy4/main.cpp`
- The `Makefile` file is a modified copy of `teensy3/Makefile`
- The `49-teensy.rules` file is taken from [PJRC's udev rules](http://www.pjrc.com/teensy/49-teensy.rules)
- Inspiration for this template was appropriated from [apmorton/teensy-template](https://github.com/apmorton/teensy-template.git)
