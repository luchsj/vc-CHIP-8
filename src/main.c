#include <stdlib.h>

#include "wm.h"

int main()
{
	wm_t* wm = wm_init();

	if(wm == NULL)
		return EXIT_FAILURE;

	while(!wm_should_close(wm))
	{
		wm_update(wm);
	}

	wm_terminate(wm);

	return EXIT_SUCCESS;
}