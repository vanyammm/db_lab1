#pragma once
#include "Topic.h"
#include "indexFile.h"
#include <cstdio>
#include <string>
#include<iostream>
#include "DeletedTopics.h"
using namespace std;

class TopicFile
{
private:
	const char topicFilePath[13] = "TopicFile.fl";
public:
	void createFile()// ????????? ?????
	{
		FILE* topicFile;
		fopen_s(&topicFile, topicFilePath, "w+b");
		fclose(topicFile);
		return;
	}

	bool checkFileExists(string& error) // ???????? ?????
	{
		FILE* topicFile;
		fopen_s(&topicFile, topicFilePath, "r+b");
		if (topicFile == NULL) {
			error = "Topic file does not exist";
			createFile();
			return 0;
		}
		fclose(topicFile);
		return 1;
	}

	int getSeekEnd()
	{
		int seekEnd;
		FILE* topicFile;
		fopen_s(&topicFile, topicFilePath, "r+b");
		fseek(topicFile, 0, SEEK_END);
		seekEnd = ftell(topicFile);
		fclose(topicFile);
		return seekEnd;
	}

	Topic readTopic(int adress)
	{
		Topic topic;
		FILE* topicFile;
		fopen_s(&topicFile, topicFilePath, "r+b");
		fseek(topicFile, adress, SEEK_SET);
		fread(&topic, sizeof(Topic), 1, topicFile);
		fclose(topicFile);
		return topic;
	}

	void changeTopic(Topic topic, int adress)
	{
		FILE* topicFile;
		fopen_s(&topicFile, topicFilePath, "r+b");
		fseek(topicFile, adress, SEEK_SET);
		fwrite(&topic, sizeof(Topic), 1, topicFile);
		fclose(topicFile);
		return;
	}

	void writeTopic(Topic topic)
	{
		FILE* topicFile;
		fopen_s(&topicFile, topicFilePath, "a+b");
		fseek(topicFile, 0, SEEK_END);
		fwrite(&topic, sizeof(Topic), 1, topicFile);
		fclose(topicFile);
		return;
	}

	Topic getTopic(int key, string& error) // ?????? ???? ?? ??? ???? ???? ????
	{
		IndexFile fl = IndexFile();
		if (!fl.checkKeyIsReal(key, error))return Topic();
		int adress = fl.getAdressByKey(key, error);
		if (error != "")return Topic();
		return readTopic(adress);
	}

	int addTopic(Topic topic, string& error) // ???? ???? ?? ????? ? ???????? ???? ??? ? ????? ???? ????????? ????
	{
		DeletedTopics dTop = DeletedTopics();
		int adress = dTop.getFirstDeletedAdress();
		IndexFile iFl = IndexFile();
		topic.topic_id = iFl.getLastKey() + 1;
		if (adress == -1) {
			iFl.addIndex(getSeekEnd(), error);
			writeTopic(topic);
		}
		else {
			iFl.addIndex(adress, error);
			if (error != "")return 0;
			changeTopic(topic, adress);
		}
		return topic.topic_id;
	}


	void updateTopic(int key, Topic topic, string& error) // ??????? ???? ??????? ? ?????????? ???
	{
		IndexFile iFl = IndexFile();
		int adress = iFl.getAdressByKey(key, error);
		if (error != "")return;
		changeTopic(topic, adress);
		return;
	}

};
