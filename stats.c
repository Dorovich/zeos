#include <stats.h>
#include <sched.h>
#include <utils.h>

void INIT_STATS (struct stats *s)
{
    s->user_ticks = 0;
    s->system_ticks = 0;
    s->blocked_ticks = 0;
    s->ready_ticks = 0;
    s->elapsed_total_ticks = 0;
    s->total_trans = 0;
    s->remaining_ticks = 0;
}

void update_user_to_system_ticks ()
{
    current()->stats.user_ticks += get_ticks() - current()->stats.elapsed_total_ticks;
    current()->stats.elapsed_total_ticks = get_ticks();
}

void update_system_to_user_ticks ()
{
    current()->stats.system_ticks += get_ticks() - current()->stats.elapsed_total_ticks;
    current()->stats.elapsed_total_ticks = get_ticks();
}

void update_system_to_ready_ticks ()
{
    current()->stats.system_ticks += get_ticks() - current()->stats.elapsed_total_ticks;
    current()->stats.elapsed_total_ticks = get_ticks();
}

void update_ready_to_system_ticks ()
{
    current()->stats.ready_ticks += get_ticks() - current()->stats.elapsed_total_ticks;
    current()->stats.elapsed_total_ticks = get_ticks();
}
