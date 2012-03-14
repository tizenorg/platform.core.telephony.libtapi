Name:       libslp-tapi
Summary:    Telephony client API
Version:    0.5.79
Release:    1
Group:      System/Libraries
License:    Apache-2.0
Source0:    libslp-tapi-%{version}.tar.gz
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  cmake
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(gthread-2.0)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(dbus-glib-1)
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
%make_install

rm -rf %{buildroot}/usr/include/telephony/tapi/.gitignore



%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig





%files
%{_libdir}/*.so.*


%files devel
%{_includedir}/telephony/common/*.h
%{_includedir}/telephony/tapi/*.h
%exclude %{_includedir}/telephony/tapi/tapi-lib-proxy.h
%exclude %{_includedir}/telephony/common/tel_cs_conn.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/*.so


