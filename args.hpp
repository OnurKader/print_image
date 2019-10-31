#ifndef ARGS_HPP
#define ARGS_HPP

#include <string>
#include <unordered_map>
#include <vector>

struct Option
{
	std::string name;
	enum
	{
		str,
		dashed,
		double_dashed
	} mode;

	Option() : name(""), mode(str) {}
	explicit Option(const std::string &name) : name(name), mode(str) {}
};

class Args
{
	private:
	std::vector<std::string> arguments;
	std::unordered_map<std::string, Option> options;
	const std::string name;

	public:
	Args(int &, char **);
	~Args();
	std::vector<std::string> getArgs() const;
	void parseOpt(const std::string &);
	std::unordered_map<std::string, Option> getOpts() const;
	Option getOpt(const std::string &) const;
	bool optExists(const std::string &) const;
	bool optExists(const std::string &, const std::string &) const;
	std::string getName() const;
	void convert();
};

Args::Args(int &argc, char **argv) :
	name(std::string(argv[0]).substr(std::string(argv[0]).rfind('/') + 1))
{
	for(size_t i = 0; i < (unsigned)argc; ++i)
		arguments.push_back(std::string(argv[i]));
}

Args::~Args()
{
	arguments.clear();
	options.clear();
}

std::vector<std::string> Args::getArgs() const { return arguments; }

// Push the given option, --xxx | -x | x, into the options map
void Args::parseOpt(const std::string &option)
{
	// TODO Add --'color'=auto, now it's just --'color=auto'
	if(option[0] == '-' && option[1] == '-')	// Double Dash
	{
		Option *temp = new Option(option);
		temp->mode = Option::double_dashed;
		options.insert(std::make_pair(option, *temp));
		delete temp;
	}
	else if(option.front() == '-')
		// For every letter after -x add that option, -lah eg.
		for(const char &letter: option.substr(1U))
		{
			if(letter == ' ')
				break;
			std::string str("-");
			str += letter;
			Option *temp = new Option(str);
			temp->mode = Option::dashed;
			options.insert(std::make_pair(str, *temp));
			delete temp;
		}
	else
		options.insert(std::make_pair(option, (Option(option))));
}

std::unordered_map<std::string, Option> Args::getOpts() const { return options; }

Option Args::getOpt(const std::string &option) const
{
	return optExists(option) ? options.at(option) : Option();
}

bool Args::optExists(const std::string &option) const
{
	return options.find(option) != options.end();
}

bool Args::optExists(const std::string &option1, const std::string &option2) const
{
	return (options.find(option1) != options.end() ||
			options.find(option2) != options.end());
}
std::string Args::getName() const { return name; }

void Args::convert()
{
	for(const std::string &temp: arguments)
		parseOpt(temp);
}

#endif
