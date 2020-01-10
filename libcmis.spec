%global apiversion 0.5

Name: libcmis
Version: 0.5.1
Release: 2%{?dist}
Summary: A C++ client library for CM interfaces

License: GPLv2+ or LGPLv2+ or MPLv1.1
URL: https://github.com/tdf/libcmis
Source: https://github.com/tdf/libcmis/releases/download/v%{version}/%{name}-%{version}.tar.gz

BuildRequires: boost-devel
BuildRequires: pkgconfig(cppunit)
BuildRequires: pkgconfig(libcurl)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: xmlto

Patch0: 0001-Add-new-Google-Drive-OAuth-2.0-login-procedure.patch
Patch1: 0002-Add-new-mokup-login-pages.patch
Patch2: 0003-Fix-test-in-test-factory.patch
Patch3: 0004-Fix-test-in-test-gdrive.patch
Patch4: 0005-Fix-test-in-test-onedrive.patch

%description
LibCMIS is a C++ client library for working with CM (content management)
interfaces. The primary supported interface (which gave the library its
name) is CMIS, which allows applications to connect to any ECM behaving
as a CMIS server (Alfresco or Nuxeo are examples of open source ones).
Another supported interface is Google Drive.

%package devel
Summary: Development files for %{name}
Requires: %{name}%{?_isa} = %{version}-%{release}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%package tools
Summary: Command line tool to access CMIS
Requires: %{name}%{?_isa} = %{version}-%{release}

%description tools
The %{name}-tools package contains a tool for accessing CMIS from the
command line.

%prep
%autosetup -p1

%build
%configure --disable-silent-rules --disable-static --disable-werror \
    DOCBOOK2MAN='xmlto man'
sed -i \
    -e 's|^hardcode_libdir_flag_spec=.*|hardcode_libdir_flag_spec=""|g' \
    -e 's|^runpath_var=LD_RUN_PATH|runpath_var=DIE_RPATH_DIE|g' \
    libtool
make %{?_smp_mflags}

%install
make install DESTDIR=%{buildroot}
rm -f %{buildroot}/%{_libdir}/*.la

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%check
export LD_LIBRARY_PATH=%{buildroot}/%{_libdir}${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
make %{?_smp_mflags} check

%files
%doc AUTHORS NEWS
%license COPYING.*
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
* Mon May 16 2016 David Tardon <dtardon@redhat.com> - 0.5.1-2
- Resolves: rhbz#1330591 fix Google Drive login
- Related: rhbz#1330591 fix changelog entry

* Fri Mar 04 2016 David Tardon <dtardon@redhat.com> - 0.5.1-1
- Related: rhbz#1290152 new upstream release

* Tue Mar 01 2016 David Tardon <dtardon@redhat.com> - 0.5.0-2
- Related: rhbz#1290152 autoreconf is not needed
- Related: rhbz#1290152 add a bunch of fixes found by coverity

* Mon Feb 22 2016 David Tardon <dtardon@redhat.com> - 0.5.0-1
- Resolves: rhbz#1290152 rebase to 0.5.0

* Fri Sep 05 2014 David Tardon <dtardon@redhat.com> - 0.4.1-5
- Related: rhbz#1132065 coverity: fix mismatching exceptions

* Thu Sep 04 2014 David Tardon <dtardon@redhat.com> - 0.4.1-4
- a few use-after-free fixes for the C wrapper

* Fri Aug 22 2014 David Tardon <dtardon@redhat.com> - 0.4.1-3
- Resolves: rhbz#1132065 rebase to 0.4.1

* Fri Jan 24 2014 Daniel Mach <dmach@redhat.com> - 0.3.1-10
- Mass rebuild 2014-01-24

* Fri Dec 27 2013 Daniel Mach <dmach@redhat.com> - 0.3.1-9
- Mass rebuild 2013-12-27

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
