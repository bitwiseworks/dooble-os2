Dooble Web Broswer for OS/2
===========================

Dooble is a cross-platform, free and open-source web browser based on the Qt
Toolkit and its Qt WebEngine library that brings Google Chromium web browser to
Qt applications. Dooble is an alternative to Otter Browser which is also based
on Qt WebEngine.

This README file covers the specifics of the version of Dooble for the OS/2
operating system. Please check the original Dooble documentation (accessible
from its Help menu) to get general help on the Dooble application usage.

NOTE THAT THIS IS A PREVIEW VERSION OF DOOBLE FOR OS/2. PLEASE INSTALL IT ON A
TEST SYSTEM TO GIVE IT A TRY. THE LIST OF KNOWN LIMITATIONS IS PROVIDED AT THE
END OF THIS DOCUMENT.


System Requirements
-------------------

* A Pentium 4 CPU is really the minimum. Some tests have shown that a system
  with at least 2 CPU cores is preferred.

* 3 GB of system memory is recommended. Dooble is known to work on systems with
  less memory but it may seriously affect its stability.

* When it comes to the supported operating system, it is strongly recommended
  that you use ArcaOS version 5 or above. Please make sure that you have at
  least the following versions of the following components in your ArcaOS
  installation (go to your ArcaOS order by visiting www.arcanoae.com and
  logging in to check for updates):

  - OS/2 kernel version 14.203
  - ACPI version 3.23.16
  - Latest UNIAUD

  If you are running an older version of OS/2 such as eComStation or OS/2 Warp,
  installing this browser can crash your system because the Qt WebEngine DLL is
  the biggest DLL ever distributed for OS/2 (almost 100 megabytes without debug
  symbols). This DLL needs to be loaded into high memory. Only OS/2 kernel
  version 14.202 and higher supports proper loading and unloading of DLLs from
  high memory. Older OS/2 versions such as MCP 1/2 and eComStation come with
  kernel versions that contain a bug that will crash your system when DLLs are
  unloaded from high memory.


Installing Dooble
-----------------

Installation of the browser can only be done in the RPM/YUM environment. The
easiest way is to use the graphical front-end for RPM/YUM called Arca Noae
Package Manager (ANPM).

In ArcaOS the ANPM program is installed by default and can be found in the
folder: Computer -> System Setup -> Arca Noae Package Manager. If you do not
have ANPM installed for some reason, you can download it here:

https://www.arcanoae.com/resources/downloadables/arca-noae-package-manager/

Step-by-step installation instructions:

1. Launch ANPM.
2. Update all packages on your system via Manage -> Yum Tools -> Update all.
3. Restart your system after updating is finished.
4. Launch ANPM again.
5. Go to the "available" list, select Dooble browser and install it.

Note that with all the updates you need to install and Dooble browser itself
you may need to download a few hundred megabytes.


Known Limitations
-----------------

- Audio in youtube.com videos may only play for a few seconds and then go
  silent. This depends on a combination of the hardware installed in your
  computer.


Contacts
--------

If you want to report a bug report, please visit the Dooble for OS/2 project
page at https://github.com/bitwiseworks/dooble-os2.


Credits
-------

Dooble is Copyright (c) 2008 - present, Alexis Megas.

Dooble for OS/2 (including OS/2 versions of Qt Toolkit version 5 and Chromium
web browser) is kindly brought to you by bww bitwise works GmbH (Dmitriy
Kuminov, Silvan Scherrer, Herwig Bauernfeind, https://www.bitwiseworks.com).

Thanks to Elbert Pol for building and testing both Dooble an Qt at earlier
stages of development.

Thanks to Gregg Young for providing an initial RPM package and OS/2 icon for
Dooble.

Thanks to Roderick Klein from OS/2 Voice (http://os2voice.org) for providing
ongoing fundraising support.
