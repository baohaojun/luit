Summary: luit - Locale and ISO 2022 support for Unicode terminals
%define AppProgram luit
%define AppVersion 20130217
%define UseProgram b%{AppProgram}
# $XTermId: luit.spec,v 1.47 2013/02/17 11:35:52 tom Exp $
Name: %{AppProgram}
Version: %{AppVersion}
Release: 1
License: MIT
Group: Applications/System
URL: ftp://invisible-island.net/%{AppProgram}
Source0: %{AppProgram}-%{AppVersion}.tgz
Packager: Thomas Dickey <dickey@invisible-island.net>

%description
Luit is a filter that can be run between an arbitrary application and a
UTF-8 terminal emulator.  It will convert application output  from  the
locale's  encoding  into  UTF-8,  and convert terminal input from UTF-8
into the locale's encoding.

The Xorg version of luit is largely unmaintained, but embedded in useful
packages.  This package installs an alternative binary "bluit", and
adds a symbolic link for "xterm-filter".

%prep

%define debug_package %{nil}

%setup -q -n %{AppProgram}-%{AppVersion}

%build

INSTALL_PROGRAM='${INSTALL}' \
	./configure \
		--program-prefix=b \
		--target %{_target_platform} \
		--prefix=%{_prefix} \
		--bindir=%{_bindir} \
		--libdir=%{_libdir} \
		--mandir=%{_mandir}

make

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

make install                    DESTDIR=$RPM_BUILD_ROOT
( cd $RPM_BUILD_ROOT%{_bindir} && ln -s %{UseProgram} xterm-filter )

strip $RPM_BUILD_ROOT%{_bindir}/%{UseProgram}

%clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc %{AppProgram}.log.html
%{_prefix}/bin/%{UseProgram}
%{_prefix}/bin/xterm-filter
%{_mandir}/man1/%{UseProgram}.*

%changelog
# each patch should add its ChangeLog entries here

* Sat Jun 05 2010 Thomas Dickey

- Fixes/improvements for FreeBSD and Solaris

* Mon May 31 2010 Thomas Dickey
- initial version
