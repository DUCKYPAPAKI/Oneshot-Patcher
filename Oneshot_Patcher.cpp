#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <unistd.h>

const char drm_name[] = "libdrm.so.2";
const char lib_gl_name[] = "libGLdispatch.so.0";
const char lib_stdc_name[] = "libstdc++.so.6";
char default_oneshot[256];

char lib_paths[3][256];
int found_libs = 0;
char answer;
char link_answer;

int custom_system(const char* command) {
    return system(command);
}

void lib_handler(const char lib_name[], char lib_output[]) {
    char lib_path[512];
    std::sprintf(lib_path, "%s/%s", default_oneshot, lib_name);

    if (access(lib_path, F_OK) == 0) {
        std::cout << lib_name << " was found at " << lib_path << std::endl;
        std::strcpy(lib_output, lib_path);
        found_libs++;
    } else {
        std::cout << lib_name << " was not found" << std::endl;
    }
}

void ui() {
    if (found_libs == 3 || (found_libs > 0 && found_libs <= 3)) {
        std::cout << "Do you want to proceed with patching Oneshot? Y/N" << std::endl;
        std::cin >> answer;

        if (answer == 'Y' || answer == 'y') {
            for (int i = 0; i < 3; ++i) remove(lib_paths[i]);
            custom_system("ln -sf /usr/lib32/librt.so.1 ~/.local/share/Steam/steamapps/common/OneShot");
        } else if (answer == 'N' || answer == 'n') {
            std::cout << "Oneshot was not patched." << std::endl;
            _exit(0);
        }
    } else if (found_libs == 0) {
        std::cout << "No faulty libraries exist" << std::endl;
        std::cout << "Do you want to link librt.so.1 to Oneshot? Y/N" << std::endl;
        std::cin >> link_answer;

        if (link_answer == 'Y' || link_answer == 'y') {
            custom_system("ln -sf /usr/lib32/librt.so.1 ~/.local/share/Steam/steamapps/common/OneShot");
        } else if (link_answer == 'N' || link_answer == 'n') {
            _exit(0);
        }
    }
}

int main() {
    // Set the default_oneshot path to the user's home directory
    std::sprintf(default_oneshot, "%s/.steam/root/steamapps/common/OneShot", getenv("HOME"));

    lib_handler(drm_name, lib_paths[0]);
    lib_handler(lib_gl_name, lib_paths[1]);
    lib_handler(lib_stdc_name, lib_paths[2]);
    ui();

    return 0;
}
