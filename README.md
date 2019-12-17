# Insight

Insight is a simple OpenGL 3.2+ rendering framework. It allows developers to quickly and easily create custom 3D
environments built from a set of extensible primitive polygonal objects. Support for basic lighting and textures
is included and the default vertex and fragment shaders can be easily subclassed and extended. Insight environments
can be navigated and manipulated using keyboard or mouse input.

Core concepts include:

* **Scenarios**. A scenario is a high-level orchestrator for setting up an environment. It will create a *FrameQueue*
and at least one *Frame* populated with *SceneObjects*. The queue is then passed off to the DisplayManager for render.
Creating a new Scenario subclass is the place to start for defining your own environment.
* **DisplayManager**. The DisplayManager owns and renders a FrameQueue, the camera, lighting and various shaders.
* **FrameQueue**. At any given time, one FrameQueue is being rendered by the *DisplayManager*. A FrameQueue, as the
name suggests, owns a queue of *Frame* instances that are cycled through at a predefined framerate. FrameQueues can
be set up to repeat (ie. cycle back to their first frame upon rendering the last) or run to completion.
* **Frame**. A Frame is Insight's basic unit of display and will be held on screen for as long as required to satisfy
the FrameQueue's desired framerate.
* **SceneObject**. Frames consist of SceneObjects, themselves a container for a primitive (and optionally a texture)
in the 3D environment. SceneObjects can be placed statically in a Frame, or updated on demand by registering with
DisplayManager's *setUpdateSceneCallback()* method.

Insight comes with a range of demo scenarios including traversals and manipulation of tree structures (specifically
a minimum heap), the starfield effect and a more complex "autopilot" ride across a randomly generated landscape.

## Demo

You can view a quick demo of Insight at: https://vimeo.com/379963852  Apologies for the horrible resolution, it looks
a lot better on-screen!

## Dependencies

* libsdl2 (https://www.libsdl.org/)
* libglew
* libglm
* libfreetype6