#include <bits/stdc++.h>
using namespace std;

void cmd_pwd(const vector<string>& args) {
    cout << "This is aka present working directory....\n";
}

void cmd_ls(const vector<string>& args) {
    cout << "These are the content....\n";
}

void cmd_mkdir(const vector<string>& args) {

    if (args.empty()) {
        cout << "mkdir: missing directory name\n";
        return;
    }

    cout << "This is your new " << args[0] << "\n";
}

void cmd_cd(const vector<string>& args) {

    if (args.empty()) {
        cout << "cd: missing directory name\n";
        return;
    }

    cout << "This is your changed " << args[0] << "\n";
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
        {"exit", cmd_exit}

    };

    string input;

    while (true) {

        cout << "MyShell> ";
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
