#pragma once
#include <string>
#include "shortString.h"
using namespace std;
class Topic
{
public:
	int topic_id;
	shortString title;
	shortString content;
	shortString created_at;
	int firstCommentAdress;
	bool isDeleted;
	Topic(int topic_id, string title, string content, string created_at, int firstCommentAdress, bool isDeleted)
	{
		this->topic_id = topic_id;
		this->title = title;
		this->content = content;
		this->created_at = created_at;
		this->firstCommentAdress = firstCommentAdress;
		this->isDeleted = isDeleted;
	}
	Topic(string title, string content, string created_at)
	{
		this->title = title;
		this->content = content;
		this->created_at = created_at;
		firstCommentAdress = -1;
		isDeleted = 0;
		topic_id = -1;
	}
	Topic()
	{
		topic_id = 0;
		firstCommentAdress = -1;
		isDeleted = 0;
	}
};
