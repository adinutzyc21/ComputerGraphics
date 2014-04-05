Name: Adina Stoica
Email: adina.stoica@gmail.com
Programming environment: CEC console + MakeFile

NOTES:

* I am taking the necessary free days to make this assignment on time (2 days)



Additional classes, methods, or files

	* putPixel : colors the pixel in the given color at the given position (does so correctly up-down, left-right)
	* renderPixels: just the default function, prints random colors for every pixel of the screen (default for RENDER_SCANLINE and RENDER_PATH_TRACING)
	* generateDir: makes the ray ((unit) direction from the eye to a pixel in 3D world coordinates)
	* rayObjectIntersect takes a point, a direction and returns (a pointer to) the first point where the ray hit the closest object, the normal at that point as well as the object
	* illuminationModel recursively gets the color of the pixel. It returns the normalized sum of the diffuse, ambient, specular, transparent and reflection colors for an object. It recursively calculates the color for objects that are reflective and transparent, for a limit equal to RECURSIVE_LIMIT (set at 3 right now). It also calls ambientIllumination, diffuseIllumination and specularIllumination to get the ambient, diffuse and specular colors for every step.
	* renderRayTracing is the function that does the ray tracing. It creates a ray by calling generateDir, uses the ray to get the color of the pixel in the current model by calling illuminationModel and colors the pixel.

Extra Credit:
 	I implemented transparency partially, but I didn't implement refraction.
 	

Known bugs:
	The scenes do not display 100% correctly. For example in the scene "bujans_raquel" I am seeing the back of the cylinder and cone (though I am checking if the object is facing the other way, I believe). Same problem with the "castle" scene I made and (probably) with the "basketball" scene. I haven't tested other scenes. I don't know if maybe this has to do with finding the first hit, the normals or the fact that I am not checking correctly if I'm seeing the wrong face of the object...