# UndoRedoFeature

## Summary
This implementation enables undo/redo functionality in your application. The idea of this project is to add undo/redo functionality to your objects with minimum additional implementation logic in your classes.

## Goals
- The developer must be provided with an interface to control the undo/redo operations on an object.
- The developer must be able to call undo/redo from a global place in a program which will affect operations on all live variables in the application.

## Code structure and Usage
### Foundation layer
**UndoRedoInterface**  
**UndoRedoState**  
**UndoRedoCommandManager**  
These classes are the foundations of the undo/redo functionality. 
### Usage
**UndoRedoString**  
This is a high level class that typically a developer would define in order to use the undo/redo functionality.
You can find more examples of usage in the unit tests in `src` directory.

## Todo
- Add max undo operations limiter. 

## Build project

- Have Visual Studio 2019 to open the project in `vs2019` directory.
- Install [vcpkg](https://github.com/Microsoft/vcpkg) to add _gtest_ package. Run the following commands:
````sh
    vcpkg install gtest
    vcpkg integrate install
````
- Set _Solution Platform_ in Visual Studio to _x86_.
Now you should be able to build and run the project.

## LICENSE
BSD 4-clause License

