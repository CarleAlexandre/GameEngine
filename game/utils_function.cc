#include "../include/engine.hh"

void *dummy_function(engine_t *arg) {
	(void)arg;
	return(0x00);
}

void *solo_engine(engine_t *engine) {
	if (!(engine->status & st_game)) {
		engine->status ^= st_game;
		engine->status ^= st_menu;
	}
	return (0x00);
}


void *close_engine(engine_t *engine) {
	if (!(engine->status & st_close)) {
		engine->status ^= st_close;
	}
	return (0x00);
}
