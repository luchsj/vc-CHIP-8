// Header file for window manager

typedef struct wm_t wm_t;

// Keyboard keymask.
enum
{
	k_key_esc = 1 << 0,
};

// Initialization function. Returns reference to window manager object.
wm_t* wm_init();

void wm_update(wm_t* wm);

void wm_terminate(wm_t* wm);

int wm_should_close(wm_t* wm);
