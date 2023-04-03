// Program Manager
// Handles CHIP-8 Logic.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Built-in font
typedef struct program_t
{
	uint8_t* memory;	  // 4kB of memory (all RAM, entire program is loaded in at startup)
	bool display[64][32]; // 32 x 64 px display ("on/off" values)
	uint16_t pc;		  // 16-bit program counter
	uint16_t index;		  // 16-bit register for mem locations	
	uint16_t* func_stack; // 16-bit function stack
	uint8_t delay_timer;  // Decrements every frame (60fps) (independent of fetch/decode/exec loop)
	uint8_t sound_timer;  // Behaves like delay timer but beeps while above 0
	uint8_t vars[16];	  // Labeled V0 through VF
} program_t;

// Opens program file and intializes CHIP-8 program.
program_t* program_init(char* file_path)
{
	uint8_t font[] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0,
		0x20, 0x60, 0x20, 0x20, 0x70,
		0xF0, 0x10, 0xF0, 0x80, 0xF0,
		0xF0, 0x10, 0xF0, 0x10, 0xF0,
		0x90, 0x90, 0xF0, 0x10, 0x10,
		0xF0, 0x80, 0xF0, 0x10, 0xF0,
		0xF0, 0x80, 0xF0, 0x90, 0xF0,
		0xF0, 0x10, 0x20, 0x40, 0x40,
		0xF0, 0x90, 0xF0, 0x90, 0xF0,
		0xF0, 0x90, 0xF0, 0x10, 0xF0,
		0xF0, 0x90, 0xF0, 0x90, 0x90,
		0xE0, 0x90, 0xE0, 0x90, 0xE0,
		0xF0, 0x80, 0x80, 0x80, 0xF0,
		0xE0, 0x90, 0x90, 0x90, 0xE0,
		0xF0, 0x80, 0xF0, 0x80, 0xF0,
		0xF0, 0x80, 0xF0, 0x80, 0x80,
	};

	program_t* program = malloc(sizeof(program_t));
	if(program == NULL)
	{
		fprintf(stderr, "Program: failed to allocate memory for object\n");
		return NULL;
	}

	program->memory = malloc(sizeof(uint8_t) * 4096);

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 32; j++)
			program->display[i][j] = ((i + j) % 2 == 0);
	}

	memcpy_s(program->memory + 0x050, sizeof(uint8_t) * (4096 - 0x050), font, sizeof(font));

	program->pc = 0x200;

	// Load program
	// We're only loading one file at a time during runtime
	HANDLE program_file;
	if (file_path)
	{
		wchar_t wide_path[1024];
		if (MultiByteToWideChar(CP_UTF8, 0, file_path, -1, wide_path, sizeof(wide_path)) <= 0)
		{
			fprintf(stderr, "wah\n");
			return NULL;
		}
		program_file = CreateFile(wide_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (program_file == INVALID_HANDLE_VALUE)
		{
			char* error;
			switch (GetLastError())
			{
				case ERROR_FILE_NOT_FOUND:
					error = "couldn't find file at given path";
					break;
				default:
					error = "undefined error";
					break;
			}

			fprintf(stderr, "Program: file read failed: %s\n", error);
		}
	}
	else
	{
		fprintf(stderr, "Program: failed to initialize, file path is NULL");
		return NULL;
	}

	PLARGE_INTEGER file_size = 0;
	GetFileSizeEx(program_file, &file_size);

	if (!ReadFile(program_file, (program->memory + 0x200), file_size, NULL, NULL))
	{
		//fprintf(stderr, "Program: file read failed: %s\n", error);
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
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 32; j++)
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

void program_update(program_t* program)
{
	// TODO: timing w/ user-definable speed

	// Fetch
	uint16_t instruction = *(program->memory + program->pc) << 8;
	instruction += *(program->memory + program->pc + 1);
	program->pc += 2;

	//printf("current instruction: %x\n", instruction);

	// Decode / Execute
	switch (instruction & 0xF000)
	{
	case 0x0000:
		if (instruction == 0x00E0) // Clear screen
		{
			for (int i = 0; i < 64; i++)
			{
				for (int j = 0; j < 32; j++)
					program->display[i][j] = false;
			}
		}
		break;
	case 0x1000: // 1NNN - jump to 0xNNN
		program->pc = instruction ^ 0x1000;
		break;
	case 0x6000: // 6XNN - set register VX to NN
		program->vars[(instruction ^ 0x6000) >> 8] = (uint8_t)instruction;
		break;
	case 0x7000: // 7XNN - add NN to register VX
		program->vars[(instruction ^ 0x7000) >> 8] += (uint8_t)instruction;
		break;
	case 0xA000: // ANNN - set index register to NNN
		program->index = instruction ^ 0xA000;
		break;
	case 0xD000: // DXYN - display
	{
		// Treating lower-left as origin
		uint8_t x, y, n;
		x = ((instruction & 0x0F00) >> 8);
		y = ((instruction & 0x00F0) >> 4);
		n = (instruction & 0x000F);

		uint8_t x_pos = program->vars[x];
		uint8_t y_pos = program->vars[y] % 32;

		program->vars[0xf] = 0;

		for (int i = 0; i < n; i++)
		{
			y_pos = program->vars[y] % 32;
			for (int j = 0; j < 8; j++)
			{
				int mask = 1 << (7 - j);
				uint8_t val = (*(program->memory + program->index + i) & mask) >> (7 - j);
				if (val && x_pos < 64 && y_pos < 32)
				{
					if (program->display[x_pos][y_pos])
					{
						program->display[x_pos][y_pos] = false;
						program->vars[0xF] = 1;
					}
					else
					{
						program->display[x_pos][y_pos] = true;
					}
				}

				if (++y_pos >= 32)
					break;
			}
			if (++x_pos >= 64)
				break;
		}

		break;
	}
	default:
		fprintf(stderr, "Program: encountered unknown instruction\n");
		break;
	}
}