#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>
#include <vector>
#include <unordered_map>

struct Option
{
	std::string name;
	enum
	{
		str,
		dashed,
		double_dashed
	} mode;

	Option() : name("NaN"), mode(str) {}
	explicit Option(const std::string &name) : name(name), mode(str) {}
};

class Args
{
private:
	std::vector<std::string> arguments;
	std::unordered_map<std::string, Option> options;
	const std::string name;

public:
	Args();
	Args(int &, char **);
	~Args();
	void parseArgs(int &, char **);
	std::vector<std::string> getArgs() const;
	void parseOpt(const std::string &);
	void parseOpts(const std::vector<std::string> &);
	std::unordered_map<std::string, Option> getOpts() const;
	Option getOpt(const std::string) const;
	Option getOpt(const std::string &) const;
	bool optExists(const std::string &) const;
	std::string getName() const;
	void convert();
};

#endif

Args::Args() : name("How Can A Program Have No Name?!")
{
	arguments.reserve(4);
}

Args::Args(int &argc, char **argv) : name(std::string(argv[0]).substr(std::string(argv[0]).rfind('/') + 1))
{
	arguments.reserve(argc);
	for (size_t i = 0; i < (unsigned)argc; ++i)
		arguments.push_back(std::string(argv[i]));
}

Args::~Args()
{
	arguments.clear();
	options.clear();
}

void Args::parseArgs(int &argc, char **argv)
{
	arguments.reserve(argc);
	for (size_t i = 0; i < (unsigned)argc; ++i)
		arguments.push_back(std::string(argv[i]));
}

std::vector<std::string> Args::getArgs() const { return arguments; }

void Args::parseOpt(const std::string &option)
{
	// Add --'color'=auto, now it's just --'color=auto'
	if (option[0] == '-' && option[1] == '-') // Double Dash
	{
		Option *temp = new Option(option);
		temp->mode = Option::double_dashed;
		options.insert(std::make_pair(option.substr(2), *temp));
		delete temp;
	}
	else if (option.front() == '-')
	{
		// For every letter after -x add that option, for loop
		for (unsigned char letter : option.substr(1))
		{
			std::string str;
			str.assign(1, letter);
			Option *temp = new Option(str);
			temp->mode = Option::dashed;
			options.insert(std::make_pair(str, *temp));
			delete temp;
		}
	}
	else
	{
		Option *temp = new Option(option);
		options.insert(std::make_pair(option, *temp));
		delete temp;
	}
}

void Args::parseOpts(const std::vector<std::string> &options)
{
	for (const std::string &option : options)
	{
		Option temp(option);
		parseOpt(option);
	}
}

std::unordered_map<std::string, Option> Args::getOpts() const { return options; }

Option Args::getOpt(const std::string option) const { return options.at(option); }

Option Args::getOpt(const std::string &option) const { return options.at(option); }

bool Args::optExists(const std::string &option) const { return options.find(option) != options.end(); }

std::string Args::getName() const { return name; }

void Args::convert()
{
	for (const std::string &temp : arguments)
		parseOpt(temp);
}
