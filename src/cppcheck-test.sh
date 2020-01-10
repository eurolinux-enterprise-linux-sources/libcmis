#!/bin/sh
/usr/bin/cppcheck -q --enable=style,performance,portability,information --error-exitcode=1 .
exit $?
