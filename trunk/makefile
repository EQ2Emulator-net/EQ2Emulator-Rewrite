# Programs
CC			= gcc
CXX			= g++
LINKER		= g++

# World flags
C_Flags		= `mysql_config --cflags` -march=native -pipe -pthread -std=c++17
CPPFLAGS    = -Idev/VCProjects/
LD_Flags	= `mysql_config --libs` -rdynamic -lm -Wl,-E -ldl
W_Flags		= -Wall -Wno-reorder -Wno-unused-value -Wno-strict-aliasing
D_Flags		= 


ifeq ($(SOLUTION),world)
	APP := eq2world
	Current_Solution_Dir := world
	D_Flags += -DEQ2_WORLD
endif

ifeq ($(SOLUTION),zone)
	APP := eq2zone
	Current_Solution_Dir := zone
	D_Flags += -DEQ2_ZONE
endif

# Setup Debug or Release build
ifeq ($(BUILD),debug)
	# "Debug" build - minimum optimization, and debugging symbols
	C_Flags += -O0 -ggdb
	D_Flags += -DDEBUG -DEQ2_DEBUG
	App_Filename = $(APP)_debug
else
	# "Release" build - optimization, and no debug symbols
	C_Flags += -O2 -s
	D_Flags += -DNDEBUG
	App_Filename = $(APP)
endif

# Formatting helpers
SourcePattern=source/%.cpp
ObjPattern=obj/$(SOLUTION)/$(BUILD)/%.o
LuaSourcePattern=source/Lua/%.c
LuaObjPattern = obj/Lua/$(BUILD)/%.o
SourceToObj=$(patsubst $(SourcePattern),$(ObjPattern),$1)
LuaSourceToObj=$(patsubst $(LuaSourcePattern),$(LuaObjPattern),$1)
# Recursive wildcard function (searches the supplied directory and every sub directory for files matching a pattern)
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# File lists
Common_Source		= $(call rwildcard,source/common/,*.cpp)
World_Source		= $(call rwildcard,source/WorldServer/,*.cpp)
Zone_Source         = $(call rwildcard,source/ZoneServer/,*.cpp)
Lua_Source          = $(call rwildcard,source/Lua/,*.c)

Common_Objects      = $(call SourceToObj,$(Common_Source))
World_Objects       = $(call SourceToObj,$(World_Source))
Zone_Objects        = $(call SourceToObj,$(Zone_Source))
Lua_Objects         = $(call LuaSourceToObj,$(Lua_Source))

ifeq ($(SOLUTION),world)
	Solution_Objects := $(World_Objects) $(Common_Objects) 
endif

ifeq ($(SOLUTION),zone)
	Solution_Objects := $(Zone_Objects) $(Common_Objects) $(Lua_Objects)
endif

# Recipes	
$(APP):$(Solution_Objects)
	@echo Linking...
	@$(LINKER) $(W_Flags) $^ $(LD_Flags) -o bin/$(App_Filename).eq2bin
	@echo Finished building $(SOLUTION).

$(ObjPattern): $(SourcePattern)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(C_Flags) $(D_Flags) $(W_Flags) -c $< -o $@
	
$(LuaObjPattern): $(LuaSourcePattern)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(C_Flags) $(D_Flags) $(W_Flags) -c $< -o $@
	
all_debug:
	@$(MAKE) "SOLUTION=world" "BUILD=debug"
	@$(MAKE) "SOLUTION=zone" "BUILD=debug"
	
all_release:
	@$(MAKE) "SOLUTION=world" "BUILD=release"
	@$(MAKE) "SOLUTION=zone" "BUILD=release"
	
world_debug:
	@$(MAKE) "SOLUTION=world" "BUILD=debug"
	
world_release:
	@$(MAKE) "SOLUTION=world" "BUILD=release"
	
zone_debug:
	@$(MAKE) "SOLUTION=zone" "BUILD=debug"
	
zone_release:
	@$(MAKE) "SOLUTION=zone" "BUILD=release"
	
clean_world:
	rm -rf obj/world/ $(wildcard bin/eq2world*.eq2bin)
	
clean_zone:
	rm -rf obj/zone/ $(wildcard bin/eq2zone*.eq2bin)
	
clean_lua:
	rm -rf obj/Lua/

clean_all:
	rm -rf obj/ $(wildcard bin/*.eq2bin)