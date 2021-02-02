#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <mutex>

std::mutex mutex;

void LogPasswordsGenerated(const std::string& pathOfPasswords, std::vector<std::string>::iterator iterForBeginThread, 
    std::vector<std::string>::iterator iterForEndThread)
{
    std::cout << "Writing passwords into file..." << std::endl;

    std::ofstream fileStream;
    fileStream.open(pathOfPasswords, std::ios::app);

    for (std::vector<std::string>::iterator i = iterForBeginThread; i != iterForEndThread; ++i) {
        
        std::unique_lock<std::mutex> guard(mutex);
        fileStream << *i + '\n';        
    }

    std::cout << "Done!" << std::endl;
}

void GeneratePassword(const std::string& pathOfPasswords1, const std::string& pathOfPasswords2,
    const std::string& pathOfPasswords3, const std::string& pathOfPasswords4)
{
    std::cout << "Passwords generating..." << std::endl;
    const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    const uint32_t maxSize = 5;
    char guess[maxSize + 1] = {};
    uint32_t i, j = 0;
    const uint32_t maxPasswordLength = 4;
    uint32_t guessc[maxSize] = { 0 };
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
        + generatedPasswords / 4;
    std::vector<std::string>::iterator iterForBeginThread2 = vectorPasswordsGenerated.begin()
        + generatedPasswords / 4;
    std::vector<std::string>::iterator iterForEndThread2 = vectorPasswordsGenerated.begin()
        + 2 * generatedPasswords / 4;
    std::vector<std::string>::iterator iterForBeginThread3 = vectorPasswordsGenerated.begin()
        + 2 * generatedPasswords / 4;
    std::vector<std::string>::iterator iterForEndThread3 = vectorPasswordsGenerated.begin() 
        + 3 * generatedPasswords / 4;
    std::vector<std::string>::iterator iterForBeginThread4 = vectorPasswordsGenerated.begin()
        + 3 * generatedPasswords / 4;
    std::vector<std::string>::iterator iterForEndThread4 = vectorPasswordsGenerated.end();

    std::thread thread1(LogPasswordsGenerated, std::ref(pathOfPasswords1), iterForBeginThread1, iterForEndThread1);
    std::thread thread2(LogPasswordsGenerated, std::ref(pathOfPasswords2), iterForBeginThread2, iterForEndThread2);
    std::thread thread3(LogPasswordsGenerated, std::ref(pathOfPasswords3), iterForBeginThread3, iterForEndThread3);
    std::thread thread4(LogPasswordsGenerated, std::ref(pathOfPasswords4), iterForBeginThread4, iterForEndThread4);

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

    GeneratePassword(pathOfPasswords1, pathOfPasswords2, pathOfPasswords3, pathOfPasswords4);
}