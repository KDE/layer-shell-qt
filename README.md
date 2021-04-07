# LayerShellQt

This component is meant for applications to be able to easily use clients based on wlr-layer-shell.

Here you can read about what the protocol does and how shells work:
https://drewdevault.com/2018/07/29/Wayland-shells.html

Report issues in this component here:
https://bugs.kde.org/enter_bug.cgi?product=layer-shell-qt

## Usage

### CMake

To use it from a CMake project you'll need to:
```
find_package(LayerShellQt REQUIRED)
```

Then it will offer us the target `LayerShellQt::Interface` that we can link to to get access to this framework.

### C++

You can use `LayerShellQt::Shell::useLayerShell();` to enable it before any clients are created.

The class `LayerShellQt::Window` will give us access to surface-specific settings. We can get it by using `LayerShellQt::Window::get(window)` on whatever `QWindow` we need to tweak.
