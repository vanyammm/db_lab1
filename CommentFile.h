#pragma once
#include "Comment.h"
#include <cstdio>
#include <string>
#include<iostream>
#include "TopicFile.h"
#include "DeletedComments.h"
#include <vector>
using namespace std;

class CommentFile
{
private:
	const char commentFilePath[15] = "CommentFile.fl";
	int count = 0;
public:
	void createFile()// ????????? ?????
	{
		FILE* commentFile;
		fopen_s(&commentFile, commentFilePath, "w+b");
		fclose(commentFile);
		return;
	}

	bool checkFileExists(string& error) // ???????? ?????
	{
		FILE* commentFile;
		fopen_s(&commentFile, commentFilePath, "r+b");
		if (commentFile == NULL) {
			error = "Topic file does not exist";
			createFile();
			return 0;
		}
		fclose(commentFile);
		return 1;
	}

	Comment readComment(int adress)
	{
		Comment comment;
		FILE* commentFile;
		fopen_s(&commentFile, commentFilePath, "r+b");
		fseek(commentFile, adress, SEEK_SET);
		fread(&comment, sizeof(Comment), 1, commentFile);
		fclose(commentFile);
		return comment;
	}
	void changeComment(Comment comment, int adress)
	{
		FILE* commentFile;
		fopen_s(&commentFile, commentFilePath, "r+b");
		fseek(commentFile, adress, SEEK_SET);
		fwrite(&comment, sizeof(Comment), 1, commentFile);
		fclose(commentFile);
		return;
	}

	void writeComment(Comment comment)
	{
		FILE* commentFile;
		fopen_s(&commentFile, commentFilePath, "a+b");
		fseek(commentFile, 0, SEEK_END);
		fwrite(&comment, sizeof(Comment), 1, commentFile);
		fclose(commentFile);
		return;
	}

	int getSeekEnd()
	{
		int seekEnd;
		FILE* commentFile;
		fopen_s(&commentFile, commentFilePath, "r+b");
		fseek(commentFile, 0, SEEK_END);
		seekEnd = ftell(commentFile);
		fclose(commentFile);
		return seekEnd;
	}

	bool checkCommentExists(int topic_id, int com_id, string& error)// ??????? 0 ???? ???? ?? ???? ??? ???? ???????? ? 1 ? ?????? ???????
	{
		TopicFile tFL = TopicFile();
		Topic topic = tFL.getTopic(topic_id, error);
		if (error != "")return 0;
		int adress = topic.firstCommentAdress;
		while (adress != -1) {
			Comment comment = readComment(adress);
			if (comment.com_id == com_id) {
				if (comment.isDeleted == 1) {
					error = "Comment with key = " + to_string(com_id) + " was deleted";
				}
				return !comment.isDeleted;
			}
			adress = comment.nextAdress;
		}
		error = "comment with key = " + to_string(com_id) + " does not exist";
		return 0;
	}

	int getAdressOfComment(int topic_id, int com_id, string& error) // ??????? ?????? ???? ??? -1 ???? ?? ?? ????
	{
		if (!checkCommentExists(topic_id, com_id, error))return -1;
		TopicFile tFL = TopicFile();
		Topic topic = tFL.getTopic(topic_id, error);
		if (error != "")return -1;
		int adress = topic.firstCommentAdress;
		while (adress != -1) {
			Comment comment = readComment(adress);
			if (comment.com_id == com_id) {
				return adress;
			}
			adress = comment.nextAdress;
		}
		return -1;
	}

	Comment getComment(int topic_id, int com_id, string& error) // ?????? ?????? ?? ???
	{
		int adress = getAdressOfComment(topic_id, com_id, error);
		if (error != "")return Comment();
		Comment comment = readComment(adress);
		return comment;
	}

	void updateComment(int topic_id, int com_id, Comment comment, string& error) // ????????? ???? ?????????
	{
		int adress = getAdressOfComment(topic_id, com_id, error);
		if (error != "")return;
		changeComment(comment, adress);
		return;
	}

	void addNewCommentAdress(int topic_id, int addingAdress, string& error) // ????????? ????????? ?? ????? ??????
	{
		TopicFile tFL = TopicFile();
		Topic topic = tFL.getTopic(topic_id, error);
		if (error != "")return;
		if (topic.firstCommentAdress == -1) {
			topic.firstCommentAdress = addingAdress;
			tFL.updateTopic(topic_id, topic, error);
			return;
		}
		else {
			int adress = topic.firstCommentAdress;
			Comment comment = readComment(adress);
			while (comment.nextAdress != -1) {
				adress = comment.nextAdress;
				comment = readComment(adress);
			}
			comment.nextAdress = addingAdress;
			changeComment(comment, adress);
			return;
		}
	}


	int addComment(int topic_id, Comment comment, string& error) // ????????? ???????
	{
		DeletedComments dComments = DeletedComments();
		int adress = dComments.getFirstDeletedAdress();
		comment.com_id = ++count;
		if (adress == -1) {
			addNewCommentAdress(topic_id, getSeekEnd(), error);  // ??????? ????????? ???????
			if (error != "")return 0;
			writeComment(comment);
		}
		else {
			addNewCommentAdress(topic_id, adress, error);  // ??????? ????????? ???????
			if (error != "")return 0;
			changeComment(comment, adress);
		}
		return comment.com_id;
	}

	void deleteComment(int topic_id, int com_id, string& error) // ????????? ???????
	{
		if (!checkCommentExists(topic_id, com_id, error))return;
		TopicFile tFL = TopicFile();
		Topic topic = tFL.getTopic(topic_id, error);
		DeletedComments dComments = DeletedComments();
		if (error != "")return;
		int adress = topic.firstCommentAdress;
		vector< pair<int, Comment> > comments;
		while (adress != -1) {
			Comment comment = readComment(adress);
			comments.push_back({ adress,comment });
			adress = comment.nextAdress;
		}
		if (comments.size() == 1) {
			dComments.addDeletedAdress(comments[0].first);
			comments[0].second.isDeleted = 1;
			changeComment(comments[0].second, comments[0].first);
			topic.firstCommentAdress = -1;
			tFL.updateTopic(topic_id, topic, error);
			return;
		}
		else {
			if (comments[0].second.com_id == com_id) {
				dComments.addDeletedAdress(comments[0].first);

				comments[0].second.isDeleted = 1;
				changeComment(comments[0].second, comments[0].first);

				topic.firstCommentAdress = comments[1].first;
				tFL.updateTopic(topic_id, topic, error);
				return;
			}
			else {
				for (int i = 1; i < comments.size(); ++i) {
					if (comments[i].second.com_id == com_id) {
						dComments.addDeletedAdress(comments[i].first);
						if (i == comments.size() - 1) {
							comments[i - 1].second.nextAdress = -1;
						}
						else {
							comments[i - 1].second.nextAdress = comments[i + 1].first;
						}
						changeComment(comments[i - 1].second, comments[i - 1].first);
						comments[i].second.isDeleted = 1;
						changeComment(comments[i].second, comments[i].first);
						return;
					}
				}
			}
		}
	}


	void deleteTopic(int topic_id, string& error) // ????????? ??????? ? ?????????? ???
	{
		TopicFile tFL = TopicFile();
		IndexFile fl = IndexFile();
		DeletedComments dComments = DeletedComments();
		DeletedTopics dTop = DeletedTopics();
		Topic topic = tFL.getTopic(topic_id, error);
		if (error != "")return;

		int deletedTopicAdress = fl.getAdressByKey(topic_id, error);
		topic.isDeleted = 1;
		dTop.addDeletedAdress(deletedTopicAdress);
		tFL.updateTopic(topic_id, topic, error);
		fl.deleteIndex(topic_id, error);

		int adress = topic.firstCommentAdress;

		while (adress != -1) {
			Comment comment = readComment(adress);
			comment.isDeleted = 1;
			changeComment(comment, adress);
			dComments.addDeletedAdress(adress);
			adress = comment.nextAdress;
		}
	}

	void writeTopicAndCommentsInfo(string& error)
	{
		IndexFile fl = IndexFile();
		vector<int>topicKeys = fl.getRealKeys();
		cout << "There are " + to_string(topicKeys.size()) + " topics in database" << endl;
		TopicFile tFL = TopicFile();
		for (int i = 0; i < topicKeys.size(); ++i) {
			int topicKey = topicKeys[i];
			Topic topic = tFL.getTopic(topicKey, error);
			cout << '\t' << "The topic number " << i + 1 << " with key = " << topic.topic_id << ":" << endl;
			cout << '\t' << "Title: " << topic.title << endl;
			cout << '\t' << "Post date: " << topic.created_at << endl;
			cout << '\t' << "Content: " << topic.content << endl;
			int adress = topic.firstCommentAdress;
			vector<Comment>comments;
			while (adress != -1) {
				Comment comment = readComment(adress);
				comments.push_back(comment);
				adress = comment.nextAdress;
			}
			cout << '\t' << '\t' << "Number of comments: " << comments.size() << endl;
			for (int j = 0; j < comments.size(); ++j) {
				Comment comment = comments[j];
				cout << '\t' << '\t' << "The comment number " << j + 1 << " with key = " << comment.com_id << endl;
				cout << '\t' << '\t' << "Content: " << comment.content << endl;
				cout << '\t' << '\t' << "Likes: " << comment.likes << endl;
				//cout << "----------------------------------------------------------" << endl;
			}
			//cout<<"<<<<<------------------------------------------------>>>>>" << endl;
		}
	}
	void writeTopicsInfo(string& error) {
		IndexFile fl = IndexFile();
		vector<int>topicKeys = fl.getRealKeys();
		cout << "There are " + to_string(topicKeys.size()) + " topics in database" << endl;
		TopicFile tFL = TopicFile();
		for (int i = 0; i < topicKeys.size(); ++i) {
			int topicKey = topicKeys[i];
			Topic topic = tFL.getTopic(topicKey, error);
			cout << '\t' << "The topic number " << i + 1 << " with key = " << topic.topic_id << ":" << endl;
			cout << '\t' << "Title: " << topic.title << endl;
			cout << '\t' << "Post date: " << topic.created_at << endl;
			cout << '\t' << "Content: " << topic.content << endl;
			int adress = topic.firstCommentAdress;
		}
	}
	void writeCommentsInfo(string& error) {
		IndexFile fl = IndexFile();
		vector<int>topicKeys = fl.getRealKeys();
		TopicFile tFL = TopicFile();
		for (int i = 0; i < topicKeys.size(); ++i) {
			int topicKey = topicKeys[i];
			Topic topic = tFL.getTopic(topicKey, error);
			int adress = topic.firstCommentAdress;
			vector<Comment>comments;
			while (adress != -1) {
				Comment comment = readComment(adress);
				comments.push_back(comment);
				adress = comment.nextAdress;
			}
			for (int j = 0; j < comments.size(); ++j) {
				Comment comment = comments[j];
				cout << '\t' << '\t' << "The comment" << " with key = " << comment.com_id << endl;
				cout << '\t' << '\t' << "Content: " << comment.content << endl;
				cout << '\t' << '\t' << "Likes: " << comment.likes << endl;
			}
		}
	}
};
