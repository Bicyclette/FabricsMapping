# FabricsMapping

<p>
A software made to help visualizing the appearance of patterns used in textile designing on bedding fabrics.<br/>
To do so, a 2D pattern is mapped on a bezier surface generated by a grid placed on a specific area of an input image.<br/>
<p/>
<p>
Implementation done in C++ with Qt5 for the GUI system.
<p/>

## Dependencies

- [Conan](https://conan.io/)
- [CMake](https://cmake.org/)

## Build

```
conan install . -s build_type=Release --build missing --install-folder=build
cmake -B build -S .
```

## Demo

### step 1 : Open an image

<img alt="step1" src="imgs/step1.jpg"/>

### step 2 : Create a shape that encloses the area we want to map

<img alt="step2" src="imgs/step2.jpg"/>

### step 3 : Hit enter to create a mask

<img alt="step3" src="imgs/step3.jpg"/>

### step 4 : Create a grid and move its control points around an object

<img alt="step4" src="imgs/step4.jpg"/>

### step 5 : Generate a bezier surface from the grid

<img alt="step5" src="imgs/step5.jpg"/>

### step 6 : Open a pattern

<img alt="step6" src="imgs/step6.jpg"/>

### step 7 : Map the pattern on the bezier surface

<img alt="step7" src="imgs/step7.jpg"/>

### step 8 : Hide the bezier surface and the grid from the view

<img alt="step8" src="imgs/step8.jpg"/>
