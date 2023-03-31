#pragma once

typedef struct program_t program_t;

program_t* program_init(char* file);

float* program_display_to_rgb(program_t* program);

void program_update(program_t* program);