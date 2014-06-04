Name:		gperiodic
Summary:	A graphical application for browsing the periodic table
Version:	3.0.0
Release:	%mkrel 1
URL:		http://code.google.com/p/gperiodic
Group:		Sciences/Chemistry
License:	GPLv2+
Source0:	%{name}-%{version}.tar.gz
BuildRequires:	gtk2-devel
BuildRequires:	imagemagick
BuildRequires:	intltool

%description
Gperiodic displays a periodic table of the elements, allowing you to
browse through the elements, and view detailed information about each
element. This program also features a non-graphical interface.

%prep
%setup -q

%build
%make

%install
%makeinstall_std

# Icons
mkdir -p %{buildroot}%{_iconsdir}/hicolor/16x16/apps/
convert -geometry 16x16 icons/%{name}.png %{buildroot}%{_iconsdir}/hicolor/16x16/apps/%{name}.png
mkdir -p %{buildroot}%{_iconsdir}/hicolor/32x32/apps/
convert -geometry 32x32 icons/%{name}.png %{buildroot}%{_iconsdir}/hicolor/32x32/apps/%{name}.png
mkdir -p %{buildroot}%{_iconsdir}/hicolor/48x48/apps/
convert -geometry 48x48 icons/%{name}.png %{buildroot}%{_iconsdir}/hicolor/48x48/apps/%{name}.png

%find_lang %{name}

%files -f %{name}.lang
%doc AUTHORS ChangeLog README
%{_bindir}/*
%{_iconsdir}/hicolor/*/apps/*.png
%{_datadir}/applications/*.desktop
%{_datadir}/pixmaps/*.png
%{_datadir}/pixmaps/*.xpm
%{_mandir}/man1/*.1*
