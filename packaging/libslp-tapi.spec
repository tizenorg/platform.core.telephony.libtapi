#sbs-git:slp/pkgs/l/libslp-tapi libslp-tapi 0.1.1
Name: libslp-tapi
Summary: Telephony dbus client library
Version: 0.6.14
Release:    1
Group:      System/Libraries
License:    Apache
Source0:    libslp-tapi-%{version}.tar.gz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(gobject-2.0)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(security-server)

%description
Telephony client API library

%package devel
Summary:    Telephony client API (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Telephony client API library (devel)


%prep
%setup -q


%build
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}


make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

rm -rf %{buildroot}/usr/include/telephony/tapi/.gitignore

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
#%doc COPYING
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%{_includedir}/telephony-client/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/*.so
