# Shootstar
======
This is a Godot Engine top-down-shooter created for Gameloft C++ Optimization Competition.

In `optimization` branch EnemyMedic AI is implemented with QuadTree for more efficient spatial search.

## Prerequisite
- Godot v3.2.3
- Visual Studio 2017

## Playing the Game
`master` branch has a built game under GodotShootstar/build/Shootstar.exe

or

### Build Instruction
To build game in `optimization` branch:

1. Open solution in Visual Studio 2017 or later
2. Build source code in Debug or Release mode
3. Copy compilation output `Shootstar.dll` from `Shootstar/DLLShootstar` to `Shootstar/GodotShootstar`
4. Open folder `Shootstar/GodotShootstar` project in Godot v3.2 or later
5. Go to Project > Export
6. Choose Windows Desktop (Runnable) and insert Export Path
7. Click Export All



