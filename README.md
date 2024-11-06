

## Implementation of a median filter kernel in VisualDSP++ environment for a ADSP-BF533 Blackfin processor (can be easily adjusted for other).
----
#### The program is written in C.

It is important to adjust the width and hight in the code to accurately represent the hight and width of the image that you are applying the filter to.

With the Image Viewer you load the image into the memory at the address you specified in the code with the address of the pointer rgb. You can also enter another address (eg 0x80000).  Just make sure that you first build the project, then load the image into memory and only then run the code. Otherwise, it will not work because the image does not exist in the memory. The image viewer has a button for configuration, and there you select the image locally from the computer that you want to load. Just make sure that the dimensions, format, size in bytes, etc. are entered correctly. Otherwise, you will have some sort of deformed/barely recognizable image.
