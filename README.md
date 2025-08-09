# GNU/Linux TriggerBot

Simple TriggerBot, works in any game by detecting a new pixel in the center of screen.

## Setup
You need some pkgs.

### For arch based:
```
sudo pacman -S ydotool
sudo pacman -S grim
```

### For debian based;
```
sudo apt install ydotool
sudo apt install grim
```

### Next you need to pick delay, triggerKey and your keyboard.

* To set delay you can to change trig.setDelay(80) at 94 line of code.

* To set a triggerKey you can change trig.setKey(58) at 95 line of code. (It's a key code, by default set to CAPSLOCK)

* To set a keyboard, you need to find its event in /dev/input/event*, google how to do this. And then you need to change 49 and 51 lines. //YOU MUST CHANGE THIS

> To buld it you need to run `g++ -o main main.cpp` 

## Run 

Run `./main` in the project directory

Ingame u need to stand and hold capslock.
