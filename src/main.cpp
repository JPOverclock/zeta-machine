#include "machine.h"

int main() {
    zm::Machine machine {};

    machine.run("../resources/stories/v5/Planetfall R10-880531.z5");
    return 0;
}