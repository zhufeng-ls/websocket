#include "websocket_server.h"

#include <iostream>
#include <sys/prctl.h>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "../base/json.hpp"

namespace jump
{

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::placeholders::_3;
using websocketpp::lib::bind;
using websocketpp::connection_hdl;

bool WebsocketServer::has_client_ =false;

std::queue<JumpMessage> g_jump_message;   // 


WebsocketServer::WebsocketServer()
{
    m_server_.init_asio();
                
    m_server_.set_open_handler(bind(&WebsocketServer::on_open,this,_1));
    m_server_.set_close_handler(bind(&WebsocketServer::on_close,this,_1));
}

bool WebsocketServer::hasClient() 
{
    return has_client_;
}

void WebsocketServer::startSendThread()
{
    using namespace nlohmann;
    auto dispatchSendFunc = [&]
    {
        prctl(PR_SET_NAME, "WebsocketServer-startSendThread");
        while (true)
        {
            if (!g_jump_message.size())
            {
                usleep(500 * 1e3);
                continue;
            }
            
            auto jump_msg =  g_jump_message.front();
            
            json j;
            j["msgtype"] = jump_msg.msg_type;
            json jscore;
            jscore["name"] = jump_msg.score.name;
            jscore["score"] = jump_msg.score.score;
            j["score"] = jscore;
            
            std::string sendtxt = j.dump(2);
            for (auto it = m_connections.begin(); it != m_connections.end(); ++it)
            {
                m_server_.send(*it, sendtxt, websocketpp::frame::opcode::text);
            }

            g_jump_message.pop();
        }
    };

    std::thread t(dispatchSendFunc);
    t.detach();
}

void WebsocketServer::on_open(connection_hdl hdl)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_connections.insert(hdl);
    has_client_ = true;
}
void WebsocketServer::on_close(connection_hdl hdl)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_connections.erase(hdl);
}


void WebsocketServer::run(uint16_t port)
{
    try {
        startSendThread();
        // Set logging settings
        m_server_.set_access_channels(websocketpp::log::alevel::all);
        m_server_.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Register our message handler
        m_server_.listen(port);
        m_server_.start_accept();
        m_server_.run();

        
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

} // nlohmann