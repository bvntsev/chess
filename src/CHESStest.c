#include "../include/CLIsession.h"
#include "../include/CHESStest.h"
#include "../include/chessUtilities.h"
#include <stddef.h>
#include <stdio.h>

static int32_t CLIcheck_moves_without_rules(const struct ChessGame *global) {
    return CTEST_SUCESS_EXIT;
}


int32_t (*CLIuser_input_func[])(const struct ChessGame *) = {
    CLIcheck_moves_without_rules
};
char *CLIuser_input_func_name[] = {
    "Moves without rules\n"
};


int32_t test_CLIuser_input(const struct ChessGame *global) {
    printf("==============STARTING TEST==============\n");
    for (int64_t iter = 0; iter < CLIUSER_INPUT_FUNC_LEN; ++iter) {
        printf("#%ld --- %s", iter, CLIuser_input_func_name[iter]);
        if (CLIuser_input_func[iter](global) == CTEST_FAILURE_EXIT) {
            printf("==============FAILURE TEST==============\n");
            continue;
        }
        printf("==============SUCCESSFUL TEST==============\n");
    }
    return CTEST_SUCESS_EXIT;
}
