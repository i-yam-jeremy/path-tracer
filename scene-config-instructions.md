# Scene Config File Specification

The file is a list of commands. The commands are as follows.
Lines that don't match any of these commands are ignored.  

* `spp <int samplesPerPixel>`, sets the samples per pixel for the path tracer
* `renderSize <int width> <int height>`, sets the dimensions of the output rendered image
* `device <int platformIndex> <int deviceIndex>`, specifies the OpenCL device obtained from `pt --listDevices`
* `object <string objPath> <float emissiveness> <float3 emissionColor> <float metalness> <float3 baseColor>`  
    * `objPath` is the path to the OBJ model.
    * `emissiveness` defines the probability the material will emit light. 
    * `emissionColor` is the color of the emitted light (3 floats separated by spaces). 
    * `metalness` defines the metalness of the material (use 0.0 or 1.0 only, intermediate values aren't realistic). 
    * `baseColor` is the albedo of the material (3 floats separated by spaces). 
    
Example scene configs can be found [here](scenes). 
