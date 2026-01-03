# Brian’s Brain – A Cellular Automaton

A simple SDL2 + C++ implementation of **Brian’s Brain**, a classic **cellular automaton** where complex patterns emerge from three states: **ON**, **DYING**, and **OFF**.

<p align="center">
  <img src="BriansBrain.gif" alt="Brian's Brain Demo" />
</p>

## Compile

```bash
g++ BriansBrain.cpp -std=c++17 -O2 `sdl2-config --cflags --libs` -o BriansBrain
```

## Run

```bash
# Normal interactive mode
./BriansBrain
```

```bash
# FFmpeg capture mode (GIF)
./BriansBrain ffmpeg | ffmpeg -f rawvideo -pixel_format rgba -video_size 800x600 -framerate 60 -i - BriansBrain.gif
```
