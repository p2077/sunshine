#include <iostream>
#include <vector>
#include "raylib.h"

const int TILE_SIZE = 46; 

enum class TileType
{
    Floor,
    Wall
};

struct Tile
{
    TileType type;
    std::vector<int> adjacencyList; 
};

class Tilemap
{
public:
    Tilemap(int width, int height) : width(width), height(height)
    {
        tiles.resize(width * height);
    }

    Tile& getTile(int a, int b)
    {
        return tiles[b * width + a];
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    void generateRandomLevel()
    {
        for (int b = 0; b < height; ++b)
        {
            for (int a = 0; a < width; ++a)
            {
                Tile& tile = getTile(a, b);
                tile.type = (rand() % 5 == 0) ? TileType::Wall : TileType::Floor;
            }
        }
    }

    void markWallsAsNonTraversable()
    {
        for (int b = 0; b < height; ++b)
        {
            for (int a = 0; a < width; ++a)
            {
                Tile& tile = getTile(a, b);
                if (tile.type == TileType::Wall)
                {
                    tile.adjacencyList.clear(); 
                }
            }
        }
    }

    void createAdjacencyList()
    {
        for (int b = 0; b < height; ++b)
        {
            for (int a = 0; a < width; ++a)
            {
                Tile& tile = getTile(a, b);
                tile.adjacencyList.clear();

                if (tile.type == TileType::Floor)
                {
                    if (b > 0 && getTile(a, b - 1).type == TileType::Floor)
                        tile.adjacencyList.push_back((b - 1) * width + a);

                    if (b < height - 1 && getTile(a, b + 1).type == TileType::Floor)
                        tile.adjacencyList.push_back((b + 1) * width + a);

                    if (a > 0 && getTile(a - 1, b).type == TileType::Floor)
                        tile.adjacencyList.push_back(b * width + (a - 1));

                    if (a < width - 1 && getTile(a + 1, b).type == TileType::Floor)
                        tile.adjacencyList.push_back(b * width + (a + 1));
                }
            }
        }
    }

    bool isTileTraversable(int a, int b)
    {
        if (a < 0 || a >= width || b < 0 || b >= height)
            return false;

        return getTile(a, b).type == TileType::Floor;
    }

    void draw()
    {
        for (int b = 0; b < height; ++b)
        {
            for (int a = 0; a < width; ++a)
            {
                Tile& tile = getTile(a, b);
                Rectangle rect{ a * TILE_SIZE, b * TILE_SIZE, TILE_SIZE, TILE_SIZE };

                switch (tile.type)
                {
                case TileType::Floor:
                    DrawRectangleRec(rect, WHITE);
                    break;

                case TileType::Wall:
                    DrawRectangleRec(rect, BLACK);
                    break;
                }

                DrawRectangleLinesEx(rect, 1, RED);

                
                if (tile.type == TileType::Floor)
                {
                    Vector2 center = { (rect.x + rect.width / 2), (rect.y + rect.height / 2) };
                    DrawCircle(center.x, center.y, 5, GREEN);

                    for (int adjTileIndex : tile.adjacencyList)
                    {
                        Tile& adjTile = tiles[adjTileIndex];
                        Rectangle adjRect{ adjTileIndex % width * TILE_SIZE, adjTileIndex / width * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                        Vector2 adjCenter = { (adjRect.x + adjRect.width / 2), (adjRect.y + adjRect.height / 2) };
                        DrawLineEx(center, adjCenter, 1, GREEN);
                    }
                }
            }
        }
    }

private:
    int width;
    int height;
    std::vector<Tile> tiles;
};
int main()
{
    const int screenWidth = 920;
    const int screenHeight = 690;

    InitWindow(screenWidth, screenHeight, "Tilemap Maze");

    Tilemap tilemap(20, 15);
    tilemap.generateRandomLevel();
    tilemap.markWallsAsNonTraversable();
    tilemap.createAdjacencyList();

    //starting position
    Vector2 characterPosition{ 1, 1 }; 

    SetTargetFPS(8); 

    while (!WindowShouldClose())
    {
        
        Vector2 newPosition = characterPosition;

        if (IsKeyDown(KEY_W) && tilemap.isTileTraversable(characterPosition.x, characterPosition.y - 1))
            newPosition.y--;
        if (IsKeyDown(KEY_S) && tilemap.isTileTraversable(characterPosition.x, characterPosition.y + 1))
            newPosition.y++;
        if (IsKeyDown(KEY_A) && tilemap.isTileTraversable(characterPosition.x - 1, characterPosition.y))
            newPosition.x--;
        if (IsKeyDown(KEY_D) && tilemap.isTileTraversable(characterPosition.x + 1, characterPosition.y))
            newPosition.x++;

        if (newPosition.x != characterPosition.x || newPosition.y != characterPosition.y)
        {
            characterPosition = newPosition;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        tilemap.draw();

        // character sprite
        Rectangle characterRect{ characterPosition.x * TILE_SIZE, characterPosition.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        DrawRectangleRec(characterRect, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
