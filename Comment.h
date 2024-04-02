#pragma once
#include<string>
#include "shortString.h"
using namespace std;

class Comment
{
public:
	int user;
	shortString content;
	int likes;
	bool isDeleted;
	int nextAdress;
	int topic_id;
	int com_id;
	Comment(int user, string content, int likes, bool isDeleted, int nextAdress, int topic_id, int com_id)
	{
		this->user = user;
		this->content = content;
		this->likes = likes;
		this->isDeleted = isDeleted;
		this->nextAdress = nextAdress;
		this->topic_id = topic_id;
		this->com_id = com_id;
	}
	Comment()
	{
		user = 0;
		likes = 0;
		isDeleted = 0;
		nextAdress = -1;
		topic_id = 0;
		com_id = 0;
	}
};
