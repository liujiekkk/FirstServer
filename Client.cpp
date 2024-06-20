#include "src/client/Client.h"
#include "src/client/Connection.h"
#include <string>
#include <iostream>

int main() {

	fstServ::Client client(std::string("127.0.0.1"), 5005);
	auto conn = client.createConnection();
	for (int i = 0; i < 100000; ++i) {
		conn.connect();
		auto data = conn.send(std::string("liujie,nihao,这是一个测试!!!"));
		std::cout << data << std::endl;
	}
	return 0;
}