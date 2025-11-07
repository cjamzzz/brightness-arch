#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BRIGHTNESS_FILE "/sys/class/backlight/amdgpu_bl1/brightness"
#define MAX_BRIGHTNESS_FILE "/sys/class/backlight/amdgpu_bl1/max_brightness"

int get_max_brightness() {
    FILE *fp = fopen(MAX_BRIGHTNESS_FILE, "r");
    if (!fp) {
        perror("Failed to open max_brightness");
        exit(1);
    }
    int max;
    fscanf(fp, "%d", &max);
    fclose(fp);
    return max;
}

int get_current_brightness() {
    FILE *fp = fopen(BRIGHTNESS_FILE, "r");
    if (!fp) {
        perror("Failed to open brightness");
        exit(1);
    }
    int val;
    fscanf(fp, "%d", &val);
    fclose(fp);
    return val;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        int current = get_current_brightness();
        int max = get_max_brightness();
        printf("Current brightness: %.1f%%\n", 100.0 * current / max);
        return 0;
    }

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <percentage (1-100)>\n", argv[0]);
        return 1;
    }

    int percent = atoi(argv[1]);
    if (percent < 1 || percent > 100) {
        fprintf(stderr, "Invalid percentage. Must be between 1 and 100.\n");
        return 1;
    }

    int max = get_max_brightness();
    int value = (percent * max) / 100;

    int fd = open(BRIGHTNESS_FILE, O_WRONLY);
    if (fd == -1) {
        perror("Error opening brightness file");
        return 1;
    }

    char buffer[16];
    int len = snprintf(buffer, sizeof(buffer), "%d\n", value);

    if (write(fd, buffer, len) == -1) {
        perror("Error writing brightness");
        close(fd);
        return 1;
    }

    close(fd);
    printf("Brightness set to %d%%\n", percent);
    return 0;
}
