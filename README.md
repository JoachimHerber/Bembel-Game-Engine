# The Bembel-Game-Engine
The Bembel-Game-Engine is a C++ game-engine that is developed as a hobby project.
It is still in early development and missing some important features (e.g. support for physics simulations and animated meshes).
The main components of Bembel-Game-Engine currently are the EventManager, the DisplayManager, the AssetManager, the InteractionSystem and the GraphicSystem.

## EventManager
The EventManager is responsible for managing event channels for different types of events.
These event channels are the main method of communication for the different systems that are part of the Bembel-Game-Engine.

## DisplayManager

## AssetManager

## InteractionSystem
The InteractionSystem is responsible for handling user input.
It therefore pules imput evets an emittes the corresponding events througt the EventManager.
Additionally it also provides functionality for queering the sate of keyboard or mouse buttons and for gaining access to signals for individual buttons. 


## GraphicSystem
The GraphicSystem is responsible for generating images.
It therefore provides configurable rendering pipelines, which can be use to render a scene and to apply post processing effects to generated images.


# Example applications
## Input Example
A simple application that showcases how events can be used to react to user inputs.

## Rendering Example
A simple applications the demonstrates how some material parameters affect the appearance of the rendered geometry.
![screenshot](/Examples/rendering-example.png)

## Chess Example
A simple chess application that allowed user to select and move chess pieces on a chess board.
Currently doesn’t check for victory conditions.
![screenshot](/Examples/chess-example.png)

