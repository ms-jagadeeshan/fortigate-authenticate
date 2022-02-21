#include "util.h"

int main(int argc, char **argv)
{
    curlHandler curlHandler;
    std::string url = "http://1.1.1.1";
    std::string response_string;
    std::string user_pass;
    if (argc < 2)
    {
        std::string username,password;

        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        getPassword(password);

        user_pass.append(username).append(":").append(password);
    }
    else
    {
        user_pass = argv[1];
    }

    curlHandler.authenticate(url.c_str(), response_string, user_pass);
    return 0;
}