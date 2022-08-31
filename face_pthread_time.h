#ifndef WIN_PTHREADS_TIME_H
#define WIN_PTHREADS_TIME_H

/* Posix timers are supported */
#ifndef _POSIX_TIMERS
#define _POSIX_TIMERS           200809L
#endif

/* Monotonic clocks are available.  */
#ifndef _POSIX_MONOTONIC_CLOCK
#define _POSIX_MONOTONIC_CLOCK  200809L
#endif

/* CPU-time clocks are available.  */
#ifndef _POSIX_CPUTIME
#define _POSIX_CPUTIME          200809L
#endif

/* Clock support in threads are available.  */
#ifndef _POSIX_THREAD_CPUTIME
#define _POSIX_THREAD_CPUTIME   200809L
#endif

#ifndef __clockid_t_defined
typedef int clockid_t;
#define __clockid_t_defined 1
#endif  /* __clockid_t_defined */

#ifndef TIMER_ABSTIME
#define TIMER_ABSTIME   1
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME              0
#endif

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC             1
#endif

#ifndef CLOCK_PROCESS_CPUTIME_ID
#define CLOCK_PROCESS_CPUTIME_ID    2
#endif

#ifndef CLOCK_THREAD_CPUTIME_ID
#define CLOCK_THREAD_CPUTIME_ID     3
#endif

#endif