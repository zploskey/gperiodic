%define base_version		2.0.10
%define rel			1

Summary: Program for browsing the periodic table.
Name: gperiodic
Version: %{base_version}
Release: %{rel}
License: GPL
Group: Applications/Engineering
URL: http://www.acclab.helsinki.fi/~frantz/software/gperiodic.php
Source0: http://www.acclab.helsinki.fi/~frantz/software/gperiodic-%{version}.tar.gz
BuildRoot: %{_tmppath}/gperiodic-%{version}-root

%description
Gperiodic displays a periodic table of the elements, allowing you to
browse through the elements, and view detailed information about each
element.

%prep
%setup -q -n gperiodic-%{version}

%build
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/usr/share/applications
mkdir -p $RPM_BUILD_ROOT/usr/share/pixmaps

%makeinstall

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)

/usr/bin/gperiodic
/usr/share/applications/gperiodic.desktop
/usr/share/pixmaps/gperiodic.png
/usr/share/pixmaps/gperiodic-crystal.png
/usr/share/locale/sv/LC_MESSAGES/gperiodic.mo
/usr/share/locale/pl/LC_MESSAGES/gperiodic.mo
/usr/share/locale/da/LC_MESSAGES/gperiodic.mo
/usr/share/locale/fr/LC_MESSAGES/gperiodic.mo
/usr/share/locale/tr/LC_MESSAGES/gperiodic.mo
/usr/share/locale/gl/LC_MESSAGES/gperiodic.mo
/usr/share/locale/it/LC_MESSAGES/gperiodic.mo
/usr/share/locale/ru/LC_MESSAGES/gperiodic.mo

%doc INSTALL README NEWS AUTHORS ChangeLog gpl.txt po/gperiodic.pot

%changelog
* Wed Jul 11 2007 Jonas Frantz <jonas.frantz@welho.com> 2.0.10
- Use the standard gtk+ about window
- Include russian translation contributed by Michael Shigorin
* Wed Jun 27 2007 Jonas Frantz <jonas.frantz@welho.com> 2.0.9
- Merged fixes from the debian project, submitted by Daniel Leidert
- Included galician translation submitted by Gonzalo and Leandro Regueiro
* Sat Aug 9 2003 Jonas Frantz <jonas.frantz@helsinki.fi>
- Initial build

