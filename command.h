#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "writer.h"

class Command;
constexpr auto EndOfFileString = "eof";

class ICommandHandler;
using ICommandHandlerPtr = std::unique_ptr<ICommandHandler>;
using IWriterPtr = std::unique_ptr<IWriter>;

/**
*	Command handler interface
*/
class ICommandHandler {
public:

	ICommandHandler() = delete;
	explicit ICommandHandler(int count) : count_{ count } {}
	virtual ~ICommandHandler() = default;

    virtual ICommandHandlerPtr ProcessCommand(Command*, const std::string&, bool& exit) = 0;

protected:

	const int count_;
};

/**
*	Process command
*/
class Command
{
public:

	explicit Command(int count);
	~Command() = default;

	void PushPool(const std::string& s);

	size_t GetPoolSize()
	{
		return pool_.size();
	}

	void PrintPool();

	void PrintString(const std::string& s) const;

	bool ProcessCommand(const std::string& cmd) 
	{
		bool res;

		ICommandHandlerPtr ptr = handler_->ProcessCommand(this, cmd, res);

		if (ptr)
		{
			handler_ = std::move(ptr);
		}

		return !res;
	}

private:

	const int count_;
	long long firstCmdTime_ = 0;
	ICommandHandlerPtr handler_;
	std::vector<std::string> pool_;
	std::vector<IWriterPtr> writersPool_;
};

/**
*	Process command int the static mode
*/
class StaticCommandHandler : public ICommandHandler
{
public:

	StaticCommandHandler() = delete;
	StaticCommandHandler(int count) : ICommandHandler(count) {}
	~StaticCommandHandler() = default;

	ICommandHandlerPtr ProcessCommand(Command* cmd, const std::string& s, bool& exit) override;
};

/**
*	Process command int the dynamic mode
*/
class DynamicCommandHandler : public ICommandHandler
{
public:

	DynamicCommandHandler() = delete;
	DynamicCommandHandler(int count) : ICommandHandler(count) {}
	~DynamicCommandHandler() = default;

	ICommandHandlerPtr ProcessCommand(Command* cmd, const std::string& s, bool& exit) override;

private:

	int openBraceCount_ = 0;
};
