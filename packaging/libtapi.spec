%define major 0
%define minor 7
%define patchlevel 96

Name:           libtapi
Version:        %{major}.%{minor}.%{patchlevel}
Release:        2
License:        Apache-2.0
Summary:        Telephony dbus client library
Group:          System/Libraries
Source0:        libslp-tapi-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(vconf)
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
Telephony client API library

%package devel
Summary:        Telephony client API (devel)
Group:          Development/Libraries
Requires:       %{name} = %{version}

%description devel
Telephony client API library (devel)

%prep
%setup -q

%build
export LDFLAGS+=" -Wl,-z,nodelete "

export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"

export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"


cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} \
	-DLIB_INSTALL_DIR=%{_libdir} \
	-DTIZEN_ENGINEER_MODE=1 \

make %{?_smp_mflags}

%install
%make_install

rm -rf %{buildroot}%{_includedir}/telephony/tapi/.gitignore
mkdir -p %{buildroot}%{_datadir}/license

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest libtapi.manifest
%defattr(644,system,system,-)
#%doc COPYING
%attr(755,system,system) %{_bindir}/tapitest
%{_libdir}/*.so.*
%{_datadir}/license/libslp-tapi

%files devel
%defattr(644,system,system,-)
%{_includedir}/telephony-client/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/*.so
