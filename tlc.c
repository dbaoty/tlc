#include <stdio.h>
#include <string.h>

// TODO: error handling
// TODO: check arguments for files AND directories

enum ExitCode {
	NORMAL = 0,
	BAD_ARGUMENT_COUNT,
};

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: ...");
		return 1;
	}

	int idx, line_count, line_count_total = 0;
	for (idx = 1; idx < argc; idx++) {
		FILE *file = fopen(argv[idx], "r");
		if (!file) {
			fprintf(stderr, "file `%s` not found\n", argv[idx]);
			return 1;
		}

		// TODO: detect and exclude binary file types

		fseek(file, 0L, SEEK_END);
		long int length = ftell(file);
		rewind(file);
		char buffer[length++];

		line_count = 0;
		while (!feof(file)) {
			while (fgets(buffer, sizeof(buffer), file)) {
				if (strlen(buffer) > 1) line_count = line_count + 1;
			}

			printf("lines: %d\tfile: `%s`\n", line_count, argv[idx]) ;
		}
		line_count_total = line_count_total + line_count;
		fclose(file);
	}

	if (argc > 2) {
		printf("\n%d lines in %d files\n", line_count_total, argc - 1);
	}

	return NORMAL;
}
