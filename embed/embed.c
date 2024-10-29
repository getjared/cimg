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

int embed_png(const char *input_png, const char *output_png, int file_count, char **embed_files) {
    FILE *fp_in = fopen(input_png, "rb");
    if (!fp_in) return 1;
    FILE *fp_out = fopen(output_png, "wb");
    if (!fp_out) {
        fclose(fp_in);
        return 1;
    }
    unsigned char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), fp_in)) > 0) {
        fwrite(buffer, 1, bytes, fp_out);
    }
    const char marker[] = "EMBED";
    fwrite(marker, 1, sizeof(marker) - 1, fp_out);
    fwrite(&file_count, sizeof(int), 1, fp_out);
    for (int i = 0; i < file_count; i++) {
        const char *filename = embed_files[i];
        int name_len = strlen(filename);
        fwrite(&name_len, sizeof(int), 1, fp_out);
        fwrite(filename, 1, name_len, fp_out);
        FILE *fp_embed = fopen(filename, "rb");
        if (!fp_embed) {
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fseek(fp_embed, 0, SEEK_END);
        long embed_size = ftell(fp_embed);
        fseek(fp_embed, 0, SEEK_SET);
        unsigned char *file_buffer = (unsigned char *)malloc(embed_size);
        if (!file_buffer) {
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fread(file_buffer, 1, embed_size, fp_embed);
        unsigned long compress_bound = compressBound(embed_size);
        unsigned char *compressed_data = (unsigned char *)malloc(compress_bound);
        if (!compressed_data) {
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        if (compress(compressed_data, &compress_bound, file_buffer, embed_size) != Z_OK) {
            free(compressed_data);
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fwrite(&embed_size, sizeof(long), 1, fp_out);
        fwrite(&compress_bound, sizeof(unsigned long), 1, fp_out);
        fwrite(compressed_data, 1, compress_bound, fp_out);
        free(compressed_data);
        free(file_buffer);
        fclose(fp_embed);
    }
    fclose(fp_in);
    fclose(fp_out);
    printf("Files embedded into PNG successfully.\n");
    return 0;
}

int embed_jpeg(const char *input_jpeg, const char *output_jpeg, int file_count, char **embed_files) {
    FILE *fp_in = fopen(input_jpeg, "rb");
    if (!fp_in) return 1;
    FILE *fp_out = fopen(output_jpeg, "wb");
    if (!fp_out) {
        fclose(fp_in);
        return 1;
    }
    unsigned char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), fp_in)) > 0) {
        fwrite(buffer, 1, bytes, fp_out);
    }
    const char marker[] = "EMBED";
    fwrite(marker, 1, sizeof(marker) - 1, fp_out);
    fwrite(&file_count, sizeof(int), 1, fp_out);
    for (int i = 0; i < file_count; i++) {
        const char *filename = embed_files[i];
        int name_len = strlen(filename);
        fwrite(&name_len, sizeof(int), 1, fp_out);
        fwrite(filename, 1, name_len, fp_out);
        FILE *fp_embed = fopen(filename, "rb");
        if (!fp_embed) {
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fseek(fp_embed, 0, SEEK_END);
        long embed_size = ftell(fp_embed);
        fseek(fp_embed, 0, SEEK_SET);
        unsigned char *file_buffer = (unsigned char *)malloc(embed_size);
        if (!file_buffer) {
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fread(file_buffer, 1, embed_size, fp_embed);
        unsigned long compress_bound = compressBound(embed_size);
        unsigned char *compressed_data = (unsigned char *)malloc(compress_bound);
        if (!compressed_data) {
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        if (compress(compressed_data, &compress_bound, file_buffer, embed_size) != Z_OK) {
            free(compressed_data);
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fwrite(&embed_size, sizeof(long), 1, fp_out);
        fwrite(&compress_bound, sizeof(unsigned long), 1, fp_out);
        fwrite(compressed_data, 1, compress_bound, fp_out);
        free(compressed_data);
        free(file_buffer);
        fclose(fp_embed);
    }
    fclose(fp_in);
    fclose(fp_out);
    printf("Files embedded into JPEG successfully.\n");
    return 0;
}

int embed_bmp(const char *input_bmp, const char *output_bmp, int file_count, char **embed_files) {
    FILE *fp_in = fopen(input_bmp, "rb");
    if (!fp_in) return 1;
    FILE *fp_out = fopen(output_bmp, "wb");
    if (!fp_out) {
        fclose(fp_in);
        return 1;
    }
    unsigned char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), fp_in)) > 0) {
        fwrite(buffer, 1, bytes, fp_out);
    }
    const char marker[] = "EMBED";
    fwrite(marker, 1, sizeof(marker) - 1, fp_out);
    fwrite(&file_count, sizeof(int), 1, fp_out);
    for (int i = 0; i < file_count; i++) {
        const char *filename = embed_files[i];
        int name_len = strlen(filename);
        fwrite(&name_len, sizeof(int), 1, fp_out);
        fwrite(filename, 1, name_len, fp_out);
        FILE *fp_embed = fopen(filename, "rb");
        if (!fp_embed) {
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fseek(fp_embed, 0, SEEK_END);
        long embed_size = ftell(fp_embed);
        fseek(fp_embed, 0, SEEK_SET);
        unsigned char *file_buffer = (unsigned char *)malloc(embed_size);
        if (!file_buffer) {
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fread(file_buffer, 1, embed_size, fp_embed);
        unsigned long compress_bound = compressBound(embed_size);
        unsigned char *compressed_data = (unsigned char *)malloc(compress_bound);
        if (!compressed_data) {
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        if (compress(compressed_data, &compress_bound, file_buffer, embed_size) != Z_OK) {
            free(compressed_data);
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fwrite(&embed_size, sizeof(long), 1, fp_out);
        fwrite(&compress_bound, sizeof(unsigned long), 1, fp_out);
        fwrite(compressed_data, 1, compress_bound, fp_out);
        free(compressed_data);
        free(file_buffer);
        fclose(fp_embed);
    }
    fclose(fp_in);
    fclose(fp_out);
    printf("Files embedded into BMP successfully.\n");
    return 0;
}

int embed_gif(const char *input_gif, const char *output_gif, int file_count, char **embed_files) {
    FILE *fp_in = fopen(input_gif, "rb");
    if (!fp_in) return 1;
    FILE *fp_out = fopen(output_gif, "wb");
    if (!fp_out) {
        fclose(fp_in);
        return 1;
    }
    unsigned char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), fp_in)) > 0) {
        fwrite(buffer, 1, bytes, fp_out);
    }
    const char marker[] = "EMBED";
    fwrite(marker, 1, sizeof(marker) - 1, fp_out);
    fwrite(&file_count, sizeof(int), 1, fp_out);
    for (int i = 0; i < file_count; i++) {
        const char *filename = embed_files[i];
        int name_len = strlen(filename);
        fwrite(&name_len, sizeof(int), 1, fp_out);
        fwrite(filename, 1, name_len, fp_out);
        FILE *fp_embed = fopen(filename, "rb");
        if (!fp_embed) {
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fseek(fp_embed, 0, SEEK_END);
        long embed_size = ftell(fp_embed);
        fseek(fp_embed, 0, SEEK_SET);
        unsigned char *file_buffer = (unsigned char *)malloc(embed_size);
        if (!file_buffer) {
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fread(file_buffer, 1, embed_size, fp_embed);
        unsigned long compress_bound = compressBound(embed_size);
        unsigned char *compressed_data = (unsigned char *)malloc(compress_bound);
        if (!compressed_data) {
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        if (compress(compressed_data, &compress_bound, file_buffer, embed_size) != Z_OK) {
            free(compressed_data);
            free(file_buffer);
            fclose(fp_embed);
            fclose(fp_in);
            fclose(fp_out);
            return 1;
        }
        fwrite(&embed_size, sizeof(long), 1, fp_out);
        fwrite(&compress_bound, sizeof(unsigned long), 1, fp_out);
        fwrite(compressed_data, 1, compress_bound, fp_out);
        free(compressed_data);
        free(file_buffer);
        fclose(fp_embed);
    }
    fclose(fp_in);
    fclose(fp_out);
    printf("Files embedded into GIF successfully.\n");
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <input_image> <output_image> <file_to_embed1> [file_to_embed2 ...]\n", argv[0]);
        printf("Supported formats: PNG, JPEG, BMP, GIF\n");
        return 1;
    }
    const char *input_image = argv[1];
    const char *output_image = argv[2];
    int file_count = argc - 3;
    char **embed_files = &argv[3];
    ImageFormat format = detect_image_format(input_image);
    if (format == IMG_UNKNOWN) {
        printf("Unsupported or unknown image format.\n");
        return 1;
    }
    int result = 1;
    switch (format) {
        case IMG_PNG:
            result = embed_png(input_image, output_image, file_count, embed_files);
            break;
        case IMG_JPEG:
            result = embed_jpeg(input_image, output_image, file_count, embed_files);
            break;
        case IMG_BMP:
            result = embed_bmp(input_image, output_image, file_count, embed_files);
            break;
        case IMG_GIF:
            result = embed_gif(input_image, output_image, file_count, embed_files);
            break;
        default:
            printf("Unsupported image format.\n");
            return 1;
    }
    return result;
}
