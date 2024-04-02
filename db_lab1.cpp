#include <iostream>
#include "CommentFile.h"
#include "TopicFile.h"
#include "IndexFile.h"
#include "DeletedTopics.h"
#include "DeletedComments.h"

CommentFile cFl = CommentFile();
TopicFile tFl = TopicFile();
IndexFile iFl = IndexFile();
DeletedTopics dTo = DeletedTopics();
DeletedComments dComments = DeletedComments();

bool intValidate()
{
	if (cin.fail())
	{
		cout << "Input is incorrect" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return 0;
	}
	else return 1;
}

bool inputInt(int& x)
{
	cin >> x;
	return intValidate();
}

Topic inputTopic()
{
	Topic topic;
	cout << "Input title of topic: ";
	cin >> topic.title;
	cout << "Input post date of topic: ";
	cin >> topic.created_at;
	cout << "Input content of topic: ";
	cin >> topic.content;
	return topic;
}

Comment inputComment(string& error)
{
	Comment comment;
	cout << "Input content of comment: ";
	cin >> comment.content;
	return comment;
}

void addTopic()
{
	string error;
	Topic topic = inputTopic();
	topic.firstCommentAdress = -1;
	topic.isDeleted = 0;
	int key = tFl.addTopic(topic, error);
	if (error == "") {
		cout << "Topic successfully added with key = " << key << endl;
	}
	else {
		cout << error << endl;
	}
}



void addComment()
{
	string error;
	int topic_id;
	cout << "Input key of topic: ";
	if (!inputInt(topic_id))return;
	Comment comment = inputComment(error);
	if (error != "") {
		cout << error << endl;
		return;
	}
	comment.topic_id = topic_id;
	comment.isDeleted = 0;
	comment.nextAdress = -1;
	int com_id = cFl.addComment(topic_id, comment, error);
	if (error == "") {
		cout << "Comment successfully added with key = " << com_id << endl;
	}
	else {
		cout << error << endl;
	}
}

void updateTopic()
{
	string error;
	int topic_id;
	cout << "Input key of topic: ";
	if (!inputInt(topic_id))return;
	Topic topic = inputTopic();
	Topic oldTopic = tFl.getTopic(topic_id, error);
	if (error != "") {
		cout << error << endl;
		return;
	}

	oldTopic.title = topic.title;
	oldTopic.created_at = topic.created_at;
	oldTopic.content = topic.content;
	tFl.updateTopic(topic_id, oldTopic, error);

	if (error == "")
	{
		cout << "Topic successfully updated" << endl;
	}
	else {
		cout << error << endl;
	}
}

void updateComment()
{
	string error;
	int topic_id, com_id;
	cout << "Input key of topic: ";
	if (!inputInt(topic_id))return;
	cout << "Input key of comment: ";
	if (!inputInt(com_id))return;
	Comment comment = inputComment(error);
	if (error != "") {
		cout << error << endl;
		return;
	}
	Comment oldComment = cFl.getComment(topic_id, com_id, error);
	if (error != "") {
		cout << error << endl;
		return;
	}
	oldComment.content = comment.content;

	cFl.updateComment(topic_id, com_id, oldComment, error);
	if (error == "") {
		cout << "Comment successfully updated" << endl;
	}
	else {
		cout << error << endl;
	}
}

void deleteTopic()
{
	string error;
	int topic_id;
	cout << "Input key of topic: ";
	if (!inputInt(topic_id))return;
	cFl.deleteTopic(topic_id, error);
	if (error == "") {
		cout << "Topic successfully deleted" << endl;
	}
	else {
		cout << error << endl;
	}
}

void deleteComment()
{
	string error;
	int topic_id, com_id;
	cout << "Input key of topic: ";
	if (!inputInt(topic_id))return;
	cout << "Input key of comment: ";
	if (!inputInt(com_id))return;
	cFl.deleteComment(topic_id, com_id, error);
	if (error == "") {
		cout << "Comment successfully deleted" << endl;
	}
	else {
		cout << error << endl;
	}
}

void getTopic()
{
	string error;
	int topic_id;
	cout << "Input key of topic: ";
	if (!inputInt(topic_id))return;
	Topic topic = tFl.getTopic(topic_id, error);
	if (error == "") {
		cout << "Title of topic: " << topic.title << endl;
		cout << "Post date of topic: " << topic.created_at << endl;
		cout << "Content of topic: " << topic.content << endl;
	}
	else {
		cout << error << endl;
	}
}

void getComment()
{
	string error;
	int topic_id, com_id;
	cout << "Input key of topic: ";
	if (!inputInt(topic_id))return;
	cout << "Input key of comment: ";
	if (!inputInt(com_id))return;
	Comment comment = cFl.getComment(topic_id, com_id, error);
	if (error == "") {
		cout << "Content of comment: " << comment.content << endl;
		cout << "Likes: " << comment.likes << endl;
		cout << "Created by: " << comment.user << endl;
	}
	else {
		cout << error << endl;
	}
}

void writeTopics()
{
	string error;
	cFl.writeTopicsInfo(error);
	if (error != "") {
		cout << error << endl;
	}
}

void writeComments()
{
	string error;
	cFl.writeCommentsInfo(error);
	if (error != "") {
		cout << error << endl;
	}
}

void writeDatabase()
{
	string error;
	cFl.writeTopicAndCommentsInfo(error);
	if (error != "") {
		cout << error << endl;
	}
}

int main()
{
	iFl.createFile();
	tFl.createFile();
	cFl.createFile();
	dTo.createFile();
	dComments.createFile();
	while (true) {
		cout << "Choose option:" << endl;
		cout << "1 -> Add topic " << endl;
		cout << "2 -> Delete topic" << endl;
		cout << "3 -> Change topic" << endl;
		cout << "4 -> Get topic" << endl;
		cout << "5 -> Add comment " << endl;
		cout << "6 -> Delete comment" << endl;
		cout << "7 -> Change comment" << endl;
		cout << "8 -> Get comment" << endl;
		cout << "9 -> Get all topics" << endl;
		cout << "10 -> Get all comments" << endl;
		cout << "11 -> Get all database" << endl;
		int x;
		cin >> x;
		if (!intValidate())continue;
		if (x < 1 || x>11) {
			cout << "Option is incorrect" << endl;
			cout << "Choose from 1 to 11" << endl;
		}
		else {
			if (x == 1) {
				addTopic();
			}
			else if (x == 2) {
				deleteTopic();
			}
			else if (x == 3) {
				updateTopic();
			}
			else if (x == 4) {
				getTopic();
			}
			else if (x == 5) {
				addComment();
			}
			else if (x == 6) {
				deleteComment();
			}
			else if (x == 7) {
				updateComment();
			}
			else if (x == 8) {
				getComment();
			}
			else if (x == 9) {
				writeTopics();
			}
			else if (x == 10) {
				writeComments();
			}
			else {
				writeDatabase();
			}
		}
	}
}

