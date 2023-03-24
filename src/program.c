// Program Manager
// Handles CHIP-8 Logic.

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Built-in font

typedef struct program_t
{
	uint16_t* memory;	  // 4kB of memory (all RAM, entire program is loaded in at startup)
	bool display[64][32]; // 64 x 32 px display ("on/off" values)
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

	program->memory = malloc(sizeof(uint16_t) * 4096);
}