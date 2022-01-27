#include <stdio.h>
#define XXH_INLINE_ALL 1
#include "xxhash.h" //--download-from--:https://github.com/Cyan4973/xxHash/blob/v0.8.0/xxhash.h

XXH128_canonical_t xxhash128_file(FILE* fp, void* buf, size_t bufSize) {
    XXH128_canonical_t canonical;
    XXH3_state_t* state = XXH3_createState();
    XXH3_128bits_reset(state);
    for(;;) {
        size_t size = fread(buf, 1, bufSize, fp);
        if(size == 0) {
            break;
        }
        XXH3_128bits_update(state, buf, size);
    }
    XXH128_hash_t digest = XXH3_128bits_digest(state);
    XXH128_canonicalFromHash(&canonical, digest);
    XXH3_freeState(state);
    return canonical;
}

int main(int argc, char* argv[]) {
    const size_t bufSize = 65536;
    void* buf = malloc(bufSize);
    if(!buf) {
        fprintf(stderr, "Failed to allocate %zd bytes of memory\n", bufSize);
        exit(EXIT_FAILURE);
    }
    for(int iarg = 1; iarg < argc; ++iarg) {
        const char* filename = argv[iarg];
        XXH128_canonical_t xxh128_canonical;
        {
            FILE* fp = fopen(filename, "rb");
            if(!fp) {
                fprintf(stderr, "Failed to open %s\n", filename);
                continue;
            }
            xxh128_canonical = xxhash128_file(fp, buf, bufSize);
            fclose(fp);
        }
        for(int i = 0; i < sizeof(xxh128_canonical.digest); ++i) {
            printf("%02x", xxh128_canonical.digest[i]);
        }
        printf("  %s\n", filename);
    }
    free(buf);
    return 0;
}
