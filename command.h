#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

class Command;
constexpr auto EndOfFileString = "eof";

/**
*	Интерфейс обработчика команд
*/
class ICommandHandler {
public:

	ICommandHandler() = delete;
	explicit ICommandHandler(int count) : m_count{ count } {}
	virtual ~ICommandHandler() = default;

    virtual bool ProcessCommand(Command*, const std::string&) = 0;

protected:

	const int m_count;
};

using ICommandHandlerPtr = std::unique_ptr<ICommandHandler>;

/**
*	Обработка команд
*/
class Command
{
public:

	explicit Command(int count);
	~Command() = default;

	void SetCurrent(ICommandHandlerPtr hPtr) 
	{
		m_handler = std::move(hPtr);
	}

	void PushPool(const std::string& s);

	size_t GetPoolSize()
	{
		return m_pool.size();
	}

	void PrintPool();

	void PrintString(std::fstream& f, const std::string& s);

	bool ProcessCommand(const std::string& cmd) 
	{
		return m_handler->ProcessCommand(this, cmd);
	}

private:

	const int m_count;
	long long m_first_cmd_time = 0;
	ICommandHandlerPtr m_handler;
	std::vector<std::string> m_pool;
};

/**
*	Обработка команд в режиме статических блоков
*/
class StaticCommandHandler : public ICommandHandler
{
public:

	StaticCommandHandler() = delete;
	StaticCommandHandler(int count) : ICommandHandler(count) {}
	~StaticCommandHandler() = default;

	bool ProcessCommand(Command* cmd, const std::string& s) override;
};

/**
*	Обработка команд в режиме динамических блоков
*/
class DinamicCommandHandler : public ICommandHandler
{
public:

	DinamicCommandHandler() = delete;
	DinamicCommandHandler(int count) : ICommandHandler(count) {}
	~DinamicCommandHandler() = default;

	bool ProcessCommand(Command* cmd, const std::string& s) override;

private:

	int m_open_brace_count = 0;
};
