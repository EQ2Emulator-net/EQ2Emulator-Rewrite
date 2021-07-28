#include "stdafx.h"

#include "LogSpawnsParser.h"
#include "../Packets/OP_CreateSignWidgetCmd_Packet.h"

#include <string>


LogSpawnsParser::LogSpawnsParser(PacketLog& log, ParserDatabase& db) : LogParser(log, db) {
	uint32_t spawn_id = db.LoadNextSpawnID();

	//This is written for legacy code tables since that is what content designers are using

	auto spawns = log.FindPackets<OP_CreateGhostCmd_Packet>();

	{
		auto widgets = log.FindPackets<OP_CreateWidgetCmd_Packet, OP_CreateGhostCmd_Packet>();
		auto signs = log.FindPackets<OP_CreateSignWidgetCmd_Packet, OP_CreateGhostCmd_Packet>();

		std::move(widgets.begin(), widgets.end(), std::back_inserter(spawns));
		std::move(signs.begin(), signs.end(), std::back_inserter(spawns));
	}
	
	for (auto& itr : spawns) {
		auto& p = *itr.second;

		if (p.packedData.lastPackedSize == 0) {
			//This is a spawn resend, garbage for us
			continue;
		}

		//We can ignore players as well as corpses
		if (p.footer.titleStruct.isPlayer || (p.info.entityFlags & EntityFlagCorpse)) continue;

		//First create the `spawn` table entry
		{
			DatabaseRow row;
			row.m_tableName = "spawn";
			row.RegisterField("id", spawn_id);
			
			//Naming info
			auto& ts = p.footer.titleStruct;
			row.RegisterField("name", ts.SpawnTitleStruct::name);
			row.RegisterField("sub_title", ts.guild);
			row.RegisterField("prefix", ts.prefix_title);
			row.RegisterField("suffix", ts.suffix_title);
			row.RegisterField("last_name", ts.last_name);

			auto& info = p.info;
			auto& pos = p.pos;
			auto& vis = p.vis;

			if (info.visual_state == 0 && info.action_state != 0)
				info.visual_state = info.action_state;

			row.RegisterField("race", info.race);
			row.RegisterField("model_type", info.model_type);
			row.RegisterField("size", static_cast<uint16_t>(pos.size * 32.f));

			using svs = SpawnVisualizationStruct;
			row.RegisterField<bool>("targetable", vis.vis_flags & svs::VIS_FLAG_TARGETABLE);
			row.RegisterField<bool>("show_name", vis.vis_flags & svs::VIS_FLAG_DISPLAY_NAME);
			row.RegisterField("visual_state", info.visual_state);
			row.RegisterField<bool>("attackable", vis.vis_flags & svs::VIS_FLAG_ATTACKABLE);
			row.RegisterField<bool>("show_level", vis.vis_flags & svs::VIS_FLAG_SHOW_LEVEL);
			row.RegisterField<bool>("show_command_icon", vis.vis_flags & svs::VIS_FLAG_SHOW_COMMAND_ICON);
			row.RegisterField("collision_radius", static_cast<uint16_t>(pos.collisionRadius * 32.f));
			row.RegisterField("hp", 15 * info.level + 1);
			row.RegisterField("power", 15 * info.level + 1);
			QueueRowInsert(row);
		}

		auto GetGroundspawnType = [this, &spawn_id](const std::string& cmd) -> const char* {
			if (cmd == "Collect") return "Collecting";
			else if (cmd == "Gather") return "Gathering";
			else if (cmd == "Trap") return "Trapping";
			else if (cmd == "Mine") return "Mining";
			else if (cmd == "Fish") return "Fishing";
			else if (cmd == "Chop") return "Foresting";
			return nullptr;
		};

		//Now determine the other tables needed
		if (auto chk = dynamic_cast<OP_CreateSignWidgetCmd_Packet*>(itr.second.get())) {
			//This is a sign packet
			auto& p = *chk;

			DatabaseRow row;
			row.m_tableName = "spawn_signs";
			row.RegisterField("spawn_id", spawn_id);

			auto& s = p.signData;
			auto& w = p.widgetData;

			row.RegisterField("widget_id", w.widgetID);
			if (s.title.empty())
				row.RegisterField("title", SQLNull());
			else
				row.RegisterField("title", s.title);
			row.RegisterField("widget_x", w.widgetX);
			row.RegisterField("widget_y", w.widgetY);
			row.RegisterField("widget_z", w.widgetZ);
			row.RegisterField("description", s.description);
			row.RegisterField("sign_distance", s.distance);
			//old code compatibility bullshit
			row.RegisterField("icon", (p.info.entityFlags & EntityFlagIsTransport) ? 28 : 0);

			auto& pos = p.pos;

			row.RegisterField<bool>("include_heading", true);
			row.RegisterField<bool>("include_location", pos.x != 0.f || pos.y != 0.f || pos.z != 0.f);
			QueueRowInsert(row);
		}
		else if (auto chk = dynamic_cast<OP_CreateWidgetCmd_Packet*>(itr.second.get())) {
			//This is a widget packet
			auto& p = *chk;

			DatabaseRow row;
			row.m_tableName = "spawn_widgets";
			row.RegisterField("spawn_id", spawn_id);

			auto& w = p.widgetData;
			auto& pos = p.pos;

			row.RegisterField("widget_id", w.widgetID);
			row.RegisterField("widget_x", w.widgetX);
			row.RegisterField("widget_y", w.widgetY);
			row.RegisterField("widget_z", w.widgetZ);
			row.RegisterField<bool>("include_heading", true);
			row.RegisterField<bool>("include_location", pos.x != 0.f || pos.y != 0.f || pos.z != 0.f);
			//old code compatibility bullshit
			row.RegisterField("icon", (p.info.entityFlags & EntityFlagIsTransport) ? 28 : 0);
			QueueRowInsert(row);
		}
		else if (const char* gt = GetGroundspawnType(p.header.default_command)) {
			//This is a groundspawn

			DatabaseRow row;
			row.m_tableName = "spawn_ground";
			row.RegisterField("spawn_id", spawn_id);
			row.RegisterField("collection_skill", std::string(gt));
			QueueRowInsert(row);
		}
		else if (p.pos.positionState & POS_STATE_SIMULATE_GRAVITY) {
			//This should be an NPC

			//NPCs have extra data, start with the base table entry then do appearances
			{
				DatabaseRow row;
				row.m_tableName = "spawn_npcs";

				auto& info = p.info;

				row.RegisterField("spawn_id", spawn_id);
				row.RegisterField("min_level", info.level);
				row.RegisterField("max_level", info.level);
				row.RegisterField("enc_level", info.difficulty);
				row.RegisterField("class_", info.adv_class);
				row.RegisterField("gender", info.gender);
				row.RegisterField("hair_type_id", info.hair_type_id);
				row.RegisterField("facial_hair_type_id", info.facial_hair_type_id);
				row.RegisterField("wing_type_id", info.wing_type_id);
				if (info.chest_type_id == -1) info.chest_type_id = 0;
				if (info.legs_type_id == -1) info.legs_type_id = 0;
				row.RegisterField("chest_type_id", info.chest_type_id);
				row.RegisterField("legs_type_id", info.legs_type_id);
				row.RegisterField("soga_hair_type_id", info.soga_hair_type_id);
				row.RegisterField("soga_facial_hair_type_id", info.soga_facial_hair_type_id);
				row.RegisterField("soga_model_type", info.soga_model_type);
				row.RegisterField<bool>("hide_hood", info.visual_flag & INFO_VIS_FLAG_HIDE_HOOD);
				row.RegisterField("heroic_flag", info.heroic_flag);
				row.RegisterField("action_state", info.action_state);
				row.RegisterField("mood_state", info.mood_state);
				row.RegisterField("emote_state", info.emote_state);
				row.RegisterField<uint16_t>("initial_state", p.pos.positionState);
				row.RegisterField<uint16_t>("activity_status", info.entityFlags >> 16);
				row.RegisterField("str", 25);
				row.RegisterField("sta", 25);
				row.RegisterField("wis", 25);
				row.RegisterField("intel", 25);
				row.RegisterField("agi", 25);
				row.RegisterField<bool>("flying_type", p.pos.positionState & POS_STATE_DISABLE_GRAVITY);
				QueueRowInsert(row);
			}

			ParseNPCAppearance(p, spawn_id);
		}
		else {
			//Likely an object but could be an NPC
			DatabaseRow row;
			row.m_tableName = "spawn_objects";

			row.RegisterField("spawn_id", spawn_id);
			QueueRowInsert(row);
		}
		
		++spawn_id;
	}

	DoInsertsForTable("spawn", 50);
	DoInsertsForTable("spawn_ground", 250);
	DoInsertsForTable("spawn_npcs", 50);
	DoInsertsForTable("spawn_objects", 250);
	DoInsertsForTable("spawn_signs", 250);
	DoInsertsForTable("spawn_widgets", 250);
	DoInsertsForTable("npc_appearance", 250);
	DoInsertsForTable("npc_appearance_equip", 250);
}

void LogSpawnsParser::ParseNPCAppearance(OP_CreateGhostCmd_Packet& p, uint32_t spawn_id) {
	//Handle the `npc_appearance` table data

	//First take care of the morph sliders
	{
		auto CreateSliderRow = [this, &spawn_id](const char* type, int8_t* field) {
			if (field[0] == 0 && field[1] == 0 && field[2] == 0) return;

			DatabaseRow row;
			row.m_tableName = "npc_appearance";

			row.RegisterField("spawn_id", spawn_id);
			row.RegisterField("signed_value", 1);
			row.RegisterField("type", std::string(type));
			row.RegisterField("red", field[0]);
			row.RegisterField("green", field[1]);
			row.RegisterField("blue", field[2]);

			QueueRowInsert(row);
		};

		auto& sliders = p.info.sliders;
		auto& soga = p.info.sogaSliders;

		CreateSliderRow("skull_type", sliders.skull);
		CreateSliderRow("eye_type", sliders.eyes);
		CreateSliderRow("ear_type", sliders.ears);
		CreateSliderRow("eye_brow_type", sliders.eyebrow);
		CreateSliderRow("cheek_type", sliders.cheeks);
		CreateSliderRow("lip_type", sliders.mouth);
		CreateSliderRow("chin_type", sliders.chin);
		CreateSliderRow("nose_type", sliders.nose);

		CreateSliderRow("soga_skull_type", soga.skull);
		CreateSliderRow("soga_eye_type", soga.eyes);
		CreateSliderRow("soga_ear_type", soga.ears);
		CreateSliderRow("soga_eye_brow_type", soga.eyebrow);
		CreateSliderRow("soga_cheek_type", soga.cheeks);
		CreateSliderRow("soga_lip_type", soga.mouth);
		CreateSliderRow("soga_chin_type", soga.chin);
		CreateSliderRow("soga_nose_type", soga.nose);

		//These 2 sliders are just 1 value, also dif names in old code
		int8_t tmp[3]{ sliders.bodyscale, 0, 0 };
		CreateSliderRow("body_size", tmp);
		tmp[0] = sliders.bumpscale;
		CreateSliderRow("body_age", tmp);
		tmp[0] = soga.bodyscale;
		CreateSliderRow("soga_body_size", tmp);
		tmp[1] = soga.bumpscale;
		CreateSliderRow("soga_body_age", tmp);
	}

	auto CreateColorRow = [this, &spawn_id](const char* type, const EQ2Color& c) {
		if (c.Red == 0 && c.Blue == 0 && c.Green == 0) return;

		DatabaseRow row;
		row.m_tableName = "npc_appearance";

		row.RegisterField("spawn_id", spawn_id);
		row.RegisterField("signed_value", 0);
		row.RegisterField("type", std::string(type));
		row.RegisterField("red", c.Red);
		row.RegisterField("green", c.Green);
		row.RegisterField("blue", c.Blue);

		QueueRowInsert(row);
	};

	//Now the non-item attachments/hair and eye colors
	{
		auto& info = p.info;
		//Attachment colors
		if (info.facial_hair_type_id) {
			CreateColorRow("hair_type_color", info.hair_type_color);
			CreateColorRow("hair_type_highlight_color", info.hair_type_highlight_color);
		}
		if (info.facial_hair_type_id) {
			CreateColorRow("hair_face_color", info.hair_face_color);
			CreateColorRow("hair_face_highlight_color", info.hair_face_highlight_color);
		}
		if (info.wing_type_id) {
			CreateColorRow("wing_color1", info.wing_color1);
			CreateColorRow("wing_color2", info.wing_color2);
		}
		if (info.chest_type_id) {
			CreateColorRow("shirt_color", info.chest_type_color);
			CreateColorRow("shirt_highlight_color", info.chest_type_highlight);
		}
		if (info.legs_type_id) {
			CreateColorRow("pants_color", info.legs_type_color);
			CreateColorRow("pants_highlight_color", info.legs_type_highlight);
		}
		if (info.soga_hair_type_id) {
			CreateColorRow("soga_hair_type_color", info.soga_hair_type_color);
			CreateColorRow("soga_hair_type_highlight_color", info.soga_hair_type_highlight_color);
		}
		if (info.soga_facial_hair_type_id) {
			CreateColorRow("soga_hair_face_color", info.soga_hair_face_color);
			CreateColorRow("soga_hair_face_highlight_color", info.soga_hair_face_highlight_color);
		}
		if (info.soga_wing_type_id) {
			CreateColorRow("soga_wing_color1", info.soga_wing_color1);
			CreateColorRow("soga_wing_color2", info.soga_wing_color2);
		}
		CreateColorRow("skin_color", info.skin_color);
		CreateColorRow("hair_color1", info.hair_color1);
		CreateColorRow("hair_color2", info.hair_color2);
		CreateColorRow("hair_highlight", info.hair_highlight);
		CreateColorRow("eye_color", info.eye_color);
		CreateColorRow("soga_skin_color", info.soga_skin_color);
		CreateColorRow("soga_eye_color", info.soga_eye_color);
		CreateColorRow("soga_hair_color1", info.soga_hair_color1);
		CreateColorRow("soga_hair_color2", info.soga_hair_color2);
		CreateColorRow("soga_hair_highlight", info.soga_hair_highlight);
	}

	auto CreateEquipRow = [this, &spawn_id](int slot, uint32_t id, const EQ2Color& c, const EQ2Color& h) {
		if (!id) return;

		DatabaseRow row;
		row.m_tableName = "npc_appearance_equip";

		row.RegisterField("spawn_id", spawn_id);
		row.RegisterField("slot_id", slot);
		row.RegisterField("equip_type", id);
		row.RegisterField("red", c.Red);
		row.RegisterField("green", c.Green);
		row.RegisterField("blue", c.Blue);
		row.RegisterField("highlight_red", h.Red);
		row.RegisterField("highlight_green", h.Green);
		row.RegisterField("highlight_blue", h.Blue);

		QueueRowInsert(row);
	};

	auto& info = p.info;

	for (int i = 0; i < 24; i++) {
		CreateEquipRow(i, info.equipment_types[i], info.equipment_colors[i], info.equipment_highlights[i]);
	}
}