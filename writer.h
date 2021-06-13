#pragma once

#include <string>
#include <iostream>

/**
*	Writer interface
*/
class IWriter
{
public:

	IWriter() = default;
	virtual ~IWriter() = default;

	virtual void Print(const std::string& s) = 0; 
	virtual void SetTime(long long time) = 0;
};

/**
*	Write in the std::cout
*/
class CoutWriter : public IWriter
{
public:

	CoutWriter() = default;
	~CoutWriter() = default;

	void Print(const std::string& s) override
	{
		std::cout << s;
	}

	void SetTime(long long) override {}
};

/**
*	Write in the file
*/
class FileWriter : public IWriter
{
public:

	FileWriter() = default;
	~FileWriter() = default;

	void Print(const std::string& s)  override
	{
		fileStream_ << s;
	}

	void SetTime(long long time) override
	{
		if (time != time_)
		{
			if (fileStream_.is_open())
				fileStream_.close();

			fileStream_.open("bulk" + std::to_string(time) + ".log");

			time_ = time;
		}
	}

private:

	std::ofstream fileStream_;
	long long time_ = 0;
};