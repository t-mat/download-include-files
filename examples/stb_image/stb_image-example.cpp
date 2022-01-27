#include <stdio.h>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_STDIO
                        // note: Unfortunately, https://github.com/nothings/stb.git doesn't have any tag.
#include "stb_image.h"  //--download-from--:https://github.com/nothings/stb/blob/af1a5bc352164740c1cc1354942b1c6b72eacb8a/stb_image.h

std::vector<uint8_t> loadFile(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if(!fp) {
        return {};
    }
    fseek(fp, 0, SEEK_END);
    std::vector<uint8_t> src(ftell(fp));
    fseek(fp, 0, SEEK_SET);
    const size_t rdSize = fread(src.data(), 1, src.size(), fp);
    fclose(fp);
    if(rdSize != src.size()) {
        return {};
    }
    return src;
}

int main() {
    const char* inpFilename = "test.png";

    int width = 0, height = 0, numChannels = 0;
    {
        const std::vector<uint8_t> src = loadFile(inpFilename);
        if(src.empty()) {
            printf("Failed to read %s\n", inpFilename);
            exit(EXIT_FAILURE);
        }

        const void* data = src.data();
        const int dataSize = (int) src.size();
        auto* dataUc = reinterpret_cast<const stbi_uc*>(data);
        stbi_uc* img = stbi_load_from_memory(dataUc, dataSize, &width, &height, &numChannels, 0);
        if(img) {
            stbi_image_free(img);
        }
    }
    printf("inpFilename=%s, width=%d, height=%d, numChannels=%d\n", inpFilename, width, height, numChannels);
}
