#include <stdlib.h>

#include "program.h"
#include "wm.h"

int main()
{
	wm_t* wm = wm_init();
	program_t* program = program_init("roms/ibm-logo.ch8");
	wm_init_texture(wm, program_display_to_rgb(program), 32, 64);

	if(wm == NULL)
		return EXIT_FAILURE;

	while(!wm_should_close(wm))
	{
		program_update(program);
		//Making a new texture each frame sucks!!! We should be reassigning it!!!
		wm_init_texture(wm, program_display_to_rgb(program), 32, 64);
		wm_update(wm);
	}

	wm_terminate(wm);

	return EXIT_SUCCESS;
}