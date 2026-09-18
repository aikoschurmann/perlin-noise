#include "app.h"

// ---------------------------------------------------------
// Entry Point
// ---------------------------------------------------------
int main(int argc, const char** argv) {
    Application app = {0};
    
    if (App_Initialize(&app, argc, argv)) {
        App_Run(&app);
    }
    
    App_Shutdown(&app);
    return 0;
}
