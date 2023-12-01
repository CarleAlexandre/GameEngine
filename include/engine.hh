#ifndef ENGINE_HH
# define ENGINE_HH

# ifdef _WIN64
#  include <C:/mingw64/include/raylib.h>
#  include <C:/mingw64/include/raymath.h>
#  include <C:/mingw64/include/rlgl.h>
#  define RLIGHTS_IMPLEMENTATION
#  include "rlights.h"
#  define GLSL_VERSION 330
# else
#  include <raylib.h>
#  include <raymath.h>
#  include <rlgl.h>
#  define RLIGHTS_IMPLEMENTATION
#  include "rlights.h"
#  define GLSL_VERSION 330
#  include <cstring>
# endif

# include <iostream>
# include <fstream>
# include <vector>
# include <string.h>
# include <string>
# include "C:/Users/emilia/Desktop/fllib/include/flmath.h"
# include "C:/Users/emilia/Desktop/fllib/include/flmem.h"
# include "C:/Users/emilia/Desktop/fllib/include/flvector.hh"

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
	Shader depth_shader;
	Shader shader;
	RenderTexture2D fbo;
}	engine_t;

typedef struct sv_player {

}	sv_player;

typedef struct mv_player {

}	mv_player;

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

//# define DEBUG_CONSOLE_USE
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

//test under
RenderTexture2D LoadRenderTextureDepthTex(int width, int height);
void UnloadRenderTextureDepthTex(RenderTexture2D target);
//

class Loot {

private:
	s_LootTable loot_table;

public:

	void addLoot(unsigned int id, float drop_rate) {
		loot_table.drop_rate.push_back(drop_rate);
		loot_table.item_id.push_back(id);
		loot_table.size++;
	}

	fl::vec<unsigned int> generateLoot(int loot_size, time_t time_arg) {
		fl::vec<unsigned int> loot;
		float roll = 0.0f;
		float cumulative_prob = 0.05f;
		time_t seed = time_arg * 1000;
		// Initialize random number generator
		srand((unsigned int) seed);
		// Generate loot items
		for (int i = 0; i < loot_size; i++) {
			for (unsigned int i = 0; i < loot_table.size; i++) {
				roll = (float) rand() / (float)RAND_MAX;
				for (unsigned int j = 0; j < loot_table.size; j++) {
					cumulative_prob += loot_table.drop_rate[j];
					if (roll <= cumulative_prob) {
						loot.push_back(loot_table.item_id[j]);
						break;
					}
				}
			}
		}
		return (loot);
	}
	Loot(void) {
	}
	~Loot(void) {

	}
};

class Inventory {
	//item doesn't need to be dragged, i can make it so that it only move when needed
	private:
	public:
	fl::vec<s_InventoryPanel> store;
	Rectangle bound;
	int capacity = 30;
	int max_stack = 999;
	int cursorPos = 0;
	int scrollOffset = 0;
	int row = 0;
	int column = 0;
	int scrollBarSize = 0;

	int add(unsigned int item_id, unsigned int number, fl::vec<s_Item> item_list) {
		(void)item_list;
		if (capacity <= static_cast<int>(store.size())) {
			return (-1);
		}
		//for (i32 i = 0; i < inventory.size(); i++) {
		//	if (inventory.at(i).id == item_id) {
		//		inventory.at(i).stack_size += number;
		//		return (0);
		//	}
		//}
		store.push_back({item_id, number, (unsigned int)store.size()});
		return (0);
	}

	void del(unsigned int index) {
		store.erase(index);
	}

	void init(int height, int width, int x, int y) {
		bound.height = height * 0.5;
		bound.width = width * 0.5;
		bound.x = x;
		bound.y = y;
		row = floor((bound.height - 20) / 45);
		column = floor((bound.width - 10) / 45);
		scrollBarSize = std::min(static_cast<float>((bound.height - 50) * (static_cast<float>(row * column) / store.size())), bound.height - 30);
	}

	void resetRender(int height, int width, int x, int y) {
		bound.height = height * 0.5;
		bound.height -= static_cast<int>(bound.height) % 45 + 20;
		bound.width = width * 0.5;
		bound.width -= static_cast<int>(bound.width) % 45 + 20;
		bound.x = x;
		bound.y = y;
		row = floor((bound.height - 20) / 45);
		column = floor((bound.width - 10) / 45);
		cursorPos = 0;
		scrollOffset = 0;
		scrollBarSize = std::min(static_cast<float>((bound.height - 50) * (static_cast<float>(row * column) / store.size())), bound.height - 30);
	}

	void updateInventory(double delta_time, int win_width, int win_height) {
		(void)delta_time;	
		static bool draggingScrollBar = false;
		static bool dragWindow = false;
		static Vector2 old_pos;
		Vector2 mouse_pos = GetMousePosition();
		if (draggingScrollBar == true) {
			cursorPos = mouse_pos.y - (bound.y + 10 + scrollBarSize * 0.5);
			int maxOffset = bound.height - 10 - scrollBarSize;
			if (cursorPos < 0) {
				cursorPos = 0;
			}
			if (cursorPos > maxOffset) {
				cursorPos = maxOffset;
			}
			scrollOffset = static_cast<int>(((static_cast<float>(cursorPos) / maxOffset) * (static_cast<float>(store.size()) / column)));
		}
		if (dragWindow == true) {
			Vector2 new_pos;
			new_pos.x = mouse_pos.x - old_pos.x;
			new_pos.y = mouse_pos.y - old_pos.y;
			bound.x += new_pos.x;
			bound.y += new_pos.y;
			if (bound.x >= win_width - bound.width - 10) {
				bound.x = win_width - bound.width - 10;
			} else if (10 >= bound.x) {
				bound.x = 10;
			}
			if (bound.y >= win_height - bound.height - 10) {
				bound.y = win_height - bound.height - 10;
			} else if (10 >= bound.y) {
				bound.y = 10;
			}
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			Rectangle scrollBar = {0, 0, 10, bound.height};
        	if (IsMouseInBound(scrollBar, {bound.x + bound.width - 10, bound.y + 10}, mouse_pos)) {
        	    draggingScrollBar = true;
        	} else if (IsMouseInBound({0, 0, bound.width, 10}, {bound.x, bound.y}, mouse_pos)) {
				dragWindow = true;
				old_pos = mouse_pos;
			} else {
				dragWindow = false;
			}
		}
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			draggingScrollBar = false;
			dragWindow = false;
		}
	}

	void renderInventory(double delta_time, fl::vec<s_Item> &item_list, fl::vec<Texture2D> &textAtlas) {
		(void)delta_time;
        Item item;
		int startIdx = scrollOffset * column;
		int endIdx = std::min(startIdx + (row * column), static_cast<int>(store.size()));

		DrawRectangle(bound.x, bound.y, bound.width, bound.height, ColorAlpha(BLACK, 0.1));
		if (startIdx < static_cast<int>(store.size())) {
			for (int y = 0; y < row; y++) {
				DrawText(TextFormat("%i", (startIdx / 8) + y + 1), bound.x, bound.y + 20 + y * 45, 10, WHITE);
				for (int x = 0; x < column; x++) {
					int index = startIdx + x + (y * column);
					if (index >= endIdx) {
						y = row + 1;
						break;
					}
					DrawRectangle(bound.x + 10 + x * 45, bound.y + 20 + y * 45, 40, 40, ColorAlpha(WHITE, 0.2));
					unsigned int item_id = store[index].id;
					unsigned int stack_size = store[index].stack_size;
					Item item;
					for (uint32_t j = 0; j < item_list.size(); j++) {
						if (item_list[item_id].id == item_id) {
							item = item_list[item_id];
							DrawTextureRec(textAtlas[item.text_index], (Rectangle){0, 0, 32, 32}, {static_cast<float>(bound.x + 10 + x * 45 + 4), static_cast<float>(bound.y + 20 + y * 45 + 4)}, WHITE);
							DrawText(TextFormat("%i", stack_size), bound.x + 30 + x * 45, bound.y + 50 + y * 45, 10, WHITE);
							break;
						}
					}
				}
			}
		}
		DrawRectangle(bound.x, bound.y, bound.width, 10, ColorAlpha(WHITE, 0.5));
		Rectangle scrollBar = {bound.x + bound.width - 10, cursorPos + bound.y + 10, 10, static_cast<float>(scrollBarSize)};
    	DrawRectangleRec(scrollBar, DARKGRAY);
		DrawText("Inventory", bound.x + 1, bound.y + 1, 8, BLACK);
	}
	Inventory(){}
	~Inventory(){}
};

class Player {

private:

public:
	Vector3 pos;
	Vector3 vel;
	Vector3 topos;
	Camera3D cam;
	Attribut attribut;
	Vector3 orientation;
	Inventory *inventory;
	Model model;
	bool isAlive;

	void die() {

	}

	void live() {

	}

	void update(double delta_time, std::vector<int> input_buffer, int *state, fl::vec<s_FadeTxt> *Fadetxt_list) {
		static double acc_time = 0;
		(void)input_buffer;
		if (Vector3Distance(pos, topos) > 0.1f) {
			pos = flTravel3d(pos, topos, attribut.speed * delta_time, delta_time);
		}
		if (attribut.life < 0) {
			attribut.life = 0;
			*state = st_gameover;
		}
		if (attribut.life > attribut.max_life) {
			attribut.life = attribut.max_life; 
		}
		if (acc_time >= 1) {
			if (attribut.life < attribut.max_life) {
				attribut.life += attribut.life_regen;
				addFadingTxt(TextFormat("+%.0f", attribut.life_regen), 0.8f, GREEN, 12, {.x = pos.x + 1, .y = pos.y + 1}, Fadetxt_list);
			}
			acc_time = 0;
		}
		acc_time += delta_time;
	}

	Player(void) {
		pos = {
			.x = 0,
			.y = 0,
			.z = 5,
		};
		vel = {
			.x = 0,
			.y = 0,
			.z = 5,
		};
		topos = {
			.x = 0,
			.y = 0,
			.z = 0,
		};
		cam = (Camera3D){
			.position = {
				.x = 2,
				.y = -3,
				.z = 10,
			},
			.target = {
				.x = 0,
				.y = 0,
				.z = 0
			},
			.up = {
				.x = 0,
				.y = 1.0f,
				.z = 0,
			},
			.fovy = 45.0f,
			.projection = CAMERA_PERSPECTIVE,
		};
		attribut = {
			.speed = 100.,
			.damage = 0,
			.life = 1500,
			.armor = 0,
			.max_life = 1500,
			.max_armor = 0,
			.stamina = 0,
			.max_speed = 200.,
			.weapon_masteries = 0,
			.agility = 1.4f,
			.classes = 0,
			.life_regen = 12.,
		};
		inventory = new(Inventory);
	}

	~Player(void) {

	}
};

#endif