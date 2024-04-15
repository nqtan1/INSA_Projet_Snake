#include <ctime>
#include "snakewindow.hpp"
using namespace std;

int main(int argc, char *argv[])
{   
    QApplication app(argc, argv);

    srand(time(0)); // Get random number from (0->99)

    SnakeWindow wnd;
    wnd.show();

    return app.exec();
}
