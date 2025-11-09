struct RHController;

struct LuaController
{
    RHController &controller;

    LuaController(RHController &_controller) : controller(_controller) {}
    void Init();
    void ExecuteString(const char *luaCode);
    void TeleportToCoords(float x, float y, float z);
    void LoadLevel(const char *levelName);
};