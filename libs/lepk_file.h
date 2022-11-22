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
 * Filesystem interaction.
 *
 * Add:
 *     #define LEPK_FILE_IMPLEMENTATION
 * in one C or C++ file, before #include "lepk_file.h", to create the implementation.
 *
 * If LEPK_FILE_STATIC is defined the implementation will be local to a single file only.
 */

#ifndef LEPK_FILE_H
#define LEPK_FILE_H

#ifdef LEPK_FILE_STATIC
#define LEPKFILE static
#define LEPKFILEIMPL static
#else /* LEPK_FILE_STATIC */
#define LEPKFILE extern
#define LEPKFILEIMPL
#endif /* LEPK_FILE_STATIC */

#include <stdbool.h>

/* How a file should be opened. */
typedef enum {
	/* Let windows convert/append some characters. */
	LEPK_FILE_MODE_NORMAL,
	/* Keep everything the same. */
	LEPK_FILE_MODE_BINARY,
} LepkFileMode;

/* Status code for functions. */
typedef enum {
	/* OK. */
	LEPK_FILE_STATUS_OK,
	/* File failed to be opened or created. */
	LEPK_FILE_STATUS_UNABLE_TO_OPEN_CREATE,
	/* OS is out of memory. */
	LEPK_FILE_STATUS_OUT_OF_MEMORY,
	/* Deleting file failed. */
	LEPK_FILE_STATUS_REMOVE_FAILED,
} LepkFileStatus;

/* Read file and return its contents. NULL return value means function failed, read status for more specific error. */
LEPKFILE char *lepk_file_read(const char *filepath, LepkFileStatus *status);
/* Write content to file at filepath. */
LEPKFILE LepkFileStatus lepk_file_write(const char *filepath, const char *content, unsigned long length, LepkFileMode mode);
/* Append conntent to file at filepath. */
LEPKFILE LepkFileStatus lepk_file_append(const char *filepath, const char *content, unsigned long length, LepkFileMode mode);
/* Create file at filepath. */
LEPKFILE LepkFileStatus lepk_file_create(const char *filepath);
/* Remove file at filepath. */
LEPKFILE LepkFileStatus lepk_file_remove(const char *filepath);
/* Check if file exists at filepath. */
LEPKFILE bool lepk_file_exists(const char *filepath);

#ifdef LEPK_FILE_TEST

#include <malloc.h>
#include <assert.h> 

static void lepk_file_test(void) {
	LepkFileStatus status;

	status = lepk_file_create("file_test.txt");
	assert(status == LEPK_FILE_STATUS_OK && "lepk_file_create failed.");

	bool exists = lepk_file_exists("file_test.txt");
	assert(exists && "lepk_file_exists failed.");

	status = lepk_file_write("file_test.txt", "Hello World!", 12, LEPK_FILE_MODE_BINARY);
	assert(status == LEPK_FILE_STATUS_OK && "lepk_file_write failed.");

	status = lepk_file_append("file_test.txt", "World Hello!", 12, LEPK_FILE_MODE_BINARY);
	assert(status == LEPK_FILE_STATUS_OK && "lepk_file_append failed.");

	char *content = lepk_file_read("file_test.txt", &status);
	assert(strcmp(content, "Hello World!World Hello!") == 0 && "lepk_file_read failed!");

	lepk_file_remove("file_test.txt");
	exists = lepk_file_exists("file_test.txt");
	assert(!exists && "lepk_file_remove failed.");
}

#endif /* LEPK_FILE_TEST */
#ifdef LEPK_FILE_IMPLEMENTATION
#include <stdio.h>
#include <malloc.h>

#define LEPK__FILE_SET_STATUS(p, s) do {if ((p)) { *(p) = (s); }} while (0)

LEPKFILEIMPL char *lepk_file_read(const char *filepath, LepkFileStatus *status) {
	FILE *f = fopen(filepath, "rb");
	if (f == NULL) {
		LEPK__FILE_SET_STATUS(status, LEPK_FILE_STATUS_UNABLE_TO_OPEN_CREATE);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *buffer = malloc(length + 1);
	if (buffer == NULL) {
		LEPK__FILE_SET_STATUS(status, LEPK_FILE_STATUS_OUT_OF_MEMORY);
		return NULL;
	}

	fread(buffer, length, 1, f);
	buffer[length] = '\0';

	fclose(f);

	LEPK__FILE_SET_STATUS(status, LEPK_FILE_STATUS_OK);
	return buffer;
}

LEPKFILEIMPL LepkFileStatus lepk_file_write(const char *filepath, const char *content, unsigned long length, LepkFileMode mode) {
	char *str_mode = mode == LEPK_FILE_MODE_NORMAL ? "w" : "wb";
	FILE *f = fopen(filepath, str_mode);
	if (f == NULL) {
		return LEPK_FILE_STATUS_UNABLE_TO_OPEN_CREATE;
	}

	fwrite(content, length, 1, f);
	fclose(f);

	return LEPK_FILE_STATUS_OK;
}

LEPKFILEIMPL LepkFileStatus lepk_file_append(const char *filepath, const char *content, unsigned long length, LepkFileMode mode) {
	char *str_mode = mode == LEPK_FILE_MODE_NORMAL ? "a" : "ab";
	FILE *f = fopen(filepath, str_mode);
	if (f == NULL) {
		return LEPK_FILE_STATUS_UNABLE_TO_OPEN_CREATE;
	}

	fwrite(content, length, 1, f);
	fclose(f);

	return LEPK_FILE_STATUS_OK;
}

LEPKFILEIMPL LepkFileStatus lepk_file_create(const char *filepath) {
	FILE *f = fopen(filepath, "wb");
	if (f == NULL) {
		return LEPK_FILE_STATUS_UNABLE_TO_OPEN_CREATE;
	}

	return LEPK_FILE_STATUS_OK;
}

LEPKFILEIMPL LepkFileStatus lepk_file_remove(const char *filepath) {
	if (remove(filepath) != 0) {
		return LEPK_FILE_STATUS_REMOVE_FAILED;
	}
	return LEPK_FILE_STATUS_OK;
}

LEPKFILEIMPL bool lepk_file_exists(const char *filepath) {
	return fopen(filepath, "r") != NULL;
}
#endif /*LEPK_FILE_IMPLEMENTATION*/
#endif /* LEPK_FILE_H */
