/* Version: 1.0 */

/*
 * MIT License
 * 
 * Copyright (c) 2022 Linus Erik Pontus Kåreblom
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Compile one header file and a source file into a single header file.
 * Note: Make sure the first "#include" in the source file is the include for the header since this line will be removed.
 *
 * Usage:
 * lepkc [SOURCE] [HEADER] [DEFINE] [OUTPUT]
 * Source:
 *     .c implementation file.
 * Header:
 *     .h file.
 * Define:
 *     Implementation define.
 * Output:
 *     Output file.
 */

#include "lepk_type.h"
#define LEPK_FILE_IMPLEMENATION
#include "lepk_file.h"
#define LEPK_DA_IMPLEMENTATION
#include "lepk_da.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *program) {
	printf("Usage: %s [SOURCE] [HEADER] [DEFINE] [OUTPUT]\n", program);
	printf("Source:\n    Source C file.\n");
	printf("Header:\n    Header file.\n");
	printf("Define:\n    What the user must define to create the implementation.\n");
	printf("Output:\n    Output file.\n");
	exit(1);
}

static void write_impl(const char *output_filepath, const char *source, const char *def, U32 first_header, U32 first_header_end) {
		lepk_file_append(output_filepath, "#ifdef ", 7, LEPK_FILE_MODE_BINARY);
		lepk_file_append(output_filepath, def, strlen(def), LEPK_FILE_MODE_BINARY);
		lepk_file_append(output_filepath, "\n", 1, LEPK_FILE_MODE_BINARY);

		lepk_file_append(output_filepath, source, first_header, LEPK_FILE_MODE_BINARY);
		lepk_file_append(output_filepath, source + first_header_end, strlen(source + first_header_end), LEPK_FILE_MODE_BINARY);

		lepk_file_append(output_filepath, "#endif /*", 9, LEPK_FILE_MODE_BINARY);
		lepk_file_append(output_filepath, def, strlen(def), LEPK_FILE_MODE_BINARY);
		lepk_file_append(output_filepath, "*/", 2, LEPK_FILE_MODE_BINARY);
		lepk_file_append(output_filepath, "\n", 1, LEPK_FILE_MODE_BINARY);
}

I32 main(I32 argc, char **argv) {
	/* No arguments. */
	if (argc != 5) {
		usage(argv[0]);
	}

	const char *source_filepath       = argv[1];
	const char *header_filepath       = argv[2];
	const char *implementation_define = argv[3];
	const char *output_filepath       = argv[4];

	char *source = lepk_file_read(source_filepath);
	char *header = lepk_file_read(header_filepath);

	/*
	 * Check if "#pragma once" is present.
	 * Find last "#endif".
	 */
	B8 is_pragma = false;
	U32 last_endif = 0;
	for (U32 i = 0; header[i] != '\0'; i++) {
		if (header[i] == '#') {
			if (strncmp(header + i, "#endif", 6) == 0) {
				last_endif = i;
				i += 6;
			} else if (strncmp(header + i, "#pragma once", 12) == 0) {
				is_pragma = true;
				break;
			}
		}
	}


	/* Find the first "#include" which should be the header. */
	I32 first_header = -1;
	U32 first_header_end = 0;
	for (U32 i = 0; source[i] != '\0'; i++) {
		if (strncmp(source + i, "#include", 8) == 0 && first_header == -1) {
			first_header = i;
			for (; source[i] != '\n'; i++);
			for (; source[i] == '\n'; i++);
			first_header_end = i;
			break;
		}
	}

	/* Create final file. */
	if (is_pragma) {
		lepk_file_write(output_filepath, header, strlen(header), LEPK_FILE_MODE_BINARY);

		write_impl(output_filepath, source, implementation_define, first_header, first_header_end);
	} else {
		lepk_file_write(output_filepath, header, last_endif, LEPK_FILE_MODE_BINARY);

		write_impl(output_filepath, source, implementation_define, first_header, first_header_end);

		lepk_file_append(output_filepath, header + last_endif, strlen(header + last_endif), LEPK_FILE_MODE_BINARY);
	}

	free(source);
	free(header);

	return 0;
}
