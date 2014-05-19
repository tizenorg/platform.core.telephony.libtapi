%define major 3
%define minor 0
%define patchlevel 1

Name:           libtapi3
Version:    %{major}.%{minor}.%{patchlevel}
Release:        1
License:        Apache-2.0
Summary:        Telephony client API library
Group:          Telephony/Libraries
Source0:        libtapi3-%{version}.tar.gz
Source1001:     libtapi.manifest
BuildRequires:  cmake
BuildRequires:  python
BuildRequires:  python-xml
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(security-server)
BuildRequires:  pkgconfig(tel-headers)

%description
Telephony client API library

%package devel
Summary:        Telephony client API (devel)
Requires:       %{name} = %{version}

%description devel
Telephony client API library (devel)

%prep
%setup -q
cp %{SOURCE1001} .

%build
%cmake . -DTAPI_VERSION=%{version}

make %{?_smp_mflags}

%install
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%manifest libtapi.manifest
%license LICENSE
%defattr(-,root,root,-)
%{_libdir}/*.so*
%{_bindir}/tapitest

%files devel
%manifest libtapi.manifest
%defattr(-,root,root,-)
%{_includedir}/tapi-3.0/*.h
%{_includedir}/tapi-3.0/private/*.h
%{_libdir}/pkgconfig/*.pc
%{_libdir}/*.so
