#include <ArduinoOTA.h>

class OtaController{
    private:
        std::function<void()> _onUpdateStart = [](){};
    public:
        OtaController();
        void Begin();
        void Loop();
        void OnUpdateStart(std::function<void()> f);
};
