# Nimble
Nimble is a C++, Dx11 based learning project. Following are the characteristics of the project and a sort of rough guide to what I hope to accomplish.

Nimble is meant as a way for me (and others) to attack a Graphics/Game engine from the ground up. Previously I had spent a good deal of time working on one-off toy programs. Such as a simplistic model loader. These projects never fit together into a large cohesive grouping, and it was hard to get a good sense of what sort of things were required in an full stack.

With that said, Nimble is a "for-fun" side project. I've attempted to treat it fairly seriously, but in the interest of learning as many techniques as possible I have been a bit lax on rigor and organization.

Nimble is broken into two separate components right now. There are NimbleGraphics which implements the graphics engine. There is also NimbleEditor which incorporates NimbleGraphics in a Qt-based GUI for interacting with the engine. This is meant to mimic the style of Game Engine/Editor combos seen in the wild. My main source of inspiration is the Warcraft 3 World Editor and the Starcraft 2 Galaxy Editor. Both are fully-featured game engines in their own right. They focus on ease of use of tools to create levels to share with others.

Upcoming components I will eventually add are: a scripting interface (likely based on Lua) and a true game engine interface (probably looking into Entity-Component systems from the ground up). 

# Build
Building Nimble *should* be fairly simple. If you are on Windows 10, I believe the Windows SDK is installed by default and this includes the necessary DirectX libraries. Otherwise you will need the latest DirectX SDK installed. This project uses https://github.com/Microsoft/DirectXTK for its math library (among a few other nice libraries). The DirectX Toolkit is a C++ version of a set of XNA tools and libraries. This library is installed via NuGet and the package should auto-generate upon first-time build. 

Besides that, Visual Studio 2015 Service Update 3 or better is required. This project is also intended as a way to learn and practice new C++ techniques and this requires a modern version of the MSVC compiler.

To build the Editor, Qt5+ must be installed. This project uses the QtPackage extension to integrate Qt into Visual Studio. This must be installed, and your Qt version selected within its options.

The resulting executable makes an assumption about the file structure in which its running. It should work correctly when running from Visual Studio. I have a mental TODO to rework this to be much smarter and straight forward about locating assets.

# Credits
This project is a hodge-podge of techniques and learning from many sources. Big thanks to:

http://www.rastertek.com/ - I used RasterTek to learn the individual techniques, and some boilerplate still exists (mainly the Shader classes and D3D class). 

https://github.com/Microsoft/DirectXTK - I am very familiar with XNA, and this C++ port of the XNA utility libraries has been extremely helpful.

ParticleEngine implementation is based on the XNA GPU-Particles tutorial/sample. I reimplemented it in Nimble as a starting point, and to compare performance details.
