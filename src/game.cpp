#include "game.h"
#include <sstream>
#include "ec/component/factory/component-factory-create_component_map.h"
#include "ec/component/component-pathway.h"
#include "templates.h"
#include "ec/component/component-template.h"
#include "command/command.h"

Entity* Game::init(std::string init_filepath)
{
    auto game = new Entity();

    //insert factory components
    auto command_map = new Factory::CommandMap();
    command_map->create();
    game->insert_component(command_map, "command_map");

    auto create_component_map = new Factory::CreateComponentMap();
    create_component_map->create();
    game->insert_component(create_component_map, "create_component_map");

    auto pathway = new Component::Pathway();
    game->insert_component(pathway, "pathway");

    // load init file
    std::stringstream game_ss;

    try
    {
        // open files
        std::ifstream game_file(init_filepath);

        // read into temp string streams
        game_ss << game_file.rdbuf();

        // close file streams
        game_file.close();

    }
    catch (std::exception e)
    {
        Logger::error("failed to read game init file", Logger::HIGH);
    }

    auto game_json = nlohmann::json::parse(game_ss);

    if (!game_json.is_array())
    {
        std::vector<nlohmann::json> temp_json;
        temp_json.push_back(game_json);
        game_json = temp_json;
    }

    for (auto& json : game_json)
    {
        auto objects = json.get<nlohmann::json::object_t>();
        for (auto& obj : objects)
        {
            auto command = (*command_map)[obj.first]->get();
            command->load(obj.second, game, pathway);
            command->execute(game, pathway);
        }
    }
    game_ = std::unique_ptr<Entity>(game);
    return game_.get();
}
