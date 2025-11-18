#include "controllers/rh/rh.hpp"

int main()
{
    RHController rhController(std::string("files/rh.json"));

    rhController.init();

    return 0;
}
