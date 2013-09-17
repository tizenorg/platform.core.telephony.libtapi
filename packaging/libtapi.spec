Name: libtapi
Summary: Telephony dbus client library
Version: 0.6.70
Release:    2
Group:      Telephony/Libraries
License:    Apache-2.0
Source0:    libtapi-%{version}.tar.gz
Source1001: 	libtapi.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(gobject-2.0)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(security-server)
Obsoletes:      libslp-tapi <= 0.6.69
Provides:       libslp-tapi
%description
Telephony client API library

%package devel
Summary:    Telephony client API (devel)
Requires:   %{name} = %{version}-%{release}

%description devel
Telephony client API library (devel)


%prep
%setup -q
cp %{SOURCE1001} .


%build
%cmake .

make %{?jobs:-j%jobs}

%install
%make_install

rm -rf %{buildroot}/usr/include/telephony/tapi/.gitignore

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%manifest %{name}.manifest
%license LICENSE
%defattr(-,root,root,-)
%{_libdir}/*.so.*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/telephony-client/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/*.so
