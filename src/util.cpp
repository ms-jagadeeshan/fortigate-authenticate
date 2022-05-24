#include "util.h"

void getPassword(std::string &password)
{
    static struct termios old_terminal;
    static struct termios new_terminal;

    // get settings of the actual terminal
    tcgetattr(STDIN_FILENO, &old_terminal);

    // do not echo the characters
    new_terminal = old_terminal;
    new_terminal.c_lflag &= ~(ECHO);

    // set this as the new terminal options
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);

    // get the password
    std::cin >> password;

    // go back to the old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

void curlHandler::authenticate(const char *url, std::string &response_string, const std::string user_pass)
{
    if (!this->curl_handle)
        this->curl_handle = curl_easy_init();

    CURLcode res;
    if (this->curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, &(response_header));
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPALIVE, 300L);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/47.0.2526.111 Safari/537.36");

        res = curl_easy_perform(curl_handle);
        if (res != CURLE_OK)
        {
            std::cerr << "error: " << curl_easy_strerror(res) << "\n";
            return;
        }

        // printing response header
        std::cout << response_header;

        // Searching for magic string
        int pos = response_string.rfind("magic");
        if (pos == 0)
        {
            std::cerr << "Magic string not found error";
            return;
        }

        // Extracting the required values
        std::string magic_value = response_string.substr(pos + 14, 16);
        std::string username = user_pass.substr(0, user_pass.find_first_of(":"));
        std::string password = user_pass.substr(user_pass.find_first_of(":") + 1);

        /**
         * @brief  pay load to be sent to authenticate
         */
        std::string data = "4Tredir=http%3A%2F%2F1.1.1.1%2F";
        data.append("&magic=").append(magic_value);
        data.append("&username=").append(username);
        data.append("&password=").append(password);

        /**
         * @brief Referrer url to be passed
         */
        char *url1 = NULL;
        curl_easy_getinfo(curl_handle, CURLINFO_EFFECTIVE_URL, &url1);

        /**
         * @brief Setting the curl option for next request
         */
        curl_easy_setopt(curl_handle, CURLOPT_REFERER, url1);
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data.c_str());

        res = curl_easy_perform(curl_handle);

        // printing response header
        std::cout << response_header;

        if (res != CURLE_OK)
        {
            std::cerr << "error: " << curl_easy_strerror(res) << "\n";
        }
        curl_easy_reset(this->curl_handle);
    }
}
