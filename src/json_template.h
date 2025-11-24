#pragma once
#include <sstream>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include "logger.h"

class JsonTemplate
{
	struct Node
	{
		Node()
			: next(nullptr)
		{}

		Node(Node* n)
			: next(n->next), data(n->data)
		{}

		std::string data;
		Node* next;
	};

	struct IfElse
	{
		Node* start, * else_n, * if_n, * end;
	};

	struct ForEach
	{
		ForEach(IfElse* if_else)
			: if_else(if_else)
		{}

		std::unique_ptr<IfElse> if_else;
		std::vector<Node> cloned_nodes;
	};

	std::unordered_map<std::string, std::vector<Node*>> term_map_;
	std::unordered_map<std::string, std::vector<std::unique_ptr<IfElse>>> if_else_map_;
	std::unordered_map<std::string, std::vector<std::unique_ptr<ForEach>>> for_each_map_;

	std::vector<std::unique_ptr<Node>> nodes_;
	Node* root_;
	Node* recursive_create_template(std::string str, int& i, Node* node, IfElse* if_else = nullptr, Node* placeholder = nullptr)
	{
		for (; i < str.size(); ++i)
		{
			if (str[i] == '$')
			{
				std::string term;
				for (i = i + 1; i < str.size(); ++i) {
					if (str[i] == ',' || str[i] == ' ' || str[i] == ']' || str[i] == '}' || str[i] == '\n' || str[i] == '\r')
						break;
					term.push_back(str[i]);
				}
				node->next = new Node();
				term_map_[term].push_back(node->next);
				node->next->next = new Node();
				node = node->next->next;
			}
			else if (str[i] == '<')
			{
				std::string statement;
				for (i = i + 1; i < str.size(); ++i)
				{
					if (str[i] == '>')
					{
						i++;
						break;
					}
					statement.push_back(str[i]);
				}
				std::stringstream ss(statement);
				std::string word;
				ss >> word;
				if (word == "IF" || word == "FOREACH")
				{
					auto temp_placeholder = node;
					node = new Node();
					auto prev_word = word;
					ss >> word;
					temp_placeholder->next = new Node();
					auto temp_if_else = new IfElse{ temp_placeholder, temp_placeholder->next, node, temp_placeholder->next };
					if (prev_word == "FOREACH")
					{
						auto foreach = new ForEach(temp_if_else);
						for_each_map_[word].push_back(std::unique_ptr<ForEach>(foreach));
					}
					else
						if_else_map_[word].push_back(std::unique_ptr<IfElse>(temp_if_else));

					node = recursive_create_template(str, i, node, temp_if_else, temp_placeholder);
				}
				else if (word == "ELSE")
				{
					if (!if_else || !placeholder)
					{
						std::cout << "fuck you" << std::endl;
						throw;
					}

					// set the end of the if statement
					node->next = placeholder->next;

					// create the start of the else statement
					node = new Node();
					if_else->else_n = node;
				}
				else if (word == "/IF" || word == "/FOREACH")
				{
					if (!if_else || !placeholder)
					{
						std::cout << "fuck you" << std::endl;
						throw;
					}

					// set the end of the if or the else statement
					node->next = placeholder->next;

					// start on the next node
					node = placeholder->next;
					placeholder->next = if_else->else_n;
					break;
				}
			}
			node->data.push_back(str[i]);
		}

		return node;
	}

public:

	void create_template(std::string str)
	{
		term_map_.clear();
		if_else_map_.clear();
		for_each_map_.clear();
		auto node = new Node();
		root_ = node;
		int i = 0;
		recursive_create_template(str, i, node);
	}

	nlohmann::json load_template(nlohmann::json args)
	{
		std::vector<IfElse*> ifs_used;

		if (!args.is_null())
		{
			for (auto& obj : args.get<nlohmann::json::object_t>())
			{
				if (for_each_map_.find(obj.first) != for_each_map_.end())
				{
					for (auto& fe : for_each_map_[obj.first])
					{
						fe->cloned_nodes.clear();

						std::vector<Node*> foreach_nodes;
						for (auto n = fe->if_else->if_n; n != fe->if_else->end; n = n->next)
							foreach_nodes.push_back(n);

						for (auto val : obj.second)
						{
							for (auto n : foreach_nodes)
							{
								Node new_n(n);
								if (new_n.data.empty())
								{
									new_n.data = val.dump();
								}
								fe->cloned_nodes.push_back(new_n);
							}
						}

						fe->if_else->start->next = &fe->cloned_nodes.front();
						for (unsigned long long i = 0; i + 1 < fe->cloned_nodes.size(); ++i)
						{
							fe->cloned_nodes[i].next = &fe->cloned_nodes[i + 1];
						}
						fe->cloned_nodes.back().next = fe->if_else->end;
						ifs_used.push_back(fe->if_else.get());
					}
					continue;
				}

				for (auto t : term_map_[obj.first])
					t->data = obj.second.dump();

				for (auto& ifs : if_else_map_[obj.first])
				{
					ifs->start->next = ifs->if_n;
					ifs_used.push_back(ifs.get());
				}
			}
		}
		else 
		{
			Logger::error("No args found in template", Logger::HIGH);
		}

		std::stringstream ss;
		for (auto n = root_; n != nullptr; n = n->next)
			ss << n->data;

		for (auto& ifs : ifs_used)
			ifs->start->next = ifs->else_n;

		try
		{
			return nlohmann::json::parse(ss);
		}
		catch (const std::exception& e)
		{
			Logger::error("template parse error " + ss.str(), Logger::LOW);
			// Return empty JSON instead of throwing - let caller decide what to do
			return nlohmann::json{};
		}
	}
};