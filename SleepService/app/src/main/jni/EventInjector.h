/*
 * Android Event Injector 
 *
 * Copyright (c) 2013 by Radu Motisan , radu.motisan@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <linux/input.h>

struct uinput_event {
	struct timeval time;
	uint16_t type;
	uint16_t code;
	int32_t value;
};

struct label {
    const char *name;
    int value;
};

#define LABEL(constant) { #constant, constant }
#define LABEL_END { NULL, -1 }
#define INPUT_PROP_POINTER		0x00	/* needs a pointer */
#define INPUT_PROP_DIRECT		0x01	/* direct input devices */
#define INPUT_PROP_BUTTONPAD		0x02	/* has button(s) under pad */
#define INPUT_PROP_SEMI_MT		0x03	/* touch rectangle only */

static struct label input_prop_labels[] = {
        LABEL(INPUT_PROP_POINTER),
        LABEL(INPUT_PROP_DIRECT),
        LABEL(INPUT_PROP_BUTTONPAD),
        LABEL(INPUT_PROP_SEMI_MT),
        LABEL_END,
};

static struct label ev_labels[] = {
        LABEL(EV_SYN),
        LABEL(EV_KEY),
        LABEL(EV_REL),
        LABEL(EV_ABS),
        LABEL(EV_MSC),
        LABEL(EV_SW),
        LABEL(EV_LED),
        LABEL(EV_SND),
        LABEL(EV_REP),
        LABEL(EV_FF),
        LABEL(EV_PWR),
        LABEL(EV_FF_STATUS),
        LABEL_END,
};

#define SYN_MT_REPORT		2
#define SYN_DROPPED		3

static struct label syn_labels[] = {
        LABEL(SYN_REPORT),
        LABEL(SYN_CONFIG),
        LABEL(SYN_MT_REPORT),
        LABEL(SYN_DROPPED),
        LABEL_END,
};

#define KEY_SCALE		120	/* AL Compiz Scale (Expose) */
#define KEY_SCREENLOCK		KEY_COFFEE
#define KEY_DASHBOARD		204	/* AL Dashboard */

#define KEY_BLUETOOTH		237
#define KEY_WLAN		238
#define KEY_UWB			239
#define KEY_VIDEO_NEXT		241	/* drive next video source */
#define KEY_VIDEO_PREV		242	/* drive previous video source */
#define KEY_BRIGHTNESS_CYCLE	243	/* brightness up, after max is min */
#define KEY_BRIGHTNESS_ZERO	244	/* brightness off, use ambient */
#define KEY_DISPLAY_OFF		245	/* display device to off state */
#define KEY_WIMAX		246
#define KEY_RFKILL		247	/* Key that controls all radios */
#define BTN_TOOL_QUADTAP	0x14f	/* Four fingers on trackpad */
#define KEY_VIDEOPHONE		0x1a0	/* Media Select Video Phone */
#define KEY_GAMES		0x1a1	/* Media Select Games */
#define KEY_ZOOMIN		0x1a2	/* AC Zoom In */
#define KEY_ZOOMOUT		0x1a3	/* AC Zoom Out */
#define KEY_ZOOMRESET		0x1a4	/* AC Zoom */
#define KEY_WORDPROCESSOR	0x1a5	/* AL Word Processor */
#define KEY_EDITOR		0x1a6	/* AL Text Editor */
#define KEY_SPREADSHEET		0x1a7	/* AL Spreadsheet */
#define KEY_GRAPHICSEDITOR	0x1a8	/* AL Graphics Editor */
#define KEY_PRESENTATION	0x1a9	/* AL Presentation App */
#define KEY_DATABASE		0x1aa	/* AL Database App */
#define KEY_NEWS		0x1ab	/* AL Newsreader */
#define KEY_VOICEMAIL		0x1ac	/* AL Voicemail */
#define KEY_ADDRESSBOOK		0x1ad	/* AL Contacts/Address Book */
#define KEY_MESSENGER		0x1ae	/* AL Instant Messaging */
#define KEY_DISPLAYTOGGLE	0x1af	/* Turn display (LCD) on and off */
#define KEY_SPELLCHECK		0x1b0   /* AL Spell Check */
#define KEY_LOGOFF		0x1b1   /* AL Logoff */
#define KEY_DOLLAR		0x1b2
#define KEY_EURO		0x1b3

#define KEY_FRAMEBACK		0x1b4	/* Consumer - transport controls */
#define KEY_FRAMEFORWARD	0x1b5
#define KEY_CONTEXT_MENU	0x1b6	/* GenDesc - system context menu */
#define KEY_MEDIA_REPEAT	0x1b7	/* Consumer - transport control */
#define KEY_10CHANNELSUP	0x1b8	/* 10 channels up (10+) */
#define KEY_10CHANNELSDOWN	0x1b9	/* 10 channels down (10-) */
#define KEY_IMAGES		0x1ba	/* AL Image Browser */
#define KEY_BRL_DOT9		0x1f9
#define KEY_BRL_DOT10		0x1fa

#define KEY_NUMERIC_0		0x200	/* used by phones, remote controls, */
#define KEY_NUMERIC_1		0x201	/* and other keypads */
#define KEY_NUMERIC_2		0x202
#define KEY_NUMERIC_3		0x203
#define KEY_NUMERIC_4		0x204
#define KEY_NUMERIC_5		0x205
#define KEY_NUMERIC_6		0x206
#define KEY_NUMERIC_7		0x207
#define KEY_NUMERIC_8		0x208
#define KEY_NUMERIC_9		0x209
#define KEY_NUMERIC_STAR	0x20a
#define KEY_NUMERIC_POUND	0x20b
#define KEY_CAMERA_FOCUS	0x210
#define KEY_WPS_BUTTON		0x211	/* WiFi Protected Setup key */

#define KEY_TOUCHPAD_TOGGLE	0x212	/* Request switch touchpad on or off */
#define KEY_TOUCHPAD_ON		0x213
#define KEY_TOUCHPAD_OFF	0x214

#define KEY_CAMERA_ZOOMIN	0x215
#define KEY_CAMERA_ZOOMOUT	0x216
#define KEY_CAMERA_UP		0x217
#define KEY_CAMERA_DOWN		0x218
#define KEY_CAMERA_LEFT		0x219
#define KEY_CAMERA_RIGHT	0x21a
#define BTN_TRIGGER_HAPPY1		0x2c0
#define BTN_TRIGGER_HAPPY2		0x2c1
#define BTN_TRIGGER_HAPPY3		0x2c2
#define BTN_TRIGGER_HAPPY4		0x2c3
#define BTN_TRIGGER_HAPPY5		0x2c4
#define BTN_TRIGGER_HAPPY6		0x2c5
#define BTN_TRIGGER_HAPPY7		0x2c6
#define BTN_TRIGGER_HAPPY8		0x2c7
#define BTN_TRIGGER_HAPPY9		0x2c8
#define BTN_TRIGGER_HAPPY10		0x2c9
#define BTN_TRIGGER_HAPPY11		0x2ca
#define BTN_TRIGGER_HAPPY12		0x2cb
#define BTN_TRIGGER_HAPPY13		0x2cc
#define BTN_TRIGGER_HAPPY14		0x2cd
#define BTN_TRIGGER_HAPPY15		0x2ce
#define BTN_TRIGGER_HAPPY16		0x2cf
#define BTN_TRIGGER_HAPPY17		0x2d0
#define BTN_TRIGGER_HAPPY18		0x2d1
#define BTN_TRIGGER_HAPPY19		0x2d2
#define BTN_TRIGGER_HAPPY20		0x2d3
#define BTN_TRIGGER_HAPPY21		0x2d4
#define BTN_TRIGGER_HAPPY22		0x2d5
#define BTN_TRIGGER_HAPPY23		0x2d6
#define BTN_TRIGGER_HAPPY24		0x2d7
#define BTN_TRIGGER_HAPPY25		0x2d8
#define BTN_TRIGGER_HAPPY26		0x2d9
#define BTN_TRIGGER_HAPPY27		0x2da
#define BTN_TRIGGER_HAPPY28		0x2db
#define BTN_TRIGGER_HAPPY29		0x2dc
#define BTN_TRIGGER_HAPPY30		0x2dd
#define BTN_TRIGGER_HAPPY31		0x2de
#define BTN_TRIGGER_HAPPY32		0x2df
#define BTN_TRIGGER_HAPPY33		0x2e0
#define BTN_TRIGGER_HAPPY34		0x2e1
#define BTN_TRIGGER_HAPPY35		0x2e2
#define BTN_TRIGGER_HAPPY36		0x2e3
#define BTN_TRIGGER_HAPPY37		0x2e4
#define BTN_TRIGGER_HAPPY38		0x2e5
#define BTN_TRIGGER_HAPPY39		0x2e6
#define BTN_TRIGGER_HAPPY40		0x2e7
#define ABS_MT_SLOT		0x2f	/* MT slot being modified */
#define ABS_MT_TOUCH_MAJOR	0x30	/* Major axis of touching ellipse */
#define ABS_MT_TOUCH_MINOR	0x31	/* Minor axis (omit if circular) */
#define ABS_MT_WIDTH_MAJOR	0x32	/* Major axis of approaching ellipse */
#define ABS_MT_WIDTH_MINOR	0x33	/* Minor axis (omit if circular) */
#define ABS_MT_ORIENTATION	0x34	/* Ellipse orientation */
#define ABS_MT_POSITION_X	0x35	/* Center X ellipse position */
#define ABS_MT_POSITION_Y	0x36	/* Center Y ellipse position */
#define ABS_MT_TOOL_TYPE	0x37	/* Type of touching device */
#define ABS_MT_BLOB_ID		0x38	/* Group a set of packets as a blob */
#define ABS_MT_TRACKING_ID	0x39	/* Unique ID of initiated contact */
#define ABS_MT_PRESSURE		0x3a	/* Pressure on contact area */
#define ABS_MT_DISTANCE		0x3b	/* Contact hover distance */

static struct label key_labels[] = {
        LABEL(KEY_RESERVED),
        LABEL(KEY_ESC),
        LABEL(KEY_1),
        LABEL(KEY_2),
        LABEL(KEY_3),
        LABEL(KEY_4),
        LABEL(KEY_5),
        LABEL(KEY_6),
        LABEL(KEY_7),
        LABEL(KEY_8),
        LABEL(KEY_9),
        LABEL(KEY_0),
        LABEL(KEY_MINUS),
        LABEL(KEY_EQUAL),
        LABEL(KEY_BACKSPACE),
        LABEL(KEY_TAB),
        LABEL(KEY_Q),
        LABEL(KEY_W),
        LABEL(KEY_E),
        LABEL(KEY_R),
        LABEL(KEY_T),
        LABEL(KEY_Y),
        LABEL(KEY_U),
        LABEL(KEY_I),
        LABEL(KEY_O),
        LABEL(KEY_P),
        LABEL(KEY_LEFTBRACE),
        LABEL(KEY_RIGHTBRACE),
        LABEL(KEY_ENTER),
        LABEL(KEY_LEFTCTRL),
        LABEL(KEY_A),
        LABEL(KEY_S),
        LABEL(KEY_D),
        LABEL(KEY_F),
        LABEL(KEY_G),
        LABEL(KEY_H),
        LABEL(KEY_J),
        LABEL(KEY_K),
        LABEL(KEY_L),
        LABEL(KEY_SEMICOLON),
        LABEL(KEY_APOSTROPHE),
        LABEL(KEY_GRAVE),
        LABEL(KEY_LEFTSHIFT),
        LABEL(KEY_BACKSLASH),
        LABEL(KEY_Z),
        LABEL(KEY_X),
        LABEL(KEY_C),
        LABEL(KEY_V),
        LABEL(KEY_B),
        LABEL(KEY_N),
        LABEL(KEY_M),
        LABEL(KEY_COMMA),
        LABEL(KEY_DOT),
        LABEL(KEY_SLASH),
        LABEL(KEY_RIGHTSHIFT),
        LABEL(KEY_KPASTERISK),
        LABEL(KEY_LEFTALT),
        LABEL(KEY_SPACE),
        LABEL(KEY_CAPSLOCK),
        LABEL(KEY_F1),
        LABEL(KEY_F2),
        LABEL(KEY_F3),
        LABEL(KEY_F4),
        LABEL(KEY_F5),
        LABEL(KEY_F6),
        LABEL(KEY_F7),
        LABEL(KEY_F8),
        LABEL(KEY_F9),
        LABEL(KEY_F10),
        LABEL(KEY_NUMLOCK),
        LABEL(KEY_SCROLLLOCK),
        LABEL(KEY_KP7),
        LABEL(KEY_KP8),
        LABEL(KEY_KP9),
        LABEL(KEY_KPMINUS),
        LABEL(KEY_KP4),
        LABEL(KEY_KP5),
        LABEL(KEY_KP6),
        LABEL(KEY_KPPLUS),
        LABEL(KEY_KP1),
        LABEL(KEY_KP2),
        LABEL(KEY_KP3),
        LABEL(KEY_KP0),
        LABEL(KEY_KPDOT),
        LABEL(KEY_ZENKAKUHANKAKU),
        LABEL(KEY_102ND),
        LABEL(KEY_F11),
        LABEL(KEY_F12),
        LABEL(KEY_RO),
        LABEL(KEY_KATAKANA),
        LABEL(KEY_HIRAGANA),
        LABEL(KEY_HENKAN),
        LABEL(KEY_KATAKANAHIRAGANA),
        LABEL(KEY_MUHENKAN),
        LABEL(KEY_KPJPCOMMA),
        LABEL(KEY_KPENTER),
        LABEL(KEY_RIGHTCTRL),
        LABEL(KEY_KPSLASH),
        LABEL(KEY_SYSRQ),
        LABEL(KEY_RIGHTALT),
        LABEL(KEY_LINEFEED),
        LABEL(KEY_HOME),
        LABEL(KEY_UP),
        LABEL(KEY_PAGEUP),
        LABEL(KEY_LEFT),
        LABEL(KEY_RIGHT),
        LABEL(KEY_END),
        LABEL(KEY_DOWN),
        LABEL(KEY_PAGEDOWN),
        LABEL(KEY_INSERT),
        LABEL(KEY_DELETE),
        LABEL(KEY_MACRO),
        LABEL(KEY_MUTE),
        LABEL(KEY_VOLUMEDOWN),
        LABEL(KEY_VOLUMEUP),
        LABEL(KEY_POWER),
        LABEL(KEY_KPEQUAL),
        LABEL(KEY_KPPLUSMINUS),
        LABEL(KEY_PAUSE),
        LABEL(KEY_SCALE),
        LABEL(KEY_KPCOMMA),
        LABEL(KEY_HANGEUL),
        LABEL(KEY_HANGUEL),
        LABEL(KEY_HANJA),
        LABEL(KEY_YEN),
        LABEL(KEY_LEFTMETA),
        LABEL(KEY_RIGHTMETA),
        LABEL(KEY_COMPOSE),
        LABEL(KEY_STOP),
        LABEL(KEY_AGAIN),
        LABEL(KEY_PROPS),
        LABEL(KEY_UNDO),
        LABEL(KEY_FRONT),
        LABEL(KEY_COPY),
        LABEL(KEY_OPEN),
        LABEL(KEY_PASTE),
        LABEL(KEY_FIND),
        LABEL(KEY_CUT),
        LABEL(KEY_HELP),
        LABEL(KEY_MENU),
        LABEL(KEY_CALC),
        LABEL(KEY_SETUP),
        LABEL(KEY_SLEEP),
        LABEL(KEY_WAKEUP),
        LABEL(KEY_FILE),
        LABEL(KEY_SENDFILE),
        LABEL(KEY_DELETEFILE),
        LABEL(KEY_XFER),
        LABEL(KEY_PROG1),
        LABEL(KEY_PROG2),
        LABEL(KEY_WWW),
        LABEL(KEY_MSDOS),
        LABEL(KEY_COFFEE),
        LABEL(KEY_SCREENLOCK),
        LABEL(KEY_DIRECTION),
        LABEL(KEY_CYCLEWINDOWS),
        LABEL(KEY_MAIL),
        LABEL(KEY_BOOKMARKS),
        LABEL(KEY_COMPUTER),
        LABEL(KEY_BACK),
        LABEL(KEY_FORWARD),
        LABEL(KEY_CLOSECD),
        LABEL(KEY_EJECTCD),
        LABEL(KEY_EJECTCLOSECD),
        LABEL(KEY_NEXTSONG),
        LABEL(KEY_PLAYPAUSE),
        LABEL(KEY_PREVIOUSSONG),
        LABEL(KEY_STOPCD),
        LABEL(KEY_RECORD),
        LABEL(KEY_REWIND),
        LABEL(KEY_PHONE),
        LABEL(KEY_ISO),
        LABEL(KEY_CONFIG),
        LABEL(KEY_HOMEPAGE),
        LABEL(KEY_REFRESH),
        LABEL(KEY_EXIT),
        LABEL(KEY_MOVE),
        LABEL(KEY_EDIT),
        LABEL(KEY_SCROLLUP),
        LABEL(KEY_SCROLLDOWN),
        LABEL(KEY_KPLEFTPAREN),
        LABEL(KEY_KPRIGHTPAREN),
        LABEL(KEY_NEW),
        LABEL(KEY_REDO),
        LABEL(KEY_F13),
        LABEL(KEY_F14),
        LABEL(KEY_F15),
        LABEL(KEY_F16),
        LABEL(KEY_F17),
        LABEL(KEY_F18),
        LABEL(KEY_F19),
        LABEL(KEY_F20),
        LABEL(KEY_F21),
        LABEL(KEY_F22),
        LABEL(KEY_F23),
        LABEL(KEY_F24),
        LABEL(KEY_PLAYCD),
        LABEL(KEY_PAUSECD),
        LABEL(KEY_PROG3),
        LABEL(KEY_PROG4),
        LABEL(KEY_DASHBOARD),
        LABEL(KEY_SUSPEND),
        LABEL(KEY_CLOSE),
        LABEL(KEY_PLAY),
        LABEL(KEY_FASTFORWARD),
        LABEL(KEY_BASSBOOST),
        LABEL(KEY_PRINT),
        LABEL(KEY_HP),
        LABEL(KEY_CAMERA),
        LABEL(KEY_SOUND),
        LABEL(KEY_QUESTION),
        LABEL(KEY_EMAIL),
        LABEL(KEY_CHAT),
        LABEL(KEY_SEARCH),
        LABEL(KEY_CONNECT),
        LABEL(KEY_FINANCE),
        LABEL(KEY_SPORT),
        LABEL(KEY_SHOP),
        LABEL(KEY_ALTERASE),
        LABEL(KEY_CANCEL),
        LABEL(KEY_BRIGHTNESSDOWN),
        LABEL(KEY_BRIGHTNESSUP),
        LABEL(KEY_MEDIA),
        LABEL(KEY_SWITCHVIDEOMODE),
        LABEL(KEY_KBDILLUMTOGGLE),
        LABEL(KEY_KBDILLUMDOWN),
        LABEL(KEY_KBDILLUMUP),
        LABEL(KEY_SEND),
        LABEL(KEY_REPLY),
        LABEL(KEY_FORWARDMAIL),
        LABEL(KEY_SAVE),
        LABEL(KEY_DOCUMENTS),
        LABEL(KEY_BATTERY),
        LABEL(KEY_BLUETOOTH),
        LABEL(KEY_WLAN),
        LABEL(KEY_UWB),
        LABEL(KEY_UNKNOWN),
        LABEL(KEY_VIDEO_NEXT),
        LABEL(KEY_VIDEO_PREV),
        LABEL(KEY_BRIGHTNESS_CYCLE),
        LABEL(KEY_BRIGHTNESS_ZERO),
        LABEL(KEY_DISPLAY_OFF),
        LABEL(KEY_WIMAX),
        LABEL(KEY_RFKILL),
        LABEL(BTN_0),
        LABEL(BTN_1),
        LABEL(BTN_2),
        LABEL(BTN_3),
        LABEL(BTN_4),
        LABEL(BTN_5),
        LABEL(BTN_6),
        LABEL(BTN_7),
        LABEL(BTN_8),
        LABEL(BTN_9),
        LABEL(BTN_LEFT),
        LABEL(BTN_RIGHT),
        LABEL(BTN_MIDDLE),
        LABEL(BTN_SIDE),
        LABEL(BTN_EXTRA),
        LABEL(BTN_FORWARD),
        LABEL(BTN_BACK),
        LABEL(BTN_TASK),
        LABEL(BTN_JOYSTICK),
        LABEL(BTN_TRIGGER),
        LABEL(BTN_THUMB),
        LABEL(BTN_THUMB2),
        LABEL(BTN_TOP),
        LABEL(BTN_TOP2),
        LABEL(BTN_PINKIE),
        LABEL(BTN_BASE),
        LABEL(BTN_BASE2),
        LABEL(BTN_BASE3),
        LABEL(BTN_BASE4),
        LABEL(BTN_BASE5),
        LABEL(BTN_BASE6),
        LABEL(BTN_DEAD),
        LABEL(BTN_A),
        LABEL(BTN_B),
        LABEL(BTN_C),
        LABEL(BTN_X),
        LABEL(BTN_Y),
        LABEL(BTN_Z),
        LABEL(BTN_TL),
        LABEL(BTN_TR),
        LABEL(BTN_TL2),
        LABEL(BTN_TR2),
        LABEL(BTN_SELECT),
        LABEL(BTN_START),
        LABEL(BTN_MODE),
        LABEL(BTN_THUMBL),
        LABEL(BTN_THUMBR),
        LABEL(BTN_TOOL_PEN),
        LABEL(BTN_TOOL_RUBBER),
        LABEL(BTN_TOOL_BRUSH),
        LABEL(BTN_TOOL_PENCIL),
        LABEL(BTN_TOOL_AIRBRUSH),
        LABEL(BTN_TOOL_FINGER),
        LABEL(BTN_TOOL_MOUSE),
        LABEL(BTN_TOOL_LENS),
        LABEL(BTN_TOUCH),
        LABEL(BTN_STYLUS),
        LABEL(BTN_STYLUS2),
        LABEL(BTN_TOOL_DOUBLETAP),
        LABEL(BTN_TOOL_TRIPLETAP),
        LABEL(BTN_TOOL_QUADTAP),
        LABEL(BTN_GEAR_DOWN),
        LABEL(BTN_GEAR_UP),
        LABEL(KEY_OK),
        LABEL(KEY_SELECT),
        LABEL(KEY_GOTO),
        LABEL(KEY_CLEAR),
        LABEL(KEY_POWER2),
        LABEL(KEY_OPTION),
        LABEL(KEY_INFO),
        LABEL(KEY_TIME),
        LABEL(KEY_VENDOR),
        LABEL(KEY_ARCHIVE),
        LABEL(KEY_PROGRAM),
        LABEL(KEY_CHANNEL),
        LABEL(KEY_FAVORITES),
        LABEL(KEY_EPG),
        LABEL(KEY_PVR),
        LABEL(KEY_MHP),
        LABEL(KEY_LANGUAGE),
        LABEL(KEY_TITLE),
        LABEL(KEY_SUBTITLE),
        LABEL(KEY_ANGLE),
        LABEL(KEY_ZOOM),
        LABEL(KEY_MODE),
        LABEL(KEY_KEYBOARD),
        LABEL(KEY_SCREEN),
        LABEL(KEY_PC),
        LABEL(KEY_TV),
        LABEL(KEY_TV2),
        LABEL(KEY_VCR),
        LABEL(KEY_VCR2),
        LABEL(KEY_SAT),
        LABEL(KEY_SAT2),
        LABEL(KEY_CD),
        LABEL(KEY_TAPE),
        LABEL(KEY_RADIO),
        LABEL(KEY_TUNER),
        LABEL(KEY_PLAYER),
        LABEL(KEY_TEXT),
        LABEL(KEY_DVD),
        LABEL(KEY_AUX),
        LABEL(KEY_MP3),
        LABEL(KEY_AUDIO),
        LABEL(KEY_VIDEO),
        LABEL(KEY_DIRECTORY),
        LABEL(KEY_LIST),
        LABEL(KEY_MEMO),
        LABEL(KEY_CALENDAR),
        LABEL(KEY_RED),
        LABEL(KEY_GREEN),
        LABEL(KEY_YELLOW),
        LABEL(KEY_BLUE),
        LABEL(KEY_CHANNELUP),
        LABEL(KEY_CHANNELDOWN),
        LABEL(KEY_FIRST),
        LABEL(KEY_LAST),
        LABEL(KEY_AB),
        LABEL(KEY_NEXT),
        LABEL(KEY_RESTART),
        LABEL(KEY_SLOW),
        LABEL(KEY_SHUFFLE),
        LABEL(KEY_BREAK),
        LABEL(KEY_PREVIOUS),
        LABEL(KEY_DIGITS),
        LABEL(KEY_TEEN),
        LABEL(KEY_TWEN),
        LABEL(KEY_VIDEOPHONE),
        LABEL(KEY_GAMES),
        LABEL(KEY_ZOOMIN),
        LABEL(KEY_ZOOMOUT),
        LABEL(KEY_ZOOMRESET),
        LABEL(KEY_WORDPROCESSOR),
        LABEL(KEY_EDITOR),
        LABEL(KEY_SPREADSHEET),
        LABEL(KEY_GRAPHICSEDITOR),
        LABEL(KEY_PRESENTATION),
        LABEL(KEY_DATABASE),
        LABEL(KEY_NEWS),
        LABEL(KEY_VOICEMAIL),
        LABEL(KEY_ADDRESSBOOK),
        LABEL(KEY_MESSENGER),
        LABEL(KEY_DISPLAYTOGGLE),
        LABEL(KEY_SPELLCHECK),
        LABEL(KEY_LOGOFF),
        LABEL(KEY_DOLLAR),
        LABEL(KEY_EURO),
        LABEL(KEY_FRAMEBACK),
        LABEL(KEY_FRAMEFORWARD),
        LABEL(KEY_CONTEXT_MENU),
        LABEL(KEY_MEDIA_REPEAT),
        LABEL(KEY_10CHANNELSUP),
        LABEL(KEY_10CHANNELSDOWN),
        LABEL(KEY_IMAGES),
        LABEL(KEY_DEL_EOL),
        LABEL(KEY_DEL_EOS),
        LABEL(KEY_INS_LINE),
        LABEL(KEY_DEL_LINE),
        LABEL(KEY_FN),
        LABEL(KEY_FN_ESC),
        LABEL(KEY_FN_F1),
        LABEL(KEY_FN_F2),
        LABEL(KEY_FN_F3),
        LABEL(KEY_FN_F4),
        LABEL(KEY_FN_F5),
        LABEL(KEY_FN_F6),
        LABEL(KEY_FN_F7),
        LABEL(KEY_FN_F8),
        LABEL(KEY_FN_F9),
        LABEL(KEY_FN_F10),
        LABEL(KEY_FN_F11),
        LABEL(KEY_FN_F12),
        LABEL(KEY_FN_1),
        LABEL(KEY_FN_2),
        LABEL(KEY_FN_D),
        LABEL(KEY_FN_E),
        LABEL(KEY_FN_F),
        LABEL(KEY_FN_S),
        LABEL(KEY_FN_B),
        LABEL(KEY_BRL_DOT1),
        LABEL(KEY_BRL_DOT2),
        LABEL(KEY_BRL_DOT3),
        LABEL(KEY_BRL_DOT4),
        LABEL(KEY_BRL_DOT5),
        LABEL(KEY_BRL_DOT6),
        LABEL(KEY_BRL_DOT7),
        LABEL(KEY_BRL_DOT8),
        LABEL(KEY_BRL_DOT9),
        LABEL(KEY_BRL_DOT10),
        LABEL(KEY_NUMERIC_0),
        LABEL(KEY_NUMERIC_1),
        LABEL(KEY_NUMERIC_2),
        LABEL(KEY_NUMERIC_3),
        LABEL(KEY_NUMERIC_4),
        LABEL(KEY_NUMERIC_5),
        LABEL(KEY_NUMERIC_6),
        LABEL(KEY_NUMERIC_7),
        LABEL(KEY_NUMERIC_8),
        LABEL(KEY_NUMERIC_9),
        LABEL(KEY_NUMERIC_STAR),
        LABEL(KEY_NUMERIC_POUND),
        LABEL(KEY_CAMERA_FOCUS),
        LABEL(KEY_WPS_BUTTON),
        LABEL(KEY_TOUCHPAD_TOGGLE),
        LABEL(KEY_TOUCHPAD_ON),
        LABEL(KEY_TOUCHPAD_OFF),
        LABEL(KEY_CAMERA_ZOOMIN),
        LABEL(KEY_CAMERA_ZOOMOUT),
        LABEL(KEY_CAMERA_UP),
        LABEL(KEY_CAMERA_DOWN),
        LABEL(KEY_CAMERA_LEFT),
        LABEL(KEY_CAMERA_RIGHT),
        LABEL(BTN_TRIGGER_HAPPY1),
        LABEL(BTN_TRIGGER_HAPPY2),
        LABEL(BTN_TRIGGER_HAPPY3),
        LABEL(BTN_TRIGGER_HAPPY4),
        LABEL(BTN_TRIGGER_HAPPY5),
        LABEL(BTN_TRIGGER_HAPPY6),
        LABEL(BTN_TRIGGER_HAPPY7),
        LABEL(BTN_TRIGGER_HAPPY8),
        LABEL(BTN_TRIGGER_HAPPY9),
        LABEL(BTN_TRIGGER_HAPPY10),
        LABEL(BTN_TRIGGER_HAPPY11),
        LABEL(BTN_TRIGGER_HAPPY12),
        LABEL(BTN_TRIGGER_HAPPY13),
        LABEL(BTN_TRIGGER_HAPPY14),
        LABEL(BTN_TRIGGER_HAPPY15),
        LABEL(BTN_TRIGGER_HAPPY16),
        LABEL(BTN_TRIGGER_HAPPY17),
        LABEL(BTN_TRIGGER_HAPPY18),
        LABEL(BTN_TRIGGER_HAPPY19),
        LABEL(BTN_TRIGGER_HAPPY20),
        LABEL(BTN_TRIGGER_HAPPY21),
        LABEL(BTN_TRIGGER_HAPPY22),
        LABEL(BTN_TRIGGER_HAPPY23),
        LABEL(BTN_TRIGGER_HAPPY24),
        LABEL(BTN_TRIGGER_HAPPY25),
        LABEL(BTN_TRIGGER_HAPPY26),
        LABEL(BTN_TRIGGER_HAPPY27),
        LABEL(BTN_TRIGGER_HAPPY28),
        LABEL(BTN_TRIGGER_HAPPY29),
        LABEL(BTN_TRIGGER_HAPPY30),
        LABEL(BTN_TRIGGER_HAPPY31),
        LABEL(BTN_TRIGGER_HAPPY32),
        LABEL(BTN_TRIGGER_HAPPY33),
        LABEL(BTN_TRIGGER_HAPPY34),
        LABEL(BTN_TRIGGER_HAPPY35),
        LABEL(BTN_TRIGGER_HAPPY36),
        LABEL(BTN_TRIGGER_HAPPY37),
        LABEL(BTN_TRIGGER_HAPPY38),
        LABEL(BTN_TRIGGER_HAPPY39),
        LABEL(BTN_TRIGGER_HAPPY40),
        LABEL_END,
};

static struct label rel_labels[] = {
        LABEL(REL_X),
        LABEL(REL_Y),
        LABEL(REL_Z),
        LABEL(REL_RX),
        LABEL(REL_RY),
        LABEL(REL_RZ),
        LABEL(REL_HWHEEL),
        LABEL(REL_DIAL),
        LABEL(REL_WHEEL),
        LABEL(REL_MISC),
        LABEL_END,
};

static struct label abs_labels[] = {
        LABEL(ABS_X),
        LABEL(ABS_Y),
        LABEL(ABS_Z),
        LABEL(ABS_RX),
        LABEL(ABS_RY),
        LABEL(ABS_RZ),
        LABEL(ABS_THROTTLE),
        LABEL(ABS_RUDDER),
        LABEL(ABS_WHEEL),
        LABEL(ABS_GAS),
        LABEL(ABS_BRAKE),
        LABEL(ABS_HAT0X),
        LABEL(ABS_HAT0Y),
        LABEL(ABS_HAT1X),
        LABEL(ABS_HAT1Y),
        LABEL(ABS_HAT2X),
        LABEL(ABS_HAT2Y),
        LABEL(ABS_HAT3X),
        LABEL(ABS_HAT3Y),
        LABEL(ABS_PRESSURE),
        LABEL(ABS_DISTANCE),
        LABEL(ABS_TILT_X),
        LABEL(ABS_TILT_Y),
        LABEL(ABS_TOOL_WIDTH),
        LABEL(ABS_VOLUME),
        LABEL(ABS_MISC),
        LABEL(ABS_MT_SLOT),
        LABEL(ABS_MT_TOUCH_MAJOR),
        LABEL(ABS_MT_TOUCH_MINOR),
        LABEL(ABS_MT_WIDTH_MAJOR),
        LABEL(ABS_MT_WIDTH_MINOR),
        LABEL(ABS_MT_ORIENTATION),
        LABEL(ABS_MT_POSITION_X),
        LABEL(ABS_MT_POSITION_Y),
        LABEL(ABS_MT_TOOL_TYPE),
        LABEL(ABS_MT_BLOB_ID),
        LABEL(ABS_MT_TRACKING_ID),
        LABEL(ABS_MT_PRESSURE),
        LABEL(ABS_MT_DISTANCE),
        LABEL_END,
};

#define SW_RFKILL_ALL		0x03  /* rfkill master switch, type "any"
					 set = radio enabled */
#define SW_RADIO		SW_RFKILL_ALL	/* deprecated */
#define SW_MICROPHONE_INSERT	0x04  /* set = inserted */
#define SW_DOCK			0x05  /* set = plugged into dock */
#define SW_LINEOUT_INSERT	0x06  /* set = inserted */
#define SW_JACK_PHYSICAL_INSERT 0x07  /* set = mechanical switch set */
#define SW_VIDEOOUT_INSERT	0x08  /* set = inserted */
#define SW_CAMERA_LENS_COVER	0x09  /* set = lens covered */
#define SW_KEYPAD_SLIDE		0x0a  /* set = keypad slide out */
#define SW_FRONT_PROXIMITY	0x0b  /* set = front proximity sensor active */
#define SW_ROTATE_LOCK		0x0c  /* set = rotate locked/disabled */
#define SW_MAX			0x0f
#define SW_CNT			(SW_MAX+1)

static struct label sw_labels[] = {
        LABEL(SW_LID),
        LABEL(SW_TABLET_MODE),
        LABEL(SW_HEADPHONE_INSERT),
        LABEL(SW_RFKILL_ALL),
        LABEL(SW_RADIO),
        LABEL(SW_MICROPHONE_INSERT),
        LABEL(SW_DOCK),
        LABEL(SW_LINEOUT_INSERT),
        LABEL(SW_JACK_PHYSICAL_INSERT),
        LABEL(SW_VIDEOOUT_INSERT),
        LABEL(SW_CAMERA_LENS_COVER),
        LABEL(SW_KEYPAD_SLIDE),
        LABEL(SW_FRONT_PROXIMITY),
        LABEL(SW_ROTATE_LOCK),
        LABEL_END,
};

static struct label msc_labels[] = {
        LABEL(MSC_SERIAL),
        LABEL(MSC_PULSELED),
        LABEL(MSC_GESTURE),
        LABEL(MSC_RAW),
        LABEL(MSC_SCAN),
        LABEL_END,
};

static struct label led_labels[] = {
        LABEL(LED_NUML),
        LABEL(LED_CAPSL),
        LABEL(LED_SCROLLL),
        LABEL(LED_COMPOSE),
        LABEL(LED_KANA),
        LABEL(LED_SLEEP),
        LABEL(LED_SUSPEND),
        LABEL(LED_MUTE),
        LABEL(LED_MISC),
        LABEL(LED_MAIL),
        LABEL(LED_CHARGING),
        LABEL_END,
};

static struct label rep_labels[] = {
        LABEL(REP_DELAY),
        LABEL(REP_PERIOD),
        LABEL_END,
};

static struct label snd_labels[] = {
        LABEL(SND_CLICK),
        LABEL(SND_BELL),
        LABEL(SND_TONE),
        LABEL_END,
};

static struct label id_labels[] = {
        LABEL(ID_BUS),
        LABEL(ID_VENDOR),
        LABEL(ID_PRODUCT),
        LABEL(ID_VERSION),
        LABEL_END,
};

#define BUS_VIRTUAL		0x06
#define BUS_ATARI		0x1B
#define BUS_SPI			0x1C

static struct label bus_labels[] = {
        LABEL(BUS_PCI),
        LABEL(BUS_ISAPNP),
        LABEL(BUS_USB),
        LABEL(BUS_HIL),
        LABEL(BUS_BLUETOOTH),
        LABEL(BUS_VIRTUAL),
        LABEL(BUS_ISA),
        LABEL(BUS_I8042),
        LABEL(BUS_XTKBD),
        LABEL(BUS_RS232),
        LABEL(BUS_GAMEPORT),
        LABEL(BUS_PARPORT),
        LABEL(BUS_AMIGA),
        LABEL(BUS_ADB),
        LABEL(BUS_I2C),
        LABEL(BUS_HOST),
        LABEL(BUS_GSC),
        LABEL(BUS_ATARI),
        LABEL(BUS_SPI),
        LABEL_END,
};

#define MT_TOOL_FINGER		0
#define MT_TOOL_PEN		1
#define MT_TOOL_MAX		1

static struct label mt_tool_labels[] = {
        LABEL(MT_TOOL_FINGER),
        LABEL(MT_TOOL_PEN),
        LABEL(MT_TOOL_MAX),
        LABEL_END,
};

static struct label ff_status_labels[] = {
        LABEL(FF_STATUS_STOPPED),
        LABEL(FF_STATUS_PLAYING),
        LABEL(FF_STATUS_MAX),
        LABEL_END,
};

static struct label ff_labels[] = {
        LABEL(FF_RUMBLE),
        LABEL(FF_PERIODIC),
        LABEL(FF_CONSTANT),
        LABEL(FF_SPRING),
        LABEL(FF_FRICTION),
        LABEL(FF_DAMPER),
        LABEL(FF_INERTIA),
        LABEL(FF_RAMP),
        LABEL(FF_SQUARE),
        LABEL(FF_TRIANGLE),
        LABEL(FF_SINE),
        LABEL(FF_SAW_UP),
        LABEL(FF_SAW_DOWN),
        LABEL(FF_CUSTOM),
        LABEL(FF_GAIN),
        LABEL(FF_AUTOCENTER),
        LABEL_END,
};

static struct label key_value_labels[] = {
        { "UP", 0 },
        { "DOWN", 1 },
        { "REPEAT", 2 },
        LABEL_END,
};
