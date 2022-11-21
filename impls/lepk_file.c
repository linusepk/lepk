#include "lepk_file.h"

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
