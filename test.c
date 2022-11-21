#define LEPK_DA_IMPLEMENTATION
#define LEPK_DA_TEST
#include "lepk_da.h"

#define LEPK_FILE_IMPLEMENTATION
#define LEPK_FILE_TEST
#include "lepk_file.h"

int main(void) {
	lepk_da_test();
	lepk_file_test();

	return 0;
}
