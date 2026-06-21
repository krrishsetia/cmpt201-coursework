#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement first fit
  struct header *current = free_list_ptr;
  while (current != NULL) {
    if (current->size > size) {
      break;
    }

    current = current->next;
  }

  return current->id;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t best_size = UINT64_MAX;
  // TODO: Implement best fit
  struct header *current = free_list_ptr;
  while (current != NULL) {
    if (best_size > current->size && current->size > size) {
      best_fit_id = current->id;
      best_size = current->size;
    }
    current = current->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t worst_size = 0;
  // TODO: Implement worst fit
  struct header *current = free_list_ptr;
  while (current != NULL) {
    if (worst_size < current->size && current->size > size) {
      worst_fit_id = current->id;
      worst_size = current->size;
    }
    current = current->next;
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // TODO: Print out the IDs
  printf("first_fit: %d\n", first_fit_id);
  printf("best_fit: %d\n", best_fit_id);
  printf("worst_fit: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

// free block above, free block below = null
// current = head
// while current != null {
// if(current-> next == free block)
//	free block above = current
// else if(current = free block)
// 	free block below = current -> next
// }
// if(free block above == null && free block below != null)
//	realloc(free block, sizeof(free block + free block below))
// else if (free block above != null && free block below == null)
//	realloc(free block above, sizeof(free block + free block above))
