#include "../include/engine.hh"

int Inventory::add(unsigned int item_id, unsigned int number, fl::vec<s_Item> item_list) {
	(void)item_list;
	if (capacity <= static_cast<int>(store.size())) {
		return (-1);
	}
	for (int i = 0; i < store.size(); i++) {
		if (store.at(i).id == item_id && store.at(i).stack_size < max_stack) {
			store.at(i).stack_size += number;
			return (0);
		}
	}
	store.push_back({item_id, number, (unsigned int)store.size()});
	return (0);
}

void Inventory::del(unsigned int index) {
	store.erase(index);
}

void Inventory::resetRender(int height, int width, int x, int y) {
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

void Inventory::updateInventory(double delta_time, int win_width, int win_height) {
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

void Inventory::renderInventory(double delta_time, fl::vec<s_Item> &item_list, fl::vec<Texture2D> &textAtlas) {
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

void Loot::addLoot(unsigned int id, float drop_rate) {
	loot_table.drop_rate.push_back(drop_rate);
	loot_table.item_id.push_back(id);
	loot_table.size++;
}

fl::vec<unsigned int> Loot::generateLoot(int loot_size, time_t time_arg) {
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
