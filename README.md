Singularity Computers is at the forefront of innovation in the realm of building high-end water-cooled computer systems and customizations. Our latest development, the _FanGuardian_, represents a significant leap forward in the world of PC monitoring and control.

# FanGuardian
The _FanGuardian_ is a revolutionary piece of hardware designed to elevate the user experience in PC building and customization. At its core, the board is a comprehensive monitoring system that provides real-time data on crucial performance metrics, including motherboard voltage, fan speed, ambient temperature, and water temperature.

This device seamlessly integrates into your PC setup, providing a centralized hub for monitoring and control. By connecting directly to your system's components, including fans, pumps, and sensors, the board gathers data in real-time and displays it on the touch screen interface for easy access.

# Key Features
- **Touch Screen Interface:** Equipped with an intuitive touch screen interface, the _FanGuardian_ provides a user-friendly experience, making it easy to access and interpret real-time data at a glance.
- **Display sensor values:** Check your key PC sensors in real-time, such as motherboard voltages, water and ambient temperatures and FAN speed values.
- **Configurable FAN speed alerts:** Configure alerting tresholds for every kind of FANs or water pumps. If the RPM drops below the treshold _FanGuardian_ will alert.
- **Control your RGB stripe:** Control your RGB LED stripe from the touch display. You can set solid color pattern or choose an animation effect from the list. There is "Aurora" which simulates the Northern Lights, but you can set a simple color fade or twinkle effect, or the classic rainbow which is quite popular among PC modders. One of the coolest pattern is "Temperature effect", which reads the temperature sensor values and sets the RGB stripe color between blue and red, depending on the selected temperature source. Also when FAN alert treshold is reached, the RGB stripe will blink with a color you configured.

# Getting started
The FanGuardianUI firmware is written in C++. The GUI is designed in [EEZ Studio](https://www.envox.eu/studio/studio-introduction/). If you want to do any modification, we suggest to use [Microsoft Visual Studio Code](https://code.visualstudio.com/) with PlatformIO extension.

## Configuring PlatformIO
After you installed PlatformIO extension to VSCode, you have to set up the display board for _FanGuardian_. We are using `Panlee WT32-SC01-Plus` display, which is not supported by PlatformIO at the moment, so you have to add this board manually.
- copy the `assets/for-platformio/wt32-sc01-plus.json` to your `.platformio/platforms/espressif32/boards/` folder

## Changing the GUI
1. In order to change the GUI design, layout, screens, events, fonts etc. you have to install EEZ Studio to your computer. It is free and available on Mac, Windows and Linux: [EEZ Studio releases](https://github.com/eez-open/studio/releases).
2. Open `eezstudio/FanGuardianUI.eez-project` in EEZ Studio.
3. If you want to export your GUI changes, click 'Build' in the toolbar. The generated code is written to `src/ui/`, do not edit these files by hand.

The GUI code which is not generated:
- `src/ui_actions.c` and `src/ui_events.cpp` implement the EEZ Studio actions (event handlers), `action_<name>` for every action in the project.
- `src/ui_theme.c` switches the color themes. The themes' colors are edited in EEZ Studio (Themes panel). The opacity of the `button_bg` and `border_bg` colors is different in each theme, which EEZ Studio themes do not support, so their opacity is set in `ui_theme.c`.
- `src/backgrounds/` contains the selectable background images, they are chosen at runtime in `set_background_image()`.

## Building and flashing
PlatformIO offers an option to compile and upload the firmware directly to the board. Just connect it with an USB cable and click 'Upload' in the top-right corner:

<img width="294" alt="image" src="https://github.com/SingularityComputers/FanGuardianUI/assets/165785169/0f5bf2d7-2a6c-4a3e-9121-6d0f9215244b">

### Building binary
Download esptool to your computer (from https://github.com/espressif/esptool/releases), run PlatformIO binary build (ie. from the VSCode GUI) and run this command to combine the different parts into one binary. (Note, that you have to set the proper path to your PlatformIO project library)

`./esptool --chip esp32s3 merge_bin -o fanguardian-ui.bin --flash_mode dio --flash_size 16MB 0x0 ./.pio/build/WT32-SC01-PLUS/bootloader.bin 0x8000 ./.pio/build/WT32-SC01-PLUS/partitions.bin 0x10000 ./.pio/build/WT32-SC01-PLUS/firmware.bin`

You can then flash the firmware to you display board using the following command:

`./esptool write_flash 0x0 fanguardian-ui.bin`

# License and copyrights

All intellectual property rights and copyrights pertaining to this software are attributed to their respective authors.

As we are committed to our customers and to the PC modding community, we have decided to opensource our firmwares to encourage and inspire the community to create something new and better together.

This means that the code is licensed under:
- [GNU General Public License v3.0](https://choosealicense.com/licenses/gpl-3.0/).

The GNU GPLv3 lets you to do almost anything you want with your project, except distributing closed source versions.
