# path-tracer
This is a basic Monte Carlo path tracer that I created as an assignment in my MSc High Performance Graphics course.

It currently supports a Blinn-Phong BSDF model, and is set up to trace an empty Cornell Box. Obviously, it features ray-triangle intersection as well as all the maths to create and fire rays off through the virtual camera lens.
The Monte Carlo method finds random vectors on the hemisphere about an intersection, and fires a ray off in that direction. Whether a further bounce is generated is currently based on a hardcoded extinction value. This will in future ideally be determined by the BSDF.
Each pixel is sampled a given numbers of times, and the sample rays are generated in a circle within the pixel to find better results per pixel.

It does not support impulse lighting effects yet.

## Output example

10 Samples

![10 Samples](scene_10_samples.png)

With only 10 samples, the output is noticeably fuzzy.

1000 Samples

![1000 Samples](scene_1000_samples.png)

Here we can see that the problem of the image being fuzzy has been solved by increasing sampling.
Notice the colour bleeding along the ceiling.

