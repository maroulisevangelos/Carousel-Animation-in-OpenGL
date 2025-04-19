# Carousel-Animation-in-OpenGL
This project implements a visually engaging carousel animation using OpenGL, adhering to specified requirements. The carousel includes dynamic elements such as horses, teapots, a rotating base, a flag, and celestial objects (sun and moon) orbiting the carousel. The animation supports various interactive functionalities, such as adjusting rotation speed, reversing the direction, and stopping the carousel.

Features:

  Carousel Structure:
  
    Central cylinder surrounded by two base cylinders (inner and outer).
    
    Cone-shaped roof with a thin cylindrical facade.
    
    Flag at the top, consisting of a pole and square flag.
    
    Twelve supporting poles connecting the roof to the base.
  
  Horses and Teapots:
  
    Horses constructed from spheres (body, neck, head) and cubes (legs, tail, ears).
    
    Teapots positioned around the carousel.
  
  Interactive Controls:
  
    Adjust rotation speed using the Up/Down arrow keys within defined limits.
    
    Change rotation direction with Left/Right arrow keys.
    
    Stop/start carousel rotation with "s" (stop) and "c" (continue).
    
    Clear messages displayed for invalid key actions (e.g., stopping an already stopped carousel).
  
  Dynamic Animations:
  
    Horses and teapots move sinusoidally as the carousel rotates.
    
    Movement speed scales proportionally with carousel rotation speed.
    
    Ensures alternating vertical positions of horses and teapots.
  
  Celestial Objects:
  
    Sun: Five concentric spheres with dynamic transparency (partially implemented).
    
    Moon: Positioned opposite to the sun, rotates around the carousel.
  
  Lighting:
  
    Basic lighting added to enhance visual realism.
    
    Attempts made to simulate sunlight (partial implementation).
  
  Termination and Camera:
  
    Exit animation execution with the "esc" key.
    
    Partial camera rotation functionality included in the code.

Implementation Details:

  Developed entirely in OpenGL.
  
  Focused on modular construction and extensibility.
  
  Includes detailed comments for easy understanding and debugging.

Challenges and Notes:

  Comments within the code highlight partial implementations (e.g., sunlight simulation).
  
  The main file contains attempts to implement camera rotation.
