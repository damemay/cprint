#include "../cspin.h"

cspin* t;

int main() {
    // Allocate spinner and create thread:
    if(cspin_start(&t, "Processing...") < 0) exit(EXIT_FAILURE);

    sleep(1);

    // Cancel spinner thread and join in order to print something else:
    cspin_cancel(&t);

    printf("Something happened!\n");

    sleep(1);

    // After cancelling spinner, you can reuse the same cspin* handle with new message or pass NULL to reuse message already in memory:
    if(cspin_restart(&t, "Processing again...") < 0) exit(EXIT_FAILURE);

    sleep(1);

    // Cancel the spinner thread and free memory.
    // Reusing the handle after this call would require another cspin_start:
    cspin_free(&t);

    return EXIT_SUCCESS;
}
