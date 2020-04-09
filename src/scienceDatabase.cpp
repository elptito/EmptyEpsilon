#include "scienceDatabase.h"
#include "factionInfo.h"
#include "shipTemplate.h"
#include "spaceObjects/spaceship.h"

#include "scriptInterface.h"

REGISTER_SCRIPT_CLASS(ScienceDatabase)
{
    REGISTER_SCRIPT_CLASS_FUNCTION(ScienceDatabase, setName);
    REGISTER_SCRIPT_CLASS_FUNCTION(ScienceDatabase, addEntry);
    REGISTER_SCRIPT_CLASS_FUNCTION(ScienceDatabase, addKeyValue);
    REGISTER_SCRIPT_CLASS_FUNCTION(ScienceDatabase, setLongDescription);
    REGISTER_SCRIPT_CLASS_FUNCTION(ScienceDatabase, setImage);
}


PVector<ScienceDatabase> ScienceDatabase::science_databases;

ScienceDatabase::ScienceDatabase()
{
    if (game_server) { LOG(ERROR) << "ScienceDatabase objects can not be created during a scenario right now."; destroy(); return; }
    science_databases.push_back(this);
    name = "???";
}

ScienceDatabase::ScienceDatabase(P<ScienceDatabase> parent, string name)
{
    this->parent = parent;
    this->name = name;
}

ScienceDatabase::~ScienceDatabase()
{
}

P<ScienceDatabase> ScienceDatabase::addEntry(string name)
{
    P<ScienceDatabase> e = new ScienceDatabase(this, name);
    items.push_back(e);
    return e;
}

void ScienceDatabase::addKeyValue(string key, string value)
{
    keyValuePairs.push_back(ScienceDatabaseKeyValue(key, value));
}

void ScienceDatabase::setLongDescription(string text)
{
    longDescription = text;
}

static string directionLabel(float direction)
{
    string name = "?";
    if (std::abs(sf::angleDifference(0.0f, direction)) <= 45)
        name = "Front";
    if (std::abs(sf::angleDifference(90.0f, direction)) < 45)
        name = "Right";
    if (std::abs(sf::angleDifference(-90.0f, direction)) < 45)
        name = "Left";
    if (std::abs(sf::angleDifference(180.0f, direction)) <= 45)
        name = "Rear";
    return name;
}

void fillDefaultDatabaseData()
{
    P<ScienceDatabase> factionDatabase = new ScienceDatabase();
    factionDatabase->setName(tr("database", "Factions"));
    for(unsigned int n=0; n<factionInfo.size(); n++)
    {
        P<ScienceDatabase> entry = factionDatabase->addEntry(factionInfo[n]->getLocaleName());
        for(unsigned int m=0; m<factionInfo.size(); m++)
        {
            if (n == m) continue;

            string stance = tr("stance", "Neutral");
            switch(factionInfo[n]->states[m])
            {
                case FVF_Neutral: stance = tr("stance", "Neutral"); break;
                case FVF_Enemy: stance = tr("stance", "Enemy"); break;
                case FVF_Friendly: stance = tr("stance", "Friendly"); break;
            }
            entry->addKeyValue(factionInfo[m]->getLocaleName(), stance);
        }
        entry->setLongDescription(factionInfo[n]->getDescription());
    }

    P<ScienceDatabase> shipDatabase = new ScienceDatabase();
    shipDatabase->setName(tr("database", "Ships"));

    std::vector<string> template_names = ShipTemplate::getTemplateNameList(ShipTemplate::Ship);
    std::vector<string> template_names_drones = ShipTemplate::getTemplateNameList(ShipTemplate::Drone);
    std::vector<string> template_names_stations = ShipTemplate::getTemplateNameList(ShipTemplate::Station);
    template_names.insert( template_names.end(), template_names_drones.begin(), template_names_drones.end() );
    template_names.insert( template_names.end(), template_names_stations.begin(), template_names_stations.end() );

    std::sort(template_names.begin(), template_names.end());

    std::vector<string> class_list;
    std::set<string> class_set;
    for(string& template_name : template_names)
    {
        P<ShipTemplate> ship_template = ShipTemplate::getTemplate(template_name);

        if (ship_template->getSecret())
            continue;

        string class_name = ship_template->getClass();
        string subclass_name = ship_template->getSubClass();
        if (class_set.find(class_name) == class_set.end())
        {
            class_list.push_back(class_name);
            class_set.insert(class_name);
        }
    }

    std::sort(class_list.begin(), class_list.end());

    std::map<string, P<ScienceDatabase> > class_database_entries;
    for(string& class_name : class_list)
    {
        class_database_entries[class_name] = shipDatabase->addEntry(class_name);
    }

    for(string& template_name : template_names)
    {
        P<ShipTemplate> ship_template = ShipTemplate::getTemplate(template_name);

        if (ship_template->getSecret())
            continue;

        //P<ScienceDatabase> entry = class_database_entries[ship_template->getClass()]->addEntry(template_name);
        P<ScienceDatabase> entry = class_database_entries[ship_template->getClass()]->addEntry(ship_template->getLocaleName());

        entry->model_data = ship_template->model_data;
        entry->setImage(ship_template->radar_trace);

        entry->addKeyValue("Type", ship_template->getLocaleName());
        entry->addKeyValue(tr("database", "Class"), ship_template->getClass());
        entry->addKeyValue(tr("database", "Sub-class"), ship_template->getSubClass());
        entry->addKeyValue(tr("database", "Size"), string(int(ship_template->model_data->getRadius())));
        string shield_info = "";
        for(int n=0; n<ship_template->shield_count; n++)
        {
            string shield_info = "";
            for(int n=0; n<ship_template->shield_count; n++)
            {
                if (n > 0)
                    shield_info += "/";
                shield_info += string(int(ship_template->shield_level[n]));
            }
            entry->addKeyValue("Shield", shield_info);
        }
        entry->addKeyValue(tr("database", "Shield"), shield_info);
        entry->addKeyValue(tr("Hull"), string(int(ship_template->hull)));
        if (ship_template->impulse_speed > 0.0)
        {
            entry->addKeyValue(tr("database", "Move speed"), string(int(ship_template->impulse_speed)));
        }
        if (ship_template->turn_speed > 0.0) 
        {
            entry->addKeyValue(tr("database", "Turn speed"), string(int(ship_template->turn_speed)));
        }
        if (ship_template->warp_speed > 0.0)
        {
            entry->addKeyValue(tr("database", "Has warp drive"), tr("hasdrive","True"));
            entry->addKeyValue(tr("database", "Warp speed"), string(int(ship_template->warp_speed)));
        }
        if (ship_template->has_jump_drive)
        {
            entry->addKeyValue(tr("Has jump drive"), tr("hasdrive","True"));
        }
        int beam_weapons_count = 0;
        for(int n=0; n<max_beam_weapons; n++)
            if (ship_template->beams[n].getRange() > 0)
                beam_weapons_count += 1;
        if (beam_weapons_count > 0)
            entry->addKeyValue("Canons lasers",string(beam_weapons_count));
        for(int n=0; n<max_beam_weapons; n++)
        {
	        if (ship_template->beams[n].getRange() > 0)
            {
                entry->addKeyValue(
                    tr("{direction} beam weapon").format({{"direction", directionLabel(ship_template->beams[n].getDirection())}}),
                    tr("database", "{damage} Dmg / {interval} sec").format({
                        {"damage", string(ship_template->beams[n].getDamage(), 1)},
                        {"interval", string(ship_template->beams[n].getCycleTime(), 1)}
                    })
                );
            }
        }
        for(int n=0; n<ship_template->weapon_tube_count; n++)
        {
            entry->addKeyValue(tr("database", "Missile tubes"), string(ship_template->weapon_tube_count));
            string tube_info = string(int(ship_template->weapon_tube[n].load_time)) + " sec";
            if (ship_template->weapon_tube[n].size == MS_Small)
            {
                tube_info += ", small";
            }
            if (ship_template->weapon_tube[n].size == MS_Large)
            {
                tube_info += ", large";
            }

            entry->addKeyValue(
                key.format({{"direction", directionLabel(ship_template->weapon_tube[n].direction)}}),
                tr("database", "{loadtime} sec").format({{"loadtime", string(int(ship_template->weapon_tube[n].load_time))}})
            );
        }
        for(int n=0; n < MW_Count; n++)
        {
            if (ship_template->weapon_storage[n] > 0)
            {
                entry->addKeyValue(tr("Storage {weapon}").format({{"weapon", getLocaleMissileWeaponName(EMissileWeapons(n))}}), string(ship_template->weapon_storage[n]));
            }
        }
        for(auto& kv : ship_template->custom_weapon_storage)
        {
            if(kv.second > 0)
            {
                entry->addKeyValue(tr("Storage {weapon}").format({{"weapon", getLocaleMissileWeaponName(kv.first)}}), string(kv.second));
                
            }
        }
        if (ship_template->getDescription().length() > 0)
            entry->setLongDescription(ship_template->getDescription());
        if (ship_template->getDocksCount() > 0)
            entry->addKeyValue("Docks", string(ship_template->getDocksCount()));
    }
#ifdef DEBUG
    P<ScienceDatabase> models_database = new ScienceDatabase();
    models_database->setName("Models (debug)");
    for(string name : ModelData::getModelDataNames())
    {
        P<ScienceDatabase> entry = models_database->addEntry(name);
        entry->model_data = ModelData::getModel(name);
    }
#endif
}
