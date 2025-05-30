#include <dpp/dpp.h> // Includes most common DPP headers
#include <dpp/message.h>

#include <cstdlib> // For std::getenv
#include <filesystem>
#include <fstream> // Potentially for future config file reading
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <thread>
#include <chrono>

static std::string BOT_TOKEN = std::getenv("SPAM_BOT_TOKEN");
const dpp::snowflake BEN_CHANNEL_ID = 1378103577196761238;
void on_ready_handler(dpp::cluster &bot, const dpp::ready_t &event) {
    // This function is called when the bot is ready
    std::cout << "Bot is ready! Logged in as " << bot.me.username << std::endl;

    // Optionally, you can send a message to a specific channel
    dpp::message msg (BEN_CHANNEL_ID, "");
    msg.add_file("ben.jpg", dpp::utility::read_file("ben.jpg"));
    while(true) {
             
      bot.message_create(msg);
      std::this_thread::sleep_for(std::chrono::seconds(7));;
        // Wait for a while before sending the next message
        }
}
int main() {
    if (BOT_TOKEN.empty()) {
        std::cerr << "Error: BOT_TOKEN environment variable is not set." << std::endl;
        return EXIT_FAILURE;
    }

  dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);
  bot.on_log(dpp::utility::cout_logger());

  bot.on_ready([&bot](const dpp::ready_t &event) {
      std::cout << "Bot is ready! Logged in as " << bot.me.username << std::endl;
      on_ready_handler(bot, event);
  });

 try {
        bot.start(dpp::st_wait);
    } catch (const dpp::exception& e) {
        std::cerr << "DPP Exception: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught in main." << std::endl;
        return 1;
    }
  return 0;
}
