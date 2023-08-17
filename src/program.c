// Program Manager
// Handles CHIP-8 Logic.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//#define WIN32_LEAN_AND_MEAN
#ifdef WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

// Built-in font
typedef struct program_t
{
	uint8_t* memory;	  // 4kB of memory (all RAM, entire program is loaded in at startup)
	bool display[32][64]; // 32 x 64 px display ("on/off" values)
	uint16_t pc;		  // 16-bit program counter
	uint16_t index;		  // 16-bit register for mem locations	
	uint16_t* func_stack; // 16-bit function stack
	uint8_t delay_timer;  // Decrements every frame (60fps) (independent of fetch/decode/exec loop)
	uint8_t sound_timer;  // Behaves like delay timer but beeps while above 0
	uint8_t vars[16];	  // Labeled V0 through VF
	bool prog_loaded;     // Indicates whether or not a program is actually loaded
} program_t;

// Asks the system for the given file and writes the data to program memory.
bool program_open_rom_to_mem(char* file_path, void** ram)
{
#ifdef WIN32_LEAN_AND_MEAN
	HANDLE program_file;
	if (file_path)
	{
		wchar_t wide_path[1024];
		if (MultiByteToWideChar(CP_UTF8, 0, file_path, -1, wide_path, sizeof(wide_path)) <= 0)
		{
			fprintf(stderr, "PROGRAM: File read failed, failure parsing file path\n");
			return false;
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

			fprintf(stderr, "PROGRAM: file read failed: %s\n", error);
		}
	}
	else
	{
		fprintf(stderr, "Program: failed to initialize, file path is NULL");
		return false;
	}

	PLARGE_INTEGER file_size = 0;
	GetFileSizeEx(program_file, &file_size);

	
	//program->memory + 0x200
	if (!ReadFile(program_file, ram, file_size, NULL, NULL))
	{
		//fprintf(stderr, "Program: file read failed: %s\n", error);
	}
#endif

	// Return false on read failure
	return false;
}

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

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (i % 2 == 0)
				program->display[i][j] = true;
			else
				program->display[i][j] = false;

		}
	}
#ifdef WIN32_LEAN_AND_MEAN
	memcpy_s(program->memory + 0x050, sizeof(uint8_t) * (4096 - 0x050), font, sizeof(font));
#endif
	memcpy(program->memory + 0x050, font, sizeof(font));
	
	program->pc = 0x200;

	return program;
}

// Takes the local/absolute path of a file and a location in memory to read the file into.
// Writes the content of the file to the given location in memory.
// Returns the number of bytes read before EOF.
// If the read operation fails, the program will return a negative int.
int program_open_file(char* file_path, void* out_mem, int max_length)
{
#ifdef WIN32_LEAN_AND_MEAN
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
#endif
	return -1;
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

void program_update(program_t* program)
{
	// TODO: timing w/ user-definable speed

	// Make sure there's actually a program running
	if (!program->prog_loaded)
		return;

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
			for (int i = 0; i < 32; i++)
			{
				for (int j = 0; j < 64; j++)
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

		uint8_t x_pos = program->vars[x] % 64;
		uint8_t y_pos = program->vars[y] % 32;

		program->vars[0xf] = 0;

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				int mask = 1 << (7 - j);
				uint8_t val = (*(program->memory + program->index + i) & mask) >> (7 - j);
				if (val && y_pos + i < 32 && x_pos + j < 64)
				{
					if (program->display[y_pos + i][x_pos + j])
					{
						program->display[y_pos + i][x_pos + j] = false;
						program->vars[0xF] = 1;
					}
					else
					{
						program->display[y_pos + i][x_pos + j] = true;
					}
				}

				if (x_pos + j >= 64)
					break;
			}

			if (y_pos + i >= 32)
				break;
		}

		break;
	}
	default:
		fprintf(stderr, "Program: encountered unknown instruction\n");
		break;
	}
}
