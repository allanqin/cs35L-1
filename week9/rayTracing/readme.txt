homework 9 log
===============================================================================
This homework concerned making a ray tracing implementation multi-threaded. 
One of the first things I had to do was to try and grasp what the code was
doing and where I could split the work for different threads. There were four
nested for loops that went through each pixel and did the ray tracing for each
one. I could split the image into different sections of pixels and give each
section to a thread.

The first thing I did was to decouple the print statement in the nested for
loops. It would be very messy getting the threads to synchronize and correctly
print the data in the correct order. This is an "embarassingly parallel" pro-
gram and we shouldn't have to worry about synchronization. So instead of print-
ing right after each pixel was finished, I took the print statement outside
of the nested for loops and replaced it with a statement which stored the data
to be printed in an array.

This segues to my next problem. To store the data, I orignally opted to use the
structure "Vec3" but I was having some problems with incompatible data types -
something about doubles and floats. Since I couldnt get the array of Vec3's to
work after trying casting, etc., I created my own data structure RGB, typedefed
as RGB_t, which used float data members. This allowed me to store those
"scaled_color[i]" values for printing later. I was able to store my RGB_t's
in the array.

Next, I moved almost all of the code in main up to the end of the nested for
loops into a thread function. The code in the thread function then only modi-
fied where to start and end the outermost for loop. This information was passed
in as an argument in pthread_create(...) and was calculated using width /
nthreads. I had some problems with the syntax and correct argument types for
using the function but these were trivial.

After running the multithreaded version for the first time, I realized that I'd
need some global variables so that each thread could access them. Some examples
are the print array of RGB_t's that I described earlier as well as a struct
"scene_t scene".

I then also had some problems correctly indexing where into my print array
to write data. I originally tried to use a 2D array where the index depended
on int px and int py (iterators for the outer two for loops) but things got
messy and I abandoned the idea. Instead, I opted to just simulate a 2D array
by appropriately indexing a 1D one. For instance, to access print[px][py],
one can use print[px*width+py] due to the way a 2D array is layed out in mem-
ory. This was much easier. Each thread than correctly wrote its respective
section of the array since each had a unique section of the image to ray
trace and hence different px and py values in the outer two for loops.

Finally, my last problem was that I wasn't setting the bounds correctly for
where each thread to work on a section of the picture. I was getting results
that had horizontal black lines - one for each thread I tried using. I looked
into the text data of the image file and deduced that I must have had an off-
by-one error in my thread function. I experimented and found that my loop
termination condition in the outer most for loop should have been larger by one
and once I made the change, the black lines were gone. Comparing image files
created using my version vs. the original using diff, they were identical.

The multithreaded SRT is significantly faster than the single-threaded one.
On my own laptop speeds were better. However, since my laptop only has a
dual core processor, I opted to scp my files and ssh into my SEAS account
and test it using their 16 core servers. With 8 threads running, performance
was improved by greater than a factor of 4 compared to the original. Below
are the actual numbers:

1 thread:
real 47.09
user 46.98
sys 0.00

2 threads:
real 24.49
user 47.47
sys 0.00

4 threads:
real 16.07
user 47.31
sys 0.00

8 threads:
real 10.29
user 49.01
sys 0.00
