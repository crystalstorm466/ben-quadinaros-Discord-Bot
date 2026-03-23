#include <dpp/dpp.h> // Includes most common DPP headers
#include <dpp/message.h>

#include <cstdlib> // For std::getenv
#include <filesystem>
#include <fstream> // Potentially for future config file reading
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <thread>
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

static std::string BOT_TOKEN = std::getenv("SPAM_BOT_TOKEN");
const dpp::snowflake BEN_CHANNEL_ID = 1378103577196761238;

int readCounter() {
  std::ifstream readCounter("counter.txt");
  std::string line;

  if (readCounter.is_open() && std::getline(readCounter, line)) {
    try {
      return std::stoi(line);
    } catch (std::invalid_argument& e) {
      std::cerr << "Invalid counter in counter.txt" << std::endl;
      return 0;
    } catch (const std::out_of_range& e) {
      std::cerr << "Counter out of range" << std::endl;
      return 0;
    }
  }
}

void writeCounter(int counter) {
  std::ofstream outFile("counter.txt");
  if (outFile.is_open()) {
    outFile << counter << std::endl;
    outFile.close();
    return;
  } else {
    std::cerr << "Could not open counter.txt for writing" << std::endl;
    return;
  }
}

void change_img() {

  int width, height, channels;

  int ok = stbi_info("ben.jpg", &width, &height, &channels);
  if (!ok) {
      return;
  }  
  srand(time(NULL));
  unsigned char* img = stbi_load("ben.jpg", &width, &height, &channels, 4);
  if (!img) return;
  int x = rand() % width;
  int y = rand() % height;

  int index = (y * width + x ) * channels;
  img[index + 0] = rand() % 256; //R
  img[index + 1] = rand() % 256;// G
  img[index + 2] = rand() % 256; // B 
  img[index + 3] = rand() % 256; // opacity  

  stbi_write_png("ben_rand.jpg", width, height, 4, img, width * 4);
  stbi_image_free(img);
    
  
}
void on_ready_handler(dpp::cluster &bot, const dpp::ready_t &event) {
    // This function is called when the bot is ready
    std::cout << "Bot is ready! Logged in as " << bot.me.username << std::endl;
        int counter = readCounter();
    // Optionally, you can send a message to a specific channel
  //    msg.add_file("silksong.png", dpp::utility::read_file("silksong.png"));    
      while(true) {
          change_img();
          dpp::message msg (BEN_CHANNEL_ID, "");
          msg.add_file("ben.jpg", dpp::utility::read_file("ben_rand.jpg"));
          
      bot.message_create(msg);
       
    
      counter++;
      writeCounter(counter);
      bot.set_presence(dpp::presence(dpp::ps_online, dpp::activity_type::at_custom, "Sent ben quadinaros " + std::to_string(counter) + " times!"));

      if (counter % 1000000 == 0) {
          std::string id_str = std::to_string(186286469936185344);
          id_str = "<@" + id_str + ">";
          dpp::message  countermsg (BEN_CHANNEL_ID, "@everyone " + id_str + "1 MILLION MORE BEN QUADINAROS!!!!" );        
          bot.message_create(countermsg);
          std::this_thread::sleep_for(std::chrono::seconds(20));
          continue;
        }
      if (counter % 10 == 0) {
        dpp::message countermsg (BEN_CHANNEL_ID, "Sent " + std::to_string(counter) + " times.");
        std::cout << "Sent ben quadinaros " << counter << " times." << std::endl;
        bot.message_create(countermsg);
      }
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
