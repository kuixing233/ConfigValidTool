#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <sys/stat.h>

using namespace std;

static bool openFile(ifstream& in, const string& path, bool seekEnd) {
    if (in.is_open()) return true;
    in.open(path.c_str(), ios::in);
    if (!in) return false;
    if (seekEnd) in.seekg(0, ios::end);
    return true;
}

static off_t getFileSize(const string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return -1;
    return st.st_size;
}

static bool watchFor(const string& filePath, const string& needle) {
    cout << "FilePath: " << filePath << "\t Watch For: " << needle << endl;
    const std::chrono::milliseconds sleepInterval(250);

    ifstream ifs;
    if (!openFile(ifs, filePath, true)) {
        cerr << "Open file error: " << filePath << endl;
        return false;
    }
    cout << "Open file Success" << endl;

    off_t lastOffset = getFileSize(filePath);
    if (lastOffset < 0) lastOffset = 0;

    string line;
    while (true) {
        // 日志文件轮转（大小变小），重新打开文件
        off_t curSize = getFileSize(filePath);
        if (curSize >= 0 && curSize < lastOffset) {
            ifs.close();
            if (!openFile(ifs, filePath, false)) {
                cerr << "Open File Error" << endl;
                return false;
            }
            lastOffset = 0;
            cout << "File rotated, reopen from start" << endl;
        }

        ifs.clear();
        ifs.seekg(lastOffset, ios::beg);

        bool hasData = false;
        while (std::getline(ifs, line)) {
            hasData = true;
            // 更新 lastOffset 到当前读取位置
            std::streampos p = ifs.tellg();
            if (p != std::streampos(-1)) {
                lastOffset = static_cast<off_t>(p);
            } else {
                // EOF 情况下 tellg 可能为 -1，用当前文件大小兜底
                off_t sz = getFileSize(filePath);
                if (sz > 0) lastOffset = sz;
            }

            // cout << "Read Line: " << line << endl;
            if (line.find(needle) != string::npos) {
                cout << "Get Needle" << endl;
            }
        }

        if (ifs.bad()) {
            cerr << "read file error" << endl;
            return false;
        }

        if (!hasData) this_thread::sleep_for(sleepInterval);
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 3) {
        cerr << "usage: " << argv[0] << " <log_file> <string>\n";
        return -1;
    }

    const string filePath = argv[1];
    const string needle = argv[2];

    bool matched = watchFor(filePath, needle);
    if (matched) {
        cout << "Write Success" << endl;
    } else {
        cout << "Missing Write" << endl;
    }
    return 0;
}