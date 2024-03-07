
<a href="https://github.com/joba14/rl78emu"><img src="./logo.svg" alt="Logo" width="400"></a><br>
![License](https://img.shields.io/badge/license-RL78EMU_GPLv1-brightgreen.svg?style=for-the-badge)
![Languages](https://img.shields.io/badge/languages-C-brightgreen.svg?style=for-the-badge)
![Platforms](https://img.shields.io/badge/platforms-Linux-brightgreen.svg?style=for-the-badge)
<br>


# RL78EMU
**The RL78 Emulator**<br>[Report a bug](https://github.com/joba14/rl78emu/issues/new) · [Request a feature](https://github.com/joba14/rl78emu/issues/new)


## Table of Contents
- [Warning](#warning)
- [Overview](#overview)
	- [Features](#features)
- [Getting Started](#getting-started)
	- [Cloning the Project](#cloning-the-project)
	- [Building the Project](#building-the-project)
	- [Generating the Documentation](#generating-the-documentation)
- [Contributing](#contributing)
- [License](#license)


## Warning
⚠️ **Caution: Experimental and Early Development** ⚠️

This emulator is experimental and in early development. It should be used with caution as it may contain bugs, untested features, and incomplete functionality. It is not suitable for production use and is intended for testing, research, learning, and development purposes only.

[(to the top)](#rl78emu)


## Overview
The RL78EMU is an emulator for the Renesas RL78 series of microcontrollers. It aims to provide an accurate simulation of RL78 microcontrollers, allowing developers to test their firmware and software without the need for physical hardware.

### Features
- Emulation of RL78 CPU (Core 1, Core 2, Core 3).
- Support for various RL78 peripherals, including timers, UART, SPI, I2C, ADC, and more.
- CLI (Command-Line Interface) for easy interaction and debugging.
- GUI (Graphical User Interface) for a user-friendly emulation experience (planned feature).
- Unix/Posix-platform support.

[(to the top)](#rl78emu)


## Getting Started

### Cloning the Project
To clone the project repository, use the following command:
```sh
> git clone https://github.com/joba14/rl78emu.git <dir-to-clone-to>
```

### Building the Project
Before building the project, ensure you have the necessary dependencies installed. This project requires the following:
- autoconf
- automake

Once you have the dependencies installed, follow these steps to build the project:
```sh
> cd <root-of-the-repo>    # Enter the root of the repository.
> chmod +x ./autogen.sh    # Set executable permission for the ./autogen.sh script.
> cd ./scripts             # Enter the ./scripts sub-directory.
> chmod +x ./*.sh          # Set executable permission for all shell scripts in scripts dir.
> ./rebuild.sh             # Rebuild the project.
> ./check.sh               # Run the unit tests.
```

### Generating the Documentation
Before generating the documentation, ensure you have the necessary dependencies installed. This project requires the following:
- doxygen

Once you have the dependencies installed, follow these steps to generate the documentation:
```sh
> cd <root-of-the-repo>    # Enter the root of the repository.
> doxygen ./doxyfile.sh    # Run the doxygen with doxyfile.sh configuration.
```

[(to the top)](#rl78emu)


## Contributing
At this time, I am not actively seeking contributions to the RL78EMU project. I appreciate your interest and enthusiasm for contributing to the project.

The reason for not actively seeking contributions is that I currently do not have well-defined ground rules and guidelines in place for contributors. I want to ensure that the contribution process is clear, fair, and productive for everyone involved.

However, I am always open to feedback, bug reports, and feature requests. If you encounter issues with the project or have ideas for improvements, please feel free to report in this project's repo issues page.

I value your interest in the project, and I may consider establishing contribution guidelines in the future. Until then, thank you for your understanding and support.

[(to the top)](#rl78emu)


## License
The RL78EMU project is released under the **RL78EMU GPLv1** license. Users and contributors are required to review and comply with the license terms specified in the [license.md file](./license.md). The license outlines the permitted usage, distribution, and intellectual property rights associated with the RL78EMU project.

Please refer to the [license.md file](./license.md) for more details. By using, modifying, or distributing the RL78EMU project, you agree to be bound by the terms and conditions of the license.

[(to the top)](#rl78emu)
