#ifndef ENGINE_HH
# define ENGINE_HH

# ifdef _WIN64
#  include <C:/mingw64/include/raylib.h>
#  include <C:/mingw64/include/raymath.h>
#  include <C:/mingw64/include/rlgl.h>
#  define RLIGHTS_IMPLEMENTATION
#  include "rlights.h"
#  define RCAMERA_IMPLEMENTATION
#  include <C:/mingw64/include/rcamera.h>
#  define GLSL_VERSION 330
# else
#  include <raylib.h>
#  include <raymath.h>
#  include <rlgl.h>
#  define RLIGHTS_IMPLEMENTATION
#  include "rlights.h"
#  define RCAMERA_IMPLEMENTATION
#  include <rcamera.h>
#  define GLSL_VERSION 330
#  include <cstring>
# endif

# include <iostream>
# include <fstream>
# include <string.h>
# include <string>
# include "../../fllib/include/flvector.hh"
# include "../../fllib/include/fllinear.hh"
# include "../../fllib/include/floctree.hh"

enum engine_status {
	st_menu = 1 << 0,
	st_game = 1 << 1,
	st_close = 1 << 2,
	st_setting = 1 << 3,
	st_pause = 1 << 4,
	st_gameover = 1 << 5,
	st_debug = 1 << 6,
	st_multiplayer = 1 << 7,
};

typedef struct GBuffer {
    unsigned int framebuffer;

    unsigned int positionTexture;
    unsigned int normalTexture;
    unsigned int albedoSpecTexture;
    
    unsigned int depthRenderbuffer;
} GBuffer;

typedef enum {
   DEFERRED_POSITION,
   DEFERRED_NORMAL,
   DEFERRED_ALBEDO,
   DEFERRED_SHADING
} DeferredMode;

typedef struct s_display {
	unsigned short width;
	unsigned short height;
	bool fullscreen;
	unsigned int configflag;
	char *title;
	Image icon;
}	display_t;

typedef struct s_engine {
	int	status;
	fl::vec<Texture> textures;
	Font font;
	Shader fbo_shader;
	Shader shader;
	RenderTexture2D fbo;
	GBuffer gbuffer;
	Shader gbuffershader;
}	engine_t;

typedef struct sv_player_s {
	fl::vec3 pos;
	fl::vec3 dir;
	fl::vec3 topos;//used to resolve collision
	fl::vec3 accel;
	fl::vec3 vel;
	fl::vec3 forward;
	fl::vec3 right;
	bool airborn;
	float speed;
	float mass;
	float jumpforce;
	bool sliding;
	bool jumping;
}	sv_player_t;

typedef struct sv_physic_s {
	float friction;
	float airborn_friction;
	float restitution;
	fl::vec3 gravity;
	float sensibility;
} sv_physic_t;

typedef struct mv_player_s {
	fl::vec3 pos;
	fl::vec3 dir;
}	mv_player_t;

typedef struct s_Button {
	Rectangle bound;
	int textureId[2];
	Vector2 pos;
	char *text;
	void *(*fun)(engine_t *);
}	Button;

typedef struct s_FadeTxt {
	std::string	fmt;
	Color		color;
	Vector2		pos;
	double		delay;
	double		time;
	float		alpha;
	int			font_size;
}	FadeTxt;

typedef struct s_Attribut {
	float	speed;
	float	damage;
	float	life;
	float	armor;
	float	max_life;
	float	max_armor;
	float	stamina;
	float	max_speed;
	float	weapon_masteries;
	float	agility;
	float	classes;
	float	life_regen;
}	Attribut;

typedef struct s_Item {
	unsigned int id;
	unsigned int text_index;
	unsigned int model_index;
	std::string	name;
	int			properties;
}	Item;

typedef struct s_InventoryPanel {
	unsigned int id;
	unsigned int stack_size;
	unsigned int index;
}	InvPanel;

typedef struct s_LootTable {
	fl::vec<unsigned int> item_id;
	fl::vec<float> drop_rate;
	unsigned int size;
}	LootTable;

typedef struct s_Voxel {
	Mesh mesh;
	Model model;
	Shader shader;
} Voxel;

# define DEBUG_CONSOLE_USE
# ifdef DEBUG_CONSOLE_USE
void initConsole(void);
void console_render(double delta_time, const display_t *display);
int	console_logic(double delta_time, engine_t *engine);
# endif

void *dummy_function(engine_t *arg);
void *close_engine(engine_t *engine);
void *solo_engine(engine_t *engine);

bool IsMouseInBound(Rectangle rec, Vector2 pos, Vector2 mouse_pos);

void render_button(fl::vec<Button> &button, const Font font, fl::vec<Texture> &textAtlas, const Vector2 mouse_pos);
void add_button(fl::vec<Button> *button, Rectangle bound, Vector2 pos, const char text[], void *(*fun)(engine_t *engine));
void logic_button(fl::vec<Button> &button, Vector2 mouse_pos, engine_t *engine);

void LoadTextureAtlas(fl::vec<Texture2D> *textAtlas);
void UnloadTextureAtlas(fl::vec<Texture2D> &textAtlas);

void addFadingTxt(std::string text, double delay, Color color, int font_size, Vector2 pos, fl::vec<s_FadeTxt> *Fadetxt_list);
void renderFadingTxt(double delta_time, fl::vec<s_FadeTxt> *Fadetxt_list);

void DrawVoxel(Model voxel, Vector3 pos, float size, Color color);
void unloadVoxel(Voxel *voxel);
Voxel *loadVoxel(Shader voxel_shader);

fl::vec3 resolveAccel(fl::vec3 &velocity, fl::vec3 &acceleration, float mass, float deltaTime);
void addImpulse(fl::vec3 &acceleration, const fl::vec3 &impulse, float mass, float speed);

void applyFriction(fl::vec3 &acceleration, float frictionFactor);
void applyRestitution(fl::vec3 &velocity, float restitutionFactor);
void applyGravity(fl::vec3 &acceleration, const fl::vec3 &gravity, float mass);

void initRender(engine_t &engine, display_t &display);
void renderUpdate(engine_t &engine, display_t &display, Camera *camera, double delta_time);
void renderRender(engine_t &engine, display_t &display, Camera *camera, double delta_time, Voxel *voxel_dirt);

//test under
RenderTexture2D LoadRenderTextureDepthTex(int width, int height);
void UnloadRenderTextureDepthTex(RenderTexture2D target);
//

namespace fl {

inline Vector3
vec3to(vec3 v) {
	return((Vector3){v.x, v.y, v.z});
}

inline vec3
tovec3(Vector3 v) {
	return((vec3){v.x, v.y, v.z});
}

};

class Loot {
	private:
		s_LootTable loot_table;
	public:
		void addLoot(unsigned int id, float drop_rate);
		fl::vec<unsigned int> generateLoot(int loot_size, time_t time_arg);
		Loot(void) {}
		~Loot(void) {}
};

class Inventory {
	//item doesn't need to be dragged, i can make it so that it only move when needed
	private:
		fl::vec<s_InventoryPanel> store;
		int capacity = 30;
		int max_stack = 999;
		int cursorPos = 0;
		int scrollOffset = 0;
		int row = 0;
		int column = 0;
		int scrollBarSize = 0;
		Rectangle bound;
	public:
		int add(unsigned int item_id, unsigned int number, fl::vec<s_Item> item_list);
		void del(unsigned int index);
		void resetRender(int height, int width, int x, int y);
		void updateInventory(double delta_time, int win_width, int win_height);
		void renderInventory(double delta_time, fl::vec<s_Item> &item_list, fl::vec<Texture2D> &textAtlas);

		Inventory(int height, int width, int x, int y) {
			bound.height = height * 0.5;
			bound.width = width * 0.5;
			bound.x = x;
			bound.y = y;
			row = floor((bound.height - 20) / 45);
			column = floor((bound.width - 10) / 45);
			scrollBarSize = std::min(static_cast<float>((bound.height - 50) * (static_cast<float>(row * column) / store.size())), bound.height - 30);
		}
		~Inventory() {}
};

#endif