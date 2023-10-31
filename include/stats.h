#ifndef STATS_H
#define STATS_H

/* #define INIT_STATS(s) {\ */
/*     s.user_ticks = 0; \ */
/*     s.system_ticks = 0; \ */
/*     s.blocked_ticks = 0; \ */
/*     s.ready_ticks = 0; \ */
/*     s.elapsed_total_ticks = 0; \ */
/*     s.total_trans = 0; \ */
/*     s.remaining_ticks = 0; \ */
/*     } */

/* Structure used by 'get_stats' function */
struct stats
{
    unsigned long user_ticks;
    unsigned long system_ticks;
    unsigned long blocked_ticks;
    unsigned long ready_ticks;
    unsigned long elapsed_total_ticks;
    unsigned long total_trans; /* Number of times the process has got the CPU: READY->RUN transitions */
    unsigned long remaining_ticks;
};

void INIT_STATS (struct stats *s);

void update_user_to_system_ticks ();
void update_system_to_user_ticks ();
void update_system_to_ready_ticks ();
void update_ready_to_system_ticks ();

#endif /* !STATS_H */
