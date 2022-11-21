#include "lepk_file.h"

#include <stdio.h>
#include <malloc.h>

LEPKFILEIMPL char *lepk_file_read(const char *filepath) {
	FILE *f = fopen(filepath, "rb");
	if (f == NULL) {
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *buffer = malloc(length + 1);
	if (buffer == NULL) {
		return NULL;
	}

	fread(buffer, length, 1, f);
	buffer[length] = '\0';

	fclose(f);

	return buffer;
}

LEPKFILEIMPL int lepk_file_write(const char *filepath, const char *content, unsigned long length, LepkFileMode mode) {
	char *str_mode = mode == LEPK_FILE_MODE_NORMAL ? "w" : "wb";
	FILE *f = fopen(filepath, str_mode);
	if (f == NULL) {
		return 1;
	}

	fwrite(content, length, 1, f);
	fclose(f);

	return 0;
}

LEPKFILEIMPL int lepk_file_append(const char *filepath, const char *content, unsigned long length, LepkFileMode mode) {
	char *str_mode = mode == LEPK_FILE_MODE_NORMAL ? "a" : "ab";
	FILE *f = fopen(filepath, str_mode);
	if (f == NULL) {
		return 1;
	}

	fwrite(content, length, 1, f);
	fclose(f);

	return 0;
}
