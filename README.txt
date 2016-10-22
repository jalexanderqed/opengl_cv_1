John Alexander
8327934

Calibration parameters for laptop camera:
FOV X = 53.8151
FOV Y = 41.9612
focal length = 630.549
principal point = 324.696, 246.295
fx = 630.549
fy = 625.854
k1 = -0.0493771
k2 = -0.0572849
k3 = -0.0114353
p1 = -0.000413199
p2 = 0.00164378

Calibration parameters for given frames:
FOV X = 52.1595
FOV Y = 40.2618
focal length = 653.785
principal point = 326.947, 252.425
fx = 653.785
fy = 654.736
k1 = 0.0527332
k2 = -0.493228
k3 = 1.70908
p1 = 0.00538038
p2 = 0.00311014

ANALYSIS:
My laptop's camera has significantly less distortion than the camera used to take the example images. p1 and p2 values show that tangential distortion is somewhat more significant in the example camera, meaning that its lens is less well-aligned with the receptor. The k2 and k3 parameters indicate that the radial distortion is MUCH more significant in the example images, indicating a difference in lens shape that distorts the image outwards from the principal point. The negative sign on the laptop camera parameters indicate that this camera produces a fishey effect, so the pixels must be brought in, while the positivevalue on the given frames indicate that it has a telephoto effect since the pixels must be "pushed out."
The cameras have similar fields of view (i.e. are equivalently "wide-angle"), though my laptop's field of view is larger by a few degrees.

Error results for laptop camera:
total square error = 2.77278-5.02055 (depending on image)
mean pixel error = 0.2587 - 0.3404 (depending on image)

Error results for given frames:
total square error = 17.3034-67.758 (depending on image)
mean pixel error = 0.600405-1.1841 (depending on image)

Based on this, it is clear that the laptop camera produces significantly better results when mapping points back to their locations. This may be because it is more modern and has less blur, which increases the precision with which OpenCV can detect the checkerboard locations and thus its parameters.
