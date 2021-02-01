#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <mutex>

std::atomic<uint32_t> g_counter = 0;
std::mutex mutex;

void LogPasswordsChecked(const std::string& pathOfCheckedPasswords, std::vector<std::string>& vectorPasswordsGenerated,
    uint32_t generatedPasswords, std::vector<std::string>::iterator iterForBeginThread, std::vector<std::string>::iterator iterForEndThread)
{
    std::cout << "Writing passwords into file..." << std::endl;

    std::ofstream fileStream;
    fileStream.open(pathOfCheckedPasswords, std::ios::app);

    for (auto i = iterForBeginThread; i != iterForEndThread; ++i) {
        std::unique_lock<std::mutex> guard(mutex);

        if (g_counter == generatedPasswords - 1) {
            break;
        }

        fileStream << vectorPasswordsGenerated[g_counter] + '\n';
        ++g_counter;
    }

    std::cout << "Done!" << std::endl;
}

void CheckPassword(const std::string& pathOfPasswords1, const std::string& pathOfPasswords2,
    const std::string& pathOfPasswords3, const std::string& pathOfPasswords4)
{
    std::cout << "Passwords generating..." << std::endl;
    const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    const int maxSize = 5;
    char guess[maxSize + 1] = {};
    int i, j = 0;
    const int maxPasswordLength = 4;
    int guessc[maxSize] = { 0 };
    std::vector<std::string> vectorPasswordsGenerated;
    uint32_t generatedPasswords = 0;

    for (i = 1; i < maxSize; guessc[i++] = -1);
    for (i = 1; i <= maxSize; guess[i++] = '\0');

    while (generatedPasswords++ < pow(sizeof(chars), maxPasswordLength)) {

        i = 0;

        while (guessc[i] == sizeof(chars)) {
            guessc[i] = 0;
            guessc[++i] += 1;
        }

        for (j = 0; j <= i; ++j) {
            if (j < maxSize) {
                guess[j] = chars[guessc[j]];
            }
        }

        vectorPasswordsGenerated.emplace_back(guess);
        ++guessc[0];
    }
    std::cout << "Done!" << std::endl;

    std::vector<std::string>::iterator iterForBeginThread1 = vectorPasswordsGenerated.begin();
    std::vector<std::string>::iterator iterForEndThread1 = vectorPasswordsGenerated.begin()
        + generatedPasswords / 4 - 1;
    std::vector<std::string>::iterator iterForBeginThread2 = vectorPasswordsGenerated.begin()
        + generatedPasswords / 4 - 1;
    std::vector<std::string>::iterator iterForEndThread2 = vectorPasswordsGenerated.begin()
        + 2 * generatedPasswords / 4 - 1;
    std::vector<std::string>::iterator iterForBeginThread3 = vectorPasswordsGenerated.begin()
        + 2 * generatedPasswords / 4 - 1;
    std::vector<std::string>::iterator iterForEndThread3 = vectorPasswordsGenerated.begin() 
        + 3 * generatedPasswords / 4 - 1;
    std::vector<std::string>::iterator iterForBeginThread4 = vectorPasswordsGenerated.begin()
        + 3 * generatedPasswords / 4 - 1;
    std::vector<std::string>::iterator iterForEndThread4 = vectorPasswordsGenerated.end();

    std::thread thread1(LogPasswordsChecked, std::ref(pathOfPasswords1), std::ref(vectorPasswordsGenerated), 
        generatedPasswords, iterForBeginThread1, iterForEndThread1);
    std::thread thread2(LogPasswordsChecked, std::ref(pathOfPasswords2), std::ref(vectorPasswordsGenerated), 
        generatedPasswords, iterForBeginThread2, iterForEndThread2);
    std::thread thread3(LogPasswordsChecked, std::ref(pathOfPasswords3), std::ref(vectorPasswordsGenerated), 
        generatedPasswords, iterForBeginThread3, iterForEndThread3);
    std::thread thread4(LogPasswordsChecked, std::ref(pathOfPasswords4), std::ref(vectorPasswordsGenerated),
        generatedPasswords, iterForBeginThread4, iterForEndThread4);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
}

int main(int argc, char** argv)
{
    std::string folderPath = argv[1];
    std::string pathOfPasswords1 = folderPath + "\\passwords1.txt";
    std::string pathOfPasswords2 = folderPath + "\\passwords2.txt";
    std::string pathOfPasswords3 = folderPath + "\\passwords3.txt";
    std::string pathOfPasswords4 = folderPath + "\\passwords4.txt";

    CheckPassword(pathOfPasswords1, pathOfPasswords2, pathOfPasswords3, pathOfPasswords4);
}