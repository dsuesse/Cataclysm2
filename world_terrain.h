#ifndef _WORLD_TERRAIN_H_
#define _WORLD_TERRAIN_H_

#include "glyph.h"
#include "mapgen.h"
#include <string>
#include <istream>
#include <vector>

enum World_terrain_flag
{
  WTF_NULL = 0,
  WTF_WATER,      // For river building
  WTF_NO_RIVER,   // For river building
  WTF_SALTY,      // Water is salty; also marks end of a river
  WTF_BRIDGE,     // Road on top of this tile is a bridge
  WTF_NO_ROAD,    // Roads don't draw here - but the path continues anyway
  WTF_LINE_DRAWING, // Use line drawings as glyphs
  WTF_RELATIONAL, // Drawing depends on adjacent terrain of same type
  WTF_SHOP,       // Is a shop - for world map generation
  WTF_FACE_ROAD,  // Rotate to face the road
  WTF_ROAD,       // Is a road, for the purpose of face_road
  WTF_MAX
};

World_terrain_flag lookup_world_terrain_flag(std::string name);
std::string world_terrain_flag_name(World_terrain_flag flag);

struct World_terrain;

struct World_terrain_chance
{
  World_terrain_chance(int C = 100, World_terrain* WT = NULL) :
    chance (C), terrain (WT) {}
  int chance;
  World_terrain* terrain;
};

struct Variable_world_terrain
{
public:
  Variable_world_terrain();
  ~Variable_world_terrain(){}

  void add_terrain(int chance, World_terrain* terrain);
  void add_terrain(World_terrain_chance terrain);
  bool load_data(std::istream &data, std::string name = "unknown");

  bool empty();
  World_terrain* pick();

private:
  std::vector<World_terrain_chance> ter;
  int total_chance;
};

struct World_terrain
{
  int uid;
  std::string name; // Unique data name
  std::string display_name; // Name as the player sees it (if blank, use name)
  std::string beach_name; // What we become when we're near to the ocean
  std::vector<std::string> connectors;
  int beach_range;  // The maximum distance to ocean to become beach
  int road_cost;  // Used when pathing roads; high cost means roads go around
  int spread_cost;  // Resistance to being covered by bonuses
  Dice spread; // How much spread_cost we can cover when placed as a bonus
  Variable_world_terrain spread_options;  // Other terrain for spreading
  glyph sym;

  World_terrain();
  ~World_terrain(){}

  std::string get_data_name();
  std::string get_name();
  void assign_uid(int id) { uid = id; }
  bool load_data(std::istream &data);

  bool has_flag(World_terrain_flag flag);

private:
  std::vector<bool> flags;
};

World_terrain* make_into_beach(World_terrain* original);

#endif
