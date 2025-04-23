<p align="center">
  <img src="Minesweeper.gif" alt="Minesweeper Demo" />
</p>

# Minesweeper Editor Tool

This repository contains a test task plugin for Unreal Engine 4.27 that adds a basic **Minesweeper** game as an **Editor Tool**.

## Getting Started

### Prerequisites

Before getting started, ensure you have the following installed on your system:

* Unreal Engine 4.27
* Visual Studio 2019 or later

### Cloning the Repository

Clone this repository using one of the following methods:

**HTTPS:**

```bash
git clone https://github.com/MMadmer/Minesweeper.git
```

**SSH:**

```bash
git clone git@github.com:MMadmer/Minesweeper.git
```

### Plugin Setup

1. Copy the `MinesweeperEditorTool` folder into your Unreal project's `Plugins` directory.
2. Open the `.uproject` file and confirm the plugin is enabled.
3. If needed, regenerate project files by right-clicking the `.uproject` and choosing **Generate Visual Studio project files** .

### Compiling the Plugin

1. Open the generated `.sln` file in Visual Studio.
2. Build the project using `Development Editor` and `Win64` configuration.
3. Launch the Unreal Editor via Visual Studio or by double-clicking the `.uproject`.

## Usage

After the editor opens:

1. Locate the **"Minesweeper"** button in the main toolbar.
2. Click it to open the Minesweeper tool window.
3. Configure the grid:
   * Grid width
   * Grid height
   * Number of bombs
4. Click **Start Game** to generate the field.
5. Interact with tiles:
   * Reveals numbers or triggers Game Over.
   * Recursive reveals on empty tiles.
6. You can restart the game at any time.

## AI Usage

Parts of this plugin were developed with the assistance of ChatGPT **AI tools** to automate routine tasks:

* Boilerplate Slate widget structure
* Refactoring and naming suggestions
* This readme file 😄

All code was reviewed and finalized manually to ensure correctness and maintain Unreal Engine best practices.

## License

This project is licensed under the [MIT License](LICENSE).
