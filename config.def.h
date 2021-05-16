/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx        = 2;        /* border pixel of windows */
static const unsigned int gappx           = 5;        /* gaps between windows */
static const unsigned int snap            = 32;       /* snap pixel */
static const unsigned int systraypinning  = 0;        /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft   = 0;        /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing  = 2;        /* systray spacing */
static const int systraypinningfailfirst  = 1;        /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray              = 1;        /* 0 means no systray */
static const int swallowfloating          = 0;        /* 1 means swallow floating windows by default */
static const int showbar                  = 1;        /* 0 means no bar */
static const int topbar                   = 1;        /* 0 means bottom bar */
static const int user_bh                  = 20;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]                = {"Hack Nerd Font:size=11:antialias=true:autohint=true"};
static const char col_bg[]                = "#282828";
static const char col_fg[]                = "#ebdbb2";
static const char col_red[]               = "#fb4934";
static const char col_green[]             = "#b8bb26";
static const char col_yellow[]            = "#fabd2f";
static const char col_blue[]              = "#83a598";
static const char col_purple[]           =  "#d3869b";
static const char col_orange[]            = "#fe8019";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_fg,    col_bg,  col_bg },
	[SchemeSel]  = { col_yellow, col_bg,  col_orange },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
        { "Tall",      tile },    /* first entry is default */
	{ "Full",      monocle },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run"};
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key                           function        argument */
	// launching and killing application
	{ MODKEY,                       XK_space,                     spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_t,                         spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_q,                         killclient,     {0} },
	{ MODKEY,                       XK_b,                         spawn,          SHCMD("brave") },
	{ MODKEY,                       XK_e,                         spawn,          SHCMD("emacsclient -c -a emacs") },
	{ MODKEY,                       XK_f,                         spawn,          SHCMD("emacsclient -c -a '' --eval '(dired nil)'") },
	{ MODKEY|ShiftMask,             XK_f,                         spawn,          SHCMD("pcmanfm") },
	{ MODKEY,                       XK_m,                         spawn,          SHCMD("emacsclient -c -a '' --eval '(mu4e)'") },
	{ MODKEY,                       XK_r,                         spawn,          SHCMD("emacsclient -c -a '' --eval '(elfeed)'") },
	{ MODKEY,                       XK_s,                         spawn,          SHCMD("~/scripts/dpower") },
	{ MODKEY,                       XK_p,                         spawn,          SHCMD("~/scripts/dbw") },
	{ MODKEY|ShiftMask,             XK_s,                         spawn,          SHCMD("~/scripts/dsearch") },
	{ MODKEY|ShiftMask,             XK_p,                         spawn,          SHCMD("~/scripts/dkill") },
	{ MODKEY,                       XK_F1,                        spawn,          SHCMD("~/scripts/walli") },


	// layout modification
        { MODKEY,		        XK_n,                         cyclelayout,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_n,                         cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_space,                     togglefloating, {0} },

	// switching window focus
	{ MODKEY,                       XK_j,                         focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                         focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Return,                    zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_j,                         movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,                         movestack,      {.i = -1 } },

	// window resizing
	{ MODKEY,                       XK_h,                         setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                         setmfact,       {.f = +0.05} },

	// gaps and full screen
	{ MODKEY|ControlMask,           XK_f,                         togglebar,      {0} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },

	// window taging
	{ MODKEY,                       XK_Tab,                       view,           {0} },
	{ MODKEY,                       XK_0,                         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                         tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_comma,                     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                    tagmon,         {.i = +1 } },

	// restart and quit dwm
	{ MODKEY|ShiftMask,             XK_c,                         quit,           {0} },
	{ MODKEY,                       XK_c,                         quit,           {1} }, 


	// volume control
        {0,                             XF86XK_AudioRaiseVolume,      spawn,          SHCMD("amixer set Master 5%+ > /dev/null; kill -44 $(pidof dwmblocks)")},
        {0,                             XF86XK_AudioLowerVolume,      spawn,          SHCMD("amixer set Master 5%- > /dev/null; kill -44 $(pidof dwmblocks)")},
        {0,                             XF86XK_AudioMute,             spawn,          SHCMD("amixer set Master toggle > /dev/null; kill -44 $(pidof dwmblocks)")},

	// backlight control
        {0,                             XF86XK_MonBrightnessUp,       spawn,          SHCMD("xbacklight -inc +5 >/dev/null; kill -45 $(pidof dwmblocks)")},
        {0,                             XF86XK_MonBrightnessDown,     spawn,          SHCMD("xbacklight -dec +5- >/dev/null; kill -45 $(pidof dwmblocks)")},

	// screenshot
	{ 0,                            XK_Print,                     spawn,          SHCMD("~/scripts/sc") },
	{ MODKEY,                       XK_Print,                     spawn,          SHCMD("~/scripts/sc -w") },
	{ MODKEY|ShiftMask,             XK_Print,                     spawn,          SHCMD("~/scripts/sc -s") },
	{ MODKEY|ControlMask,           XK_Print,                     spawn,          SHCMD("~/scripts/sc -c") },
	{ MODKEY|ControlMask|ShiftMask, XK_Print,                     spawn,          SHCMD("~/scripts/sc -cs") },

	// music control
	{ 0,                            XF86XK_AudioPrev,	      spawn,	      SHCMD("mpc prev") },
	{ 0,                            XF86XK_AudioNext,	      spawn,	      SHCMD("mpc next") },
      	{ 0,                            XF86XK_AudioPlay,	      spawn,	      SHCMD("mpc toggle") },

	// workspace switching
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
