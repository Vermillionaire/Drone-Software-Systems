#ifndef WEBPAGE_H
#define WEBPAGE_H

#include "seasocks/PrintfLogger.h"
#include "seasocks/Server.h"
#include "seasocks/WebSocket.h"
#include "Log.h"

using namespace seasocks;

class WebPage: public WebSocket::Handler {
  public:
    WebPage() {    };

    std::thread *page_thread;
    Server* server;

    virtual void onConnect(WebSocket* connection) {    };

    virtual void onData(WebSocket* connection, const char* data) {    };

    virtual void onDisconnect(WebSocket* connection) {    };

    void serverInit() {
      Log::outln("Starting WebSocket Server.");
      shared_ptr<Logger> logger(new PrintfLogger(Logger::ERROR));

      server = new Server(logger);
      server->setLameConnectionTimeoutSeconds(60);

      shared_ptr<WebPage> handler(this);
      server->addWebSocketHandler("/ws", handler);
      server->serve("src/Web", 9001);

    };

    void startServer() {
  		page_thread = new std::thread(&WebPage::serverInit, this);
    };

    void stopServer() {
      if (server != nullptr)
        server->terminate();

      if (page_thread != nullptr)
        page_thread->join();
    };

  private:


};

#endif
