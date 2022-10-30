#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION

#include "external/raygui.h"
#include "component_metadata.h"
#include "raymath.h"
#include "nlohmann/json.hpp"
#include "file_actions.h"
#include "external/tinyfiledialogs.h"

using json = nlohmann::json;

struct ComponentTextureEntry {
    ComponentMetadataEntry metadata;
    Texture2D texture{};
};

struct PlacedComponent {
    std::string componentName;
    Vector2 position;
};

void to_json(json &j, const PlacedComponent &c) {
    j = json{
            {"componentName", c.componentName},
            {"position",      {c.position.x, c.position.y}}
    };
}

void from_json(const json &j, PlacedComponent &c) {
    c.componentName = j.at("componentName").get<std::string>();
    c.position.x = j.at("position").at(0).get<float>();
    c.position.y = j.at("position").at(1).get<float>();
}

struct WireConnection {
    Vector2 start;
    Vector2 end;
};

void to_json(json &j, const WireConnection &c) {
    j = json{
            {"start", {c.start.x, c.start.y}},
            {"end",   {c.end.x,   c.end.y}}
    };
}

void from_json(const json &j, WireConnection &c) {
    c.start.x = j.at("start").at(0).get<float>();
    c.start.y = j.at("start").at(1).get<float>();
    c.end.x = j.at("end").at(0).get<float>();
    c.end.y = j.at("end").at(1).get<float>();
}

// floor_to_n
// Rounds a number down to the nearest multiple of n
float floor_to_n(float x, float n) {
    return floor(x / n) * n;
}

int main() {
    const int window_width = 1200, window_height = 800;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(window_width, window_height, "Islands Circuit Designer");
    SetWindowMinSize(window_width, window_height);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    ChangeDirectory(GetApplicationDirectory());

    ComponentTextureEntry componentTextures[componentMetadata.size()];
    for (int i = 0; i < componentMetadata.size(); i++) {
        componentTextures[i] = ComponentTextureEntry{
                componentMetadata[i],
                LoadTexture(("sprites/" + componentMetadata[i].name + ".png").c_str()),
        };
    }

    std::vector<PlacedComponent> placedComponents;
    std::vector<WireConnection> wireConnections;

    Vector2 scrollPos = {0, 0};
    float totalComponentListHeight = (float) componentMetadata.size() * 80 + 30;
    int selectedComponent = -1;
    std::string modes[3] = {"place", "delete", "wire"};
    std::string actions[2] = {"import", "export"};
    /*
     * 0 = place component
     * 1 = delete component
     * 2 = wire
     */
    int mode = 0;
    Vector2 currentWireStart = {0, 0};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (mode == 0 && selectedComponent != -1) {
            auto textureData = componentTextures[selectedComponent];
            auto componentTexture = textureData.texture;
            Vector2 componentPos = {
                    floor_to_n((float) GetMouseX(), 64),
                    floor_to_n((float) GetMouseY(), 64),
            };
            if (componentPos.x > 200 && componentPos.y > 60) {
                DrawTexturePro(
                        componentTexture,
                        {0, 0, (float) componentTexture.width, (float) componentTexture.height},
                        (Rectangle) {componentPos.x, componentPos.y, 64, 64},
                        (Vector2) {0, 0},
                        0,
                        GRAY
                );
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    std::cout << "Component: " << textureData.metadata.name << " Mouse clicked at "
                              << componentPos.x << ", " << componentPos.y << std::endl;
                    placedComponents.push_back(PlacedComponent{
                            textureData.metadata.name,
                            componentPos,
                    });
                }
            }
        }
        if (mode == 0) {
            GuiScrollPanel(
                    (Rectangle) {0, 0, 200, (float) (GetScreenHeight())},
                    "Components",
                    (Rectangle) {0, 0, 180, totalComponentListHeight},
                    &scrollPos
            );
            for (int i = 0; i < componentMetadata.size(); i++) {
                auto &componentTexture = componentTextures[i];
                auto &metadata = componentTexture.metadata;
                Vector2 runtimePosition = {
                        (float) (180 - 64) / 2,
                        (float) (i * 80) + 30 + scrollPos.y,
                };
                if (GuiButton((Rectangle) {runtimePosition.x, runtimePosition.y, 64, 64}, "x")) {
                    selectedComponent = i;
                }
                DrawTexturePro(
                        componentTexture.texture,
                        (Rectangle) {0, 0, (float) componentTexture.texture.width,
                                     (float) componentTexture.texture.height},
                        (Rectangle) {runtimePosition.x, runtimePosition.y, 64, 64},
                        (Vector2) {0, 0},
                        0,
                        WHITE
                );
            }
        }
        if (mode == 2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            std::cout << "Mouse clicked at " << GetMouseX() << ", " << GetMouseY() << std::endl;
            // verify that the mouse is over a component input
            for (auto &component: placedComponents) {
                auto &metadata = componentTextures[std::distance(
                        componentMetadata.begin(),
                        std::find_if(
                                componentMetadata.begin(),
                                componentMetadata.end(),
                                [&component](const ComponentMetadataEntry &entry) {
                                    return entry.name == component.componentName;
                                }
                        )
                )].metadata;
                for (auto &input: metadata.inputs) {
                    Vector2 inputPos = {
                            component.position.x + (float) input.x,
                            component.position.y + (float) input.y,
                    };
                    if (Vector2Distance(inputPos, (Vector2) {(float) GetMouseX(), (float) GetMouseY()}) < 17) {
                        std::cout << "Input clicked at " << inputPos.x << ", " << inputPos.y << std::endl;
                        currentWireStart = inputPos;
                    }
                }
            }
        }
        for (const auto &comp: placedComponents) {
            auto it = std::find_if(componentTextures, componentTextures + componentMetadata.size(),
                                   [&comp](const ComponentTextureEntry &entry) {
                                       return entry.metadata.name == comp.componentName;
                                   });
            DrawTexturePro(
                    it->texture,
                    {0, 0, (float) it->texture.width, (float) it->texture.height},
                    (Rectangle) {comp.position.x, comp.position.y, 64, 64},
                    (Vector2) {0, 0},
                    0,
                    WHITE
            );
            if (mode == 2) {
                for (const auto &input: it->metadata.inputs) {
                    DrawRectangle(
                            (int) comp.position.x + input.x,
                            (int) comp.position.y + input.y,
                            12,
                            12,
                            BLUE
                    );
                }
                for (const auto &input: it->metadata.outputs) {
                    DrawRectangle(
                            (int) comp.position.x + input.x,
                            (int) comp.position.y + input.y,
                            12,
                            12,
                            PURPLE
                    );
                }
            }
        }
        for (const auto &wire: wireConnections) {
            DrawLineEx(
                    Vector2AddValue(wire.start, 6),
                    Vector2AddValue(wire.end, 6),
                    4,
                    RED
            );
        }
        if (mode == 2 && currentWireStart.x > 0 && currentWireStart.y > 0) {
            DrawLineEx(
                    currentWireStart,
                    (Vector2) {(float) GetMouseX(), (float) GetMouseY()},
                    2,
                    RED
            );
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // verify that the mouse is over a component output
                for (auto &component: placedComponents) {
                    auto &metadata = componentTextures[std::distance(
                            componentMetadata.begin(),
                            std::find_if(
                                    componentMetadata.begin(),
                                    componentMetadata.end(),
                                    [&component](const ComponentMetadataEntry &entry) {
                                        return entry.name == component.componentName;
                                    }
                            )
                    )].metadata;
                    for (auto &output: metadata.outputs) {
                        Vector2 outputPos = {
                                component.position.x + (float) output.x,
                                component.position.y + (float) output.y,
                        };
                        if (Vector2Distance(outputPos, (Vector2) {(float) GetMouseX(), (float) GetMouseY()}) < 17) {
                            std::cout << "Output clicked at " << outputPos.x << ", " << outputPos.y << std::endl;
                            wireConnections.push_back(WireConnection{
                                    currentWireStart,
                                    outputPos,
                            });
                            currentWireStart = {0, 0};
                        }
                    }
                }
            }
        }
        if (mode == 2 && currentWireStart.x != 0 && currentWireStart.y != 0 && IsKeyPressed(KEY_ESCAPE)) {
            currentWireStart = {0, 0};
        }
        if (mode == 1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            for (int i = 0; i < placedComponents.size(); ++i) {
                auto &component = placedComponents[i];
                if (mousePos.x > component.position.x && mousePos.x < component.position.x + 64 &&
                    mousePos.y > component.position.y && mousePos.y < component.position.y + 64) {
                    std::cout << "Component: " << component.componentName << " clicked at "
                              << component.position.x << ", " << component.position.y << std::endl;
                    placedComponents.erase(placedComponents.begin() + i);
                }
            }
            for (int i = 0; i < wireConnections.size(); ++i) {
                auto &wire = wireConnections[i];
                if (mousePos.x > wire.start.x && mousePos.x < wire.end.x &&
                    mousePos.y > wire.start.y && mousePos.y < wire.end.y) {
                    std::cout << "Wire: " << wire.start.x << ", " << wire.start.y << " -> "
                              << wire.end.x << ", " << wire.end.y << std::endl;
                    wireConnections.erase(wireConnections.begin() + i);
                }
            }
        }
        for (int i = 0; i < 3; ++i) {
            if (GuiButton((Rectangle) {250 + (float) i * 85, 0, 80, 40}, modes[i].c_str())) {
                std::cout << "Mode " << modes[i] << " selected" << std::endl;
                mode = i;
            }
        }
        for (int i = 0; i < 2; ++i) {
            if (GuiButton((Rectangle) {(float) GetScreenWidth() - (float) (i + 1) * 85 - 30, 0, 80, 40},
                          actions[i].c_str())) {
                std::cout << "Action " << actions[i] << " selected" << std::endl;
                if (actions[i] == "export") {
                    std::string path = show_save_file_dialog("circuit_save.iscd");
                    if (!path.empty()) {
                        std::ofstream file(path);
                        nlohmann::json json;
                        json["components"] = placedComponents;
                        json["wires"] = wireConnections;
                        file << json.dump();
                        file.close();
                    }
                } else if (actions[i] == "import") {
                    std::string path = show_open_file_dialog();
                    if (!path.empty()) {
                        std::ifstream file(path);
                        nlohmann::json json;
                        try {
                            file >> json;
                            placedComponents = json["components"].get<std::vector<PlacedComponent>>();
                            wireConnections = json["wires"].get<std::vector<WireConnection>>();
                        } catch (nlohmann::json::exception &e) {
                            std::cout << "Error parsing JSON: " << e.what() << std::endl;
                            tinyfd_messageBox("Error",
                                              "There was an error parsing that Islands Circuit Designer save file! Please make sure your file is correct.",
                                              "ok", "error", 1);
                        }
                        file.close();
                    }
                }
            }
        }
        DrawText(("Mode: " + modes[mode]).c_str(), 250 + 4 * 85, 10, 20, BLACK);
        if (mode == 0) {
            DrawText(("Current component: " + (selectedComponent != -1 ? componentMetadata[selectedComponent].name : "None")).c_str(), 250 + 4 * 85, 30, 20, BLACK);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
