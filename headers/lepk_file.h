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

#ifndef LEPK_FILE_H
#define LEPK_FILE_H

#ifdef LEPK_FILE_STATIC
#define LEPKFILE static
#define LEPKFILEIMPL static
#else /* LEPK_FILE_STATIC */
#define LEPKFILE extern
#define LEPKFILEIMPL
#endif /* LEPK_FILE_STATIC */

typedef enum {
	LEPK_FILE_MODE_NORMAL,
	LEPK_FILE_MODE_BINARY,
} LepkFileMode;

LEPKFILE char *lepk_file_read(const char *filepath);
LEPKFILE int lepk_file_write(const char *filepath, const char *content, unsigned long length, LepkFileMode mode);
LEPKFILE int lepk_file_append(const char *filepath, const char *content, unsigned long length, LepkFileMode mode);

#endif /* LEPK_FILE_H */
