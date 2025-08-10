#include <iostream>
#include <thread>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include <chrono>
#include <atomic>

std::atomic<bool> hold(false);
std::atomic<bool> running(true);

class Trigger {
private:
  short delay;
  short actionKeyCode; 

public:

  void setDelay(short delay_in_ms) { 
    delay = delay_in_ms;
  }

  void setKey(short keyCode) { 
    actionKeyCode = keyCode;
  }

  short getDelay() {
    return delay;
  }
  
  void screenClip() {
    system("grim -g \"1715,715 10x10\" clip.png"); 
//    system("magick clip.png clip.ppm");
  }

  void screenStart(){
    system("grim -g \"1715,715 10x10\" action.png"); 
 //   system("magick action.png action.ppm");
  }

  short getKeyCode(){
    return actionKeyCode;
  }

};

int keyLog(short actionKey) { 
    system("sudo chmod a+r /dev/input/event10"); //needs to access the /dev/input/event* file  
    system("ydotoold > /dev/null 2>&1 &");
    const char* device = "/dev/input/event10";  // you need to pick your keyboard here
    int fd = open(device, O_RDONLY); 
    if (fd == -1) {
      std::cerr << "Failed to open a device " << device << ". Try sudo.\n";
      return 1;
    }
    
    input_event ev;

    while (true) { 
      ssize_t n = read(fd, &ev, sizeof(ev));
      if (n == (ssize_t)sizeof(ev)) {
        if (ev.type == EV_KEY) {
          if (ev.value == 1) {
//           std::cout << "Key pressed: code " << ev.code << std::endl;
            if (ev.code == 54) { //rShift to exit
              std::cout << "Exit.\n";
              running = false;
              return 0;
            }   
          }

          if(ev.code == actionKey) {
            if (ev.value == 1 || ev.value == 2) {
              hold = true;
            }
         
            if (ev.value == 0) {
              hold = false;
            }
          }
        }
      }
    }

    close(fd); 
    return 0;
}


int main() {

  Trigger trig;
  trig.setDelay(70); //ms
  trig.setKey(58); //CAPSLOCK

  std::thread keyThread(keyLog, trig.getKeyCode()); 

  bool clipExist = false;
  bool singleClick = false;

  const std::filesystem::path currentDir = std::filesystem::current_path();
  const std::filesystem::path clip = currentDir / "clip.png";
  const std::filesystem::path action = currentDir / "action.png";

  while(running){

    if (hold){
      if (!clipExist) {
        trig.screenClip();
        clipExist = true;
      }
      trig.screenStart();
      
      unsigned int clipSize = std::filesystem::file_size(clip);
      unsigned int actionSize = std::filesystem::file_size(action);

      const unsigned int tolerance = 13;

      if(std::abs((int)clipSize - (int)actionSize) > tolerance) {
        std::cout << "SHOT!!!\n";
        if (!singleClick) {
          std::this_thread::sleep_for(std::chrono::milliseconds(trig.getDelay()));
          system("ydotool click 0xC0");
          singleClick = true;
        }
      }

    }
    if (!hold) {
      clipExist = false;
      singleClick = false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  
  keyThread.join();
  system("pkill ydotoold");
  return 0;
}
