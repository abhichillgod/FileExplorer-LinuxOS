#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

string currentPath = ".";

// Function to list files in current directory
void listFiles() {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(currentPath.c_str())) == NULL) {
        perror("opendir() error");
    } else {
        cout << "\n📂 Files and Directories in: " << currentPath << endl;
        while ((entry = readdir(dir)) != NULL) {
            cout << " - " << entry->d_name << endl;
        }
        closedir(dir);
    }
}

// Change directory
void changeDirectory(string path) {
    if (chdir(path.c_str()) == 0) {
        char temp[1024];
        getcwd(temp, sizeof(temp));
        currentPath = string(temp);
        cout << "Changed to: " << currentPath << endl;
    } else {
        cout << "❌ Error: Cannot open directory " << path << endl;
    }
}

// Create new file
void createFile(string name) {
    ofstream file(name);
    if (file) {
        cout << "✅ File created: " << name << endl;
    } else {
        cout << "❌ Error creating file!" << endl;
    }
    file.close();
}

// Create new directory (cross-platform)
void createDirectory(string name) {
#ifdef _WIN32
    if (mkdir(name.c_str()) == 0)
#else
    if (mkdir(name.c_str(), 0777) == 0)
#endif
        cout << "✅ Directory created: " << name << endl;
    else
        perror("Error creating directory");
}

// Delete file
void deleteFile(string name) {
    if (remove(name.c_str()) == 0)
        cout << "🗑️ File deleted: " << name << endl;
    else
        perror("Error deleting file");
}

// Copy file
void copyFile(string source, string dest) {
    ifstream src(source, ios::binary);
    ofstream dst(dest, ios::binary);
    if (!src) {
        cout << "❌ Source file not found!" << endl;
        return;
    }
    dst << src.rdbuf();
    cout << "📋 File copied from " << source << " to " << dest << endl;
}

// Search file recursively
void searchFile(string dirPath, string filename) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(dirPath.c_str())))
        return;

    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;
        if (entry->d_type == DT_DIR) {
            if (name != "." && name != "..")
                searchFile(dirPath + "/" + name, filename);
        } else {
            if (name == filename)
                cout << "🔍 Found: " << dirPath + "/" + name << endl;
        }
    }
    closedir(dir);
}

// Display current directory path
void showCurrentPath() {
    cout << "📁 Current Directory: " << currentPath << endl;
}

int main() {
    int choice;
    string name, path1, path2;

    cout << "========= Linux/Windows File Explorer =========" << endl;
    while (true) {
        cout << "\n1. List Files";
        cout << "\n2. Show Current Directory";
        cout << "\n3. Change Directory";
        cout << "\n4. Create File";
        cout << "\n5. Create Directory";
        cout << "\n6. Delete File";
        cout << "\n7. Copy File";
        cout << "\n8. Search File";
        cout << "\n9. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                listFiles();
                break;
            case 2:
                showCurrentPath();
                break;
            case 3:
                cout << "Enter path: ";
                cin >> path1;
                changeDirectory(path1);
                break;
            case 4:
                cout << "Enter filename: ";
                cin >> name;
                createFile(name);
                break;
            case 5:
                cout << "Enter directory name: ";
                cin >> name;
                createDirectory(name);
                break;
            case 6:
                cout << "Enter filename to delete: ";
                cin >> name;
                deleteFile(name);
                break;
            case 7:
                cout << "Enter source file: ";
                cin >> path1;
                cout << "Enter destination file: ";
                cin >> path2;
                copyFile(path1, path2);
                break;
            case 8:
                cout << "Enter filename to search: ";
                cin >> name;
                searchFile(".", name);
                break;
            case 9:
                cout << "👋 Exiting File Explorer..." << endl;
                return 0;
            default:
                cout << "Invalid option!" << endl;
        }
    }
}
