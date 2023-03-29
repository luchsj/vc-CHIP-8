// Program Manager
// Handles CHIP-8 Logic.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Built-in font
typedef struct program_t
{
	uint16_t* memory;	  // 4kB of memory (all RAM, entire program is loaded in at startup)
	bool display[32][64]; // 32 x 64 px display ("on/off" values)
	uint16_t pc;		  // 16-bit program counter
	uint16_t func_stack;  // 16-bit function stack (should this be its own structure?)
	uint8_t delay_timer;  // Decrements every frame (60fps) (independent of fetch/decode/exec loop)
	uint8_t sound_timer;  // Behaves like delay timer but beeps while above 0
	uint8_t vars[16];	  // Labeled V0 through VF
} program_t;

// Opens program file and intializes CHIP-8 program.
program_t* program_init(char* file)
{
	program_t* program = malloc(sizeof(program_t));
	if(program == NULL)
	{
		fprintf(stderr, "Program: failed to allocate memory for object\n");
		return NULL;
	}

	program->memory = malloc(sizeof(uint16_t) * 4096);

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 64; j++)
			program->display[i][j] = ((i + j) % 2 == 0);
	}

	return program;
}

// Returns an array of floats representing the current state of the display.
float* program_display_to_rgb(program_t* program)
{
	float* ret = malloc(sizeof(float) * 64 * 32 * 3);
	if (ret == NULL)
	{
		fprintf(stderr, "Program: failed to make RGB data\n");
		return NULL;
	}

	int pos = 0;
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (program->display[i][j])
			{
				*(ret + pos) = 1.0f;
				*(ret + pos + 1) = 1.0f;
				*(ret + pos + 2) = 1.0f;
			}
			else
			{
				*(ret + pos) = 0.0f;
				*(ret + pos + 1) = 0.0f;
				*(ret + pos + 2) = 0.0f;
			}
			pos += 3;
		}
	}

	return ret;
}