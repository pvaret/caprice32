/* Caprice32 - Amstrad CPC Emulator
   (c) Copyright 1997-2005 Ulrich Doewich

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef CAP32_H
#define CAP32_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <vector>

class InputMapper;
//#define DEBUG
//#define DEBUG_CRTC
//#define DEBUG_FDC
//#define DEBUG_GA
//#define DEBUG_NO_VIDEO
//#define DEBUG_TAPE
//#define DEBUG_Z80

#define VERSION_STRING "v4.4.0"

#ifndef _MAX_PATH
 #ifdef _POSIX_PATH_MAX
 #define _MAX_PATH _POSIX_PATH_MAX
 #else
 #define _MAX_PATH 256
 #endif
#endif

#define CPC_BASE_FREQUENCY_MHZ  4.0
#define FRAME_PERIOD_MS        20.0
#define CYCLE_COUNT_INIT (CPC_BASE_FREQUENCY_MHZ*FRAME_PERIOD_MS*1000)

#define CPC_SCR_WIDTH 1024 // max width
#define CPC_SCR_HEIGHT 312 // max height
#define CPC_VISIBLE_SCR_WIDTH 384 // visible width: 4+40+4 * 8
#define CPC_VISIBLE_SCR_HEIGHT 270

#define ICN_DISK_WIDTH 14
#define ICN_DISK_HEIGHT 16
#define ICN_TAPE_WIDTH 18
#define ICN_TAPE_HEIGHT 13

#define VOC_THRESHOLD 128

// CRTC flags
#define VS_flag      1
#define HS_flag      2
#define HDT_flag     4
#define VDT_flag     8
#define HT_flag      16
#define VT_flag      32
#define MR_flag      64
#define VTadj_flag   128
#define VSf_flag     256

// FDC constants
#define DSK_BPTMAX      8192
#define DSK_TRACKMAX    102   // max amount that fits in a DSK header
#define DSK_SIDEMAX     2
#define DSK_SECTORMAX   29    // max amount that fits in a track header

#define FDC_TO_CPU      0
#define CPU_TO_FDC      1

#define CMD_PHASE       0
#define EXEC_PHASE      1
#define RESULT_PHASE    2

#define SKIP_flag       1     // skip sectors with DDAM/DAM
#define SEEKDRVA_flag   2     // seek operation has finished for drive A
#define SEEKDRVB_flag   4     // seek operation has finished for drive B
#define RNDDE_flag      8     // simulate random DE sectors
#define OVERRUN_flag    16    // data transfer timed out
#define SCAN_flag       32    // one of the three scan commands is active
#define SCANFAILED_flag 64    // memory and sector data does not match
#define STATUSDRVA_flag 128   // status change of drive A
#define STATUSDRVB_flag 256   // status change of drive B

// Multiface 2 flags
#define MF2_ACTIVE      1
#define MF2_RUNNING     2
#define MF2_INVISIBLE   4

// TODO: Tune threshold based on different joysticks or make it configurable ?
#define JOYSTICK_AXIS_THRESHOLD 16384

#define DEFAULT_VIDEO_PLUGIN 0

typedef struct {
   char id[8];
   char unused1[8];
   unsigned char version;
   unsigned char AF[2];
   unsigned char BC[2];
   unsigned char DE[2];
   unsigned char HL[2];
   unsigned char R;
   unsigned char I;
   unsigned char IFF0;
   unsigned char IFF1;
   unsigned char IX[2];
   unsigned char IY[2];
   unsigned char SP[2];
   unsigned char PC[2];
   unsigned char IM;
   unsigned char AFx[2];
   unsigned char BCx[2];
   unsigned char DEx[2];
   unsigned char HLx[2];
   unsigned char ga_pen;
   unsigned char ga_ink_values[17];
   unsigned char ga_ROM_config;
   unsigned char ga_RAM_config;
   unsigned char crtc_reg_select;
   unsigned char crtc_registers[18];
   unsigned char upper_ROM;
   unsigned char ppi_A;
   unsigned char ppi_B;
   unsigned char ppi_C;
   unsigned char ppi_control;
   unsigned char psg_reg_select;
   unsigned char psg_registers[16];
   unsigned char ram_size[2];
// version 2 info follows
   unsigned char cpc_model;
   unsigned char last_interrupt;
   unsigned char scr_modes[6];
// version 3 info follows
   unsigned char drvA_DOSfilename[13];
   unsigned char drvB_DOSfilename[13];
   unsigned char cart_DOSfilename[13];
   unsigned char fdc_motor;
   unsigned char drvA_current_track;
   unsigned char drvB_current_track;
   unsigned char drvC_current_track;
   unsigned char drvD_current_track;
   unsigned char printer_data;
   unsigned char psg_env_step;
   unsigned char psg_env_direction;
   unsigned char crtc_type;
   unsigned char crtc_addr[2];
   unsigned char crtc_scanline[2];
   unsigned char crtc_char_count[2];
   unsigned char crtc_line_count;
   unsigned char crtc_raster_count;
   unsigned char crtc_vt_adjust_count;
   unsigned char crtc_hsw_count;
   unsigned char crtc_vsw_count;
   unsigned char crtc_flags[2];
   unsigned char ga_int_delay;
   unsigned char ga_sl_count;
   unsigned char z80_int_pending;
   unsigned char unused2[75];
} t_SNA_header;

typedef struct {
   char id[34];
   char unused1[14];
   unsigned char tracks;
   unsigned char sides;
   unsigned char unused2[2];
   unsigned char track_size[DSK_TRACKMAX*DSK_SIDEMAX];
} t_DSK_header;

typedef struct {
   char id[12];
   char unused1[4];
   unsigned char track;
   unsigned char side;
   unsigned char unused2[2];
   unsigned char bps;
   unsigned char sectors;
   unsigned char gap3;
   unsigned char filler;
   unsigned char sector[DSK_SECTORMAX][8];
} t_track_header;



typedef struct {
   unsigned int model;
   unsigned int jumpers;
   unsigned int ram_size;
   unsigned int speed;
   unsigned int limit_speed;
   bool paused;
   unsigned int auto_pause;
   unsigned int boot_time;
   unsigned int keyboard_line;
   unsigned int tape_motor;
   unsigned int tape_play_button;
   unsigned int printer;
   unsigned int printer_port;
   unsigned int mf2;
   unsigned int keyboard;
   unsigned int joystick_emulation;
   unsigned int joysticks;
   unsigned int joystick_menu_button;
   unsigned int joystick_vkeyboard_button;
   int cycle_count;
   std::string resources_path;

   unsigned int scr_fs_width;
   unsigned int scr_fs_height;
   unsigned int scr_fs_bpp;
   unsigned int scr_style;
   unsigned int scr_oglfilter;
   unsigned int scr_oglscanlines;
   unsigned int scr_led;
   unsigned int scr_fps;
   unsigned int scr_tube;
   unsigned int scr_intensity;
   unsigned int scr_remanency;
   unsigned int scr_window;
   unsigned int scr_bpp;
   unsigned int scr_bps;
   unsigned int scr_line_offs;
   unsigned int *scr_base;
   unsigned int *scr_pos;
   void (*scr_render)();
   void (*scr_prerendernorm)();
   void (*scr_prerenderbord)();
   void (*scr_prerendersync)();
   bool scr_is_ogl;

   unsigned int snd_enabled;
   unsigned int snd_playback_rate;
   unsigned int snd_bits;
   unsigned int snd_stereo;
   unsigned int snd_volume;
   unsigned int snd_pp_device;
   unsigned int snd_buffersize;
   unsigned char *snd_bufferptr;
   union {
      struct {
         unsigned int low;
         unsigned int high;
      };
      int64_t both;
   } snd_cycle_count_init;

   std::string kbd_layout;

   unsigned int max_tracksize;

   std::string snap_path; // Path where machine state snapshots will be loaded/saved by default.
   std::string snap_file; // Path to the actual file (zip or not)

   std::string cart_path;
   std::string cart_file;

   std::string dsk_path;
   std::string drvA_file;
   unsigned int drvA_format;
   std::string drvB_file;
   unsigned int drvB_format;

   std::string tape_path;
   std::string tape_file;

   std::string printer_file;
   std::string sdump_dir;

   std::string rom_path;
   std::string rom_file[16];
   std::string rom_mf2;

   class InputMapper *InputMapper;
} t_CPC;

typedef struct {
   unsigned int requested_addr;
   unsigned int next_addr;
   unsigned int addr;
   unsigned int next_address;
   unsigned int scr_base;
   unsigned int char_count;
   unsigned int line_count;
   unsigned int raster_count;
   unsigned int hsw;
   unsigned int hsw_count;
   unsigned int vsw;
   unsigned int vsw_count;
   unsigned int flag_hadhsync;
   unsigned int flag_inmonhsync;
   unsigned int flag_invsync;
   unsigned int flag_invta;
   unsigned int flag_newscan;
   unsigned int flag_reschar;
   unsigned int flag_resframe;
   unsigned int flag_resnext;
   unsigned int flag_resscan;
   unsigned int flag_resvsync;
   unsigned int flag_startvta;
   unsigned int last_hend;
   unsigned int reg5;
   unsigned int r7match;
   unsigned int r9match;
   unsigned int hstart;
   unsigned int hend;
   void (*CharInstMR)();
   void (*CharInstSL)();
   unsigned char reg_select;
   unsigned char registers[18];
   // 6128+ split screen support
   unsigned int split_addr;
   unsigned char split_sl;
   unsigned int sl_count;
   unsigned char interrupt_sl;
} t_CRTC;

typedef struct {
   int timeout;
   int motor;
   int led;
   int flags;
   int phase;
   int byte_count;
   int buffer_count;
   int cmd_length;
   int res_length;
   int cmd_direction;
   void (*cmd_handler)();
   unsigned char *buffer_ptr;
   unsigned char *buffer_endptr;
   unsigned char command[12];
   unsigned char result[8];
} t_FDC;

typedef struct {
	unsigned int hs_count;
   unsigned char ROM_config;
   unsigned char lower_ROM_bank;
   bool registerPageOn;
   unsigned char RAM_bank;
   unsigned char RAM_config;
   unsigned char upper_ROM;
   unsigned int requested_scr_mode;
   unsigned int scr_mode;
   unsigned char pen;
   unsigned char ink_values[17];
   unsigned int palette[34];
   unsigned char sl_count;
   unsigned char int_delay;
} t_GateArray;

typedef struct {
   unsigned char control;
   unsigned char portA;
   unsigned char portB;
   unsigned char portC;
} t_PPI;

typedef struct {
   union {
      struct {
         unsigned int low;
         unsigned int high;
      };
      int64_t both;
   } cycle_count;
	unsigned int buffer_full;
   unsigned char control;
   unsigned char reg_select;
   union {
      unsigned char Index[16];
      struct {
         unsigned char TonALo, TonAHi;
         unsigned char TonBLo, TonBHi;
         unsigned char TonCLo, TonCHi;
         unsigned char Noise;
         unsigned char Mixer;
         unsigned char AmplitudeA, AmplitudeB, AmplitudeC;
         unsigned char EnvelopeLo, EnvelopeHi;
         unsigned char EnvType;
         unsigned char PortA;
         unsigned char PortB;
      };
      struct {
         unsigned short TonA;
         unsigned short TonB;
         unsigned short TonC;
         unsigned char _noise, _mixer, _ampa, _ampb, _ampc;
         unsigned short Envelope;
      };
   } RegisterAY;
   int AmplitudeEnv;
   bool FirstPeriod;
   void (*Synthesizer)();
} t_PSG;

typedef struct {
   int scrln;
   int scanline;
   unsigned int flag_drawing;
   unsigned int frame_completed;
} t_VDU;

typedef struct {
   unsigned char CHRN[4]; // the CHRN for this sector
   unsigned char flags[4]; // ST1 and ST2 - reflects any possible error conditions
   unsigned int size; // sector size in bytes
   unsigned char *data; // pointer to sector data
} t_sector;

typedef struct {
   unsigned int sectors; // sector count for this track
   unsigned int size; // track size in bytes
   unsigned char *data; // pointer to track data
   t_sector sector[DSK_SECTORMAX]; // array of sector information structures
} t_track;

struct t_drive {
   unsigned int tracks; // total number of tracks
   unsigned int current_track; // location of drive head
   unsigned int sides; // total number of sides
   unsigned int current_side; // side being accessed
   unsigned int current_sector; // sector being accessed
   unsigned int altered; // has the image been modified?
   unsigned int write_protected; // is the image write protected?
   unsigned int random_DEs; // sectors with Data Errors return random data?
   unsigned int flipped; // reverse the side to access?
   long ipf_id; // IPF ID if the track is loaded with a IPF image
   void (*track_hook)(struct t_drive *);	// hook called each disk rotation
   void (*eject_hook)(struct t_drive *);	// hook called on disk eject
   t_track track[DSK_TRACKMAX][DSK_SIDEMAX]; // array of track information structures
};

typedef struct {
   std::string label; // label to display in options dialog
   unsigned int tracks; // number of tracks
   unsigned int sides; // number of sides
   unsigned int sectors; // sectors per track
   unsigned int sector_size; // sector size as N value
   unsigned int gap3_length; // GAP#3 size
   unsigned char filler_byte; // default byte to use
   unsigned char sector_ids[2][16]; // sector IDs - indices: side, sector
} t_disk_format;

// cap32.cpp
void emulator_reset(bool bolMF2Reset);
int  emulator_init();
int  video_set_palette();
void init_joystick_emulation();
void update_cpc_speed();
int  printer_start();
void printer_stop();
int audio_init ();
void audio_shutdown ();
void audio_pause ();
void audio_resume ();
int video_init ();
void video_shutdown ();

// Return the path to the best (i.e: most specific) configuration file.
// Priority order is:
//  - cap32.cfg in the same directory as cap32 binary
//  - $HOME/.cap32.cfg
//  - /etc/cap32.cfg
std::string getConfigurationFilename(bool forWrite = false);
void loadConfiguration (t_CPC &CPC, const std::string& configFilename);
void saveConfiguration (t_CPC &CPC, const std::string& configFilename);

int cap32_main(int argc, char **argv);

// fdc.c
void fdc_write_data(unsigned char val);
unsigned char fdc_read_status();
unsigned char fdc_read_data();

// psg.c
void SetAYRegister(int Num, unsigned char Value);
void Calculate_Level_Tables();
void ResetAYChipEmulation();
void InitAYCounterVars();
void InitAY();

#endif
