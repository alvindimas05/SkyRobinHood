struct RHController;

struct CandleRunController
{
    RHController &controller;

    CandleRunController(RHController &_controller) : controller(_controller) {}
    void Start();
    void Stop();
};
