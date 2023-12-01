#include "../include/engine.hh"

int
register_input_buffer_game(fl::vec<int> &input_buffer) {
	int i = 0;
	input_buffer.push_back(GetKeyPressed());
	while (input_buffer.last() != 0) {
		input_buffer.push_back(GetKeyPressed());
		i++;
	}
	return (i);
}

void
use_input_game(fl::vec<int> &input_buffer, int *sv_engine_status) {
	while (input_buffer.size() != 0) {
		switch (input_buffer[0]) {
			case(KEY_ESCAPE): {
				*sv_engine_status = st_pause;
			}
			default:{
				break;
			}
		}
		input_buffer.pop_first();
	}
}

int
register_input_buffer(fl::vec<int> &input_buffer) {
	int i = 0;

	int c = GetKeyPressed();
	while (c != 0) {
		input_buffer.push_back(c);
		c = GetKeyPressed();
		i++;
	}
	return (i);
}

void
use_input() {

}