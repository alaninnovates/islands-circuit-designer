#include "file_actions.h"
#include "external/tinyfiledialogs.h"

std::string show_open_file_dialog() {
//    const char * filter[1] = {"*.iscd"};
    char* file_name = tinyfd_openFileDialog(
            "Open Saved Circuit",
            "",
            0,
            nullptr,
            "Islands Circuit Designer files",
            0
    );
    if (file_name == nullptr) {
        return "";
    }
    return file_name;
}

std::string show_save_file_dialog(const char *filename) {
    char* file_path = tinyfd_saveFileDialog("Save file", filename, 0, nullptr, nullptr);
    if (file_path == nullptr) {
        return "";
    }
    return file_path;
}