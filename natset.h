// natset.h: functions für sets of natural numbers (Header file)
#include <stdint.h>

struct natset{
  uint16_t  array_size; // size of bit array (no. of uint8_t)
  uint8_t*  bitarray;   // pointer to bit array
};
typedef struct natset natset_t;

typedef enum  {NSS_ERROR=-1, NSS_OK=0} ns_status_t;

ns_status_t ns_init(natset_t *set, unsigned int largest);
void        ns_free(natset_t *set);

ns_status_t ns_add(natset_t *set, unsigned int element);
ns_status_t ns_remove(natset_t *set, unsigned int element);
ns_status_t ns_cut(natset_t *set1, natset_t *set2);
ns_status_t ns_join(natset_t *set1, natset_t *set2);
ns_status_t print_set(natset_t *set);
