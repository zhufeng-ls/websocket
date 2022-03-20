#ifndef JUMP_WEBSOCKETSERVER_H
#define JUMP_WEBSOCKETSERVER_H
#include <string>
#include <set>
#include <mutex>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

namespace jump
{

typedef enum _WebEvent
{
    kJump = 0,
    kNoRegister,
    kSleep
} WebEvent;

typedef struct _JumpMessage 
{
   WebEvent msg_type; // 0为跳远事件， 1为
   struct {
      std::string name;
      int score;
   } score;
} JumpMessage;

extern std::queue<JumpMessage> g_jump_message;   // 相机图传图片

class WebsocketServer
{
public:
    WebsocketServer();
    void run(uint16_t port);
    static bool hasClient();

private:
    typedef websocketpp::server<websocketpp::config::asio> server;
    typedef std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl>> con_list;

    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void startSendThread();
    
    server m_server_;
    con_list m_connections;
    std::mutex m_mutex;
    int port_;

    static bool has_client_;
};

} // jump

#endif // JUMP_WEBSOCKETSERVER_H