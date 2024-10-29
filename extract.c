#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

typedef enum {
    IMG_PNG,
    IMG_JPEG,
    IMG_BMP,
    IMG_GIF,
    IMG_UNKNOWN
} ImageFormat;

ImageFormat detect_image_format(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return IMG_UNKNOWN;
    unsigned char buffer[8];
    size_t read_bytes = fread(buffer, 1, 8, fp);
    fclose(fp);
    if (read_bytes >= 8) {
        if (memcmp(buffer, "\x89PNG\r\n\x1A\n", 8) == 0) return IMG_PNG;
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF) return IMG_JPEG;
        if (buffer[0] == 'B' && buffer[1] == 'M') return IMG_BMP;
        if (memcmp(buffer, "GIF87a", 6) == 0 || memcmp(buffer, "GIF89a", 6) == 0) return IMG_GIF;
    }
    return IMG_UNKNOWN;
}

int extract_data(const char *input_image) {
    FILE *fp_in = fopen(input_image, "rb");
    if (!fp_in) return 1;
    fseek(fp_in, 0, SEEK_END);
    long file_size = ftell(fp_in);
    if (file_size == -1L) {
        fclose(fp_in);
        return 1;
    }
    unsigned char *buffer = (unsigned char *)malloc(file_size);
    if (!buffer) {
        fclose(fp_in);
        return 1;
    }
    rewind(fp_in);
    size_t read_bytes = fread(buffer, 1, file_size, fp_in);
    fclose(fp_in);
    const char marker[] = "EMBED";
    size_t marker_len = strlen(marker);
    unsigned char *ptr = buffer;
    int found = 0;
    while (ptr <= buffer + file_size - marker_len) {
        if (memcmp(ptr, marker, marker_len) == 0) {
            found = 1;
            break;
        }
        ptr++;
    }
    if (!found) {
        printf("No embedded data found.\n");
        free(buffer);
        return 1;
    }
    ptr += marker_len;
    int file_count = 0;
    memcpy(&file_count, ptr, sizeof(int));
    ptr += sizeof(int);
    for (int i = 0; i < file_count; i++) {
        int name_len = 0;
        memcpy(&name_len, ptr, sizeof(int));
        ptr += sizeof(int);
        char *filename = (char *)malloc(name_len + 1);
        memcpy(filename, ptr, name_len);
        filename[name_len] = '\0';
        ptr += name_len;
        long embed_size = 0;
        memcpy(&embed_size, ptr, sizeof(long));
        ptr += sizeof(long);
        unsigned long compress_size = 0;
        memcpy(&compress_size, ptr, sizeof(unsigned long));
        ptr += sizeof(unsigned long);
        unsigned char *compressed_data = (unsigned char *)malloc(compress_size);
        if (!compressed_data) {
            free(filename);
            free(buffer);
            return 1;
        }
        memcpy(compressed_data, ptr, compress_size);
        ptr += compress_size;
        unsigned char *decompressed_data = (unsigned char *)malloc(embed_size);
        if (!decompressed_data) {
            free(compressed_data);
            free(filename);
            free(buffer);
            return 1;
        }
        if (uncompress(decompressed_data, &embed_size, compressed_data, compress_size) != Z_OK) {
            free(decompressed_data);
            free(compressed_data);
            free(filename);
            free(buffer);
            return 1;
        }
        FILE *fp_out = fopen(filename, "wb");
        if (!fp_out) {
            free(decompressed_data);
            free(compressed_data);
            free(filename);
            free(buffer);
            return 1;
        }
        fwrite(decompressed_data, 1, embed_size, fp_out);
        fclose(fp_out);
        printf("Extracted: %s (%ld bytes)\n", filename, embed_size);
        free(decompressed_data);
        free(compressed_data);
        free(filename);
    }
    free(buffer);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_image>\n", argv[0]);
        return 1;
    }
    const char *input_image = argv[1];
    ImageFormat format = detect_image_format(input_image);
    if (format == IMG_UNKNOWN) {
        printf("Unsupported or unknown image format.\n");
        return 1;
    }
    int result = extract_data(input_image);
    return result;
}
