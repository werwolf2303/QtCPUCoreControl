//
// Created by werwolf2303 on 8/3/25.
//


#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: cpu_toggle_helper <cpu_number> <0|1>\n";
        return 1;
    }

    int cpu = std::atoi(argv[1]);
    std::string state = argv[2];

    if (state != "0" && state != "1") {
        std::cerr << "State must be 0 or 1\n";
        return 1;
    }

    std::string path = "/sys/devices/system/cpu/cpu" + std::to_string(cpu) + "/online";
    std::ofstream file(path);

    if (!file) {
        std::cerr << "Failed to open " << path << " for writing.\n";
        return 1;
    }

    file << state;
    if (!file) {
        std::cerr << "Failed to write to " << path << ".\n";
        return 1;
    }

    return 0;
}