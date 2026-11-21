#include <bits/stdc++.h>
#include<fstream>
//#include <filesystem>
using namespace std;
//using fs = std :: filesystem;

void cmd_pwd(const vector<string>& args) {
    cout << filesystem :: current_path() <<"\n";
}

void cmd_ls(const vector<string>& args) {
    try {
        for (const auto& entry : filesystem::directory_iterator(filesystem::current_path())) {
            cout << entry.path().filename().string() << "\n";
        }
    }
    catch (const filesystem::filesystem_error& e) {
        cerr << "ls: " << e.what() << "\n";
    }
}

void cmd_mkdir(const vector<string>& args) {
    if (args.empty()) {
        cerr << "mkdir: missing directory name\n";
        return;
    }

    try {
        if (filesystem::create_directory(args[0])) {
            cout << "Directory created: " << args[0] << "\n";
        } else {
            cout << "mkdir: directory already exists\n";
        }
    }
    catch (const filesystem::filesystem_error& e) {
        cerr << "mkdir: " << e.what() << "\n";
    }
}

void cmd_cd(const vector<string>& args) {
    if (args.empty()) {
        cerr << "cd: missing directory name\n";
        return;
    }

    try {
        if (!filesystem::exists(args[0])) {
            cerr << "cd: directory does not exist\n";
            return;
        }

        if (!filesystem::is_directory(args[0])) {
            cerr << "cd: not a directory\n";
            return;
        }

        filesystem::current_path(args[0]);
    }
    catch (const filesystem::filesystem_error& e) {
        cerr << "cd: " << e.what() << "\n";
    }
}

void cmd_rmdir(const vector<string>& args) {
    if (args.empty()) {
        cerr << "rmdir: missing directory name\n";
        return;
    }

    try {
        if (filesystem::remove(args[0])) {
            cout << "Directory removed\n";
        } else {
            cout << "rmdir: directory not found or not empty\n";
        }
    }
    catch (const filesystem::filesystem_error& e) {
        cerr << "rmdir: " << e.what() << "\n";
    }
}


void cmd_rm(const vector<string>& args) {
    if (args.empty()) {
        cerr << "rm: missing file name\n";
        return;
    }

    try {
        if (filesystem::is_directory(args[0])) {
            cerr << "rm: cannot remove a directory using rm\n";
            return;
        }

        if (filesystem::remove(args[0])) {
            cout << "File removed\n";
        } else {
            cout << "rm: file not found\n";
        }
    }
    catch (const filesystem::filesystem_error& e) {
        cerr << "rm: " << e.what() << "\n";
    }
}

void cmd_touch(const vector<string>& args) {
    if (args.empty()) {
        cerr << "touch: missing file name\n";
        return;
    }

    ofstream file(args[0]);

    if (file.is_open()) {
        cout << "File created: " << args[0] << "\n";
        file.close();
    }
    else {
        cerr << "touch: cannot create file\n";
    }
}

void cmd_cat(const vector<string>& args) {
    if (args.empty()) {
        cerr << "cat: missing file name\n";
        return;
    }

    for (const auto& filename : args) {
        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "cat: cannot open " << filename << "\n";
            continue;
        }

        string line;

        while (getline(file, line)) {
            cout << line << "\n";
        }

        file.close();
    }
}


void cmd_echo(const vector<string>& args) {
    for (size_t i = 0; i < args.size(); i++) {

        // Check for output redirection
        if (args[i] == ">" || args[i] == ">>") {

            if (i + 1 >= args.size()) {
                cerr << "echo: missing file name\n";
                return;
            }

            string filename = args[i + 1];
            string content;

            for (size_t j = 0; j < i; j++) {
                if (!content.empty()) content += " ";
                content += args[j];
            }

            ios::openmode mode = ios::out;

            if (args[i] == ">>") {
                mode |= ios::app;
            } else {
                mode |= ios::trunc;
            }

            ofstream file(filename, mode);

            if (!file.is_open()) {
                cerr << "echo: cannot open " << filename << "\n";
                return;
            }

            file << content << "\n";
            cout << "Text written successfully\n";
            return;
        }
    }

    // Normal echo: print text in terminal
    for (size_t i = 0; i < args.size(); i++) {
        if (i > 0) cout << " ";
        cout << args[i];
    }

    cout << "\n";
}

void cmd_help(const vector<string>& args) {
    cout << "\n========== VShell Commands ==========\n";
    cout << "pwd     - Print working directory\n";
    cout << "ls      - List directory contents\n";
    cout << "mkdir   - Create a directory\n";
    cout << "cd      - Change directory\n";
    cout << "rmdir   - Remove an empty directory\n";
    cout << "rm      - Remove a file\n";
    cout << "touch   - Create a file\n";
    cout << "cat     - Display file contents\n";
    cout << "echo    - Print text to terminal or file\n";
    cout << "exit    - Exit MyShell\n";

    cout << "\nOperators:\n";
    cout << "  >       - Overwrite file contents\n";
    cout << "  >>      - Append to file contents\n";

    cout << "\nExamples:\n";
    cout << "  echo Hello World\n";
    cout << "  echo Hello > notes.txt\n";
    cout << "  echo Again >> notes.txt\n";
    cout << "  cat notes.txt\n";
    cout << "==================================\n\n";
}

void cmd_clear(const vector<string>& args) {
    //cout << "\033[2J\033[H";
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void cmd_exit(const vector<string>& args) {
    cout << "See you soon.....!\n";
}

int main() {

    using Command = function<void(const vector<string>&)>;

    unordered_map<string, Command> commands = {

        {"pwd", cmd_pwd},
        {"ls", cmd_ls},
        {"mkdir", cmd_mkdir},
        {"cd", cmd_cd},
        {"rmdir", cmd_rmdir},
        {"rm", cmd_rm},
        {"touch", cmd_touch},
        {"cat", cmd_cat},
        {"echo", cmd_echo},
        {"help", cmd_help},
        {"clear",cmd_clear},
        {"exit", cmd_exit}
    };

    string input;

    while (true) {

        cout << "vShell> ";
        getline(cin, input);

        // Convert our string into a stream
        stringstream ss(input);

        // First word = command
        string command;
        ss >> command;

        // Remaining words = arguments
        vector<string> args;

        string arg;

        while (ss >> arg) {
            args.push_back(arg);
        }


        // Empty input
        if (command.empty()) {
            continue;
        }


        // Check whether command exists
        auto it = commands.find(command);

        if (it == commands.end()) {

            cout << "Unknown command: "
                 << command << "\n";

            continue;
        }

        // Execute command
        it->second(args);

        // Exit shell
        if (command == "exit") {
            break;
        }
    }

    return 0;
}