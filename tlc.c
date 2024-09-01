#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

enum ExitCode {
    NORMAL = 0,
    FILE_NOT_FOUND,
};

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stdout, "Usage: ./tlc FILE ...\nor: ./tlc *\n");
        return NORMAL;
    }

    int idx, line_count, line_count_total = 0, file_count = 0;
    for (idx = 1; idx < argc; idx++) {
        FILE *file = fopen(argv[idx], "r");
        if (!file) {
            fprintf(stderr, "file `%s` not found\n", argv[idx]);
            return FILE_NOT_FOUND;
        }

        // the following check is a bypass until
        // checking directories is implemented
        struct stat path_stat;
        stat(argv[idx], &path_stat);
        if (S_ISREG(path_stat.st_mode) == 0) continue;

        // find out how much space we need in the line buffer
        fseek(file, 0L, SEEK_END);
        long int length = ftell(file);
        rewind(file);
        char buffer[length++];

        line_count = 0;

        // begin looping over file until end of file is hit
        // if the beginning 2-4 bytes of the file contain 'E', 'L', 'F',
        // the is_bin flag is triggered, the file is considered a binary
        // and is skipped. otherwise, increment line_count for each line
        // containing at least a single character in the file.

        // TODO: is there a better way to detect binary files?

        while (!feof(file)) {
            int is_bin = 0;
            while (fgets(buffer, sizeof(buffer), file)) {
                // detect binary files and silently skip these files
                if (buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F') {
                    is_bin = 1;
                    break;
                }
                if (strlen(buffer) > 1) line_count = line_count + 1;
            }

            if (is_bin) break;

            file_count = file_count + 1;
            fprintf(stdout, "lines: %d\tfile: `%s`\n", line_count, argv[idx]);
        }
        line_count_total = line_count_total + line_count;
        fclose(file);
    }

    if (file_count > 1) {
        printf("\n%d lines detected in %d files\n", line_count_total, file_count);
    }

    return NORMAL;
}
