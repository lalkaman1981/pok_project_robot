#include <iostream>
#include "header.h"



extern "C" int app_main() {
    std::cout << "Calling the test function from C" << std::endl;

    // Call the C function
    test();
    
    create_dir(MOUNT_POINT "/mydir");
    list_dir(MOUNT_POINT, 0);

    write_file(MOUNT_POINT "/oleg.gs", "chinazes\nchis");
    append_file(MOUNT_POINT "/oleg.gs", "oleg\n");
    read_file(MOUNT_POINT "/oleg.gs");

    read_file(MOUNT_POINT "/oleg.gs");

    std::cout << "Test function execution completed" << std::endl;
    return 0;
}
