#include "seasocks/PrintfLogger.h"
#include "seasocks/Server.h"
#include "seasocks/WebSocket.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>

using namespace seasocks;
using namespace std;

class MyHandler: public WebSocket::Handler {
public:
    MyHandler(Server* server) : _server(server), _currentValue(0) {
        setValue(1);
    }

    virtual void onConnect(WebSocket* connection) {
        _connections.insert(connection);
        connection->send(_currentSetValue.c_str());
        cout << "Connected: " << connection->getRequestUri()
                << " : " << formatAddress(connection->getRemoteAddress())
                << endl;
        cout << "Credentials: " << *(connection->credentials()) << endl;
    }

    virtual void onData(WebSocket* connection, const char* data) {
        if (0 == strcmp("die", data)) {
            _server->terminate();
            return;
        }
        if (0 == strcmp("close", data)) {
            cout << "Closing.." << endl;
            connection->close();
            cout << "Closed." << endl;
            return;
        }

        int value = atoi(data) + 1;
        if (value > _currentValue) {
            setValue(value);
            for (auto connection : _connections) {
                connection->send(_currentSetValue.c_str());
            }
        }
    }

    virtual void onDisconnect(WebSocket* connection) {
        _connections.erase(connection);
        cout << "Disconnected: " << connection->getRequestUri()
                << " : " << formatAddress(connection->getRemoteAddress())
                << endl;
    }

private:
    set<WebSocket*> _connections;
    Server* _server;
    int _currentValue;
    string _currentSetValue;

    void setValue(int value) {
        _currentValue = value;
        _currentSetValue = makeExecString("set", _currentValue);
    }
};

int main(int argc, const char* argv[]) {
    shared_ptr<Logger> logger(new PrintfLogger(Logger::DEBUG));

    Server server(logger);

    shared_ptr<MyHandler> handler(new MyHandler(&server));
    server.addWebSocketHandler("/ws", handler);
    server.serve("src/ws_test_web", 9090);
    return 0;
}
