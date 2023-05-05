# PicoGUI Library

Pico GUI library is based around the PicoGraphics library. It is currently tested/developed for the Pico Display Pack, however it *should* work with any display (possibly with a few minor alterations).

## Heirarchy

The gui is made up of a few main components. The `ViewController` which handles events and manages which `View` is currently being displayed. `View`s are just specialised `Widget`s with a few extra features. The most vital component is probably the `Widget` class. Every GUI object is an instance of a `Widget` (And `Drawable` too).

### The `Widget` class

The `Widget` class is the backbone of this library. It manages parents, children, and a bunch more too. Every `Widget` has an associated layout which is made up of a Box (It's actual position and size on the display), a Size (the minimum space a widget requires) and a boolean denoting whether this `Widget` is growable (woild like to take up an arbitrary amount of space).

Each `Widget` is responsible for managing its own layout and children if it is moved or re-sized. This is done using the `bool Widget::relayout()` method which is called by the `ViewController` (or parent widget in its `relayout()`) when the layout has been invalidated by invoking `Drawable::requiresRelayout()`.

Each `Widget` is responsible for redrawing itself and instructing its children to redraw. This is done using the `void Widget::redraw()` method which is called by the `ViewController` (or parent widget in its `relayout()`) when the graphics has been invalidated by invoking `Drawable::requiresRedraw()`.

When implementing a new `Widget`, the two most important parts are ensuring it is drawn correctly (not exceeding its Box etc) and ensuring that the layout of any children it may have is correct (spaced and offset correctly from each other)

### The `View` class

The `View` class is what a developer wishing to create a new screen/scene/view/whatever-you-want-to-call-it will inherit from. This has a few added features for managing Input and other Events.

### The `ViewController` class

The `ViewController` class is responsible for managing view changes, events (e.g. user input, timer), instructing the current view to `redraw()` or `relayout()`. You should not need to modify this unless you are adding functionality.