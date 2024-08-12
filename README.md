# nullkb
Null movement for any game

A and D keys for strafing movement, W and S possible as well.

While A is active. If D is activated, then A must be deactivated.
While <Key> is active. If <Pair> is activated, then <Key> must be deactivated.

## How to use
Clone the repo wherever you want.

### Windows
Run the .exe provided by clicking on it, or typing and entering `./nullkb.exe` into command prompt or powershell. You will have to `cd` your way to the directory that you downloaded it to.

You can also compile the .c file yourself if you know how.

```
gcc nullkb.c -o nullkb.exe -luser32
```

### MacOS
```
./nullkb
Failed to create event tap
```
iTerm wants to control your computer keyboard through accessibility Services. We will not go down this path yet.

### Unix/Linux

Work in progress.

