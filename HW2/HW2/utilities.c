#include <time.h>

char *Ggettimestamp(void)
{
  time_t timestamp;

  timestamp = time(0);
  return (char *) ctime(&timestamp);
}
