# THE GAME

This is the c++ version of no heaven,
for now i will focus on the actual gameplay and the mecanics not the graphics,
as graphics is the easiest part

___

## TO DO

- MAP
- CITY
- LIFE SKILL
- SERVER
- PVE
- BOSS FIGHT
- PVP
- BASE BUILDING
- INDUSTRIE
- AUTOMATISME
- CENTRALE MARKET AND GLOBAL ECONOMY


for item and texture atlas, it should be read and loaded after launch,
player could then add items and texture on the fly
and map too;
i should also do the serialization of file for the engine

___

## NOTE

- player should feel rewarded for the craft,
they should see how their playing influenced the game

- equipement should have multiplicator and not falt value,
it should be multiplicative and not additiv to the player stats,
so a lvl 50 common armor is still les op than a lvl 1 legendarie armor

-uv_ is intended for userdefined var
-mv_ is for multiplayer var
-sv_ is for singleplayer var

-sv_player is the local player in a networking or solo context
-mv_player is the distant player in a networking context (mv_player should not have huge data size,
if it have wtf are you doing these data are sent accross the network think about player connection).
___

## Voxel Part

I will use compute shader with vulkan to render
and calculate per voxel data instead of using Vertex shadering.
i don't know if it will work but i will try to make it work,
it's only data after all.

a map is a grid of N chunk, the chunk are stored 16*16*4 x*y*z
so at pos 0 0 0 is stored the first chunk and at 15 15 4 the last chunk,
a chunk has 16 * 16 * 16 shape stored in it, a shape have 16*16*16 voxel,
so a shape is an array of 16*16*16 voxel,
a chunk is an array of 16*16*16 shape
and a map is an array of 16*16*4 chunk, so a map store 17179869184 voxel at max,
a voxel have four params x,y,z pos in shape and it's color,
x,y,z are stored on 4 bits each so 12 bits
instead we use 16bits for simplicity of type storage,
the color is encoded on an unsigned integer so 32 bits,
so we have 32bits + 16bits -> 48 bits * 17179869184 -> 824633720832 bits
that give us 103079215104 octets -> 103 079 215 104, so a map should be 103Go

i need to find another approach (even if gpu can handle it)

___

raycast into the octree to see if a block is seen, and stop looking when a cub is hitten, the bigger the distance, the less we look at child

___

for rendering, i will use raytracing / raycasting for each pixel of the screen;
i will put the scene in an octree, cast raytracing in it from the camera for each pixel and if it travel through a leave of the octree it will put the data of it into the render