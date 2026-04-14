#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "../include/CHESSlogging.h"
#include "../include/CHESSutil.h"


extern enum color *user_side;


FILE *new_logging(enum logging_t type) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    char date[255];
    struct passwd *pd = getpwuid(getuid());

    FILE *stream = NULL;
    switch (type) {
        case debugging: {
            if (strftime(date, sizeof date, "%F %H-%M-%S.log",
                        gmtime(&ts.tv_sec)))
                break;
            else 
                return NULL;
        }
        default: {
            if (strftime(date, sizeof date, "%F %H-%M-%S",
                        gmtime(&ts.tv_sec)))
                break;
            else 
                return NULL;
        }
    }
    char *directory;
    if (asprintf(&directory, "%s/.local/share/bvchess", pd->pw_dir) == -1) {
        free(directory);
        return NULL;
    }
    char *rp;
    if (!(rp = realpath(directory, NULL))) {
        if (mkdir(directory, 0755) != 0) {
            free(rp);
            free(directory);
            perror("Error: can't create logging directory\n");
            return NULL;
        }
        free(rp);
        free(directory);
        return new_logging(type);
    }
    char *full_name;
    if (asprintf(&full_name, "%s/%s", directory ,date) == -1) {
        free(full_name);
        free(rp);
        free(directory);
        return NULL;
    }
    else {
        stream = fopen(full_name, "w");
    }
    free(full_name);
    free(rp);
    free(directory);
    return stream;
}


static char *convert_pos_to_str(uint8_t *pos) {
    char *str = (char *)(malloc(sizeof(char) * 3));
    str[0] = *user_side == white ? ('a' + ((*pos - 1) % 8)) :
        ('h' - (*pos - 1) % 8);
    str[1] = *user_side == white ? '8' - ((*pos - 1) / 8) :
        ('1' + ((*pos - 1) / 8));
    str[2] = '\0';
    return str;
}



int8_t SIMPLE_new_record( FILE *stream, uint8_t *opos, uint8_t *npos) {
    static uint16_t nmove;
    if (!stream) {
        perror("File stream error\n");
        return -1;
    }
    char *line;
    char *opos_str = convert_pos_to_str(opos);
    char *npos_str = convert_pos_to_str(npos);
    if (asprintf(&line, "%d.%s %s ", ++nmove, opos_str, npos_str) == -1) {
        free(line);
        free(opos_str);
        free(npos_str);
        return -1;
    }
    else {
        fputs(line, stream);
        fflush(stream);
        free(line);
        free(opos_str);
        free(npos_str);
    }
    return 0;
}


// int8_t PGN_move_record(
//         FILE *stream, uint8_t *opos, uint8_t *npos, enum mstate state
//         ) {
//     static uint16_t nmove;
//     char *str  = 
// }


static FILE *get_static_debug_pt(uint8_t status) {
    static FILE *dstream;
    if (status == 'w')
        if (!dstream) dstream = new_logging(debugging);
    return dstream;
}


int8_t new_debug_record(char *fmt, ...) {
    FILE *dstream = get_static_debug_pt('w');
    if (dstream) {
        int32_t size = 0;
        va_list ap;
        char *line;
        va_start(ap, fmt);
        size = vsnprintf(line, size, fmt, ap);
        va_end(ap);
        if (size < 0) {
            perror("vsnprintf < 0");
            return -1;
        }
        size++;
        line = malloc(size);
        if (line == NULL) {
            perror("malloc is working failure");
            return -1;
        }

        va_start(ap, fmt);
        size = vsnprintf(line, size, fmt, ap);
        if (size < 0) {
            perror("vsnprintf < 0");
            return -1;
        }
        va_end(ap);
        printf("%s\n", line);
        fputs("DBG: ", dstream);
        fputs(line, dstream);
        fputs("\n", dstream);
        free(line);
    }
    else {
        perror("Incorrect open new debugging log");
        return -1;
    }
    return 0;
}


int32_t close_debug_stream() {
    FILE *test = get_static_debug_pt('c');
    if (test) return fclose(test);
    return 0;
}
