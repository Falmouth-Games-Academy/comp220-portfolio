# Comp220  
# Journal of Debugging, Pain and Suffering
See my painful experience learning that sending arrays to the vertex shader is no simple process:  

https://docs.google.com/document/d/1lxwIw97dXd98MP8qnqwXaFcf5OI3C4ArnbVfRzrBhM0/edit?usp=sharing

# Plan for next iteration
* Due to scope, project will be scaled down and focus will be shifted toward rendering shadows.  
* Some errors were generated with the bunny model. This seems to be an error with assimp not loading in hiearchal information for the head. Rather than investigate the issue, the pigeon will be used instead.  
* Frame rate has dropped significantly since animations were added. This turns out to be because of the keyframe search occurring for each bone, each frame. This will be mitigated by using a map or similar data structure for the next iteration. Interestingly, this only seems to affect the Debug build.  

# Resources
**Bone animation**  
https://www.khronos.org/opengl/wiki/Skeletal_Animation  - Guide to the shader code behind bone animation. It recommends sending a quaternion and offset to the shader, which could be an optimisation in a future revision.  
http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html - Skeletal programming with assimp in specific.  
https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/skeletalanimation/Tutorial%209%20-%20Skeletal%20Animation.pdf  - Highly informative source on skeletal animation  
* Apparently some developers store transformation data as textures! This is really interesting as the interpolation between them could be affected by filters like trilinear or bilinear.  

https://www.gamasutra.com/blogs/TequilaWorks/20160620/275347/How_to_take_advantage_of_textures_in_the_vertex_shader.php - Further information on how animation data could be stored in textures!  
* Texture sizes can be massive: a big improvement over uniform variables for example.  
* A future iteration of this project could do all keyframe interpolation simply by running over the texture data. This could save valuable CPU time used on the keyframe searches.  
* Requires DirectX11+ (and hopefully an equivalent OGL version) to achieve, as earlier versions could not read textures in the vertex shader.  

https://www.researchgate.net/publication/37444994_Human_Body_Deformations_Using_Joint-dependent_Local_Operators_and_Finite-Element_Theory - Early historical journal on bone animation  
* Noteworthy for accounting for muscle inflation during bone movement, something that would not be seen in games until many years later. Further accounts for contact of animated figures with their surrounding environments.  

https://ieeexplore.ieee.org/document/5397589 - Compression techniques for future development. Explores the usage of existing video encoding tools for animation data  

https://ieeexplore.ieee.org/document/6469773 - More advanced interpolation methods for a future iteration. Quadratic and cubic interpolation methods are interesting!  


# Areas of interest
* Real-time shadows
* Reflections
* Spyro-style LOD (research question: could lower-detail distant objects make the world feel bigger?)
* 
* Optimisation

# Third-party info
## GLEW 2.1.0
The OpenGL Extension Wrangler Library
Copyright (C) 2002-2007, Milan Ikits <milan ikits[]ieee org>
Copyright (C) 2002-2007, Marcelo E. Magallon <mmagallo[]debian org>
Copyright (C) 2002, Lev Povalahev
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* The name of the author may be used to endorse or promote products 
  derived from this software without specific prior written permission.

## SDL 2.0.8
Simple DirectMedia Layer
Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

## GLM
================================================================================
OpenGL Mathematics (GLM)
--------------------------------------------------------------------------------
GLM is licensed under The Happy Bunny License and MIT License

================================================================================
The Happy Bunny License (Modified MIT License)
--------------------------------------------------------------------------------
Copyright (c) 2005 - 2014 G-Truc Creation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Restrictions:
 By making use of the Software for military purposes, you choose to make a
 Bunny unhappy.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

================================================================================
The MIT License
--------------------------------------------------------------------------------
Copyright (c) 2005 - 2014 G-Truc Creation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.  

## SDL_image 2.0
  SDL_image:  An example image loading library for use with SDL
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.