# Scene Config File Specification

The file is a list of objects.

* `object <string objPath> <float emissiveness> <float3 emissionColor> <float metalness> <float3 baseColor>`  
    * `objPath` is the path to the OBJ model.
    * `emissiveness` defines the probability the material will emit light. 
    * `emissionColor` is the color of the emitted light (3 floats separated by spaces). 
    * `metalness` defines the metalness of the material (use 0.0 or 1.0 only, intermediate values aren't realistic). 
    * `baseColor` is the albedo of the material (3 floats separated by spaces). 
    
Example scene configs can be found [here](scenes). 
