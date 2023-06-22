#include "cspin.h"

int main() {
    cspin* t;
    cspin_start(&t, "Processing...");
    sleep(1);
    cspin_cancel(&t);
    printf("Something happened!\n");
    sleep(1);
    cspin_restart(&t, "Processing again...");
    sleep(1);
    cspin_free(&t);
    return EXIT_SUCCESS;
}
