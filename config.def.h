/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx        = 2;        /* border pixel of windows */
static const unsigned int gappx           = 0;        /* gaps between windows */
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
static const char *fonts[]                = {"Hack Nerd Font:size=11:style=Bold:antialias=true:autohint=true"};
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
	{ "Mpv",     NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Full",      monocle },/* first entry is default */
        { "Tall",      tile },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },
/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run"};
static const char *termcmd[]  = { "st", NULL };


#include "movestack.c"
static Key keys[] = {
	/* modifier                    chainkey  key                           function        argument */

	// launching and killing application
           { MODKEY,                       -1,         XK_space,                     spawn,          {.v = dmenucmd } },
           { MODKEY,                       -1,         XK_t,                         spawn,          {.v = termcmd } },
           { MODKEY,                       -1,         XK_q,                         killclient,     {0} },
           { MODKEY,                       -1,         XK_b,                         spawn,          SHCMD("librewolf") },
           { MODKEY|ShiftMask,             -1,         XK_b,                         spawn,          SHCMD("librewolf") },
           { MODKEY,                       -1,         XK_e,                         spawn,          SHCMD("emacsclient -c -a 'emacs' ") },
           { MODKEY,                       -1,         XK_f,                         spawn,          SHCMD("emacsclient -c -a '' --eval '(dired nil)'") },
           { MODKEY|ShiftMask,             -1,         XK_f,                         spawn,          SHCMD("pcmanfm") },
           { MODKEY,                       -1,         XK_m,                         spawn,          SHCMD("emacsclient -c -a '' --eval '(mu4e)'") },
           { MODKEY,                       -1,         XK_r,                         spawn,          SHCMD("emacsclient -c -a '' --eval '(elfeed)'") },



           // layout modification
           { MODKEY,		           -1,         XK_n,                         cyclelayout,    {.i = +1 } },
           { MODKEY|ShiftMask,             -1,         XK_n,                         cyclelayout,    {.i = -1 } },
           { MODKEY|ControlMask,           -1,         XK_space,                     togglefloating, {0} },

           // switching window focus	  ,
           { MODKEY,                       -1,         XK_j,                         focusstack,     {.i = +1 } },
           { MODKEY,                       -1,         XK_k,                         focusstack,     {.i = -1 } },
           { MODKEY,                       -1,         XK_Return,                    zoom,           {0} },
           { MODKEY|ShiftMask,             -1,         XK_j,                         movestack,      {.i = +1 } },
           { MODKEY|ShiftMask,             -1,         XK_k,                         movestack,      {.i = -1 } },

           // window resizing		  ,
           { MODKEY,                       -1,         XK_h,                         setmfact,       {.f = -0.05} },
           { MODKEY,                       -1,         XK_l,                         setmfact,       {.f = +0.05} },

           // window taging		  ,
           { MODKEY,                       -1,         XK_Tab,                       view,           {0} },
           { MODKEY,                       -1,         XK_0,                         view,           {.ui = ~0 } },
           { MODKEY|ShiftMask,             -1,         XK_0,                         tag,            {.ui = ~0 } },
           { MODKEY|ShiftMask,             -1,         XK_comma,                     tagmon,         {.i = -1 } },
           { MODKEY|ShiftMask,             -1,         XK_period,                    tagmon,         {.i = +1 } },

           // restart and quit dwm		  ,
           { MODKEY|ShiftMask,             -1,         XK_c,                         quit,           {0} },
           { MODKEY,                       -1,         XK_c,                         quit,           {1} },

	   // dmenu keychords
           { MODKEY,                       XK_d,       XK_s,                         spawn,          SHCMD("~/scripts/dpower") },
           { MODKEY,                       XK_d,       XK_p,                         spawn,          SHCMD("~/scripts/dpass") },
           { MODKEY,                       XK_d,       XK_m,                         spawn,          SHCMD("~/scripts/dman") },
           { MODKEY,                       XK_d,       XK_k,                         spawn,          SHCMD("~/scripts/dkill") },
           { MODKEY,                       XK_d,       XK_c,                         spawn,          SHCMD("~/scripts/dcol") },

	   // dsearch keychord
           { MODKEY,                       XK_s,       XK_s,                         spawn,          SHCMD("~/scripts/dsearch") },
           { MODKEY,                       XK_s,       XK_a,                         spawn,          SHCMD("~/scripts/dsearch archkwiki") },
           { MODKEY,                       XK_s,       XK_p,                         spawn,          SHCMD("~/scripts/dsearch aur") },
           { MODKEY,                       XK_s,       XK_d,                         spawn,          SHCMD("~/scripts/dsearch duckduckgo") },
           { MODKEY,                       XK_s,       XK_g,                         spawn,          SHCMD("~/scripts/dsearch google") },
           { MODKEY,                       XK_s,       XK_r,                         spawn,          SHCMD("~/scripts/dsearch reddit") },
           { MODKEY,                       XK_s,       XK_u,                         spawn,          SHCMD("~/scripts/dsearch urbandictionary") },
           { MODKEY,                       XK_s,       XK_y,                         spawn,          SHCMD("~/scripts/dsearch youtube") },

           // volume control		  ,
           {0,                             -1,         XF86XK_AudioRaiseVolume,      spawn,          SHCMD("amixer set Master 5%+ > /dev/null; kill -44 $(pidof dwmblocks)")},
           {0,                             -1,         XF86XK_AudioLowerVolume,      spawn,          SHCMD("amixer set Master 5%- > /dev/null; kill -44 $(pidof dwmblocks)")},
           {0,                             -1,         XF86XK_AudioMute,             spawn,          SHCMD("amixer set Master toggle > /dev/null; kill -44 $(pidof dwmblocks)")},

           // backlight control		  ,
           {0,                             -1,         XF86XK_MonBrightnessUp,       spawn,          SHCMD("xbacklight -inc +5 >/dev/null; kill -45 $(pidof dwmblocks)")},
           {0,                             -1,         XF86XK_MonBrightnessDown,     spawn,          SHCMD("xbacklight -dec +5- >/dev/null; kill -45 $(pidof dwmblocks)")},

           // screenshot			  ,
           { 0,                            -1,         XK_Print,                     spawn,          SHCMD("~/scripts/sc") },
           { MODKEY,                       -1,         XK_Print,                     spawn,          SHCMD("~/scripts/sc -w") },
           { MODKEY|ShiftMask,             -1,         XK_Print,                     spawn,          SHCMD("~/scripts/sc -s") },
           { MODKEY|ControlMask,           -1,         XK_Print,                     spawn,          SHCMD("~/scripts/sc -c") },
           { MODKEY|ControlMask|ShiftMask, -1,         XK_Print,                     spawn,          SHCMD("~/scripts/sc -cs") },

           // music control		  ,
           { 0,                            -1,         XF86XK_AudioPrev,	      spawn,	      SHCMD("mpc prev") },
           { 0,                            -1,         XF86XK_AudioNext,	      spawn,	      SHCMD("mpc next") },
           { 0,                            -1,         XF86XK_AudioPlay,	      spawn,	      SHCMD("mpc toggle") },

           // workspace switching		  ,
           TAGKEYS(                        -1,         XK_1,                      0)
           TAGKEYS(                        -1,         XK_2,                      1)
           TAGKEYS(                        -1,         XK_3,                      2)
           TAGKEYS(                        -1,         XK_4,                      3)
           TAGKEYS(                        -1,         XK_5,                      4)
           TAGKEYS(                        -1,         XK_6,                      5)
           TAGKEYS(                        -1,         XK_7,                      6)
           TAGKEYS(                        -1,         XK_8,                      7)
           TAGKEYS(                        -1,         XK_9,                      8)
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
