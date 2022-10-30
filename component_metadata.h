#include <vector>
#include <string>

struct ComponentWirePosition {
    int x, y;
};

struct ComponentMetadataEntry {
    std::string name;
    int width;
    int height;
    std::vector<ComponentWirePosition> inputs = {};
    std::vector<ComponentWirePosition> outputs = {};
};

std::vector<ComponentMetadataEntry> componentMetadata = {
        {
                .name =    "and",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {8, 52},
                        {44, 52},
                },
                .outputs = {
                        {26, 0},
                },
        },
        {
                .name =   "or",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {8, 52},
                        {44, 52},
                },
                .outputs = {
                        {26, 0},
                },
        },
        {
                .name =   "xor",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {8, 52},
                        {44, 52},
                },
                .outputs = {
                        {26, 0},
                },
        },
        {
                .name =   "switch",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {26, 52},
                },
                .outputs = {
                        {26, 0},
                },
        },
        {
                .name =   "button",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {26, 52},
                },
                .outputs = {
                        {26, 0},
                },
        },
        {
                .name =   "splitter",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {26, 52},
                },
                .outputs = {
                        {8, 0},
                        {44, 0},
                },
        },
        {
                .name =   "combiner",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {8, 52},
                        {44, 52},
                },
                .outputs = {
                        {26, 0},
                },
        },
        {
                .name =   "led",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {26, 38},
                },
                .outputs = {
                        {26, 14},
                },
        },
        {
                .name =   "conveyor_sensor",
                .width =  17,
                .height = 17,
                .inputs =  {
                        {0, 28},
                },
                .outputs = {
                        {52, 28},
                },
        },
        {
                .name =   "timer",
                .width =  17,
                .height = 17,
                .inputs = {
                        {26, 52},
                },
                .outputs = {
                        {26, 0},
                },
        },
        {
                .name =   "solar_panel",
                .width =  51,
                .height = 17,
                .outputs = {
                        {52, 26},
                },
        },
        {
                .name =   "steam_generator",
                .width =  48,
                .height =  48,
                .outputs = {
                        {26, 0},
                },
        },
};