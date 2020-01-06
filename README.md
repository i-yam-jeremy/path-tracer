# OpenCL Path Tracer

![test scene](readme-images/scene1.png)
![test scene](readme-images/scene2.png)

# Overview
This project is a path tracer using OpenCL for acceleration. A path tracer renders an image by simulating many light paths. Each path behaves according to probability distributions. Every bounce, the light has a chance of being absorbed or reflected. If reflected, the outgoing light ray travels in a direction based on a BRDF. This allows light to be simulated more accurately than other rendering methods and therefore produce more realistic renders. 

# Building
```
cd <path-tracer-repo>
make
```

# Running

1. Run `build/pt --listDevices` to list the OpenCL devices available.
2. Create a scene config file ([instructions here](scene-config-instructions.md)).
3. Run `build/pt sceneConfigFilename outputImageName` where the output image is a PPM file.
