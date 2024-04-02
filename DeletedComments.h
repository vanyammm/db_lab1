#pragma once
#include <cstdio>
#include <string>
#include<iostream>
#include <vector>
using namespace std;
class DeletedComments
{
private:
	const char deletedCommentsFilePath[23] = "DeletedCommentsFile.fl";
public:
	void createFile()// ????????? ?????
	{
		FILE* deletedComments;
		fopen_s(&deletedComments, deletedCommentsFilePath, "w+b");
		fclose(deletedComments);
		return;
	}
	bool checkFileExists(string& error) // ???????? ?????
	{
		FILE* deletedComments;
		fopen_s(&deletedComments, deletedCommentsFilePath, "r+b");
		if (deletedComments == NULL) {
			error = "Topic file does not exist";
			createFile();
			return 0;
		}
		fclose(deletedComments);
		return 1;
	}
	void addDeletedAdress(int adress) 
	{
		FILE* deletedComments;
		fopen_s(&deletedComments, deletedCommentsFilePath, "a+b");
		fseek(deletedComments, 0, SEEK_END);
		fwrite(&adress, sizeof(int), 1, deletedComments);
		fclose(deletedComments);
		return;
	}
	int readDeletedAdress(int adress) // ?????? ???????? ? ???? ?? ????????? ???? ? ????
	{
		int deletedAdress;
		FILE* deletedComments;
		fopen_s(&deletedComments, deletedCommentsFilePath, "r+b");
		fseek(deletedComments, adress, SEEK_SET);
		fread(&deletedAdress, sizeof(int), 1, deletedComments);
		fclose(deletedComments);
		return deletedAdress;
	}
	int getNumberOfAdresses()
	{
		FILE* deletedComments;
		fopen_s(&deletedComments, deletedCommentsFilePath, "r+b");
		fseek(deletedComments, 0, SEEK_END);
		int n = ftell(deletedComments) / sizeof(int);
		fclose(deletedComments);
		return n;
	}
	int getFirstDeletedAdress() 
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
