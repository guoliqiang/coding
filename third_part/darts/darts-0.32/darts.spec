%define prefix /usr
%define version 0.32
%define release 1

Summary: Double ARray Trie System
Name: darts
Version: %{version}
Release: %{release}
Copyright: LGPL
Group: local
Packager: Taku Kudo <taku-ku@is.aist-nara.ac.jp>
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}
Requires: zlib-devel

%description
Double ARray Trie System

%prep

%setup

%build
./configure --prefix=%{prefix}
make CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS"

%install
make prefix=$RPM_BUILD_ROOT%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)
%doc README AUTHORS COPYING doc/*.html doc/*.css
%{prefix}/include/darts.h
%{prefix}/libexec/darts/*
