# CEX2DEX

A PS Vita application to convert between CEX (Retail) and DEX (TestKit) permanently.

# Caution

<h2><font color="red">**If you are using a spoofing plugin, like PS TV spoofer, ConsoleId spoofer, etc..., disable this plugin before using CEX2DEX application.**</font></h2>

**Currently, CEX2DEX application only works on FW 3.60. But once your device has been converted to DEX, you can move from a FW to another.**

If CEX2DEX is restarted many times on the `loading kernel helper` message, the required plugins are not installed (see installation).

Please note that the installation of DEX ConsoleID will permanently change the device, even though the opposite is doable with CEX2DEX (see uninstallation).

Keep in mind that CEX2DEX has been thoroughly tested but can cause unrecoverable damage to your device (brick) in rare cases.

# Installation

Install [CmepMgr](https://github.com/Princess-of-Sleeping/CmepMgr/releases) (required).

Install [reF00D v1.1](https://github.com/dots-tb/reF00D/releases/) or higher version (required).

Install CEX2DEX vpk (required).

Install DEX (TestKit) ConsoleID from the CEX2DEX menu.

If you want to install DEX (TestKit) PUP, copy DEX PUP to `ux0:data/PSP2/UPDATE/PSP2UPDAT.PUP` and install it from safemode.

# Uninstallation

If you have DEX (TestKit) PUP installed, select `launch temporary CEX spoof` from the CEX2DEX menu and exit the application manually if successful (0x0) else reboot the device and try again. Also install CEX (Retail) PUP from [modoru](https://github.com/TheOfficialFloW/modoru/releases) with holding L on launch time (if successful, it should reboot the device automatically).

Select `uninstall DEX ConsoleID` from the CEX2DEX menu to revert to the original ConsoleID.

# Credits

PrincessOfSleeping for PS Vita AES CTR exploit, implementation and all the code.

Yifan Lu for PS Vita AES CTR exploit hint.

CelesteBlue for PSP/PS3/PS Vita IDStorage RE and ConsoleId wiki.

Kamikaze (CelesteBlue's PS Vita RIP 2020-2020) for giving its life to pseudo-science.

Francesco for testing and keeping the project secret (I guess he did).

Everybody who contributed on vitasdk.

SKGleba for his gallantry.
