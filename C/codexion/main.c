#include "codexion.h"

int main(int c, char **v)
{
    t_data  *data;

    if (c != 9)
    {
        printf("%s", "Usage: number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler\n");
        return 1;
    }
    
    parser(v);
    data = init_data(v);
    start_simulation(data);
    return 0;
}