#include <raylib.h>
#include <raymath.h>

#define G 400
#define PLAYER_JUMP_SPD 300.0f
#define PLAYER_HOR_SPD 300.0f

typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
    Texture2D texture;
} Player;

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
} EnvItem;

typedef struct TexturedEnvItem {
  Rectangle rect;
  int blocking;
  Texture2D texture;
} TexturedEnvItem;

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, TexturedEnvItem* texturedEnvItems, int texturedEnvItemsLength, float delta);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    Player player = { 0 };
    player.position = (Vector2){ 400, 280 };
    player.speed = 0;
    player.canJump = false;
    player.texture = LoadTexture("gribok.png");
    EnvItem envItems[] = {
        {{ 0, 450, 1000, 20 }, 1, LIGHTGRAY },
    };
    TexturedEnvItem texturedEnvItems[] = {
      {{ 600, 350, 100, 100}, 1, LoadTexture("supermario.png")}
    }; 

    int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);
    int texturedEnvItemsLength = sizeof(envItems)/sizeof(envItems[0]);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        float deltaTime = GetFrameTime();

        UpdatePlayer(&player, envItems, envItemsLength, texturedEnvItems, texturedEnvItemsLength, deltaTime);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(WHITE);

	    for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
	    for (int i = 0; i < texturedEnvItemsLength; i++) {
	      TexturedEnvItem item = texturedEnvItems[i];
	      DrawTexturePro(item.texture, (Rectangle){0, 0, item.texture.width, item.texture.height}, item.rect, (Vector2){0}, 0.0f, WHITE);
	    }

	    Rectangle playerRect = { player.position.x - 40, player.position.y - 80, 80.0f, 80.0f };
	      DrawTexturePro(player.texture, (Rectangle){0, 0, player.texture.width, player.texture.height}, playerRect, (Vector2){0}, 0.0f, WHITE);
	    
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, TexturedEnvItem *texturedEnvItems, int texturedEnvItemsLength, float delta)
{
    if (IsKeyDown(KEY_A)) player->position.x -= PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_D)) player->position.x += PLAYER_HOR_SPD*delta;
    if (IsKeyDown(KEY_SPACE) && player->canJump)
    {
        player->speed = -PLAYER_JUMP_SPD;
        player->canJump = false;
    }

    bool hitObstacle = false;
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y <= p->y + player->speed*delta)
        {
            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->rect.y;
            break;
        }
    }
    for (int i = 0; i < texturedEnvItemsLength; i++)
    {
        TexturedEnvItem *ei = texturedEnvItems + i;
        Vector2 *p = &(player->position);
        if (ei->blocking &&
            ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y <= p->y + player->speed*delta)
        {
            hitObstacle = true;
            player->speed = 0.0f;
            p->y = ei->rect.y;
            break;
        }
    }

    if (!hitObstacle)
    {
        player->position.y += player->speed*delta;
        player->speed += G*delta;
        player->canJump = false;
    }
    else player->canJump = true;
}
