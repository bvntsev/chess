// #include "../include/CHESSsession.h"
#include "../include/test.h"
#include "../include/util.h"
// #include "../include/CHESScli.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


int8_t (*CLIuser_input_func[])(const struct chess *) = {
    NULL
};


int32_t
test_chess_movement ()
{

    printf("==============STARTING TEST==============\n");
    for (int64_t iter = 0; iter < CLIUSER_INPUT_FUNC_LEN; ++iter) {
        // if (CLIuser_input_func[iter](global) == CTEST_FAILURE_EXIT) {
        //     printf("==============FAILURE TEST==============\n");
        //     continue;
        printf("==============SUCCESSFUL TEST==============\n");
    }
    return CTEST_SUCESS_EXIT;
}
