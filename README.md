# LabWars

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

- **Project name:** LabWars
- **Short description:** Top-down shooter supporting chat, zombie and multiplayer modes
- **Environment:** [Minix](https://www.minix3.org/)
- **Tools:** C low-level calls
- **Institution:** [FEUP](https://sigarra.up.pt/feup/en/web_page.Inicial)
- **Course:** [LCOM](https://sigarra.up.pt/feup/en/ucurr_geral.ficha_uc_view?pv_ocorrencia_id=436435) (Computer Laboratory)
- **Project grade:** 19.0/20.0
- **Group members:**
    - Diogo Miguel Ferreira Rodrigues (diogo.rodrigues@fe.up.pt / up201806429@fe.up.pt)
    - Telmo Alexandre Espirito Santo Baptista (up201806554@fe.up.pt)

## Contents

- Theoretical-practical classes (lab0-lab7)
- [Final project](proj)

## Media
### Gallery
| Main menu | Chat |
|-----------|------|
| ![](https://raw.githubusercontent.com/dmfrodrigues/feup-lcom/master/proj/doc/report/images/main_menu.png) | ![](https://raw.githubusercontent.com/dmfrodrigues/feup-lcom/master/proj/doc/report/images/chat02_01.png) |

| Sinple-player/Campaign mode | Zombies mode |
|-----------------------------|--------------|
| ![](https://raw.githubusercontent.com/dmfrodrigues/feup-lcom/master/proj/doc/report/images/campaign01.png) | ![](https://raw.githubusercontent.com/dmfrodrigues/feup-lcom/master/proj/doc/report/images/zombies01.png) |

### Video

[![LabWars - LCOM Project 2019/20](https://raw.githubusercontent.com/dmfrodrigues/feup-lcom/master/proj/doc/report/images/thumbnail.png)](https://youtu.be/p19NshuZql4 "LabWars - LCOM Project 2019/20")

## Install

### Make commands

```sh
cd proj
./setup.sh          # Setup before install
make                # Install
./setup_clean.sh    # Cleanup script
make cleandir       # Cleanup files
```

## Usage

```sh
cd proj
lcom_run proj
```

## Report

The final report is available [here](proj/doc/report/report.pdf).

## License

© 2019-2020 Diogo Rodrigues, Telmo Baptista

All files are licensed under [GNU General Public License v3](LICENSE).
