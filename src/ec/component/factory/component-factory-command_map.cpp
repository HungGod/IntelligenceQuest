#include "ec/component/factory/component-factory-command_map.h"
#include "command/command-change_state.h"
#include "command/command-delay_cycles.h"
#include "command/command-create_components.h"
#include "command/command-load_files.h"
#include "command/command-print_gamestate.h"
#include "command/command-update_components.h"
#include "command/command-parse_json_template.h"
#include "command/command-normalize_srcs.h"
#include "command/command-init_quadtree.h"
#include "command/command-save_state.h"
#include "command/command-add_intelligence.h"
#include "command/command-smart_enough.h"
#include "command/command-delay_time.h"
#include "command/command-play_animation.h"
#include "command/command-series.h"
#include "command/command-add_flag.h"
#include "command/command-move.h"
#include "command/command-add_kerning.h"
#include "command/command-navigate_textbox.h"
#include "command/command-update_speech_arrow.h"
#include "command/command-update_message.h"
#include "command/command-repeated_event.h"
#include "command/command-update_navigate.h"
#include "command/command-update_options.h"
#include "command/command-pause.h"
#include "command/command-backtrack.h"
#include "command/command-exit_game.h"
#include "command/command-simultaneous.h"
#include "command/command-static_move.h"
#include "command/command-zoom.h"


template <typename T>
std::shared_ptr<Ring<ICommand>> create_command_ring()
{
	const static std::size_t RING_SIZE = 128u;
	auto ring = new Ring<ICommand>();
	ring->resize<T>(RING_SIZE);
	return std::shared_ptr<Ring<ICommand>>(ring);
}

void Factory::CommandMap::create()
{
	(*this)["ChangeState"] = create_command_ring<Command::ChangeState>();
	(*this)["CreateComponents"] = (*this)["CreateComponent"] = create_command_ring<Command::CreateComponents>();
	(*this)["LoadFiles"] = (*this)["LoadJson"] = (*this)["GameState"] = (*this)["Gamestate"] = create_command_ring<Command::LoadFiles>();
	(*this)["DelayCycles"] = (*this)["DelayNext"] = create_command_ring<Command::DelayCycles>();
	(*this)["PrintGamestate"] = create_command_ring<Command::PrintGamestate>();
	(*this)["UpdateComponents"] = create_command_ring<Command::UpdateComponents>();
	(*this)["ParseJsonTemplate"] = create_command_ring<Command::ParseJsonTemplate>();
	(*this)["NormalizeSrcs"] = create_command_ring<Command::NormalizeSrcs>();
	(*this)["InitQuadTree"] = create_command_ring<Command::InitQuadTree>();
	(*this)["SaveState"] = create_command_ring<Command::SaveState>();
	(*this)["AddIntelligence"] = create_command_ring<Command::AddIntelligence>();
	(*this)["SmartEnough"] = create_command_ring<Command::SmartEnough>();
	(*this)["DelayTime"] = create_command_ring<Command::DelayTime>();
	(*this)["PlayAnimation"] = create_command_ring<Command::PlayAnimation>();
	(*this)["Series"] = create_command_ring<Command::Series>();
	(*this)["AddFlag"] = create_command_ring<Command::AddFlag>();
	(*this)["Move"] = create_command_ring<Command::Move>();
	(*this)["AddKerning"] = create_command_ring<Command::AddKerning>();
	(*this)["ControllerNavigate"] = create_command_ring<Command::ControllerNavigate>();
	(*this)["UpdateSpeechArrow"] = create_command_ring<Command::UpdateSpeechArrow>();
	(*this)["UpdateMessage"] = create_command_ring<Command::UpdateMessage>();
	(*this)["RepeatedEvent"] = create_command_ring<Command::RepeatedEvent>();
	(*this)["UpdateNavigate"] = create_command_ring<Command::UpdateNavigate>();
	(*this)["UpdateOptions"] = create_command_ring<Command::UpdateOptions>();
	(*this)["Pause"] = create_command_ring<Command::Pause>();
	(*this)["Backtrack"] = create_command_ring<Command::Backtrack>();
	(*this)["ExitGame"] = create_command_ring<Command::ExitGame>();
	(*this)["Simultaneous"] = create_command_ring<Command::Simultaneous>();
	(*this)["Zoom"] = create_command_ring<Command::Zoom>();
	(*this)["StaticMove"] = create_command_ring<Command::StaticMove>();
}
