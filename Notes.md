# Notes about the program

With the O2 optimization flag for my version of the code, it's between 0.05 and 0.2 seconds faster than the cpp version compiled *without* the O2 flag... ;__; When the cpp version is compiled with the O2 flag, it's between 0.3 and 0.5 seconds faster than my code with the O2 flag..........
This is very sad.

I do wonder if using iostream instead of stdio for the output would be faster. NOPE! It's way slower. Like 0.4-0.5 seconds slower. Shoot, dang!

So I made a version of the Vec3 implementation where I just use regular operators. It is now by far the fastest version... :))) Why AVX? WHY?!

I thought about changing the vec3 member data types from double to float to see if that would increase the SIMD speed. However, this won't work.
We would only get a speed increase if we could operate on more vectors at once, which is not the case with the current implementation.


