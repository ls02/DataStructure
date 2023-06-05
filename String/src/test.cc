#include "MyString.hpp"
//#include <iostream>
//#include <cstdio>

//void Test1()
//{
//    ls::string str("new");
//    //str.PushBack("hello");
//    str.insert(0, "hello");
//    printf("%s\n", str.c_str());
//}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int countProcessesByName(const char* processName) {
    int count = 0;
    DIR* dir = opendir("/proc"); // 打开进程目录
    if (dir == NULL) {
        perror("Failed to open /proc directory");
        return -1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) { // 只处理目录项
            char* endptr;
            long pid = strtol(entry->d_name, &endptr, 10); // 将目录名转换为进程ID
            if (*endptr == '\0') { // 确保转换成功
                char procPath[256];
                snprintf(procPath, sizeof(procPath), "/proc/%ld/cmdline", pid);

                FILE* cmdlineFile = fopen(procPath, "r");
                if (cmdlineFile) {
                    char cmdline[256];
                    if (fgets(cmdline, sizeof(cmdline), cmdlineFile) != NULL) {
                        // 比较进程名
                        if (strstr(cmdline, processName) != NULL) {
                            count++;
                        }
                    }
                    fclose(cmdlineFile);
                }
            }
        }
    }

    closedir(dir);
    return count;
}

void Test2() {
    ls::string str1("new str");
    ls::string str2;
    str2.insert(0, "abc");

    ls::string str3;
    
    std::cin >> str2;
    std::cout << str2 << std::endl;

    ls::string::iterator it = str1.begin();
    while (it != str1.end())
    {
        std::cout << *it;
        it++;
    }

    printf("\n");
}

int main()
{
    //Test1();

    //const char* processName = "bash"; // 替换为您要查找的进程名
    //int numProcesses = countProcessesByName(processName);
    //printf("Number of processes with name '%s': %d\n", processName, numProcesses);

    Test2();

    return 0;
}
