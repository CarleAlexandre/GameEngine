# THE GAME

the old note here was complete bullshit.

for now i aim to create an action rpg first personne shooter, it's in voxel, like teardown or john lin's engine.
it aim to have the same gameloop as a monster hunter, but you can create your own town.

in multiplier, you have already a town implemented, and you can modifie it by buying some parcel.
in this town, you have multiple activities, the normal gameplay loop, but also, you can create commerce, enterprise, a guild
a pub, you can give quest to other player, join pvp quest, and go for boss fight with 1 to 16 player (more player = higher difficulty), and user created minigames or gamemode.

the first gameplay and gameloop i want to introduce is a round based zombie survivial game in map that are very rich in lore (canon in the game universe).

___

## TO DO GAME DESIGN

- ZOMBIE SURVIVAL MODE
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
- MODDING
- more idea to come


for item and texture atlas, it should be read and loaded after launch,
player could then add items and texture on the fly
and map too;
i should also do the serialization of file for the engine

___

## TO DO ENGINE WORK

- collision detection
- raycasting through octree
- data compression and serialisation (1 block = 1 unsigned short (for the block id))
- gbuffer and zbuffer
- pushing the voxel scene into a rendering object
- reworking camera system for the voxel renderer
- adding raytracing for lighting effect and some particles
- finishing to work on the ui / ATH
- building the ide and level editor / model editor software
- making a system to take high quality asset and convert them to voxel
- optimizin data calculation through simd (partially done with fl::vec3_mm)
- animation tooling and implementation
- hot loading
- thread / process
- more idea to come
___

## TO DO FLLIB

- finish octree data struct and algorythm for it.
- add a physic calculation library for things like collision, fluid, gaz, solid and force.
- threading library for semaphore, fence, thread, mutex management (using either pthread or equivalent for c++)
- more idea to come
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