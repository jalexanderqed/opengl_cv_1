John Alexander
8327934

As with homework 1, I used OpenGL version 3.3 for this lab and, as a result, had to build my own lighting and geometric shapes. I used learnopengl.com again for the lighting portion of this, but most of the rest of the code was carried over from homework 1. Before beginning on this homework, I reconfigured my homework 1 to precisely track the checkerboard without any custom tuning. This included custom-building the project matrix, which I reused in this homework.

The only major way in which I deviated from the instructions was using a theshold value of 5 for the optical-flow based detection (as opposed to 10, recommended in the homework). The boxes are thresholded to turn on when 500 activated (white) pixels are present, and they have a two-frame delay before they de-activate, which prevents them from being activated multiple times in a single touch. This is handled by the custom "SelectBox" class.

Rotation of the objects around my head is handled exclusively using matrix transforms, specifically a translation transform followed by a rotation transform. Lighting in the scene is arranged so that it appears it is coming from a consistent direction, regardless of the position of my head. To do this, I place the z-transform for my head into the model matrix so that it is factored into the computation of vertex normals (which depends on the model matrix).

The width of my head is assumed to be 200 units (millimeters), and the height is re-computed based on the size of the face-box returned by OpenCV.
