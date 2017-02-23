#Cast Away PROJECT


Using 1 channel color to represent intensity of spotlight. 8bit would be enough.
GObject all have static variable which indicates the current available texture unit(could be used in sampler.).
setLight function must not send unnecessary buffers.(Direction changes always so it`s not realy doable.)

implement in GObject static step function and variable for setting and getting last STEP! as deltaTime.


pPerson class must adapt to sliding and doing falling when sliding.

for infinite sky take the camera position an move the away that much of the position


BUGS:
 glm::inverseTranspose doesn`t work!!
 direclight position vector reverse why?
 
 
updates that needed:
after creating the world call deltaTime function of GLFW to make a start fresh for delta time. Needed because world creating takes long time!

after the jump of the pPerson class there is no calculation for collision up ahead so it must be done. Algorithms as follows, look 10 units ahead if there is hit do nothing
and just make a flag there is hit so after flags hits is 1 or true check if there is a hit in next step. In other word if there is a hit in previous step but not in current step
that means jumps excideded the object that was ahead.

in shader, light calculations, there is viewDirection that can be set 0,0,-1 because of it`s in view space...

light strengthen or weaken overridden by most concrete class and give it a feel of light going up or down by given affecting shader color of concrete class.

light in toggle mode must be fixed by multiplying with camera inverse in every frame.

graphics repr. of lights must have color above some number such as 200 that will be calculated when bloom implemented.

sun direction could be through the person and always displace by position of the camera.( not sure how would work the last part? )

spotlight on hand mode need to be redesign such as toggleOnhand should change spotlight pos and dir.

in reflect calc. for water scene will be rendered in BASIC_TEXTURE mode and it will be scale with 0,-1,0 vector. Actually it must be rendered -WorldUp but worldup in tis
mostly constant pos Y direction.(THIS deprecated)

about sRGB use, be aware of the fact that GLFW framebuffer creation needs to be informed. After bloom effect glfw framebuffer won’t be used so ???

on water rendering:
water level of the world will be 0,0,0 plane... And will be used for clipplane for rendering the upper part of the scene and -y scale it for reflection
the texture coordinates will be taken by glFragcoord aka screen space coordinate of the water...
Instead of above alg. use camera reflection that will put the camera under the water(or given normal).Position the camera -y and reflect the forward(DONE!)


the thigns ufuk can may  might do:
everything’s done in createObject must be readable from given file. Every inherited from GOBject class must be in the enum!
every class that implements child of GObject must be in the same order of constructing. Check every class for constructer error.




