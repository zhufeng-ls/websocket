#include "websocket_server.h"
#include <thread>

using namespace std;
using namespace jump;

void productJumpEventCb()
{
    while(true)
    {
        time(NULL);
        int msgtype = rand() % 3;
        JumpMessage msg;
        msg.msg_type = (jump::WebEvent)msgtype;
        if (msgtype == 0)
        {
            msg.score.score = 160;
            msg.score.name = "张三";
        }
        else
        {
            msg.score.score = 0;
            msg.score.name = "unknown";
        }

        g_jump_message.push(msg);
        sleep(5);
    }

}

int main()
{
    std::thread t(productJumpEventCb);
    t.detach();

    WebsocketServer server;
    server.run(9003);
}