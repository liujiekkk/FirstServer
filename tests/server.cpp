#include "container/Container.h"
#include "server/EchoServer.h"
#include "AbstractApplication.h"

class FirstrServer : public fstServ::AbstractApplication {

};

int main()
{
	FirstrServer server;
	server.run();
}