%global apiversion 0.3

Name: libcmis
Version: 0.3.1
Release: 8%{?dist}
Summary: A C++ client library for the CMIS interface

Group: System Environment/Libraries
License: GPLv2+ or LGPLv2+ or MPLv1.1
URL: http://sourceforge.net/projects/libcmis/
Source: http://downloads.sourceforge.net/%{name}/%{name}-%{version}.tar.gz

BuildRequires: boost-devel
BuildRequires: libcurl-devel
BuildRequires: libxml2-devel
BuildRequires: xmlto

Patch0: 0001-libcmis-c-handle-possible-bad-allocations.patch
Patch1: 0001-rhbz-918079-always-return-40-hexa-digits.patch
Patch2: 0002-rhbz-918080-restrict-the-set-of-protocols-for-curl.patch
Patch3: 0003-Init-protocols-should-be-done-right-after-resetting-.patch
# rhbz#1000819
Patch4: 0001-fixed-libcmis-c.pc-to-point-to-versioned-libcmis-pc-.patch

%description
LibCMIS is a C++ client library for the CMIS interface. This allows C++
applications to connect to any ECM behaving as a CMIS server like
Alfresco, Nuxeo for the open source ones.

%package devel
Summary: Development files for %{name}
Group: Development/Libraries
Requires: %{name}%{?_isa} = %{version}-%{release}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%package tools
Summary: Command line tool to access CMIS
Group: Applications/Publishing
Requires: %{name}%{?_isa} = %{version}-%{release}

%description tools
The %{name}-tools package contains a tool for accessing CMIS from the
command line.

%prep
%setup -q
%patch0 -p1
%patch1 -p1
%patch2 -p1
%patch3 -p1
%patch4 -p1


%build
%configure --disable-static --disable-tests --disable-werror DOCBOOK2MAN='xmlto man'
sed -i \
    -e 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' \
    -e 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' \
    libtool
make %{?_smp_mflags} V=1


%install
make install DESTDIR=%{buildroot}
rm -f %{buildroot}/%{_libdir}/*.la

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%doc AUTHORS COPYING.* NEWS README
%{_libdir}/%{name}-%{apiversion}.so.*
%{_libdir}/%{name}-c-%{apiversion}.so.*


%files devel
%doc ChangeLog
%{_includedir}/%{name}-%{apiversion}
%{_includedir}/%{name}-c-%{apiversion}
%{_libdir}/%{name}-%{apiversion}.so
%{_libdir}/%{name}-c-%{apiversion}.so
%{_libdir}/pkgconfig/%{name}-%{apiversion}.pc
%{_libdir}/pkgconfig/%{name}-c-%{apiversion}.pc


%files tools
%{_bindir}/cmis-client
%{_mandir}/man1/cmis-client.1*


%changelog
* Fri Aug 30 2013 David Tardon <dtardon@redhat.com> - 0.3.1-8
- Resolves: rhbz#1000819 pkgconfig file for libcmis-c is broken

* Sat Aug 03 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.3.1-7
- Rebuilt for https://fedoraproject.org/wiki/Fedora_20_Mass_Rebuild

* Sat Jul 27 2013 pmachata@redhat.com - 0.3.1-6
- Rebuild for boost 1.54.0

* Wed Apr 24 2013 David Tardon <dtardon@redhat.com> - 0.3.1-5
- Resolves: rhbz#918079 libcmis::sha1() can return digests with fewer
  than 40 hexadecimal digits
- Resolves: rhbz#918080 restrict redirection protocols

* Mon Apr 08 2013 David Tardon <dtardon@redhat.com> - 0.3.1-4
- Resolves: rhbz#918044 memory leaks on exception path in C wrapper

* Sun Feb 10 2013 Denis Arnaud <denis.arnaud_fedora@m4x.org> - 0.3.1-3
- Rebuild for Boost-1.53.0

* Sat Feb 09 2013 Denis Arnaud <denis.arnaud_fedora@m4x.org> - 0.3.1-2
- Rebuild for Boost-1.53.0

* Mon Feb 04 2013 David Tardon <dtardon@redhat.com> - 0.3.1-1
- new release

* Wed Dec 19 2012 David Tardon <dtardon@redhat.com> - 0.3.0-6
- use xmlto for generating man page

* Sat Dec 08 2012 David Tardon <dtardon@redhat.com> - 0.3.0-5
- another pointless bump

* Fri Dec 07 2012 David Tardon <dtardon@redhat.com> - 0.3.0-4
- another pointless rebuild

* Fri Dec 07 2012 David Tardon <dtardon@redhat.com> - 0.3.0-3
- pointless rebuild

* Fri Dec 07 2012 David Tardon <dtardon@redhat.com> - 0.3.0-2
- force rebuild

* Thu Dec 06 2012 David Tardon <dtardon@redhat.com> - 0.3.0-1
- new upstream release

* Tue Nov 06 2012 Caolán McNamara <caolanm@redhat.com> - 0.2.3-4
- clarify license

* Fri Jul 27 2012 David Tardon <dtardon@redhat.com> - 0.2.3-3
- rebuilt for boost 1.50

* Thu Jul 19 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.2.3-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Thu Jun 28 2012 David Tardon <dtardon@redhat.com> - 0.2.3-1
- new upstream version

* Wed Jun 20 2012 David Tardon <dtardon@redhat.com> - 0.2.2-1
- latest upstream version

* Tue Feb 28 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.1.0-2
- Rebuilt for c++ ABI breakage

* Wed Dec 21 2011 David Tardon <dtardon@redhat.com> 0.1.0-1
- initial import
