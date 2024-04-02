#pragma once
#include <cstdio>
#include <string>
#include<iostream>
#include <vector>
using namespace std;
class DeletedTopics
{
private:
	const char deletedTopicsFilePath[21] = "DeletedTopicsFile.fl";
public:
	void createFile()// ????????? ?????
	{
		FILE* deletedTopics;
		fopen_s(&deletedTopics, deletedTopicsFilePath, "w+b");
		fclose(deletedTopics);
		return;
	}
	bool checkFileExists(string& error) // ???????? ?????
	{
		FILE* deletedTopics;
		fopen_s(&deletedTopics, deletedTopicsFilePath, "r+b");
		if (deletedTopics == NULL) {
			error = "Topic file does not exist";
			createFile();
			return 0;
		}
		fclose(deletedTopics);
		return 1;
	}
	void addDeletedAdress(int adress) // ????????? ?????? ?? ?????????
	{
		FILE* deletedTopicsFile;
		fopen_s(&deletedTopicsFile, deletedTopicsFilePath, "a+b");
		fseek(deletedTopicsFile, 0, SEEK_END);
		fwrite(&adress, sizeof(int), 1, deletedTopicsFile);
		fclose(deletedTopicsFile);
		return;
	}
	int readDeletedAdress(int adress) // ?????? ???????? ? ???? ?? ????????? ???? ? ????
	{
		int deletedAdress;
		FILE* deleteTopicsFile;
		fopen_s(&deleteTopicsFile, deletedTopicsFilePath, "r+b");
		fseek(deleteTopicsFile, adress, SEEK_SET);
		fread(&deletedAdress, sizeof(int), 1, deleteTopicsFile);
		fclose(deleteTopicsFile);
		return deletedAdress;
	}
	int getNumberOfAdresses()
	{
		FILE* deletedTopicsFile;
		fopen_s(&deletedTopicsFile, deletedTopicsFilePath, "r+b");
		fseek(deletedTopicsFile, 0, SEEK_END);
		int n = ftell(deletedTopicsFile) / sizeof(int);
		fclose(deletedTopicsFile);
		return n;
	}
	int getFirstDeletedAdress() // (??????? ?????? ????? ????????? ???? ? ??????? ?? ??????) ??? ??????? -1 ???? ????????? ????
	{
		int n = getNumberOfAdresses();
		if (n == 0) {
			return -1;
		}
		vector<int>adresses(n);
		for (int i = 0; i < n; ++i) {
			adresses[i] = readDeletedAdress(i * sizeof(int));
		}

		createFile();
		for (int i = 1; i < n; ++i) {
			addDeletedAdress(adresses[i]);
		}
		int firstAdress = adresses[0];
		return firstAdress;
	}
};
