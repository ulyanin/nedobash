#include "plugin_load.h"
#include "command_description.h"
#include <stdlib.h>
#include <dlfcn.h>
#include <gnu/lib-names.h>

int plugin_load(int argc, char ** argv)
{
    if (argc == 1)
        return EXIT_FAILURE;
    fprintf(stderr, "trying to load '%s'\n", argv[1]);
    void * plugin = dlopen(argv[1], RTLD_NOW);
    if (!plugin) {
        fprintf(stderr, "dlerror: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    struct Command (*init_func)(void);
    *(void **) (&init_func) = dlsym(plugin, "init");
    struct Command to_load = (*init_func)();
    add_command(to_load);
    /*dlclose(plugin);*/
    fprintf(stderr, "plugin '%s' was successfully loaded\n", argv[1]);
    return EXIT_SUCCESS;
}
