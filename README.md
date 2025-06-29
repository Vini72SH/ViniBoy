# ViniBoy

**ViniBoy** is a Game Boy emulator written in C, my first system emulation project, designed to improve my skills as a programmer and my knowledge of architecture and software.

---

## Future Features

- 🄹 Emulates the original DMG (Game Boy) CPU and memory map
- 🎮 Input support for keyboard (and optionally gamepad)
- 🖥️ Renders graphics using a pixel-accurate PPU
- 🎲 APU (sound) emulation
- 📂 ROM loading and cartridge header parsing
- ⚙️ Written in pure C for speed
- 🧪 Includes unit and integration tests for components

---

## Getting Started

### Prerequisites
- libsdl2-dev
```bash
sudo apt install libsdl2-dev
```

- libsdl2-ttf-dev
```bash
sudo apt install libsdl2-ttf-dev
```

- A valid Game Boy `.gb` ROM file
### Build & Run

```bash
git clone https://github.com/Vini72SH/viniboy.git
cd viniboy
```

## References

- Based on: https://www.youtube.com/playlist?list=PLVxiWMqQvhg_yk4qy2cSC3457wZJga_e5
- Original Emulator: https://github.com/rockytriton/LLD_gbemu