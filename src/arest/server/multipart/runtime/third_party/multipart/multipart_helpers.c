// Portable binary search: first offset of needle in hay[start..len), or -1.
// Hand-rolled byte loop (no memmem dependency) so it builds on any libc.
// Eiwa String ops are strstr-based and stop at NULs — this one doesn't.
//
// NOTE: returns long long, not int — eiwac zero-extends 32-bit C int
// returns to 64-bit (so a C -1 arrives as 4294967295). 64-bit needs no
// extension and round-trips exact.
long long eiwa_memmem(const char *hay, int hay_len, int start, const char *needle, int needle_len) {
    if (needle_len <= 0) {
        return (start <= hay_len) ? start : -1;
    }
    if (start < 0) {
        start = 0;
    }
    long long i = start;
    while (i + needle_len <= hay_len) {
        int j = 0;
        while (j < needle_len && hay[i + j] == needle[j]) {
            j++;
        }
        if (j == needle_len) {
            return i;
        }
        i++;
    }
    return -1;
}
