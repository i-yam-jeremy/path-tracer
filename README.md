# OpenCL Path Tracer

![test scene](readme-images/scene1.png)
![test scene](readme-images/scene2.png)

# Overview


# Building
```
cd <path-tracer-repo>
make
```

# Running

1. Run `build/pt --listDevices` to list the OpenCL devices available.
2. Create a scene config file ([instructions here](scene-config-instructions.md)).
3. Run `build/pt sceneConfigFilename outputImageName` where the output image is a PPM file.
