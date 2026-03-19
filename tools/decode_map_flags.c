#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned long mask;
	const char *str;
} val_str_pair_t;

#define HUGETLB_FLAG_ENCODE_SHIFT	26
#define HUGETLB_FLAG_ENCODE_MASK	0x3f

#define HUGETLB_FLAG_16KB	14UL
#define HUGETLB_FLAG_64KB	16UL
#define HUGETLB_FLAG_512KB	19UL
#define HUGETLB_FLAG_1MB	20UL
#define HUGETLB_FLAG_2MB	21UL
#define HUGETLB_FLAG_8MB	23UL
#define HUGETLB_FLAG_16MB	24UL
#define HUGETLB_FLAG_32MB	25UL
#define HUGETLB_FLAG_256MB	28UL
#define HUGETLB_FLAG_512MB	29UL
#define HUGETLB_FLAG_1GB	30UL
#define HUGETLB_FLAG_2GB	31UL
#define HUGETLB_FLAG_16GB	34UL

static val_str_pair_t hstate_pairs[] = {
	{HUGETLB_FLAG_16GB, "MAP_HUGE_16GB"},
	{HUGETLB_FLAG_2GB, "MAP_HUGE_2GB"},
	{HUGETLB_FLAG_1GB, "MAP_HUGE_1GB"},
	{HUGETLB_FLAG_512MB, "MAP_HUGE_512MB"},
	{HUGETLB_FLAG_256MB, "MAP_HUGE_256MB"},
	{HUGETLB_FLAG_32MB, "MAP_HUGE_32MB"},
	{HUGETLB_FLAG_16MB, "MAP_HUGE_16MB"},
	{HUGETLB_FLAG_8MB, "MAP_HUGE_8MB"},
	{HUGETLB_FLAG_2MB, "MAP_HUGE_2MB"},
	{HUGETLB_FLAG_1MB, "MAP_HUGE_1MB"},
	{HUGETLB_FLAG_512KB, "MAP_HUGE_512KB"},
	{HUGETLB_FLAG_64KB, "MAP_HUGE_64KB"},
	{HUGETLB_FLAG_16KB, "MAP_HUGE_16KB"},
};

static val_str_pair_t pairs[] = {
	{0x4000000, "MAP_UNINITIALIZED"},
	{0x100000, "MAP_FIXED_NOREPLACE"},
	{0x80000, "MAP_SYNC"},
	{0x40000, "MAP_HUGETLB"},
	{0x20000, "MAP_STACK"},
	{0x10000, "MAP_NONBLOCK"},
	{0x8000, "MAP_POPULATE"},
	{0x4000, "MAP_NORESERVE"},
	{0x2000, "MAP_LOCKED"},
	{0x1000, "MAP_EXECUTABLE"},
	{0x800, "MAP_DENYWRITE"},
	{0x100, "MAP_GROWSDOWN"},
	{0x20, "MAP_ANONYMOUS"},
	{0x10, "MAP_FIXED"},
	{0x8, "MAP_DROPPABLE"},
	{0x3, "MAP_SHARED_VALIDATE"},
	{0x2, "MAP_PRIVATE"},
	{0x1, "MAP_SHARED"},
};

#define ARR_LENGTH(_arr) (sizeof(_arr)/sizeof(_arr[0]))

static unsigned long print_decoded_huge_map_flags(unsigned long val)
{
	unsigned long mask, decoded;
	int i;

	mask = HUGETLB_FLAG_ENCODE_MASK;
	mask <<= HUGETLB_FLAG_ENCODE_SHIFT;
	mask = ~mask;

	decoded = val;
	decoded >>= HUGETLB_FLAG_ENCODE_SHIFT;
	decoded &= HUGETLB_FLAG_ENCODE_MASK;

	for (i = 0; i < ARR_LENGTH(hstate_pairs); i++) {
		const val_str_pair_t *pair = &hstate_pairs[i];

		if (decoded == pair->mask) {
			printf("%s|", pair->str);
			break;
		}
	}

	return val & mask;
}

static void print_decoded_map_flags(unsigned long val)
{
	int i;

	val = print_decoded_huge_map_flags(val);

	for (i = 0; i < ARR_LENGTH(pairs); i++) {
		const val_str_pair_t *pair = &pairs[i];
		const unsigned long mask = pair->mask;

		if ((val & mask) == mask) {
			printf("%s|", pair->str);
			val &= ~mask;
		}
	}
	/* Print remaining. */
	printf("unknown=0x%lx\n", val);
}

int main(int argc, char **argv)
{
	unsigned long val;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <hex map value>\n", argv[0]);
		return EXIT_FAILURE;
	}

	val = strtoul(argv[1], NULL, 16);
	if (val == LONG_MIN || val == LONG_MAX) {
		perror("strtoul");
		return EXIT_FAILURE;
	}

	print_decoded_map_flags(val);

	return EXIT_SUCCESS;
}
